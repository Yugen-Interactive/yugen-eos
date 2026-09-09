import pathlib
import re
import sys

REPO = pathlib.Path(__file__).resolve().parents[1]
WF = REPO / ".github" / "workflows"

REQUIRED_TOP = ["name:", "on:", "jobs:"]
SECRET_REF = re.compile(r"\$\{\{\s*secrets\.[A-Z_]+\s*\}")

failures = []
for f in sorted(WF.glob("*.yml")):
    text = f.read_text(errors="replace")
    for key in REQUIRED_TOP:
        if key not in text:
            failures.append("%s missing %s" % (f.name, key))
    if "\t" in text:
        failures.append("%s contains tabs" % f.name)
    if "runs-on:" not in text:
        failures.append("%s has no runner" % f.name)
    if "EOS_SDK" in text and not SECRET_REF.search(text):
        failures.append("%s mentions SDK without secret ref" % f.name)
    for hard in ["AKIA", "xoxb-", "BEGIN PRIVATE", "password="]:
        if hard in text:
            failures.append("%s may contain a secret literal" % f.name)

if failures:
    print("FAIL:")
    for item in failures:
        print("  ", item)
    sys.exit(1)
print("PASS: %d workflow files structurally valid." % len(list(WF.glob("*.yml"))))
