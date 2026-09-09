import re
import pathlib

b = pathlib.Path(r"D:\yugen-eos\third_party\eos_sdk\SDK\Include")
T = {f.name: f.read_text(errors="replace") for f in b.glob("*.h")}
A = "\n".join(T.values())

def struct(name, length=800):
    i = A.find("EOS_STRUCT(" + name)
    print("--- " + name + " ---")
    print(A[i:i + length] if i >= 0 else "NOT FOUND")

def decls(pattern):
    print("--- " + pattern + " ---")
    for m in sorted(set(re.findall(pattern, A))):
        print("   ", m)

struct("EOS_Sessions_QueryInvitesOptions", 600)
struct("EOS_Sessions_SendInviteOptions", 800)
struct("EOS_Sessions_RejectInviteOptions", 600)
decls(r"EOS_Sessions_(OnQueryInvitesCompleteCallback|QueryInvitesCallbackInfo|OnSendInviteCompleteCallback|SendInviteCallbackInfo|OnRejectInviteCompleteCallback|RejectInviteCallbackInfo)")
decls(r"EOS_DECLARE_FUNC\([^)]*\) EOS_Sessions_UpdateSessionModification")
struct("EOS_SessionModification_AddAttributeOptions", 800)
struct("EOS_Sessions_AttributeData", 700)
decls(r"EOS_DECLARE_FUNC\([^)]*\) EOS_SessionDetails_CopySessionAttributeBy\w+")
decls(r"EOS_DECLARE_FUNC\([^)]*\) EOS_SessionDetails_GetSessionAttributeCount")
struct("EOS_Lobby_CreateLobbySearchOptions", 600)
decls(r"EOS_DECLARE_FUNC\([^)]*\) EOS_Lobby_CreateLobbySearch")
decls(r"EOS_LobbySearch_\w+")
struct("EOS_Lobby_CopyLobbyDetailsHandleOptions", 700)
decls(r"EOS_DECLARE_FUNC\([^)]*\) EOS_Lobby_CopyLobbyDetailsHandle")
decls(r"EOS_DECLARE_FUNC\([^)]*\) EOS_LobbyDetails_(GetMemberCount|GetLobbyOwner|CopyInfo|GetAttributeCount|CopyAttributeByIndex)")
struct("EOS_Achievements_QueryDefinitionsOptions", 600)
decls(r"EOS_DECLARE_FUNC\([^)]*\) EOS_Achievements_(QueryDefinitions|GetAchievementDefinitionCount|CopyAchievementDefinitionV2ByIndex)")
decls(r"EOS_Achievements_(OnQueryDefinitionsCompleteCallback|QueryDefinitionsCallbackInfo)")
decls(r"EOS_DECLARE_FUNC\([^)]*\) EOS_UI_(PauseSocialOverlay|IsSocialOverlayPaused|GetFriendsVisible|SetDisplayPreference|GetNotificationLocationPreference)")
decls(r"EOS_UI_(ShowBlockPlayer|ShowReportPlayer|ShowNativeProfile)")
struct("EOS_Sanctions_CreatePlayerSanctionAppealOptions", 700)
decls(r"EOS_DECLARE_FUNC\([^)]*\) EOS_RTC_GetAudioInterface")
decls(r"EOS_DECLARE_FUNC\([^)]*\) EOS_RTCAudio_\w+")
decls(r"EOS_DECLARE_FUNC\([^)]*\) EOS_Metrics_EndPlayerSession")
struct("EOS_Metrics_EndPlayerSessionOptions", 700)
