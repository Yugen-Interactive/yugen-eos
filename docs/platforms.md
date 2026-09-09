Platforms

Windows. Fully built and linked locally, debug and stub modes.
Runtime needs EOSSDK-Win64-Shipping.dll beside the binary.

Linux. Builds in CI on ubuntu runners. Runtime needs
libEOSSDK-Linux-Shipping.so. Overlay UI is unavailable where EOS
does not ship it.

macOS. Builds in CI on macos runners, universal arch. Runtime needs
libEOSSDK-Mac-Shipping.dylib. Notarization is the developer's job.

Android. Builds in CI with the Android SDK package, arm64 and
arm32. CI installs NDK 23.2.8568313, the version pinned by
godot-cpp 4.3. Runtime uses libEOSSDK.so from the aar. Needs Studio
credentials, manifest entries, and the DevAuthTool flow documented
in the SDK samples.

iOS. Builds in CI on macos runners with the iOS SDK xcframework.
Device signing and entitlements are outside this wrapper.

Every platform loads the native library at runtime through the
same loader. A missing library is a configuration error, never a
crash. Interfaces that EOS does not provide on a platform return
EOS_NotImplemented with the interface name in context, and
is_available reports false.
