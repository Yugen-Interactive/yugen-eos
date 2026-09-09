SDK updates

1. Download the new SDK from the Developer Portal and extract it
outside the repo.

2. Point EOS_SDK_DIR at it and regenerate:

python3 scripts/generate_bindings.py

3. Run the symbol check to list new, removed, or renamed APIs:

python3 tests/test_eos_symbols.py

4. Rebuild both modes and run the full test set.

5. Update src/eos_version.hpp constants, the SConstruct default,
docs/status.md, and the addon version.

6. When Epic removes or renames a function, keep the old GDScript
method working when possible and map it to the replacement, or
return a result whose reason names the SDK change. Never silently
change behavior across SDK versions.

7. Record the supported SDK version in the release notes.
