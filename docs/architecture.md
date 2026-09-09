Architecture

Layers, top to bottom:

1. Godot-facing API. GDScript autoload EOS plus facades per
interface, and the YugenEOS GDExtension singleton underneath.
GDScript code never sees pointers, structs, or C callbacks.

2. EOS abstraction layer. C++ interface classes, one per EOS
interface group, owning no EOS handles directly except through
registries with explicit release in destructors.

3. Native binding layer. src/core/eos_bindings.gen.hpp declares a
table of function pointers with signatures parsed from the real
SDK headers. scripts/generate_bindings.py regenerates it. No EOS
symbol is linked. The table loads through the platform loader and
fails closed when a symbol is missing.

4. Platform implementation. src/core/eos_loader.cpp resolves the
native library per OS: EOSSDK-Win64-Shipping.dll on Windows,
libEOSSDK-Linux-Shipping.so on Linux, libEOSSDK-Mac-Shipping.dylib
on macOS, libEOSSDK.so on Android, EOSSDK framework on iOS.

5. EOS SDK binaries. Provided by the developer, never committed.
See third_party/eos_sdk/README.md.

Cross-cutting pieces:

Result codes. src/core/eos_result maps EOS_EResult values verified
against eos_result.h to name and message. Unknown codes keep their
numeric value and are grouped by range.

Errors. Every fallible call returns a Dictionary with code, name,
message, operation, context, and ok. Nothing fails silently.

Callbacks. Native callbacks run on whatever thread EOS uses. Each
callback builds a plain Dictionary, pushes it into a mutex-guarded
queue, and frees its RequestContext exactly once. Operations with
retry semantics free the context only on operation-complete results.
The Godot thread drains the queue in tick and emits signals.

Handles. Opaque EOS handles stay in C++. GDScript passes user ids
as strings and session results as integer registry refs. Registries
release native handles in destructors.

Threading. Only tick and signal emission touch Godot objects, always
on the thread that calls tick. The queue is the single crossing
point.

Logging. EOS log callback forwards to Godot with per-level control.
A sanitizer redacts secret, token, password, encryption, key,
credential, and auth assignments before printing.
