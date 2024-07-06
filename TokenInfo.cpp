#include "TokenInfo.h"
#include "Helper.h"

#include <sddl.h>
#include <iostream>
#include <iomanip>
#pragma comment(lib, "Advapi32.lib")

void* GetTokenInfo(HANDLE token, TOKEN_INFORMATION_CLASS desiredInfo) {
	DWORD length = 0;
	GetTokenInformation(token, desiredInfo, nullptr, 0, &length);
	if (length == 0) {
		ThrowSysError();
	}

	void* output = malloc(length);
	if (!GetTokenInformation(token, desiredInfo, output, length, &length)) {
		ThrowSysError();
	}

	return output;
}

void FormatHex(BYTE* value, DWORD length) {
	std::wcout << L"0x" << std::hex << std::uppercase << std::setw(2) << std::setfill(L'0');
	for (DWORD i = length - 1; i != 0xFFFFFFFF; i--)
	{
		std::wcout << value[i];
	}
	std::wcout << std::dec;
}
void FormatBinary(BYTE* value, DWORD length) {
	for (DWORD i = 0; i < length; i++)
	{
		if (i != 0) { std::wcout << L" "; }
		if ((value[i] & 128) == 0) { std::wcout << L"0"; }
		else { std::wcout << L"1"; }
		if ((value[i] & 64) == 0) { std::wcout << L"0"; }
		else { std::wcout << L"1"; }
		if ((value[i] & 32) == 0) { std::wcout << L"0"; }
		else { std::wcout << L"1"; }
		if ((value[i] & 16) == 0) { std::wcout << L"0"; }
		else { std::wcout << L"1"; }
		if ((value[i] & 8) == 0) { std::wcout << L"0"; }
		else { std::wcout << L"1"; }
		if ((value[i] & 4) == 0) { std::wcout << L"0"; }
		else { std::wcout << L"1"; }
		if ((value[i] & 2) == 0) { std::wcout << L"0"; }
		else { std::wcout << L"1"; }
		if ((value[i] & 1) == 0) { std::wcout << L"0"; }
		else { std::wcout << L"1"; }
	}
}
void FormatBool(DWORD value) {
	if (value == 0) {
		std::wcout << L"False";
	}
	else {
		std::wcout << L"True";
	}
}
void FormatSID(PSID value) {
	TCHAR name[256];
	TCHAR domain[256];
	DWORD nameLen = sizeof(name) / sizeof(TCHAR);
	DWORD domainLen = sizeof(domain) / sizeof(TCHAR);
	SID_NAME_USE sidType;
	BOOL valid = LookupAccountSid(NULL, value, name, &nameLen, domain, &domainLen, &sidType);

	if (valid) {
		if (domainLen != 0)
		{
			std::wcout << domain << L"/";
		}
		std::wcout << name;
	}
	else {
		sidType = SidTypeInvalid;
	}

	switch (sidType) {
	case SidTypeUser:
		std::wcout << L" (User)";
		break;
	case SidTypeGroup:
		std::wcout << L" (Group)";
		break;
	case SidTypeDomain:
		std::wcout << L" (Domain)";
		break;
	case SidTypeAlias:
		std::wcout << L" (Alias)";
		break;
	case SidTypeWellKnownGroup:
		std::wcout << L" (Well Known Group)";
		break;
	case SidTypeDeletedAccount:
		std::wcout << L" (Deleted Account)";
		break;
	case SidTypeComputer:
		std::wcout << L" (Computer)";
		break;
	case SidTypeLabel:
		std::wcout << L" (Label)";
		break;
	case SidTypeLogonSession:
		std::wcout << L" (Logon Session)";
		break;
	case SidTypeInvalid:
	case SidTypeUnknown:
	default:
		if (value == 0) { std::wcout << L"Null SID"; }
		else { std::wcout << L"Invalid/Unknown SID"; }
		break;
	}
}
void FormatLUID(LUID value) {
	WCHAR privilegeName[256];
	DWORD nameLen = sizeof(privilegeName) / sizeof(privilegeName[0]);

	if (LookupPrivilegeName(NULL, &value, privilegeName, &nameLen)) {
		std::wcout << privilegeName;
	}
	else {
		FormatHex(reinterpret_cast<BYTE*>(&value), 8);
	}
}

void PrintTokenUser(HANDLE token) {
	TOKEN_USER* tokenUser = reinterpret_cast<TOKEN_USER*>(GetTokenInfo(token, TokenUser));

	std::wcout << L"Token User:" << std::endl;
	std::wcout << "    SID: "; FormatSID(tokenUser->User.Sid); std::wcout << std::endl;
	std::wcout << "    Attributes: "; FormatBinary(reinterpret_cast<BYTE*>(&tokenUser->User.Attributes), 4); std::wcout << std::endl;

	free(tokenUser);
}
void PrintTokenGroups(HANDLE token) {
	TOKEN_GROUPS* tokenGroups = reinterpret_cast<TOKEN_GROUPS*>(GetTokenInfo(token, TokenGroups));

	std::wcout << L"Token Groups [" << tokenGroups->GroupCount << L"]:";
	if (tokenGroups->GroupCount == 0) {
		std::wcout << L" None";
	}
	std::wcout << std::endl;
	for (DWORD i = 0; i < tokenGroups->GroupCount; i++)
	{
		if (i != 0) { std::wcout << std::endl; }
		std::wcout << "    SID: "; FormatSID(tokenGroups->Groups[i].Sid); std::wcout << std::endl;
		std::wcout << "    Attributes: "; FormatBinary(reinterpret_cast<BYTE*>(&tokenGroups->Groups[i].Attributes), 4); std::wcout << std::endl;
	}

	free(tokenGroups);
}
void PrintTokenPrivileges(HANDLE token) {
	TOKEN_PRIVILEGES* tokenPrivileges = reinterpret_cast<TOKEN_PRIVILEGES*>(GetTokenInfo(token, TokenPrivileges));

	std::wcout << L"Token Privileges [" << tokenPrivileges->PrivilegeCount << L"]:";
	if (tokenPrivileges->PrivilegeCount == 0) {
		std::wcout << L" None";
	}
	std::wcout << std::endl;
	for (DWORD i = 0; i < tokenPrivileges->PrivilegeCount; i++)
	{
		if (i != 0) { std::wcout << std::endl; }
		std::wcout << "    LUID: "; FormatLUID(tokenPrivileges->Privileges[i].Luid); std::wcout << std::endl;
		std::wcout << "    Attributes: "; FormatBinary(reinterpret_cast<BYTE*>(&tokenPrivileges->Privileges[i].Attributes), 4); std::wcout << std::endl;
	}

	free(tokenPrivileges);
}
void PrintTokenOwner(HANDLE token) {
	TOKEN_OWNER* tokenOwner = reinterpret_cast<TOKEN_OWNER*>(GetTokenInfo(token, TokenOwner));

	std::wcout << L"Token Owner: "; FormatSID(tokenOwner->Owner); std::wcout << std::endl;

	free(tokenOwner);
}
void PrintTokenPrimaryGroup(HANDLE token) {
	TOKEN_PRIMARY_GROUP* tokenPrimaryGroup = reinterpret_cast<TOKEN_PRIMARY_GROUP*>(GetTokenInfo(token, TokenPrimaryGroup));

	std::wcout << L"Token Primary Group: "; FormatSID(tokenPrimaryGroup->PrimaryGroup); std::wcout << std::endl;

	free(tokenPrimaryGroup);
}
void PrintTokenDefaultDacl(HANDLE token) {
	TOKEN_DEFAULT_DACL* tokenDefaultDacl = reinterpret_cast<TOKEN_DEFAULT_DACL*>(GetTokenInfo(token, TokenDefaultDacl));

	std::wcout << L"Token Default DACL:" << std::endl;
	std::wcout << L"    ACL Revision: " << tokenDefaultDacl->DefaultDacl->AclRevision << std::endl;
	std::wcout << L"    Sbz1: "; FormatBinary(&tokenDefaultDacl->DefaultDacl->Sbz1, 1); std::wcout << std::endl;
	std::wcout << L"    ACL Size: " << tokenDefaultDacl->DefaultDacl->AclSize << std::endl;
	std::wcout << L"    ACE Count: " << tokenDefaultDacl->DefaultDacl->AceCount << std::endl;
	std::wcout << L"    Sbz2: "; FormatBinary(reinterpret_cast<BYTE*>(&tokenDefaultDacl->DefaultDacl->Sbz2), 2); std::wcout << std::endl;
	std::wcout << L"    ACE List: TODO" << std::endl;

	free(tokenDefaultDacl);
}
void PrintTokenSource(HANDLE token) {
	TOKEN_SOURCE* tokenSource = reinterpret_cast<TOKEN_SOURCE*>(GetTokenInfo(token, TokenSource));

	std::wcout << L"Token Source:" << std::endl;

	std::wcout << L"    Identifier: ";
	FormatLUID(tokenSource->SourceIdentifier);
	std::wcout << std::endl;

	std::wcout << L"    Name: " << tokenSource->SourceName;
	std::wcout << std::endl;

	free(tokenSource);
}
void PrintTokenType(HANDLE token) {
	TOKEN_TYPE* tokenType = reinterpret_cast<TOKEN_TYPE*>(GetTokenInfo(token, TokenType));

	std::wcout << L"Token Type: ";
	if (*tokenType == TokenPrimary) {
		std::wcout << L"Primary";
	}
	else if (*tokenType == TokenImpersonation) {
		std::wcout << L"Impersonation";
	}
	else {
		std::wcout << L"Invalid";
	}
	std::wcout << std::endl;

	free(tokenType);
}
void PrintTokenImpersonationLevel(HANDLE token) {
	TOKEN_TYPE* tokenType = reinterpret_cast<TOKEN_TYPE*>(GetTokenInfo(token, TokenType));
	if (*tokenType != TokenImpersonation) {
		std::wcout << L"Token Impersonation Level: N/A (Not An Impersonation Token)" << std::endl;
		free(tokenType);
		return;
	}
	else {
		free(tokenType);
	}

	SECURITY_IMPERSONATION_LEVEL* tokenImpersonationLevel = reinterpret_cast<SECURITY_IMPERSONATION_LEVEL*>(GetTokenInfo(token, TokenImpersonationLevel));

	std::wcout << L"Token Impersonation Level: ";
	if (*tokenImpersonationLevel == SecurityAnonymous) {
		std::wcout << L"Anonymous";
	}
	else if (*tokenImpersonationLevel == SecurityIdentification) {
		std::wcout << L"Identification";
	}
	else if (*tokenImpersonationLevel == SecurityImpersonation) {
		std::wcout << L"Impersonation";
	}
	else if (*tokenImpersonationLevel == SecurityDelegation) {
		std::wcout << L"Delegation";
	}
	else {
		std::wcout << L"Invalid";
	}
	std::wcout << std::endl;

	free(tokenImpersonationLevel);
}
void PrintTokenStatistics(HANDLE token) {
	TOKEN_STATISTICS* tokenStatistics = reinterpret_cast<TOKEN_STATISTICS*>(GetTokenInfo(token, TokenStatistics));

	std::wcout << L"Token Statistics:" << std::endl;
	std::wcout << L"    Token ID: ";
	FormatLUID(tokenStatistics->TokenId);
	std::wcout << std::endl;
	std::wcout << L"    Authentication ID: ";
	FormatLUID(tokenStatistics->AuthenticationId);
	std::wcout << std::endl;
	std::wcout << L"    Expiration Time (Unsupported): " << tokenStatistics->ExpirationTime.QuadPart << std::endl;
	std::wcout << L"    Token Type: ";
	if (tokenStatistics->TokenType == TokenPrimary) {
		std::wcout << L"Primary";
	}
	else if (tokenStatistics->TokenType == TokenImpersonation) {
		std::wcout << L"Impersonation";
	}
	else {
		std::wcout << L"Invalid";
	}
	std::wcout << std::endl;
	if (tokenStatistics->TokenType != TokenImpersonation) {
		std::wcout << L"    Impersonation Level: N/A (Not An Impersonation Token)" << std::endl;
	}
	else {
		std::wcout << L"    Impersonation Level: ";
		if (tokenStatistics->ImpersonationLevel == SecurityAnonymous) {
			std::wcout << L"Anonymous";
		}
		else if (tokenStatistics->ImpersonationLevel == SecurityIdentification) {
			std::wcout << L"Identification";
		}
		else if (tokenStatistics->ImpersonationLevel == SecurityImpersonation) {
			std::wcout << L"Impersonation";
		}
		else if (tokenStatistics->ImpersonationLevel == SecurityDelegation) {
			std::wcout << L"Delegation";
		}
		else {
			std::wcout << L"Invalid";
		}
		std::wcout << std::endl;
	}
	std::wcout << L"    Dynamic Charged: " << tokenStatistics->DynamicCharged << std::endl;
	std::wcout << L"    Dynamic Available: " << tokenStatistics->DynamicAvailable << std::endl;
	std::wcout << L"    Group Count: " << tokenStatistics->GroupCount << std::endl;
	std::wcout << L"    Privilege Count: " << tokenStatistics->PrivilegeCount << std::endl;
	std::wcout << L"    Modified ID: ";
	FormatLUID(tokenStatistics->ModifiedId);
	std::wcout << std::endl;

	free(tokenStatistics);
}
void PrintTokenRestrictedSids(HANDLE token) {
	TOKEN_GROUPS* tokenRestrictedSids = reinterpret_cast<TOKEN_GROUPS*>(GetTokenInfo(token, TokenRestrictedSids));

	std::wcout << L"Token Restricted SIDs [" << tokenRestrictedSids->GroupCount << L"]:";
	if (tokenRestrictedSids->GroupCount == 0) {
		std::wcout << L" None";
	}
	std::wcout << std::endl;

	for (DWORD i = 0; i < tokenRestrictedSids->GroupCount; i++)
	{
		if (i != 0) {
			std::wcout << std::endl;
		}

		std::wcout << L"    SID: ";
		FormatSID(tokenRestrictedSids->Groups[i].Sid);
		std::wcout << std::endl;

		std::wcout << L"    Attributes: ";
		FormatBinary(reinterpret_cast<BYTE*>(&tokenRestrictedSids->Groups[i].Attributes), 4);
		std::wcout << std::endl;
	}

	free(tokenRestrictedSids);
}
void PrintTokenSessionID(HANDLE token) {
	DWORD* tokenSessionId = reinterpret_cast<DWORD*>(GetTokenInfo(token, TokenSessionId));

	std::wcout << L"Token Session ID: " << *tokenSessionId << std::endl;

	free(tokenSessionId);
}
void PrintTokenGroupsAndPrivileges(HANDLE token) {
	TOKEN_GROUPS_AND_PRIVILEGES* tokenGroupsAndPrivileges = reinterpret_cast<TOKEN_GROUPS_AND_PRIVILEGES*>(GetTokenInfo(token, TokenGroupsAndPrivileges));

	std::wcout << L"Token Groups And Privileges: TODO" << std::endl;

	free(tokenGroupsAndPrivileges);
}
void PrintTokenSessionReference(HANDLE token) {
	// TODO
}
void PrintTokenSandBoxInert(HANDLE token) {
	DWORD* tokenSandBoxInert = reinterpret_cast<DWORD*>(GetTokenInfo(token, TokenSandBoxInert));

	std::wcout << L"Token Sand Box Inert: ";
	FormatBool(*tokenSandBoxInert);
	std::wcout << std::endl;

	free(tokenSandBoxInert);
}
void PrintTokenAuditPolicy(HANDLE token) {
	// TODO
}
void PrintTokenOrigin(HANDLE token) {
	TOKEN_ORIGIN* tokenOrigin = reinterpret_cast<TOKEN_ORIGIN*>(GetTokenInfo(token, TokenOrigin));

	std::wcout << L"Token Origin: ";
	FormatLUID(tokenOrigin->OriginatingLogonSession);
	std::wcout << std::endl;

	free(tokenOrigin);
}
void PrintTokenElevationType(HANDLE token) {
	TOKEN_ELEVATION_TYPE* tokenElevationType = reinterpret_cast<TOKEN_ELEVATION_TYPE*>(GetTokenInfo(token, TokenElevationType));

	std::wcout << L"Token Elevation Type: ";
	if (*tokenElevationType == TokenElevationTypeDefault) {
		std::wcout << L"Default";
	}
	else if (*tokenElevationType == TokenElevationTypeFull) {
		std::wcout << L"Full";
	}
	else if (*tokenElevationType == TokenElevationTypeLimited) {
		std::wcout << L"Limited";
	}
	else {
		std::wcout << L"Invalid";
	}
	std::wcout << std::endl;

	free(tokenElevationType);
}
void PrintTokenLinkedToken(HANDLE token) {
	TOKEN_LINKED_TOKEN* tokenLinkedToken = reinterpret_cast<TOKEN_LINKED_TOKEN*>(GetTokenInfo(token, TokenLinkedToken));

	std::wcout << L"Token Linked Token: ";
	FormatHex(reinterpret_cast<BYTE*>(&tokenLinkedToken->LinkedToken), 8);
	std::wcout << std::endl;

	free(tokenLinkedToken);
}
void PrintTokenElevation(HANDLE token) {
	TOKEN_ELEVATION* tokenElevation = reinterpret_cast<TOKEN_ELEVATION*>(GetTokenInfo(token, TokenElevation));

	std::wcout << L"Token Is Elevated: ";
	FormatBool(tokenElevation->TokenIsElevated);
	std::wcout << std::endl;

	free(tokenElevation);
}
void PrintTokenHasRestrictions(HANDLE token) {
	DWORD* tokenHasRestrictions = reinterpret_cast<DWORD*>(GetTokenInfo(token, TokenHasRestrictions));

	std::wcout << L"Token Has Restrictions: ";
	FormatBool(*tokenHasRestrictions);
	std::wcout << std::endl;

	free(tokenHasRestrictions);
}
void PrintTokenAccessInformation(HANDLE token) {
	TOKEN_ACCESS_INFORMATION* tokenAccessInformation = reinterpret_cast<TOKEN_ACCESS_INFORMATION*>(GetTokenInfo(token, TokenAccessInformation));

	std::wcout << L"Token Access Information: TODO" << std::endl;

	free(tokenAccessInformation);
}
void PrintTokenVirtualizationAllowed(HANDLE token) {
	DWORD* tokenVirtualizationAllowed = reinterpret_cast<DWORD*>(GetTokenInfo(token, TokenVirtualizationAllowed));

	std::wcout << L"Token Virtualization Allowed: ";
	FormatBool(*tokenVirtualizationAllowed);
	std::wcout << std::endl;

	free(tokenVirtualizationAllowed);
}
void PrintTokenVirtualizationEnabled(HANDLE token) {
	DWORD* tokenVirtualizationEnabled = reinterpret_cast<DWORD*>(GetTokenInfo(token, TokenVirtualizationEnabled));

	std::wcout << L"Token Virtualization Enabled: ";
	FormatBool(*tokenVirtualizationEnabled);
	std::wcout << std::endl;

	free(tokenVirtualizationEnabled);
}
void PrintTokenIntegrityLevel(HANDLE token) {
	TOKEN_MANDATORY_LABEL* tokenIntegrityLevel = reinterpret_cast<TOKEN_MANDATORY_LABEL*>(GetTokenInfo(token, TokenIntegrityLevel));

	std::wcout << L"Token Integrity Level:" << std::endl;

	std::wcout << "    SID: ";
	FormatSID(tokenIntegrityLevel->Label.Sid);
	std::wcout << std::endl;

	std::wcout << "    Attributes: ";
	FormatBinary(reinterpret_cast<BYTE*>(&tokenIntegrityLevel->Label.Attributes), 4);
	std::wcout << std::endl;

	free(tokenIntegrityLevel);
}
void PrintTokenUIAccess(HANDLE token) {
	DWORD* tokenUIAccess = reinterpret_cast<DWORD*>(GetTokenInfo(token, TokenUIAccess));

	std::wcout << L"Token UI Access: ";
	FormatBool(*tokenUIAccess);
	std::wcout << std::endl;

	free(tokenUIAccess);
}
void PrintTokenMandatoryPolicy(HANDLE token) {
	TOKEN_MANDATORY_POLICY* tokenMandatoryPolicy = reinterpret_cast<TOKEN_MANDATORY_POLICY*>(GetTokenInfo(token, TokenMandatoryPolicy));

	std::wcout << L"Token Mandatory Policy: " << tokenMandatoryPolicy->Policy << std::endl;

	free(tokenMandatoryPolicy);
}
void PrintTokenLogonSid(HANDLE token) {
	TOKEN_GROUPS* tokenLogonSid = reinterpret_cast<TOKEN_GROUPS*>(GetTokenInfo(token, TokenLogonSid));

	std::wcout << L"Token Logon Sids [" << tokenLogonSid->GroupCount << L"]:";
	if (tokenLogonSid->GroupCount == 0) {
		std::wcout << L" None";
	}
	std::wcout << std::endl;

	for (DWORD i = 0; i < tokenLogonSid->GroupCount; i++)
	{
		if (i != 0) {
			std::wcout << std::endl;
		}

		std::wcout << "    SID: ";
		FormatSID(tokenLogonSid->Groups[i].Sid);
		std::wcout << std::endl;

		std::wcout << "    Attributes: ";
		FormatBinary(reinterpret_cast<BYTE*>(&tokenLogonSid->Groups[i].Attributes), 4);
		std::wcout << std::endl;
	}

	free(tokenLogonSid);
}
void PrintTokenIsAppContainer(HANDLE token) {
	DWORD* tokenIsAppContainer = reinterpret_cast<DWORD*>(GetTokenInfo(token, TokenIsAppContainer));

	std::wcout << L"Token Is App Container: ";
	FormatBool(*tokenIsAppContainer);
	std::wcout << std::endl;

	free(tokenIsAppContainer);
}
void PrintTokenCapabilities(HANDLE token) {
	TOKEN_GROUPS* tokenCapabilities = reinterpret_cast<TOKEN_GROUPS*>(GetTokenInfo(token, TokenCapabilities));

	std::wcout << L"Token Capabilities [" << tokenCapabilities->GroupCount << L"]:";
	if (tokenCapabilities->GroupCount == 0) {
		std::wcout << L" None";
	}
	std::wcout << std::endl;

	for (DWORD i = 0; i < tokenCapabilities->GroupCount; i++)
	{
		if (i != 0) {
			std::wcout << std::endl;
		}

		std::wcout << "    SID: ";
		FormatSID(tokenCapabilities->Groups[i].Sid);
		std::wcout << std::endl;

		std::wcout << "    Attributes: ";
		FormatBinary(reinterpret_cast<BYTE*>(&tokenCapabilities->Groups[i].Attributes), 4);
		std::wcout << std::endl;
	}

	free(tokenCapabilities);
}
void PrintTokenAppContainerSid(HANDLE token) {
	TOKEN_APPCONTAINER_INFORMATION* tokenAppContainerSid = reinterpret_cast<TOKEN_APPCONTAINER_INFORMATION*>(GetTokenInfo(token, TokenAppContainerSid));

	std::wcout << L"Token App Container SID: ";
	FormatSID(tokenAppContainerSid->TokenAppContainer);
	std::wcout << std::endl;

	free(tokenAppContainerSid);
}
void PrintTokenAppContainerNumber(HANDLE token) {
	DWORD* tokenAppContainerNumber = reinterpret_cast<DWORD*>(GetTokenInfo(token, TokenAppContainerNumber));

	std::wcout << L"Token App Container Number: " << *tokenAppContainerNumber << std::endl;

	free(tokenAppContainerNumber);
}
void PrintTokenUserClaimAttributes(HANDLE token) {
	CLAIM_SECURITY_ATTRIBUTES_INFORMATION* tokenUserClaimAttributes = reinterpret_cast<CLAIM_SECURITY_ATTRIBUTES_INFORMATION*>(GetTokenInfo(token, TokenUserClaimAttributes));

	std::wcout << L"Token User Claim Attributes:" << std::endl;
	std::wcout << L"    Version: " << tokenUserClaimAttributes->Version << std::endl;
	std::wcout << L"    Reserved: " << tokenUserClaimAttributes->Reserved << std::endl;
	std::wcout << L"    Attributes [" << tokenUserClaimAttributes->AttributeCount << "]:";
	if (tokenUserClaimAttributes->AttributeCount == 0) {
		std::wcout << " None";
	}
	std::wcout << std::endl;
	for (DWORD i = 0; i < tokenUserClaimAttributes->AttributeCount; i++)
	{
		CLAIM_SECURITY_ATTRIBUTE_V1 attribute = tokenUserClaimAttributes->Attribute.pAttributeV1[i];
		std::wcout << L"        Name: " << attribute.Name << std::endl;
		std::wcout << L"        Value Type: " << attribute.ValueType << std::endl;
		std::wcout << L"        Reserved: " << attribute.Reserved << std::endl;
		std::wcout << L"        Flags: "; FormatBinary(reinterpret_cast<BYTE*>(&attribute.Flags), 4); std::wcout << std::endl;
		std::wcout << L"        Value Count: " << attribute.ValueCount << std::endl;
		std::wcout << L"        Values:" << std::endl;
		for (DWORD j = 0; j < attribute.ValueCount; j++)
		{
			std::wcout << L"            TODO" << std::endl;
		}
		std::wcout << std::endl;
	}

	free(tokenUserClaimAttributes);
}
void PrintTokenDeviceClaimAttributes(HANDLE token) {
	CLAIM_SECURITY_ATTRIBUTES_INFORMATION* tokenDeviceClaimAttributes = reinterpret_cast<CLAIM_SECURITY_ATTRIBUTES_INFORMATION*>(GetTokenInfo(token, TokenDeviceClaimAttributes));

	std::wcout << L"Token Device Claim Attributes: TODO" << std::endl;

	free(tokenDeviceClaimAttributes);
}
void PrintTokenRestrictedUserClaimAttributes(HANDLE token) {
	// TODO
}
void PrintTokenRestrictedDeviceClaimAttributes(HANDLE token) {
	// TODO
}
void PrintTokenDeviceGroups(HANDLE token) {
	TOKEN_GROUPS* tokenDeviceGroups = reinterpret_cast<TOKEN_GROUPS*>(GetTokenInfo(token, TokenDeviceGroups));

	std::wcout << L"Token Device Groups [" << tokenDeviceGroups->GroupCount << L"]:";
	if (tokenDeviceGroups->GroupCount == 0) {
		std::wcout << L" None";
	}
	std::wcout << std::endl;

	for (DWORD i = 0; i < tokenDeviceGroups->GroupCount; i++)
	{
		if (i != 0) {
			std::wcout << std::endl;
		}

		std::wcout << "    SID: ";
		FormatSID(tokenDeviceGroups->Groups[i].Sid);
		std::wcout << std::endl;

		std::wcout << "    Attributes: ";
		FormatBinary(reinterpret_cast<BYTE*>(&tokenDeviceGroups->Groups[i].Attributes), 4);
		std::wcout << std::endl;
	}

	free(tokenDeviceGroups);
}
void PrintTokenRestrictedDeviceGroups(HANDLE token) {
	std::wcout << L"Token Restricted Device Groups: TODO Seems to be broken at a system level." << std::endl;
	return;

	TOKEN_GROUPS* tokenRestrictedDeviceGroups = reinterpret_cast<TOKEN_GROUPS*>(GetTokenInfo(token, TokenRestrictedDeviceGroups));

	std::wcout << L"Token Restricted Device Groups (" << tokenRestrictedDeviceGroups->GroupCount << L"):" << std::endl;

	for (DWORD i = 0; i < tokenRestrictedDeviceGroups->GroupCount; i++)
	{
		if (i != 0) {
			std::wcout << std::endl;
		}

		std::wcout << "    SID: ";
		FormatSID(tokenRestrictedDeviceGroups->Groups[i].Sid);
		std::wcout << std::endl;

		std::wcout << "    Attributes: ";
		FormatBinary(reinterpret_cast<BYTE*>(&tokenRestrictedDeviceGroups->Groups[i].Attributes), 4);
		std::wcout << std::endl;
	}

	free(tokenRestrictedDeviceGroups);
}
void PrintTokenSecurityAttributes(HANDLE token) {
	// TODO
}
void PrintTokenIsRestricted(HANDLE token) {
	// TODO
}
void TokenProcessTrustLevel(HANDLE token) {
	// TODO
}
void TokenPrivateNameSpace(HANDLE token) {
	// TODO
}
void TokenSingletonAttributes(HANDLE token) {
	// TODO
}
void PrintTokenBnoIsolation(HANDLE token) {
	// TODO 
}
void PrintTokenChildProcessFlags(HANDLE token) {
	// TODO 
}
void PrintTokenIsLessPrivilegedAppContainer(HANDLE token) {
	// TODO 
}
void PrintTokenIsSandboxed(HANDLE token) {
	// TODO 
}
void PrintTokenIsAppSilo(HANDLE token) {
	// TODO 
}
void PrintTokenLoggingInformation(HANDLE token) {
	// TODO 
}
void PrintMaxTokenInfoClass(HANDLE token) {
	// TODO 
}

void PrintTokenInfo(HANDLE token) {
	std::wcout << L"Token: "; FormatHex(reinterpret_cast<BYTE*>(&token), 8); std::wcout << std::endl << std::endl;
	PrintTokenUser(token); std::wcout << std::endl;
	PrintTokenGroups(token); std::wcout << std::endl;
	PrintTokenPrivileges(token); std::wcout << std::endl;
	PrintTokenOwner(token); std::wcout << std::endl;
	PrintTokenPrimaryGroup(token); 	std::wcout << std::endl;
	PrintTokenDefaultDacl(token); std::wcout << std::endl;
	PrintTokenSource(token); std::wcout << std::endl;
	PrintTokenType(token); std::wcout << std::endl;
	PrintTokenImpersonationLevel(token); std::wcout << std::endl;
	PrintTokenStatistics(token); std::wcout << std::endl;
	PrintTokenRestrictedSids(token); std::wcout << std::endl;
	PrintTokenSessionID(token); std::wcout << std::endl;
	PrintTokenGroupsAndPrivileges(token); std::wcout << std::endl;
	PrintTokenSandBoxInert(token); std::wcout << std::endl;
	PrintTokenOrigin(token); std::wcout << std::endl;
	PrintTokenElevationType(token); std::wcout << std::endl;
	PrintTokenLinkedToken(token); std::wcout << std::endl;
	PrintTokenElevation(token); std::wcout << std::endl;
	PrintTokenHasRestrictions(token); std::wcout << std::endl;
	PrintTokenAccessInformation(token); std::wcout << std::endl;
	PrintTokenVirtualizationAllowed(token); std::wcout << std::endl;
	PrintTokenVirtualizationEnabled(token); std::wcout << std::endl;
	PrintTokenIntegrityLevel(token); std::wcout << std::endl;
	PrintTokenUIAccess(token); std::wcout << std::endl;
	PrintTokenMandatoryPolicy(token); std::wcout << std::endl;
	PrintTokenLogonSid(token); std::wcout << std::endl;
	PrintTokenIsAppContainer(token); std::wcout << std::endl;
	PrintTokenCapabilities(token); std::wcout << std::endl;
	PrintTokenAppContainerSid(token); std::wcout << std::endl;
	PrintTokenAppContainerNumber(token); std::wcout << std::endl;
	PrintTokenUserClaimAttributes(token); std::wcout << std::endl;
	PrintTokenDeviceClaimAttributes(token); std::wcout << std::endl;
	PrintTokenDeviceGroups(token); std::wcout << std::endl;
	PrintTokenRestrictedDeviceGroups(token); std::wcout << std::endl;
}