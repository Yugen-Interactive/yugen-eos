import os
import sys

EOS_SDK_DIR = os.environ.get("EOS_SDK_DIR", "third_party/eos_sdk")
GODOT_CPP_DIR = os.environ.get("GODOT_CPP_DIR", "godot-cpp")
TARGET_PLATFORM = os.environ.get("YUGEN_EOS_PLATFORM", "windows")
TARGET_ARCH = os.environ.get("YUGEN_EOS_ARCH", "x86_64")

godot_cpp_sconstruct = os.path.join(GODOT_CPP_DIR, "SConstruct")
if not os.path.isfile(godot_cpp_sconstruct):
    print("godot-cpp not found at %s. Clone it or set GODOT_CPP_DIR." % GODOT_CPP_DIR)
    print("Clone: git clone --branch godot-4.3-stable --depth 1 https://github.com/godotengine/godot-cpp.git")
    sys.exit(2)

env = SConscript(os.path.join(GODOT_CPP_DIR, "SConstruct"))

sdk_include = os.path.join(EOS_SDK_DIR, "SDK", "Include")
alt_include = os.path.join(EOS_SDK_DIR, "Include")
has_sdk = os.path.isfile(os.path.join(sdk_include, "eos_sdk.h"))
if not has_sdk:
    has_sdk = os.path.isfile(os.path.join(alt_include, "eos_sdk.h"))

defines = ["YUGEN_EOS_VERSION=\"0.1.0\"", "YUGEN_EOS_SDK_VERSION=\"1.19.1\""]
if has_sdk:
    defines.append("YUGEN_EOS_HAS_SDK=1")
    if os.path.isdir(sdk_include):
        env.Append(CPPPATH=[sdk_include])
    if os.path.isdir(alt_include):
        env.Append(CPPPATH=[alt_include])
    print("YugenEOS: EOS SDK headers found, building with native SDK support.")
else:
    defines.append("YUGEN_EOS_HAS_SDK=0")
    print("YugenEOS: EOS SDK headers not found, building in stub mode.")

env.Append(CPPDEFINES=defines)
env.Append(CPPPATH=["src"])

sources = Glob("src/*.cpp") + Glob("src/core/*.cpp") + Glob("src/interfaces/*.cpp")
library_name = "yugen_eos.%s.%s" % (TARGET_PLATFORM, TARGET_ARCH)
library = env.SharedLibrary("bin/%s%s" % (library_name, env["SHLIBSUFFIX"]), source=sources)
Default(library)
