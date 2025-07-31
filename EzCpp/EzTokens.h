// Approved 11/14/2024

#pragma once
#include <Windows.h>
#include <ostream>

/*
Token Information Classes:

TokenUser
TokenGroups
TokenPrivileges
TokenOwner
TokenPrimaryGroup
TokenDefaultDacl
TokenSource
TokenType
TokenImpersonationLevel
TokenStatistics
TokenRestrictedSids
TokenSessionId
TokenGroupsAndPrivileges
TokenSessionReference
TokenSandBoxInert
TokenAuditPolicy
TokenOrigin
TokenElevationType
TokenLinkedToken
TokenElevation
TokenHasRestrictions
TokenAccessInformation
TokenVirtualizationAllowed
TokenVirtualizationEnabled
TokenIntegrityLevel
TokenUIAccess
TokenMandatoryPolicy
TokenLogonSid
TokenIsAppContainer
TokenCapabilities
TokenAppContainerSid
TokenAppContainerNumber
TokenUserClaimAttributes
TokenDeviceClaimAttributes
TokenRestrictedUserClaimAttributes
TokenRestrictedDeviceClaimAttributes
TokenDeviceGroups
TokenRestrictedDeviceGroups
TokenSecurityAttributes
TokenIsRestricted
TokenProcessTrustLevel
TokenPrivateNameSpace
TokenSingletonAttributes
TokenBnoIsolation
TokenChildProcessFlags
TokenIsLessPrivilegedAppContainer
TokenIsSandboxed
TokenIsAppSilo
TokenLoggingInformation
MaxTokenInfoClass

*/

/*
Token Information Types:

SID_AND_ATTRIBUTES					       = TokenUser
TOKEN_GROUPS*						       = TokenGroups
TOKEN_PRIVILEGES*					       = TokenPrivileges
PSID								       = TokenOwner
PSID								       = TokenPrimaryGroup
TOKEN_DEFAULT_DACL*					       = TokenDefaultDacl
TOKEN_SOURCE						       = TokenSource
TOKEN_TYPE							       = TokenType
SECURITY_IMPERSONATION_LEVEL		       = TokenImpersonationLevel
TOKEN_STATISTICS					       = TokenStatistics
TOKEN_GROUPS*						       = TokenRestrictedSids
DWORD								       = TokenSessionId
TOKEN_GROUPS_AND_PRIVILEGES			       = TokenGroupsAndPrivileges
void								       = TokenSessionReference
BOOL								       = TokenSandBoxInert
void								       = TokenAuditPolicy
LUID								       = TokenOrigin
TOKEN_ELEVATION_TYPE				       = TokenElevationType
HANDLE								       = TokenLinkedToken
BOOL								       = TokenElevation
BOOL								       = TokenHasRestrictions
TOKEN_ACCESS_INFORMATION			       = TokenAccessInformation
BOOL								       = TokenVirtualizationAllowed
BOOL								       = TokenVirtualizationEnabled
SID_AND_ATTRIBUTES					       = TokenIntegrityLevel
BOOL								       = TokenUIAccess
DWORD								       = TokenMandatoryPolicy
TOKEN_GROUPS*						       = TokenLogonSid
BOOL								       = TokenIsAppContainer
TOKEN_GROUPS*						       = TokenCapabilities
PSID								       = TokenAppContainerSid
DWORD								       = TokenAppContainerNumber
CLAIM_SECURITY_ATTRIBUTES_INFORMATION*     = TokenUserClaimAttributes
CLAIM_SECURITY_ATTRIBUTES_INFORMATION*     = TokenDeviceClaimAttributes
void								       = TokenRestrictedUserClaimAttributes
void								       = TokenRestrictedDeviceClaimAttributes
TOKEN_GROUPS*						       = TokenDeviceGroups
TOKEN_GROUPS*						       = TokenRestrictedDeviceGroups
CLAIM_SECURITY_ATTRIBUTES_INFORMATION*     = TokenSecurityAttributes
BOOL								       = TokenIsRestricted
PSID								       = TokenProcessTrustLevel
BOOL								       = TokenPrivateNameSpace
CLAIM_SECURITY_ATTRIBUTES_INFORMATION*     = TokenSingletonAttributes
TOKEN_BNO_ISOLATION_INFORMATION		       = TokenBnoIsolation
void								       = TokenChildProcessFlags
void								       = TokenIsLessPrivilegedAppContainer
BOOL								       = TokenIsSandboxed
void								       = TokenIsAppSilo
void								       = TokenLoggingInformation
void								       = MaxTokenInfoClass

*/



// Getting info about tokens
SID_AND_ATTRIBUTES EzGetTokenUser(HANDLE token);
TOKEN_GROUPS* EzGetTokenGroups(HANDLE token);
TOKEN_PRIVILEGES* EzGetTokenPrivileges(HANDLE token);
PSID EzGetTokenOwner(HANDLE token);
PSID EzGetTokenPrimaryGroup(HANDLE token);
TOKEN_DEFAULT_DACL* EzGetTokenDefaultDacl(HANDLE token);
TOKEN_SOURCE EzGetTokenSource(HANDLE token);
TOKEN_TYPE EzGetTokenType(HANDLE token);
SECURITY_IMPERSONATION_LEVEL EzGetTokenImpersonationLevel(HANDLE token);
TOKEN_STATISTICS EzGetTokenStatistics(HANDLE token);
TOKEN_GROUPS* EzGetTokenRestrictedSids(HANDLE token);
DWORD EzGetTokenSessionId(HANDLE token);
TOKEN_GROUPS_AND_PRIVILEGES EzGetTokenGroupsAndPrivileges(HANDLE token);
void EzGetTokenSessionReference(HANDLE token);
BOOL EzGetTokenSandBoxInert(HANDLE token);
void EzGetTokenAuditPolicy(HANDLE token);
LUID EzGetTokenOrigin(HANDLE token);
TOKEN_ELEVATION_TYPE EzGetTokenElevationType(HANDLE token);
HANDLE EzGetTokenLinkedToken(HANDLE token);
BOOL EzGetTokenElevation(HANDLE token);
BOOL EzGetTokenHasRestrictions(HANDLE token);
TOKEN_ACCESS_INFORMATION EzGetTokenAccessInformation(HANDLE token);
BOOL EzGetTokenVirtualizationAllowed(HANDLE token);
BOOL EzGetTokenVirtualizationEnabled(HANDLE token);
SID_AND_ATTRIBUTES EzGetTokenIntegrityLevel(HANDLE token);
BOOL EzGetTokenUIAccess(HANDLE token);
DWORD EzGetTokenMandatoryPolicy(HANDLE token);
TOKEN_GROUPS* EzGetTokenLogonSid(HANDLE token);
BOOL EzGetTokenIsAppContainer(HANDLE token);
TOKEN_GROUPS* EzGetTokenCapabilities(HANDLE token);
PSID EzGetTokenAppContainerSid(HANDLE token);
DWORD EzGetTokenAppContainerNumber(HANDLE token);
CLAIM_SECURITY_ATTRIBUTES_INFORMATION* EzGetTokenUserClaimAttributes(HANDLE token);
CLAIM_SECURITY_ATTRIBUTES_INFORMATION* EzGetTokenDeviceClaimAttributes(HANDLE token);
void EzGetTokenRestrictedUserClaimAttributes(HANDLE token);
void EzGetTokenRestrictedDeviceClaimAttributes(HANDLE token);
TOKEN_GROUPS* EzGetTokenDeviceGroups(HANDLE token);
TOKEN_GROUPS* EzGetTokenRestrictedDeviceGroups(HANDLE token);
CLAIM_SECURITY_ATTRIBUTES_INFORMATION* EzGetTokenSecurityAttributes(HANDLE token);
BOOL EzGetTokenIsRestricted(HANDLE token);
PSID EzGetTokenProcessTrustLevel(HANDLE token);
BOOL EzGetTokenPrivateNameSpace(HANDLE token);
CLAIM_SECURITY_ATTRIBUTES_INFORMATION* EzGetTokenSingletonAttributes(HANDLE token);
TOKEN_BNO_ISOLATION_INFORMATION EzGetTokenBnoIsolation(HANDLE token);
void EzGetTokenChildProcessFlags(HANDLE token);
void EzGetTokenIsLessPrivilegedAppContainer(HANDLE token);
BOOL EzGetTokenIsSandboxed(HANDLE token);
void EzGetTokenIsAppSilo(HANDLE token);
void EzGetTokenLoggingInformation(HANDLE token);
void EzGetMaxTokenInfoClass(HANDLE token);



// Setting info about tokens
void EzSetTokenUser(HANDLE token, SID_AND_ATTRIBUTES value);
void EzSetTokenGroups(HANDLE token, TOKEN_GROUPS* value);
void EzSetTokenPrivileges(HANDLE token, TOKEN_PRIVILEGES* value);
void EzSetTokenOwner(HANDLE token, PSID value);
void EzSetTokenPrimaryGroup(HANDLE token, PSID value);
void EzSetTokenDefaultDacl(HANDLE token, TOKEN_DEFAULT_DACL* value);
void EzSetTokenSource(HANDLE token, TOKEN_SOURCE value);
void EzSetTokenType(HANDLE token, TOKEN_TYPE value);
void EzSetTokenImpersonationLevel(HANDLE token, SECURITY_IMPERSONATION_LEVEL value);
void EzSetTokenStatistics(HANDLE token, TOKEN_STATISTICS value);
void EzSetTokenRestrictedSids(HANDLE token, TOKEN_GROUPS* value);
void EzSetTokenSessionId(HANDLE token, DWORD value);
void EzSetTokenGroupsAndPrivileges(HANDLE token, TOKEN_GROUPS_AND_PRIVILEGES value);
void EzSetTokenSessionReference(HANDLE token /* void value */);
void EzSetTokenSandBoxInert(HANDLE token, BOOL value);
void EzSetTokenAuditPolicy(HANDLE token /* void value */);
void EzSetTokenOrigin(HANDLE token, LUID value);
void EzSetTokenElevationType(HANDLE token, TOKEN_ELEVATION_TYPE value);
void EzSetTokenLinkedToken(HANDLE token, HANDLE value);
void EzSetTokenElevation(HANDLE token, BOOL value);
void EzSetTokenHasRestrictions(HANDLE token, BOOL value);
void EzSetTokenAccessInformation(HANDLE token, TOKEN_ACCESS_INFORMATION value);
void EzSetTokenVirtualizationAllowed(HANDLE token, BOOL value);
void EzSetTokenVirtualizationEnabled(HANDLE token, BOOL value);
void EzSetTokenIntegrityLevel(HANDLE token, SID_AND_ATTRIBUTES value);
void EzSetTokenUIAccess(HANDLE token, BOOL value);
void EzSetTokenMandatoryPolicy(HANDLE token, DWORD value);
void EzSetTokenLogonSid(HANDLE token, TOKEN_GROUPS* value);
void EzSetTokenIsAppContainer(HANDLE token, BOOL value);
void EzSetTokenCapabilities(HANDLE token, TOKEN_GROUPS* value);
void EzSetTokenAppContainerSid(HANDLE token, PSID value);
void EzSetTokenAppContainerNumber(HANDLE token, DWORD value);
void EzSetTokenUserClaimAttributes(HANDLE token, CLAIM_SECURITY_ATTRIBUTES_INFORMATION* value);
void EzSetTokenDeviceClaimAttributes(HANDLE token, CLAIM_SECURITY_ATTRIBUTES_INFORMATION* value);
void EzSetTokenRestrictedUserClaimAttributes(HANDLE token /* void value */);
void EzSetTokenRestrictedDeviceClaimAttributes(HANDLE token /* void value */);
void EzSetTokenDeviceGroups(HANDLE token, TOKEN_GROUPS* value);
void EzSetTokenRestrictedDeviceGroups(HANDLE token, TOKEN_GROUPS* value);
void EzSetTokenSecurityAttributes(HANDLE token, CLAIM_SECURITY_ATTRIBUTES_INFORMATION* value);
void EzSetTokenIsRestricted(HANDLE token, BOOL value);
void EzSetTokenProcessTrustLevel(HANDLE token, PSID value);
void EzSetTokenPrivateNameSpace(HANDLE token, BOOL value);
void EzSetTokenSingletonAttributes(HANDLE token, CLAIM_SECURITY_ATTRIBUTES_INFORMATION* value);
void EzSetTokenBnoIsolation(HANDLE token, TOKEN_BNO_ISOLATION_INFORMATION value);
void EzSetTokenChildProcessFlags(HANDLE token /* void value */);
void EzSetTokenIsLessPrivilegedAppContainer(HANDLE token /* void value */);
void EzSetTokenIsSandboxed(HANDLE token, BOOL value);
void EzSetTokenIsAppSilo(HANDLE token /* void value */);
void EzSetTokenLoggingInformation(HANDLE token /* void value */);
void EzSetMaxTokenInfoClass(HANDLE token /* void value */);



// Printing info about tokens to an Ansi stream
void EzPrintTokenUserA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenGroupsA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenPrivilegesA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenOwnerA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenPrimaryGroupA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenDefaultDaclA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenSourceA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenTypeA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenImpersonationLevelA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenStatisticsA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenRestrictedSidsA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenSessionIdA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenGroupsAndPrivilegesA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenSessionReferenceA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenSandBoxInertA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenAuditPolicyA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenOriginA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenElevationTypeA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenLinkedTokenA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenElevationA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenHasRestrictionsA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenAccessInformationA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenVirtualizationAllowedA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenVirtualizationEnabledA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenIntegrityLevelA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenUIAccessA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenMandatoryPolicyA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenLogonSidA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenIsAppContainerA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenCapabilitiesA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenAppContainerSidA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenAppContainerNumberA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenUserClaimAttributesA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenDeviceClaimAttributesA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenRestrictedUserClaimAttributesA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenRestrictedDeviceClaimAttributesA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenDeviceGroupsA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenRestrictedDeviceGroupsA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenSecurityAttributesA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenIsRestrictedA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenProcessTrustLevelA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenPrivateNameSpaceA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenSingletonAttributesA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenBnoIsolationA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenChildProcessFlagsA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenIsLessPrivilegedAppContainerA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenIsSandboxedA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenIsAppSiloA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenLoggingInformationA(HANDLE token, std::ostream& outputStream);
void EzPrintMaxTokenInfoClassA(HANDLE token, std::ostream& outputStream);

// Printing info about tokens to a Unicode stream
void EzPrintTokenUserW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenGroupsW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenPrivilegesW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenOwnerW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenPrimaryGroupW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenDefaultDaclW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenSourceW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenTypeW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenImpersonationLevelW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenStatisticsW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenRestrictedSidsW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenSessionIdW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenGroupsAndPrivilegesW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenSessionReferenceW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenSandBoxInertW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenAuditPolicyW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenOriginW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenElevationTypeW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenLinkedTokenW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenElevationW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenHasRestrictionsW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenAccessInformationW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenVirtualizationAllowedW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenVirtualizationEnabledW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenIntegrityLevelW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenUIAccessW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenMandatoryPolicyW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenLogonSidW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenIsAppContainerW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenCapabilitiesW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenAppContainerSidW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenAppContainerNumberW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenUserClaimAttributesW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenDeviceClaimAttributesW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenRestrictedUserClaimAttributesW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenRestrictedDeviceClaimAttributesW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenDeviceGroupsW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenRestrictedDeviceGroupsW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenSecurityAttributesW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenIsRestrictedW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenProcessTrustLevelW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenPrivateNameSpaceW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenSingletonAttributesW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenBnoIsolationW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenChildProcessFlagsW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenIsLessPrivilegedAppContainerW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenIsSandboxedW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenIsAppSiloW(HANDLE token, std::wostream& outputStream);
void EzPrintTokenLoggingInformationW(HANDLE token, std::wostream& outputStream);
void EzPrintMaxTokenInfoClassW(HANDLE token, std::wostream& outputStream);

// Macros for printing info about tokens to an Ansi or Unicode stream
#ifdef UNICODE
#define EzPrintTokenUser EzPrintTokenUserW
#else
#define EzPrintTokenUser EzPrintTokenUserA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenGroups EzPrintTokenGroupsW
#else
#define EzPrintTokenGroups EzPrintTokenGroupsA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenPrivileges EzPrintTokenPrivilegesW
#else
#define EzPrintTokenPrivileges EzPrintTokenPrivilegesA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenOwner EzPrintTokenOwnerW
#else
#define EzPrintTokenOwner EzPrintTokenOwnerA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenPrimaryGroup EzPrintTokenPrimaryGroupW
#else
#define EzPrintTokenPrimaryGroup EzPrintTokenPrimaryGroupA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenDefaultDacl EzPrintTokenDefaultDaclW
#else
#define EzPrintTokenDefaultDacl EzPrintTokenDefaultDaclA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenSource EzPrintTokenSourceW
#else
#define EzPrintTokenSource EzPrintTokenSourceA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenType EzPrintTokenTypeW
#else
#define EzPrintTokenType EzPrintTokenTypeA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenImpersonationLevel EzPrintTokenImpersonationLevelW
#else
#define EzPrintTokenImpersonationLevel EzPrintTokenImpersonationLevelA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenStatistics EzPrintTokenStatisticsW
#else
#define EzPrintTokenStatistics EzPrintTokenStatisticsA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenRestrictedSids EzPrintTokenRestrictedSidsW
#else
#define EzPrintTokenRestrictedSids EzPrintTokenRestrictedSidsA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenSessionId EzPrintTokenSessionIdW
#else
#define EzPrintTokenSessionId EzPrintTokenSessionIdA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenGroupsAndPrivileges EzPrintTokenGroupsAndPrivilegesW
#else
#define EzPrintTokenGroupsAndPrivileges EzPrintTokenGroupsAndPrivilegesA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenSessionReference EzPrintTokenSessionReferenceW
#else
#define EzPrintTokenSessionReference EzPrintTokenSessionReferenceA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenSandBoxInert EzPrintTokenSandBoxInertW
#else
#define EzPrintTokenSandBoxInert EzPrintTokenSandBoxInertA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenAuditPolicy EzPrintTokenAuditPolicyW
#else
#define EzPrintTokenAuditPolicy EzPrintTokenAuditPolicyA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenOrigin EzPrintTokenOriginW
#else
#define EzPrintTokenOrigin EzPrintTokenOriginA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenElevationType EzPrintTokenElevationTypeW
#else
#define EzPrintTokenElevationType EzPrintTokenElevationTypeA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenLinkedToken EzPrintTokenLinkedTokenW
#else
#define EzPrintTokenLinkedToken EzPrintTokenLinkedTokenA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenElevation EzPrintTokenElevationW
#else
#define EzPrintTokenElevation EzPrintTokenElevationA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenHasRestrictions EzPrintTokenHasRestrictionsW
#else
#define EzPrintTokenHasRestrictions EzPrintTokenHasRestrictionsA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenAccessInformation EzPrintTokenAccessInformationW
#else
#define EzPrintTokenAccessInformation EzPrintTokenAccessInformationA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenVirtualizationAllowed EzPrintTokenVirtualizationAllowedW
#else
#define EzPrintTokenVirtualizationAllowed EzPrintTokenVirtualizationAllowedA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenVirtualizationEnabled EzPrintTokenVirtualizationEnabledW
#else
#define EzPrintTokenVirtualizationEnabled EzPrintTokenVirtualizationEnabledA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenIntegrityLevel EzPrintTokenIntegrityLevelW
#else
#define EzPrintTokenIntegrityLevel EzPrintTokenIntegrityLevelA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenUIAccess EzPrintTokenUIAccessW
#else
#define EzPrintTokenUIAccess EzPrintTokenUIAccessA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenMandatoryPolicy EzPrintTokenMandatoryPolicyW
#else
#define EzPrintTokenMandatoryPolicy EzPrintTokenMandatoryPolicyA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenLogonSid EzPrintTokenLogonSidW
#else
#define EzPrintTokenLogonSid EzPrintTokenLogonSidA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenIsAppContainer EzPrintTokenIsAppContainerW
#else
#define EzPrintTokenIsAppContainer EzPrintTokenIsAppContainerA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenCapabilities EzPrintTokenCapabilitiesW
#else
#define EzPrintTokenCapabilities EzPrintTokenCapabilitiesA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenAppContainerSid EzPrintTokenAppContainerSidW
#else
#define EzPrintTokenAppContainerSid EzPrintTokenAppContainerSidA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenAppContainerNumber EzPrintTokenAppContainerNumberW
#else
#define EzPrintTokenAppContainerNumber EzPrintTokenAppContainerNumberA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenUserClaimAttributes EzPrintTokenUserClaimAttributesW
#else
#define EzPrintTokenUserClaimAttributes EzPrintTokenUserClaimAttributesA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenDeviceClaimAttributes EzPrintTokenDeviceClaimAttributesW
#else
#define EzPrintTokenDeviceClaimAttributes EzPrintTokenDeviceClaimAttributesA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenRestrictedUserClaimAttributes EzPrintTokenRestrictedUserClaimAttributesW
#else
#define EzPrintTokenRestrictedUserClaimAttributes EzPrintTokenRestrictedUserClaimAttributesA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenRestrictedDeviceClaimAttributes EzPrintTokenRestrictedDeviceClaimAttributesW
#else
#define EzPrintTokenRestrictedDeviceClaimAttributes EzPrintTokenRestrictedDeviceClaimAttributesA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenDeviceGroups EzPrintTokenDeviceGroupsW
#else
#define EzPrintTokenDeviceGroups EzPrintTokenDeviceGroupsA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenRestrictedDeviceGroups EzPrintTokenRestrictedDeviceGroupsW
#else
#define EzPrintTokenRestrictedDeviceGroups EzPrintTokenRestrictedDeviceGroupsA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenSecurityAttributes EzPrintTokenSecurityAttributesW
#else
#define EzPrintTokenSecurityAttributes EzPrintTokenSecurityAttributesA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenIsRestricted EzPrintTokenIsRestrictedW
#else
#define EzPrintTokenIsRestricted EzPrintTokenIsRestrictedA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenProcessTrustLevel EzPrintTokenProcessTrustLevelW
#else
#define EzPrintTokenProcessTrustLevel EzPrintTokenProcessTrustLevelA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenPrivateNameSpace EzPrintTokenPrivateNameSpaceW
#else
#define EzPrintTokenPrivateNameSpace EzPrintTokenPrivateNameSpaceA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenSingletonAttributes EzPrintTokenSingletonAttributesW
#else
#define EzPrintTokenSingletonAttributes EzPrintTokenSingletonAttributesA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenBnoIsolation EzPrintTokenBnoIsolationW
#else
#define EzPrintTokenBnoIsolation EzPrintTokenBnoIsolationA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenChildProcessFlags EzPrintTokenChildProcessFlagsW
#else
#define EzPrintTokenChildProcessFlags EzPrintTokenChildProcessFlagsA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenIsLessPrivilegedAppContainer EzPrintTokenIsLessPrivilegedAppContainerW
#else
#define EzPrintTokenIsLessPrivilegedAppContainer EzPrintTokenIsLessPrivilegedAppContainerA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenIsSandboxed EzPrintTokenIsSandboxedW
#else
#define EzPrintTokenIsSandboxed EzPrintTokenIsSandboxedA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenIsAppSilo EzPrintTokenIsAppSiloW
#else
#define EzPrintTokenIsAppSilo EzPrintTokenIsAppSiloA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintTokenLoggingInformation EzPrintTokenLoggingInformationW
#else
#define EzPrintTokenLoggingInformation EzPrintTokenLoggingInformationA
#endif // UNICODE

#ifdef UNICODE
#define EzPrintMaxTokenInfoClass EzPrintMaxTokenInfoClassW
#else
#define EzPrintMaxTokenInfoClass EzPrintMaxTokenInfoClassA
#endif // UNICODE

// General printing functions to print all info
void EzPrintTokenInfoA(HANDLE token, std::ostream& outputStream);
void EzPrintTokenInfoW(HANDLE token, std::wostream& outputStream);
#ifdef UNICODE
#define EzPrintTokenInfo EzPrintTokenInfoW
#else
#define EzPrintTokenInfo EzPrintTokenInfoA
#endif // UNICODE