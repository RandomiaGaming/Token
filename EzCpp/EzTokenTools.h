// Approved 11/14/2024

#pragma once
#include <Windows.h>

// Working with the current token
HANDLE EzOpenCurrentToken();
HANDLE EzDuplicateCurrentToken();

// Impersonating tokens
void EzImpersonate(HANDLE token);
void EzStopImpersonating();
void EzImpersonateWinLogon();
void EzImpersonateLsass();

// Enabling/disabling token privileges
LUID EzLookupPrivilege(LPCWSTR privilege);
void EzEnableAllPrivileges(HANDLE token);
void EzDisableAllPrivileges(HANDLE token);
void EzEnablePrivilege(HANDLE token, LUID privilege);
void EzDisablePrivilege(HANDLE token, LUID privilege);
BOOL EzTokenHasPrivilege(HANDLE token, LUID privilege);

// Starting processes with tokens
PROCESS_INFORMATION EzLaunchAsToken(HANDLE token, LPCWSTR exePath);
PROCESS_INFORMATION EzLaunchAsToken(HANDLE token);
PROCESS_INFORMATION EzLaunchAsUser(HANDLE token, LPCWSTR exePath);
PROCESS_INFORMATION EzLaunchAsUser(HANDLE token);
BOOL EzLaunchWithUAC(LPCWSTR exePath);
BOOL EzLaunchWithUAC();

// Token privilege escalation
void EzGrantUIAccessToToken(HANDLE token);
void EzMakeTokenInteractive(HANDLE token);
void EzGiveTokenSystemIntegrity(HANDLE token);
void EzStealCreateTokenPermission(HANDLE token);
HANDLE EzCreateGodToken();
BOOL EzIsGodToken(HANDLE token);