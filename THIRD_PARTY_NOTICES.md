Yugen EOS wrapper license notes

Wrapper license
MIT. See LICENSE.

EOS SDK license and restrictions
Epic Online Services SDK is proprietary software owned by Epic Games, Inc.
Its use is governed by the Epic Online Services Terms, the Epic Developer
Portal agreement, and the license bundled with the SDK download.
This repository never redistributes EOS binaries or headers.

Files the user must provide
Download the EOS SDK from the Epic Developer Portal and place it under
third_party/eos_sdk, or set EOS_SDK_DIR, or provide EOS_SDK_URL plus
EOS_SDK_TOKEN in CI. Required layout: SDK/Include/eos_sdk.h plus the
platform binary for each target (Windows dll, Linux so, macOS dylib,
Android aar, iOS xcframework).

What this project may distribute
Wrapper source code, build scripts, generator, tests, examples, docs.

What this project must not distribute
EOS headers, EOS static or dynamic libraries, DevAuthTool credentials,
ProductId, SandboxId, DeploymentId, ClientId, ClientSecret, encryption
keys, or any credential belonging to a real product.
