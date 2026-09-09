Yugen EOS: Epic Online Services wrapper for Godot 4.

Wrapper 0.1.0, Godot 4.3+, EOS SDK 1.19.1.

Layout: addons/yugen_eos holds the Godot-facing plugin, autoload,
and GDScript facades. src holds the GDExtension core and interface
classes. generator output lives in src/core/eos_bindings.gen.*.
tests run without credentials. examples are small per-feature
scenes scripts. docs are the manual, docs/status.md is the honest
feature list.

Build with SCons, configure under Project Settings yugen_eos, keep
secrets in environment variables. See docs/installation.md and
docs/configuration.md.

License: wrapper MIT, see LICENSE. EOS SDK is Epic property with
its own terms, see THIRD_PARTY_NOTICES.md. No SDK binaries or
credentials are distributed in this repo.
