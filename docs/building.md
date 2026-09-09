Building

Local Windows with SDK headers:

scons platform=windows arch=x86_64 -j6

Stub mode without the SDK, for API and logic work:

EOS_SDK_DIR=C:\empty scons platform=windows arch=x86_64

The SConstruct detects SDK/Include/eos_sdk.h and defines
YUGEN_EOS_HAS_SDK accordingly. Both modes must compile. Release
artifacts use target=template_release.

godot-cpp must be present as a directory or through GODOT_CPP_DIR.
CI clones branch godot-4.3-stable with depth 1.

Regenerate bindings after an SDK update:

python3 scripts/generate_bindings.py

Commit the regenerated src/core/eos_bindings.gen files. The CI
freshness test fails when they drift from the headers.

Verify before pushing:

python3 tests/test_eos_symbols.py
python3 tests/test_gdscript_facades.py
python3 tests/test_bindings_fresh.py
python3 tests/test_docs_status.py
