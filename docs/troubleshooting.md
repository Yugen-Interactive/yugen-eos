Troubleshooting

Native library not loaded. get_diagnostics shows native_loaded
false and loader_error text. Place the platform library next to
the binary with the exact name from docs/architecture.md.

EOS_NotConfigured on initialize. Product name or version empty, or
the library failed to load. The context reason says which.

EOS_InvalidUser on id calls. The id string is not a valid EOS id.
Round-trip ids through the login result instead of typing them.

Overlay does nothing. The Social Overlay needs Dev Portal
configuration and a platform where EOS ships it. Headless and
server builds return NotImplemented.

Callbacks never arrive. tick must run. The autoload timer drives
it. If you disabled the autoload, call EOS.tick on _process.

Stale search handles. Session refs die with the interface owner.
Re-run search after scene changes that free the EOS node.

Android login loops. Device ID and credential types need the
manifest and keystore from the SDK samples. Check logcat for the
native loader line first.

iOS framework missing at runtime. Embed EOSSDK.xcframework in the
export preset. The wrapper links nothing at build time, so Xcode
will not warn you.
