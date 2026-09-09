import os
import re
import pathlib
import sys

SDK_DIR = os.environ.get("EOS_SDK_DIR", "")
CANDIDATES = [
    pathlib.Path(SDK_DIR) / "SDK" / "Include" if SDK_DIR else None,
    pathlib.Path(SDK_DIR) / "Include" if SDK_DIR else None,
    pathlib.Path(r"C:\Users\hxgohxrr\AppData\Local\Temp\opencode\eos_headers\SDK\Include"),
    pathlib.Path(r"D:\yugen-eos\third_party\eos_sdk\SDK\Include"),
]
INCLUDE = next((p for p in CANDIDATES if p is not None and (p / "eos_sdk.h").is_file()), None)
if INCLUDE is None:
    print("EOS SDK headers not found. Set EOS_SDK_DIR.")
    sys.exit(2)

SRC = pathlib.Path(r"D:\yugen-eos\src")
OUT = SRC / "core" / "eos_bindings.gen.hpp"
OUT_CPP = SRC / "core" / "eos_bindings.gen.cpp"

CORE = {"EOS_Initialize", "EOS_Shutdown", "EOS_Platform_Tick", "EOS_GetVersion",
        "EOS_EResult_IsOperationComplete"}

used = set(CORE)
for f in list(SRC.rglob("*.cpp")) + list(SRC.rglob("*.hpp")):
    if f.name == "eos_bindings.gen.hpp":
        continue
    t = f.read_text(errors="replace")
    used.update(re.findall(r"\b(EOS_[A-Z]\w*)\s*\(", t))

decls = {}
for h in INCLUDE.glob("*.h"):
    text = h.read_text(errors="replace")
    for m in re.finditer(r"EOS_DECLARE_FUNC\s*\((.*?)\)\s*(EOS_\w+)\s*\((.*?)\)\s*;", text, re.S):
        ret = " ".join(m.group(1).split())
        name = m.group(2)
        args = " ".join(m.group(3).split())
        decls[name] = (ret, args)

missing_decl = sorted(n for n in used if n not in decls)
if missing_decl:
    print("WARNING: used but not declared in headers:")
    for n in missing_decl:
        print("   ", n)

names = sorted(n for n in used if n in decls)
print("functions bound:", len(names))

lines = []
lines.append("#pragma once")
lines.append("")
lines.append("#if YUGEN_EOS_HAS_SDK")
lines.append("")
lines.append('#include "eos_base.h"')
lines.append('#include "eos_sdk.h"')
lines.append("#include <godot_cpp/classes/ref_counted.hpp>")
lines.append("")
lines.append("namespace yugen {")
lines.append("")
lines.append("class EOSLoader;")
lines.append("")
lines.append("struct EOSApiTable {")
for n in names:
    ret, args = decls[n]
    lines.append("    typedef %s (EOS_CALL *%s_fn)(%s);" % (ret, n, args))
lines.append("")
for n in names:
    lines.append("    %s_fn %s = nullptr;" % (n, n))
lines.append("")
lines.append("    int missing_count = 0;")
lines.append("    bool load(EOSLoader *loader);")
lines.append("};")
lines.append("")
lines.append("}")
lines.append("")
lines.append("#else")
lines.append("")
lines.append("namespace yugen {")
lines.append("")
lines.append("struct EOSApiTable {};")
lines.append("")
lines.append("}")
lines.append("")
lines.append("#endif")
OUT.write_text("\n".join(lines) + "\n")
print("wrote", OUT)

cpp = []
cpp.append('#include "core/eos_bindings.gen.hpp"')
cpp.append("")
cpp.append("#if YUGEN_EOS_HAS_SDK")
cpp.append("")
cpp.append('#include "core/eos_loader.hpp"')
cpp.append("")
cpp.append("namespace yugen {")
cpp.append("")
cpp.append("bool EOSApiTable::load(EOSLoader *loader) {")
cpp.append("    missing_count = 0;")
cpp.append("    if (loader == nullptr || !loader->is_loaded()) {")
cpp.append("        return false;")
cpp.append("    }")
for n in names:
    cpp.append("    %s = (%s_fn)loader->find_symbol(\"%s\");" % (n, n, n))
    cpp.append("    if (%s == nullptr) { missing_count++; }" % n)
cpp.append("    return missing_count == 0;")
cpp.append("}")
cpp.append("")
cpp.append("}")
cpp.append("")
cpp.append("#endif")
OUT_CPP.write_text("\n".join(cpp) + "\n")
print("wrote", OUT_CPP)
