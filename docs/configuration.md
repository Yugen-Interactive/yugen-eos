Configuration

Project Settings keys, all under yugen_eos: product_id, sandbox_id,
deployment_id, client_id, product_name, product_version,
tick_interval_ms, log_level. The editor plugin registers them with
defaults on first enable.

YugenEOSConfig is a Resource with the same fields plus
cache_directory. client_secret and encryption_key are plain members
that load_from_project_settings fills only from environment
variables. They are never written to disk by the wrapper and never
printed by the logger.

Minimal GDScript boot:

var result: Dictionary = await EOS.initialize()
if not bool(result.get("ok", false)):
    push_error(result.get("message"))

initialize returns EOS_AlreadyConfigured on double init and
EOS_NotConfigured with a reason when ids are missing or the native
library failed to load. Diagnostics are available at any time:

var diag: Dictionary = EOS.get_diagnostics()

It reports initialized state, sdk version, platform, pending
callbacks, native load state, and recent errors without secrets.
