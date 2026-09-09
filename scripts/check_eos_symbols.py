import re
import pathlib
import sys

SDK = pathlib.Path(r"C:\Users\hxgohxrr\AppData\Local\Temp\opencode\eos_headers\SDK\Include")
SRC = pathlib.Path(r"D:\yugen-eos\src")

headers = {}
for f in SDK.glob("*.h"):
    headers[f.name] = f.read_text(errors="replace")
all_text = "\n".join(headers.values())

symbols = set()
for f in SRC.rglob("*.cpp"):
    t = f.read_text(errors="replace")
    symbols.update(re.findall(r"\bEOS_[A-Za-z0-9_]+\b", t))
for f in SRC.rglob("*.hpp"):
    t = f.read_text(errors="replace")
    symbols.update(re.findall(r"\bEOS_[A-Za-z0-9_]+\b", t))

skip_prefix = ("EOSInterface", "EOSInterfaceBase", "EOSResult", "EOSError", "EOSLogger",
               "EOSLoader", "EOSCallbackQueue", "EOSPlatformManager", "EOSAuth", "EOSConnect",
               "EOSFriends", "EOSPresence", "EOSUserInfo", "EOSSessions", "EOSLobbies", "EOSP2P",
               "EOSStats", "EOSAchievements", "EOSLeaderboards", "EOSStorage", "EOSEcom",
               "EOSUI", "EOSSocial")
missing = []
for s in sorted(symbols):
    if s.startswith(skip_prefix):
        continue
    if s not in all_text:
        missing.append(s)

print("TOTAL EOS symbols used:", len(symbols))
if missing:
    print("MISSING in SDK headers:")
    for m in missing:
        print("  ", m)
    sys.exit(1)
print("All EOS symbols found in SDK 1.19.1 headers.")
