import re
import pathlib

b = pathlib.Path(r"D:\yugen-eos\third_party\eos_sdk\SDK\Include")
T = {f.name: f.read_text(errors="replace") for f in b.glob("*.h")}
A = "\n".join(T.values())

def struct(name, length=700):
    i = A.find("EOS_STRUCT(" + name)
    print("--- " + name + " ---")
    print(A[i:i + length] if i >= 0 else "NOT FOUND")

def decls(pattern):
    print("--- " + pattern + " ---")
    for m in sorted(set(re.findall(pattern, A))):
        print("   ", m)

struct("EOS_Sessions_UpdateSessionModificationOptions", 600)
decls(r"EOS_DECLARE_FUNC\([^)]*\) EOS_Sessions_CopyActiveSessionHandle")
struct("EOS_SessionDetails_CopySessionAttributeByIndexOptions", 600)
decls(r"EOS_Sessions_AttributeData_Release|EOS_SessionDetails_Release")
struct("EOS_Lobby_JoinLobbyOptions", 900)
struct("EOS_LobbySearch_FindOptions", 600)
struct("EOS_LobbySearch_CopySearchResultByIndexOptions", 600)
struct("EOS_LobbySearch_GetSearchResultCountOptions", 500)
decls(r"EOS_LobbyDetails_Release")
struct("EOS_LobbyDetails_GetMemberCountOptions", 500)
struct("EOS_LobbyDetails_CopyInfoOptions", 600)
struct("EOS_LobbyDetails_GetLobbyOwnerOptions", 500)
struct("EOS_Achievements_CopyAchievementDefinitionV2ByIndexOptions", 500)
i = A.find("EOS_STRUCT(EOS_Achievements_DefinitionV2")
print("--- DefinitionV2 ---")
print(A[i:i + 900] if i >= 0 else "NOT FOUND")
struct("EOS_UI_PauseSocialOverlayOptions", 500)
struct("EOS_UI_GetFriendsVisibleOptions", 500)
struct("EOS_UI_SetDisplayPreferenceOptions", 500)
decls(r"EOS_Sanctions_OnCreatePlayerSanctionAppealCallback|EOS_Sanctions_CreatePlayerSanctionAppealCallbackInfo")
struct("EOS_RTCAudio_UpdateSendingVolumeOptions", 700)
struct("EOS_RTCAudio_SetAudioInputSettingsOptions", 700)
decls(r"EOS_DECLARE_FUNC\([^)]*\) EOS_Mods_\w+")
decls(r"EOS_DECLARE_FUNC\([^)]*\) EOS_KWS_QueryAgeGate")
decls(r"EOS_DECLARE_FUNC\([^)]*\) EOS_CustomInvites_SendCustomInvite")
decls(r"EOS_DECLARE_FUNC\([^)]*\) EOS_IntegratedPlatform_Get\w+")
decls(r"EOS_DECLARE_FUNC\([^)]*\) EOS_(Progressionsnapshot|ProgressionSnapshot)_\w+")
struct("EOS_RTCAudio_UpdateReceivingVolumeOptions", 600)
