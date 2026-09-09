import os
import pathlib
import re
import sys

REPO = pathlib.Path(__file__).resolve().parents[1]
SRC = REPO / "src"

OWN_PREFIX = ("EOSInterface", "EOSResult", "EOSError", "EOSLogger",
              "EOSLoader", "EOSCallbackQueue", "EOSPlatformManager", "EOSAuth",
              "EOSConnect", "EOSFriends", "EOSPresence", "EOSUserInfo",
              "EOSSessions", "EOSLobbies", "EOSP2P", "EOSStats",
              "EOSAchievements", "EOSLeaderboards", "EOSStorage", "EOSEcom",
              "EOSUI", "EOSSocial", "EOSApiTable")

OWN_EXTRA = {"EOS_Auth_Error", "EOS_Ecom_Error", "EOS_Friends_Error",
             "EOS_Sessions_Error", "EOS_Unknown", "EOS_SDK_DIR"}


def find_include():
    env = os.environ.get("EOS_SDK_DIR", "")
    for cand in [pathlib.Path(env) / "SDK" / "Include" if env else None,
                 pathlib.Path(env) / "Include" if env else None,
                 REPO / "third_party" / "eos_sdk" / "SDK" / "Include"]:
        if cand is not None and (cand / "eos_sdk.h").is_file():
            return cand
    return None


def main():
    include = find_include()
    if include is None:
        print("SKIP: EOS SDK headers not found, symbol check needs headers.")
        return 0
    all_text = "\n".join(f.read_text(errors="replace") for f in include.glob("*.h"))
    symbols = set()
    for f in list(SRC.rglob("*.cpp")) + list(SRC.rglob("*.hpp")):
        if f.name.startswith("eos_bindings.gen"):
            continue
        symbols.update(re.findall(r"\bEOS_[A-Za-z0-9_]+\b",
                                  f.read_text(errors="replace")))
    missing = [s for s in sorted(symbols)
               if not s.startswith(OWN_PREFIX) and s not in all_text
               and not s.endswith("_fn") and s not in OWN_EXTRA]
    if missing:
        print("FAIL: symbols missing from EOS SDK headers:")
        for m in missing:
            print("  ", m)
        return 1
    print("PASS: %d EOS symbols verified against SDK headers." % len(symbols))
    return 0


if __name__ == "__main__":
    sys.exit(main())
