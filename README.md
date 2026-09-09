Yugen EOS: Epic Online Services wrapper for Godot 4.

Wrapper 0.2.0, Godot 4.3+, EOS SDK 1.19.1.

Copyright (c) 2026 Yugen Interactive.

This software is released under the MIT License. See LICENSE
for the full terms.

Layout: addons/yugen_eos holds the Godot-facing plugin, autoload,
and GDScript facades. src holds the GDExtension core and interface
classes. generator output lives in src/core/eos_bindings.gen.*.
tests run without credentials. examples are small per-feature
scripts. The manual lives in the wiki, Status is the feature list.

Build with SCons, configure under Project Settings yugen_eos, keep
secrets in environment variables. See the wiki Installation and
Configuration pages:

https://github.com/Yugen-Interactive/yugen-eos/wiki/Installation
https://github.com/Yugen-Interactive/yugen-eos/wiki/Configuration

EOS SDK is Epic Games property with its own terms, see
THIRD_PARTY_NOTICES.md. No SDK binaries or credentials are
distributed in this repo.
