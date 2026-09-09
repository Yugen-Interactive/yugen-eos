import re
import pathlib

SRC = pathlib.Path(r"D:\yugen-eos\src")

def method_name(ref):
    return ref.split("::")[-1]

count = 0
for f in list(SRC.rglob("*.cpp")) + list(SRC.rglob("*.hpp")):
    t = f.read_text(errors="replace")
    orig = t
    def repl_m(m):
        ref = m.group(1)
        return "godot::ClassDB::bind_method(D_METHOD(\"%s\"), %s);" % (method_name(ref), ref)
    t = re.sub(r"godot::ClassDB::bind_method\(godot::DMethodBind\(\), (&[\w:]+)\);", repl_m, t)
    def repl_s(m):
        cls, ref = m.group(1), m.group(2)
        return "godot::ClassDB::bind_static_method(\"%s\", D_METHOD(\"%s\"), %s);" % (cls, method_name(ref), ref)
    t = re.sub(r"godot::ClassDB::bind_static_method\(\"(\w+)\", godot::DMethodBind\(\), (&[\w:]+)\);", repl_s, t)
    if t != orig:
        f.write_text(t)
        count += 1

print("files updated:", count)
