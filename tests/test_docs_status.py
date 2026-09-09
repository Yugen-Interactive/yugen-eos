import pathlib
import sys

REPO = pathlib.Path(__file__).resolve().parents[1]
STATUS = REPO / "docs" / "status.md"
VALID = {"implemented", "partial", "not-supported", "requires-external",
         "requires-backend", "unavailable-on-platform"}


def main():
    if not STATUS.is_file():
        print("FAIL: docs/status.md missing.")
        return 1
    text = STATUS.read_text(errors="replace")
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
    print("PASS: docs/status.md uses only documented states.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
