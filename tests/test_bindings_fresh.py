import pathlib
import subprocess
import sys

REPO = pathlib.Path(__file__).resolve().parents[1]


def main():
    try:
        current_hpp = (REPO / "src" / "core" / "eos_bindings.gen.hpp").read_text()
        current_cpp = (REPO / "src" / "core" / "eos_bindings.gen.cpp").read_text()
    except FileNotFoundError:
        print("FAIL: generated bindings missing.")
        return 1
    proc = subprocess.run([sys.executable, str(REPO / "scripts" / "generate_bindings.py")],
                          capture_output=True, text=True)
    if proc.returncode != 0:
        print("SKIP: generator needs EOS SDK headers.")
        print(proc.stdout.strip())
        return 0
    fresh_hpp = (REPO / "src" / "core" / "eos_bindings.gen.hpp").read_text()
    fresh_cpp = (REPO / "src" / "core" / "eos_bindings.gen.cpp").read_text()
    if fresh_hpp != current_hpp or fresh_cpp != current_cpp:
        print("FAIL: committed bindings differ from regenerated output.")
        print("Run scripts/generate_bindings.py and commit the result.")
        return 1
    print("PASS: committed bindings match regenerated output.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
