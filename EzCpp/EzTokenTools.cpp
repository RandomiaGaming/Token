#include "EzTokenTools.h"
#include "EzCore.h"
#include "EzHelper.h"
#include "EzTokens.h"
#include <sddl.h>
#include <tlhelp32.h>

// Working with the current token
HANDLE EzOpenCurrentToken() {
	HANDLE output;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &output)) {
		throw EzError::FromCode(GetLastError(), __FILE__, __LINE__);
	}
	return output;
}
HANDLE EzDuplicateCurrentToken() {
	HANDLE currentToken = EzOpenCurrentToken();

	HANDLE currentTokenCopy;
	if (!DuplicateTokenEx(currentToken, TOKEN_ALL_ACCESS, NULL, SecurityDelegation, TokenPrimary, &currentTokenCopy)) {
		CloseHandle(currentToken);
		throw EzError::FromCode(GetLastError(), __FILE__, __LINE__);
	}
	CloseHandle(currentToken);

	return currentTokenCopy;
}

// Impersonating tokens
void EzImpersonate(HANDLE token) {
	if (!ImpersonateLoggedOnUser(token)) {
		if (!SetThreadToken(NULL, token)) {
			throw EzError::FromCode(GetLastError(), __FILE__, __LINE__);
		}
	}
}
void EzStopImpersonating() {
	if (!SetThreadToken(NULL, NULL)) {
		throw EzError::FromCode(GetLastError(), __FILE__, __LINE__);
	}
	if (!RevertToSelf()) {
		throw EzError::FromCode(GetLastError(), __FILE__, __LINE__);
	}
}
void EzImpersonateWinLogon() {
	DWORD lastError = 0;

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapshot == INVALID_HANDLE_VALUE) {
		throw EzError::FromCode(GetLastError(), __FILE__, __LINE__);
	}

	DWORD winLogonPID = 0;
	PROCESSENTRY32 processEntry;
	processEntry.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(snapshot, &processEntry)) {
		lastError = GetLastError();
		CloseHandle(snapshot);
		throw EzError::FromCode(lastError, __FILE__, __LINE__);
	}
	do {
		if (lstrcmp(processEntry.szExeFile, L"winlogon.exe") == 0) {
			winLogonPID = processEntry.th32ProcessID;
			break;
		}
	} while (Process32Next(snapshot, &processEntry));
	lastError = GetLastError();
	if (lastError != 0 && lastError != ERROR_NO_MORE_FILES) {
		CloseHandle(snapshot);
		throw EzError::FromCode(lastError, __FILE__, __LINE__);
	}
	CloseHandle(snapshot);

	if (winLogonPID == 0) {
		throw EzError::FromMessageA("WinLogon.exe could not be found in the list of running processes.", __FILE__, __LINE__);
	}

	HANDLE winLogon = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, winLogonPID);
	if (winLogon == NULL) {
		throw EzError::FromCode(GetLastError(), __FILE__, __LINE__);
	}

	HANDLE winLogonToken;
	if (!OpenProcessToken(winLogon, TOKEN_QUERY | TOKEN_DUPLICATE, &winLogonToken)) {
		lastError = GetLastError();
		CloseHandle(winLogon);
		throw EzError::FromCode(lastError, __FILE__, __LINE__);
	}

	try {
		EzImpersonate(winLogonToken);
	}
	catch (...) {
		CloseHandle(winLogonToken);
		CloseHandle(winLogon);
		throw;
	}

	EzCloseHandleSafely(winLogonToken);
	EzCloseHandleSafely(winLogon);
}
void EzImpersonateLsass() {
	DWORD lastError = 0;

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapshot == INVALID_HANDLE_VALUE) {
		throw EzError::FromCode(GetLastError(), __FILE__, __LINE__);
	}

	DWORD lsassPID = 0;
	PROCESSENTRY32 processEntry;
	processEntry.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(snapshot, &processEntry)) {
		lastError = GetLastError();
		CloseHandle(snapshot);
		throw EzError::FromCode(lastError, __FILE__, __LINE__);
	}
	do {
		if (lstrcmp(processEntry.szExeFile, L"lsass.exe") == 0) {
			lsassPID = processEntry.th32ProcessID;
			break;
		}
	} while (Process32Next(snapshot, &processEntry));
	lastError = GetLastError();
	if (lastError != 0 && lastError != ERROR_NO_MORE_FILES) {
		CloseHandle(snapshot);
		throw EzError::FromCode(lastError, __FILE__, __LINE__);
	}
	CloseHandle(snapshot);

	if (lsassPID == 0) {
		throw EzError::FromMessageA("Lsass.exe could not be found in the list of running processes.", __FILE__, __LINE__);
	}

	HANDLE lsass = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, lsassPID);
	if (lsass == NULL) {
		throw EzError::FromCode(GetLastError(), __FILE__, __LINE__);
	}

	HANDLE lsassToken;
	if (!OpenProcessToken(lsass, TOKEN_QUERY | TOKEN_DUPLICATE, &lsassToken)) {
		lastError = GetLastError();
		CloseHandle(lsass);
		throw EzError::FromCode(lastError, __FILE__, __LINE__);
	}

	try {
		EzImpersonate(lsassToken);
	}
	catch (...) {
		CloseHandle(lsassToken);
		CloseHandle(lsass);
		throw;
	}

	EzCloseHandleSafely(lsassToken);
	EzCloseHandleSafely(lsass);
}

// Enabling/disabling token privileges
LUID EzLookupPrivilege(LPCWSTR privilege) {
	LUID output = { };
	if (!LookupPrivilegeValue(NULL, privilege, &output))
	{
		throw EzError::FromCode(GetLastError(), __FILE__, __LINE__);
	}
	return output;
}
void EzEnableAllPrivileges(HANDLE token) {
	DWORD lastError = 0;

	TOKEN_PRIVILEGES* privileges = EzGetTokenPrivileges(token);

	for (DWORD i = 0; i < privileges->PrivilegeCount; i++) {
		privileges->Privileges[i].Attributes = SE_PRIVILEGE_ENABLED;
	}

	if (!AdjustTokenPrivileges(token, FALSE, privileges, sizeof(TOKEN_PRIVILEGES) * (sizeof(LUID_AND_ATTRIBUTES) * (privileges->PrivilegeCount - 1)), NULL, NULL))
	{
		delete[] privileges;
		throw EzError::FromCode(GetLastError(), __FILE__, __LINE__);
	}
	lastError = GetLastError();
	if (lastError == ERROR_NOT_ALL_ASSIGNED) {
		delete[] privileges;
		throw EzError::FromCode(lastError, __FILE__, __LINE__);
	}
	delete[] privileges;
}
void EzDisableAllPrivileges(HANDLE token) {
	if (!AdjustTokenPrivileges(token, TRUE, NULL, 0, NULL, NULL))
	{
		throw EzError::FromCode(GetLastError(), __FILE__, __LINE__);
	}
}
void EzEnablePrivilege(HANDLE token, LUID privilege) {
	DWORD lastError = 0;

	TOKEN_PRIVILEGES tp = { };
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = privilege;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if (!AdjustTokenPrivileges(token, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL))
	{
		throw EzError::FromCode(GetLastError(), __FILE__, __LINE__);
	}
	// Required secondarry check because AdjustTokenPrivileges returns successful if some but not all permissions were adjusted.
	lastError = GetLastError();
	if (lastError == ERROR_NOT_ALL_ASSIGNED) {
		throw EzError::FromCode(lastError, __FILE__, __LINE__);
	}
}
void EzDisablePrivilege(HANDLE token, LUID privilege) {
	TOKEN_PRIVILEGES tp = { };
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = privilege;
	tp.Privileges[0].Attributes = 0;

	if (!AdjustTokenPrivileges(token, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL))
	{
		throw EzError::FromCode(GetLastError(), __FILE__, __LINE__);
	}
}
BOOL EzTokenHasPrivilege(HANDLE token, LUID privilege) {
	TOKEN_PRIVILEGES* tokenPrivileges = EzGetTokenPrivileges(token);

	for (DWORD i = 0; i < tokenPrivileges->PrivilegeCount; i++)
	{
		if (tokenPrivileges->Privileges[i].Luid.LowPart == privilege.LowPart && tokenPrivileges->Privileges[i].Luid.HighPart == privilege.HighPart) {
			delete[] tokenPrivileges;
			return TRUE;
		}
	}

	delete[] tokenPrivileges;
	return FALSE;
}

// Starting processes with tokens
PROCESS_INFORMATION EzLaunchAsToken(HANDLE token, LPCWSTR exePath) {
	STARTUPINFO startupInfo = { };
	PROCESS_INFORMATION processInfo = { };
	if (!CreateProcessWithTokenW(token, LOGON_WITH_PROFILE, exePath, NULL, 0, NULL, NULL, &startupInfo, &processInfo)) {
		throw EzError::FromCode(GetLastError(), __FILE__, __LINE__);
	}

	return processInfo;
}
PROCESS_INFORMATION EzLaunchAsToken(HANDLE token) {
	LPWSTR currentExePath = EzGetCurrentExePath();
	PROCESS_INFORMATION processInfo = { };
	try {
		EzLaunchAsToken(token, currentExePath);
	}
	catch (...) {
		delete[] currentExePath;
		throw;
	}
	delete[] currentExePath;

	return processInfo;
}
PROCESS_INFORMATION EzLaunchAsUser(HANDLE token, LPCWSTR exePath) {
	STARTUPINFO startupInfo = { };
	PROCESS_INFORMATION processInfo = { };
	if (!CreateProcessAsUser(token, exePath, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &processInfo)) {
		throw EzError::FromCode(GetLastError(), __FILE__, __LINE__);
	}

	return processInfo;
}
PROCESS_INFORMATION EzLaunchAsUser(HANDLE token) {
	LPWSTR currentExePath = EzGetCurrentExePath();
	PROCESS_INFORMATION processInfo = { };
	try {
		processInfo = EzLaunchAsUser(token, currentExePath);
	}
	catch (...) {
		delete[] currentExePath;
		throw;
	}
	delete[] currentExePath;

	return processInfo;
}
BOOL EzLaunchWithUAC(LPCWSTR exePath) {
	// NOTE: Returns TRUE if the user selects yes to the UAC else FALSE.
	DWORD lastError = 0;

	SHELLEXECUTEINFO shellExecuteInfo = { };
	shellExecuteInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	shellExecuteInfo.fMask = 0;
	shellExecuteInfo.hwnd = 0;
	shellExecuteInfo.lpVerb = L"runas";
	shellExecuteInfo.lpFile = exePath;
	shellExecuteInfo.lpParameters = NULL;
	shellExecuteInfo.lpDirectory = NULL;
	shellExecuteInfo.nShow = SW_SHOWNORMAL;
	shellExecuteInfo.hInstApp = NULL;
	shellExecuteInfo.lpIDList = NULL;
	shellExecuteInfo.lpClass = NULL;
	shellExecuteInfo.hkeyClass = NULL;
	shellExecuteInfo.dwHotKey = 0;
	shellExecuteInfo.hIcon = NULL;
	shellExecuteInfo.hMonitor = NULL;
	shellExecuteInfo.hProcess = NULL;
	if (!ShellExecuteEx(&shellExecuteInfo)) {
		lastError = GetLastError();
		if (lastError == ERROR_CANCELLED) {
			return FALSE;
		}
		else {
			throw EzError::FromCode(lastError, __FILE__, __LINE__);
		}
	}
	return TRUE;
}
BOOL EzLaunchWithUAC() {
	LPWSTR currentExePath = EzGetCurrentExePath();
	BOOL uacAccepted = FALSE;
	try {
		uacAccepted = EzLaunchWithUAC(currentExePath);
	}
	catch (...) {
		delete[] currentExePath;
		throw;
	}
	delete[] currentExePath;

	return uacAccepted;
}

// Token privilege escalation
void EzGrantUIAccessToToken(HANDLE token) {
	// Impersonate WinLogon.exe
	EzImpersonateWinLogon();

	// Give UIAccess to the token with the privilages we now have.
	EzSetTokenUIAccess(token, TRUE);

	// Stop impersonating WinLogon.exe's process token.
	EzStopImpersonating();
}
void EzMakeTokenInteractive(HANDLE token) {
	// Impersonate WinLogon.exe
	EzImpersonateWinLogon();

	// Get current session id.
	DWORD activeConsoleSessionId = WTSGetActiveConsoleSessionId();
	if (activeConsoleSessionId == 0xFFFFFFFF) {
		throw EzError::FromMessageA("Could not create an interactive token because there is no active session currently.", __FILE__, __LINE__);
	}

	// Change the session ID of the token to the current session ID.
	EzSetTokenSessionId(token, activeConsoleSessionId);

	// Stop impersonating WinLogon.exe's process token.
	EzStopImpersonating();
}
void EzGiveTokenSystemIntegrity(HANDLE token) {
	// Lookup the system integrity level SID.
	PSID systemIntegritySid = NULL;
	if (!ConvertStringSidToSid(L"S-1-16-16384", &systemIntegritySid)) {
		throw EzError::FromCode(GetLastError(), __FILE__, __LINE__);
	}

	// Assign system integrity level to the token.
	SID_AND_ATTRIBUTES tokenIntegrityLevel = { };
	tokenIntegrityLevel.Sid = systemIntegritySid;
	tokenIntegrityLevel.Attributes = SE_GROUP_INTEGRITY | SE_GROUP_INTEGRITY_ENABLED | SE_GROUP_MANDATORY;
	EzSetTokenIntegrityLevel(token, tokenIntegrityLevel);

	// Cleanup and return.
	LocalFree(systemIntegritySid);
}
void EzStealCreateTokenPermission(HANDLE token) {
	// Impersonate Lsass.exe
	EzImpersonateLsass();

	// Give UIAccess to the token with the privilages we now have.
	TOKEN_PRIVILEGES tp = {};
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = EzLookupPrivilege(SE_CREATE_TOKEN_NAME);
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED | SE_PRIVILEGE_ENABLED_BY_DEFAULT;
	EzSetTokenPrivileges(token, &tp);

	// Stop impersonating WinLogon.exe's process token.
	EzStopImpersonating();
}
HANDLE EzCreateGodToken() {
	/* KNOWN ISSUE
	NtCreateToken only works with pointers to stack memory or pointers to
	heap memory allocated with LocalAlloc or GlobalAlloc. C++ style new[]
	or C style malloc will not work.
	*/
	/* KNOWN ISSUE
	The SE_UNSOLICITED_INPUT_NAME privilege is not supported on Windows 10
	home edition and therefore is not given to the god token.
	*/

	typedef struct _UNICODE_STRING {
		USHORT Length;
		USHORT MaximumLength;
		PWSTR Buffer;
	} UNICODE_STRING, * PUNICODE_STRING;
	typedef struct OBJECT_ATTRIBUTES {
		ULONG Length;
		HANDLE RootDirectory;
		PUNICODE_STRING ObjectName;
		ULONG Attributes;
		PVOID SecurityDescriptor;
		PVOID SecurityQualityOfService;
	} OBJECT_ATTRIBUTES, * POBJECT_ATTRIBUTES;
	typedef NTSTATUS(*PNtCreateToken)(PHANDLE TokenHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, TOKEN_TYPE TokenType, PLUID AuthenticationId, PLARGE_INTEGER ExpirationTime, PTOKEN_USER TokenUser, PTOKEN_GROUPS TokenGroups, PTOKEN_PRIVILEGES TokenPrivileges, PTOKEN_OWNER TokenOwner, PTOKEN_PRIMARY_GROUP TokenPrimaryGroup, PTOKEN_DEFAULT_DACL TokenDefaultDacl, PTOKEN_SOURCE TokenSource);

	DWORD lastError = 0;
	NTSTATUS nt = 0;

	// Impersonate Lsass to get the SE_CREATE_TOKEN_NAME privilege
	EzImpersonateLsass();

	// Load NtCreateToken function from ntdll.dll
	PNtCreateToken NtCreateToken = reinterpret_cast<PNtCreateToken>(EzGetFunctionAddressW(L"NtCreateToken", L"ntdll.dll"));

	// Get sids for users, groups, and integrity levels we need.
	PSID systemUserSid = NULL;
	if (!ConvertStringSidToSid(L"S-1-5-18", &systemUserSid)) {
		lastError = GetLastError();
		EzStopImpersonating();
		throw EzError::FromCode(lastError, __FILE__, __LINE__);
	}
	PSID administratorsGroupSid = NULL;
	if (!ConvertStringSidToSid(L"S-1-5-32-544", &administratorsGroupSid)) {
		lastError = GetLastError();
		EzStopImpersonating();
		throw EzError::FromCode(lastError, __FILE__, __LINE__);
	}
	PSID authenticatedUsersGroupSid = NULL;
	if (!ConvertStringSidToSid(L"S-1-5-11", &authenticatedUsersGroupSid)) {
		lastError = GetLastError();
		EzStopImpersonating();
		throw EzError::FromCode(lastError, __FILE__, __LINE__);
	}
	PSID everyoneGroupSid = NULL;
	if (!ConvertStringSidToSid(L"S-1-1-0", &everyoneGroupSid)) {
		lastError = GetLastError();
		EzStopImpersonating();
		throw EzError::FromCode(lastError, __FILE__, __LINE__);
	}
	PSID systemIntegrityLevelSid = NULL;
	if (!ConvertStringSidToSid(L"S-1-16-16384", &systemIntegrityLevelSid)) {
		lastError = GetLastError();
		EzStopImpersonating();
		throw EzError::FromCode(lastError, __FILE__, __LINE__);
	}
	PSID trustedInstallerUserSid = NULL;
	if (!ConvertStringSidToSid(L"S-1-5-80-956008885-3418522649-1831038044-1853292631-2271478464", &trustedInstallerUserSid)) {
		lastError = GetLastError();
		EzStopImpersonating();
		throw EzError::FromCode(lastError, __FILE__, __LINE__);
	}

	ACCESS_MASK desiredAccess = TOKEN_ALL_ACCESS;

	SECURITY_QUALITY_OF_SERVICE securityQualityOfService = { };
	securityQualityOfService.Length = sizeof(SECURITY_QUALITY_OF_SERVICE);
	securityQualityOfService.ImpersonationLevel = SecurityAnonymous;
	securityQualityOfService.ContextTrackingMode = SECURITY_STATIC_TRACKING;
	securityQualityOfService.EffectiveOnly = FALSE;

	OBJECT_ATTRIBUTES objectAttributes = { };
	objectAttributes.Length = sizeof(OBJECT_ATTRIBUTES);
	objectAttributes.RootDirectory = NULL;
	objectAttributes.ObjectName = NULL;
	objectAttributes.Attributes = 0;
	objectAttributes.SecurityDescriptor = NULL;
	objectAttributes.SecurityQualityOfService = &securityQualityOfService;

	TOKEN_TYPE tokenType = TokenPrimary;

	LUID authenticationID = SYSTEM_LUID;

	LARGE_INTEGER expirationTime = { 0xFFFFFFFFFFFFFFFF };

	TOKEN_USER tokenUser = { };
	tokenUser.User.Sid = systemUserSid;
	tokenUser.User.Attributes = 0;

	constexpr DWORD tokenGroupsGroupCount = 5;
	PTOKEN_GROUPS tokenGroups = reinterpret_cast<PTOKEN_GROUPS>(LocalAlloc(LPTR, sizeof(TOKEN_GROUPS) + ((tokenGroupsGroupCount - 1) * sizeof(SID_AND_ATTRIBUTES))));
	tokenGroups->GroupCount = tokenGroupsGroupCount;
	tokenGroups->Groups[0].Sid = administratorsGroupSid;
	tokenGroups->Groups[0].Attributes = SE_GROUP_ENABLED | SE_GROUP_ENABLED_BY_DEFAULT | SE_GROUP_MANDATORY | SE_GROUP_OWNER;
	tokenGroups->Groups[1].Sid = authenticatedUsersGroupSid;
	tokenGroups->Groups[1].Attributes = SE_GROUP_ENABLED | SE_GROUP_ENABLED_BY_DEFAULT | SE_GROUP_MANDATORY;
	tokenGroups->Groups[2].Sid = everyoneGroupSid;
	tokenGroups->Groups[2].Attributes = SE_GROUP_ENABLED | SE_GROUP_ENABLED_BY_DEFAULT | SE_GROUP_MANDATORY;
	tokenGroups->Groups[3].Sid = systemIntegrityLevelSid;
	tokenGroups->Groups[3].Attributes = SE_GROUP_INTEGRITY | SE_GROUP_INTEGRITY_ENABLED | SE_GROUP_MANDATORY;
	tokenGroups->Groups[4].Sid = trustedInstallerUserSid;
	tokenGroups->Groups[4].Attributes = SE_GROUP_ENABLED | SE_GROUP_ENABLED_BY_DEFAULT | SE_GROUP_MANDATORY;

	constexpr DWORD tokenPrivilegesPrivilegeCount = 35;
	PTOKEN_PRIVILEGES tokenPrivileges = reinterpret_cast<PTOKEN_PRIVILEGES>(LocalAlloc(LPTR, sizeof(PTOKEN_PRIVILEGES) + ((tokenPrivilegesPrivilegeCount - 1) * sizeof(LUID_AND_ATTRIBUTES))));
	tokenPrivileges->PrivilegeCount = 35;
	for (int i = 0; i < tokenPrivilegesPrivilegeCount; i++)
	{
		tokenPrivileges->Privileges[i].Attributes = SE_PRIVILEGE_ENABLED | SE_PRIVILEGE_ENABLED_BY_DEFAULT;
	}
	tokenPrivileges->Privileges[0].Luid = EzLookupPrivilege(SE_CREATE_TOKEN_NAME);
	tokenPrivileges->Privileges[1].Luid = EzLookupPrivilege(SE_ASSIGNPRIMARYTOKEN_NAME);
	tokenPrivileges->Privileges[2].Luid = EzLookupPrivilege(SE_LOCK_MEMORY_NAME);
	tokenPrivileges->Privileges[3].Luid = EzLookupPrivilege(SE_INCREASE_QUOTA_NAME);
	tokenPrivileges->Privileges[4].Luid = EzLookupPrivilege(SE_MACHINE_ACCOUNT_NAME);
	tokenPrivileges->Privileges[5].Luid = EzLookupPrivilege(SE_TCB_NAME);
	tokenPrivileges->Privileges[6].Luid = EzLookupPrivilege(SE_SECURITY_NAME);
	tokenPrivileges->Privileges[7].Luid = EzLookupPrivilege(SE_TAKE_OWNERSHIP_NAME);
	tokenPrivileges->Privileges[8].Luid = EzLookupPrivilege(SE_LOAD_DRIVER_NAME);
	tokenPrivileges->Privileges[9].Luid = EzLookupPrivilege(SE_SYSTEM_PROFILE_NAME);
	tokenPrivileges->Privileges[10].Luid = EzLookupPrivilege(SE_SYSTEMTIME_NAME);
	tokenPrivileges->Privileges[11].Luid = EzLookupPrivilege(SE_PROF_SINGLE_PROCESS_NAME);
	tokenPrivileges->Privileges[12].Luid = EzLookupPrivilege(SE_INC_BASE_PRIORITY_NAME);
	tokenPrivileges->Privileges[13].Luid = EzLookupPrivilege(SE_CREATE_PAGEFILE_NAME);
	tokenPrivileges->Privileges[14].Luid = EzLookupPrivilege(SE_CREATE_PERMANENT_NAME);
	tokenPrivileges->Privileges[15].Luid = EzLookupPrivilege(SE_BACKUP_NAME);
	tokenPrivileges->Privileges[16].Luid = EzLookupPrivilege(SE_RESTORE_NAME);
	tokenPrivileges->Privileges[17].Luid = EzLookupPrivilege(SE_SHUTDOWN_NAME);
	tokenPrivileges->Privileges[18].Luid = EzLookupPrivilege(SE_DEBUG_NAME);
	tokenPrivileges->Privileges[19].Luid = EzLookupPrivilege(SE_AUDIT_NAME);
	tokenPrivileges->Privileges[20].Luid = EzLookupPrivilege(SE_SYSTEM_ENVIRONMENT_NAME);
	tokenPrivileges->Privileges[21].Luid = EzLookupPrivilege(SE_CHANGE_NOTIFY_NAME);
	tokenPrivileges->Privileges[22].Luid = EzLookupPrivilege(SE_REMOTE_SHUTDOWN_NAME);
	tokenPrivileges->Privileges[23].Luid = EzLookupPrivilege(SE_UNDOCK_NAME);
	tokenPrivileges->Privileges[24].Luid = EzLookupPrivilege(SE_SYNC_AGENT_NAME);
	tokenPrivileges->Privileges[25].Luid = EzLookupPrivilege(SE_ENABLE_DELEGATION_NAME);
	tokenPrivileges->Privileges[26].Luid = EzLookupPrivilege(SE_MANAGE_VOLUME_NAME);
	tokenPrivileges->Privileges[27].Luid = EzLookupPrivilege(SE_IMPERSONATE_NAME);
	tokenPrivileges->Privileges[28].Luid = EzLookupPrivilege(SE_CREATE_GLOBAL_NAME);
	tokenPrivileges->Privileges[29].Luid = EzLookupPrivilege(SE_TRUSTED_CREDMAN_ACCESS_NAME);
	tokenPrivileges->Privileges[30].Luid = EzLookupPrivilege(SE_RELABEL_NAME);
	tokenPrivileges->Privileges[31].Luid = EzLookupPrivilege(SE_INC_WORKING_SET_NAME);
	tokenPrivileges->Privileges[32].Luid = EzLookupPrivilege(SE_TIME_ZONE_NAME);
	tokenPrivileges->Privileges[33].Luid = EzLookupPrivilege(SE_CREATE_SYMBOLIC_LINK_NAME);
	tokenPrivileges->Privileges[34].Luid = EzLookupPrivilege(SE_DELEGATE_SESSION_USER_IMPERSONATE_NAME);

	TOKEN_OWNER tokenOwner = { };
	tokenOwner.Owner = administratorsGroupSid;

	TOKEN_PRIMARY_GROUP tokenPrimaryGroup = { };
	tokenPrimaryGroup.PrimaryGroup = administratorsGroupSid;

	PTOKEN_DEFAULT_DACL tokenDefaultDacl = reinterpret_cast<PTOKEN_DEFAULT_DACL>(LocalAlloc(LPTR, sizeof(PTOKEN_DEFAULT_DACL)));
	tokenDefaultDacl->DefaultDacl = NULL;

	PTOKEN_SOURCE tokenSource = reinterpret_cast<PTOKEN_SOURCE>(LocalAlloc(LPTR, sizeof(TOKEN_SOURCE)));
	tokenSource->SourceIdentifier = { };
	tokenSource->SourceIdentifier.HighPart = 69;
	tokenSource->SourceIdentifier.LowPart = 420;
	memcpy(tokenSource->SourceName, "MYSTERY", TOKEN_SOURCE_LENGTH);

	HANDLE token = 0;
	nt = NtCreateToken(&token, desiredAccess, &objectAttributes, tokenType, &authenticationID, &expirationTime, &tokenUser, tokenGroups, tokenPrivileges, &tokenOwner, &tokenPrimaryGroup, tokenDefaultDacl, tokenSource);
	if (FAILED(nt)) {
		throw EzError::FromNT(nt, __FILE__, __LINE__);
	}

	DWORD activeConsoleSessionId = WTSGetActiveConsoleSessionId();
	if (activeConsoleSessionId != 0xFFFFFFFF) {
		EzSetTokenSessionId(token, activeConsoleSessionId);
	}

	EzSetTokenUIAccess(token, TRUE);

	EzSetTokenVirtualizationAllowed(token, TRUE);

	EzSetTokenMandatoryPolicy(token, TOKEN_MANDATORY_POLICY_OFF);

	// Cleanup and return
	EzStopImpersonating();

	LocalFree(systemUserSid);
	LocalFree(administratorsGroupSid);
	LocalFree(authenticatedUsersGroupSid);
	LocalFree(everyoneGroupSid);
	LocalFree(systemIntegrityLevelSid);
	LocalFree(trustedInstallerUserSid);

	LocalFree(tokenGroups);
	LocalFree(tokenPrivileges);
	LocalFree(tokenDefaultDacl);
	LocalFree(tokenSource);

	return token;
}
BOOL EzIsGodToken(HANDLE token) {
	TOKEN_SOURCE tokenSource = EzGetTokenSource(token);
	return tokenSource.SourceIdentifier.HighPart == 69 && tokenSource.SourceIdentifier.LowPart == 420 && lstrcmpA(tokenSource.SourceName, "MYSTERY") == 0;
}