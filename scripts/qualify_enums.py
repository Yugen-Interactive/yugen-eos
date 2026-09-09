import pathlib
import re

SRC = pathlib.Path(r"D:\yugen-eos\src")

qualified = {
    "EOS_Success": "EOS_EResult",
    "EOS_AS_BasicProfile": "EOS_EAuthScopeFlags",
    "EOS_AS_FriendsList": "EOS_EAuthScopeFlags",
    "EOS_AS_Presence": "EOS_EAuthScopeFlags",
    "EOS_OSPF_PublicAdvertised": "EOS_EOnlineSessionPermissionLevel",
    "EOS_OSPF_JoinViaPresence": "EOS_EOnlineSessionPermissionLevel",
    "EOS_LPL_PUBLICADVERTISED": "EOS_ELobbyPermissionLevel",
    "EOS_LPL_JOINVIAPRESENCE": "EOS_ELobbyPermissionLevel",
    "EOS_PR_ReliableUnordered": "EOS_EPacketReliability",
    "EOS_PR_UnreliableUnordered": "EOS_EPacketReliability",
    "EOS_RC_NoRelays": "EOS_ERelayControl",
    "EOS_RC_AllowRelays": "EOS_ERelayControl",
    "EOS_LA_Max": "EOS_ELeaderboardAggregation",
    "EOS_MAIT_Epic": "EOS_EMetricsAccountIdType",
    "EOS_UCT_Unknown": "EOS_EUserControllerType",
}

files = 0
for f in list(SRC.rglob("*.cpp")) + list(SRC.rglob("*.hpp")):
    t = f.read_text(errors="replace")
    orig = t
    for name, scope in qualified.items():
        t = re.sub(r"(?<!:)\b%s\b(?!::)" % re.escape(name), "%s::%s" % (scope, name), t)
    if t != orig:
        f.write_text(t)
        files += 1

print("files updated:", files)
