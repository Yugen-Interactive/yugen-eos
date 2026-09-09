CI and CD

Workflows live in .github/workflows. One per platform plus a test
job. Runners: windows-latest, ubuntu-latest, macos-latest. Android
uses ubuntu with the NDK, iOS uses macos with Xcode.

Each build job prepares the toolchain, clones godot-cpp at
godot-4.3-stable, resolves the EOS SDK, compiles with SCons, runs
the Python tests, and uploads the library as an artifact. Release
tags additionally attach binaries to a GitHub release.

EOS SDK resolution order: EOS_SDK_URL with EOS_SDK_TOKEN for an
authenticated download, else a pre-placed third_party/eos_sdk
directory, else stub mode. Secrets are never logged. The public
repo never contains SDK binaries or credentials.

Required secrets for full builds: EOS_SDK_URL, EOS_SDK_TOKEN.
Optional: EOS_SDK_ANDROID_URL, EOS_SDK_IOS_URL when mobile
packages differ from desktop.
