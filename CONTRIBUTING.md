Contributing

Scope first. Check the wiki Status page and open an issue before
large work. One interface or one layer per pull request.

Rules: no comments in C++ code, names carry meaning. No emojis
anywhere. No invented EOS APIs, every native call must match the
pinned SDK headers and pass tests/test_eos_symbols.py. Regenerate
bindings with scripts/generate_bindings.py when headers change and
commit the output.

Both build modes must compile: full SDK mode and stub mode without
headers. Add Python tests for new checks and GDScript tests under
tests/gdscript for runtime behavior. Update the wiki Status page
with the honest state of anything touched.

Security: never commit credentials, tokens, keys, or SDK binaries.
Redact secrets from logs and pastes. Client pull requests must not
include server secrets.
