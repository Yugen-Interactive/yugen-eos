import re
import pathlib

SRC = pathlib.Path(r"D:\yugen-eos\src")
SKIP = {"eos_bindings.gen.hpp", "eos_bindings.gen.cpp"}

bound = set()
gen = (SRC / "core" / "eos_bindings.gen.hpp").read_text(errors="replace")
for m in re.finditer(r"typedef .*?\(\s*EOS_CALL\s*\*(EOS_\w+)_fn\)", gen):
    bound.add(m.group(1))
print("bound functions:", len(bound))

pat_call = re.compile(r"(?<![.\->:\w])(EOS_[A-Z]\w*)\s*\(")

def is_fn(name):
    return name in bound

files = 0
for f in list(SRC.rglob("*.cpp")):
    if f.name in SKIP:
        continue
    t = f.read_text(errors="replace")
    orig = t
    t = t.replace("EOS_EResult_IsOperationComplete(",
                  "ctx->table->EOS_EResult_IsOperationComplete(")
    def repl(m):
        name = m.group(1)
        if is_fn(name):
            return "bindings()->" + name + "("
        return m.group(0)
    t = pat_call.sub(repl, t)
    t = re.sub(r"make_context\(platform->get_queue\(\)\.ptr\(\), (\"[^\"]+\"), context\)",
               r"make_request(\1, context)", t)
    if t != orig:
        f.write_text(t)
        files += 1

print("files updated:", files)
