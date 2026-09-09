import os
import pathlib
import re
import sys

REPO = pathlib.Path(__file__).resolve().parents[1]
ADDONS = REPO / "addons" / "yugen_eos"
SRC = REPO / "src"


def main():
    failures = []
    core_methods = set()
    for f in (SRC / "yugen_eos.cpp").read_text().split("YugenEOS::")[1:]:
        core_methods.add(f.split("(")[0].strip())
    for facade in sorted((ADDONS / "facades").glob("*.gd")):
        text = facade.read_text(errors="replace")
        for m in re.finditer(r'_call_core\("([a-z_0-9]+)"', text):
            name = m.group(1)
            if name not in core_methods:
                failures.append("%s calls unknown core method %s" % (facade.name, name))
    if failures:
        print("FAIL:")
        for f in failures:
            print("  ", f)
        return 1
    print("PASS: all GDScript facade calls resolve to YugenEOS core methods.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
