Place the EOS SDK here. This directory is git-ignored.

Expected layout:

third_party/eos_sdk/SDK/Include/eos_sdk.h
third_party/eos_sdk/SDK/Bin/<Platform>/...

Or set EOS_SDK_DIR to an SDK checkout elsewhere. In CI the SDK can
arrive through EOS_SDK_URL and EOS_SDK_TOKEN secrets. Nothing
proprietary is ever committed. See docs/installation.md.
