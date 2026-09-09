Installation

Requirements: Godot 4.3 or newer, Python 3.10 or newer, SCons 4,
a C++17 compiler, and the EOS SDK 1.19.1 from the Epic Developer
Portal.

1. Clone this repository and godot-cpp at branch godot-4.3-stable
next to it, or set GODOT_CPP_DIR.

2. Place the EOS SDK so that SDK/Include/eos_sdk.h resolves. Either
extract it under third_party/eos_sdk, which is git-ignored, or set
EOS_SDK_DIR to its location. See third_party/eos_sdk/README.md.

3. Build the extension:

scons platform=windows arch=x86_64

Cross targets use their platform name, for example
platform=linux, platform=macos, platform=android, platform=ios.
Android and iOS need their SDK packages and toolchains, see
docs/platforms.md and the workflow files.

4. Copy the matching EOS native library next to the game binary or
install path. The wrapper loads it at runtime and reports a clear
error when it is absent.

5. Open the project in Godot, enable the YugenEOS plugin, and keep
the EOS autoload active. Configure ids in Project Settings under
yugen_eos. Secrets come from environment variables
EOS_CLIENT_SECRET and EOS_ENCRYPTION_KEY, never from settings files.

Without the SDK the build still succeeds in stub mode and every
call returns EOS_NotImplemented with a reason. The GDScript API is
identical, so games boot and menus render without credentials.
