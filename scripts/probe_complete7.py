import re
import pathlib

b = pathlib.Path(r"D:\yugen-eos\third_party\eos_sdk\SDK\Include")
T = {f.name: f.read_text(errors="replace") for f in b.glob("*.h")}
A = "\n".join(T.values())

def grab(marker, length=700):
    i = A.find(marker)
    print("--- " + marker + " ---")
    print(A[i:i + length] if i >= 0 else "NOT FOUND")

grab("EOS_SessionDetails_CopySessionAttributeByIndex(EOS_HSessionDetails")
grab("EOS_Sessions_UpdateSessionModification(EOS_HSessions")
grab("EOS_Lobby_CopyLobbyDetailsHandle(EOS_HLobby")
grab("EOS_LobbyDetails_CopyInfo(EOS_HLobbyDetails")
grab("EOS_LobbyDetails_GetMemberByIndex(EOS_HLobbyDetails")
grab("EOS_Achievements_DefinitionV2_Release")
grab("EOS_UI_ENotificationLocation,")
grab("EOS_STRUCT(EOS_Mods_EnumerateModsOptions")
grab("EOS_STRUCT(EOS_KWS_QueryAgeGateOptions")
grab("EOS_STRUCT(EOS_CustomInvites_SendCustomInviteOptions")
grab("EOS_IntegratedPlatform_")
grab("EOS_ProgressionSnapshot_")
grab("EOS_Sanctions_OnCreatePlayerSanctionAppealCallback")
grab("SetAudioInputSettings(EOS_HRTCAudio")
grab("EOS_STRUCT(EOS_LobbyDetails_CopyAttributeByIndexOptions")
grab("EOS_STRUCT(EOS_Lobby_Info")
grab("EOS_STRUCT(EOS_LobbyDetails_MemberInfo")
