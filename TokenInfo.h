#pragma once
#include <Windows.h>

void PrintTokenUser(HANDLE token);
void PrintTokenGroups(HANDLE token);
void PrintTokenPrivileges(HANDLE token);
void PrintTokenOwner(HANDLE token);
void PrintTokenPrimaryGroup(HANDLE token);
void PrintTokenDefaultDacl(HANDLE token);
void PrintTokenSource(HANDLE token);
void PrintTokenType(HANDLE token);
void PrintTokenImpersonationLevel(HANDLE token);
void PrintTokenStatistics(HANDLE token);
void PrintTokenRestrictedSids(HANDLE token);
void PrintTokenSessionID(HANDLE token);
void PrintTokenGroupsAndPrivileges(HANDLE token);
void PrintTokenSandBoxInert(HANDLE token);
void PrintTokenOrigin(HANDLE token);
void PrintTokenElevationType(HANDLE token);
void PrintTokenLinkedToken(HANDLE token);
void PrintTokenElevation(HANDLE token);
void PrintTokenHasRestrictions(HANDLE token);
void PrintTokenAccessInformation(HANDLE token);
void PrintTokenVirtualizationAllowed(HANDLE token);
void PrintTokenVirtualizationEnabled(HANDLE token);
void PrintTokenIntegrityLevel(HANDLE token);
void PrintTokenUIAccess(HANDLE token);
void PrintTokenMandatoryPolicy(HANDLE token);
void PrintTokenLogonSid(HANDLE token);
void PrintTokenIsAppContainer(HANDLE token);
void PrintTokenCapabilities(HANDLE token);
void PrintTokenAppContainerSid(HANDLE token);
void PrintTokenAppContainerNumber(HANDLE token);
void PrintTokenUserClaimAttributes(HANDLE token);
void PrintTokenDeviceClaimAttributes(HANDLE token);
void PrintTokenDeviceGroups(HANDLE token);
void PrintTokenRestrictedDeviceGroups(HANDLE token);

void PrintTokenInfo(HANDLE token);