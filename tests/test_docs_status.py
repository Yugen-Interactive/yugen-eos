import os
import pathlib
import sys

REPO = pathlib.Path(__file__).resolve().parents[1]
VALID = {"implemented", "partial", "not-supported", "requires-external",
         "requires-backend", "unavailable-on-platform"}


def find_status():
    env = os.environ.get("WIKI_DIR", "")
    candidates = []
    if env:
        candidates.append(pathlib.Path(env) / "Status.md")
    candidates.append(REPO.parent / "yugen-eos.wiki" / "Status.md")
    candidates.append(REPO / "docs" / "status.md")
    for cand in candidates:
        if cand.is_file():
            return cand
    return None


def main():
    status = find_status()
    if status is None:
        print("SKIP: wiki checkout not found, Status check needs it.")
        return 0
    text = status.read_text(errors="replace")
    failures = []
    for line in text.splitlines():
        line = line.strip()
        if line.startswith("|") and "interface" not in line.lower() and "---" not in line:
            cells = [c.strip() for c in line.strip("|").split("|")]
            if len(cells) >= 2 and cells[1] not in VALID:
                failures.append(line)
    if failures:
        print("FAIL: status rows with unknown state:")
        for f in failures:
            print("  ", f)
        return 1
    print("PASS: Status uses only documented states (%s)." % status)
    return 0


if __name__ == "__main__":
    sys.exit(main())
