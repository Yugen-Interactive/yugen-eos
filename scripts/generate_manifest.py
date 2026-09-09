import json
import os
import pathlib
import re
import sys

CANDIDATES = []
ENV = os.environ.get("EOS_SDK_DIR", "")
if ENV:
    CANDIDATES.append(pathlib.Path(ENV) / "SDK" / "Include")
    CANDIDATES.append(pathlib.Path(ENV) / "Include")
CANDIDATES.append(pathlib.Path(r"C:\Users\hxgohxrr\AppData\Local\Temp\opencode\eos_headers\SDK\Include"))
CANDIDATES.append(pathlib.Path(r"D:\yugen-eos\third_party\eos_sdk\SDK\Include"))
INCLUDE = next((p for p in CANDIDATES if (p / "eos_sdk.h").is_file()), None)
if INCLUDE is None:
    print("EOS SDK headers not found. Set EOS_SDK_DIR.")
    sys.exit(2)

REPO = pathlib.Path(__file__).resolve().parents[1]
OUT = REPO / "bindings" / "api_manifest.json"
OUT.parent.mkdir(parents=True, exist_ok=True)

version_text = (INCLUDE / "eos_version.h").read_text(errors="replace")
version = {}
for key in ["EOS_MAJOR_VERSION", "EOS_MINOR_VERSION", "EOS_PATCH_VERSION", "EOS_HOTFIX_VERSION"]:
    m = re.search(r"#define\s+%s\s+(\d+)" % key, version_text)
    version[key] = int(m.group(1)) if m else 0

interfaces = {}
for h in sorted(INCLUDE.glob("eos_*.h")):
    text = h.read_text(errors="replace")
    funcs = sorted(set(re.findall(r"EOS_DECLARE_FUNC\s*\(.*?\)\s*(EOS_\w+)\s*\(", text, re.S)))
    latest = sorted(set(re.findall(r"#define\s+(EOS_\w+_API_LATEST)\s+\d+", text)))
    if funcs or latest:
        interfaces[h.stem] = {"functions": funcs, "api_latest": latest}

manifest = {
    "sdk_version": "%d.%d.%d.%d" % (version["EOS_MAJOR_VERSION"], version["EOS_MINOR_VERSION"],
                                    version["EOS_PATCH_VERSION"], version["EOS_HOTFIX_VERSION"]),
    "wrapper_version": "0.1.0",
    "godot_minimum": "4.3",
    "interfaces": interfaces,
}
OUT.write_text(json.dumps(manifest, indent=2) + "\n")
print("wrote", OUT, "with", len(interfaces), "headers")
