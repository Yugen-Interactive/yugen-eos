API

Entry: EOS.initialize() returns a result Dictionary and emits
initialized. EOS.tick runs on a timer and drains native callbacks.
EOS.shutdown releases handles and unloads the library.

Conventions: every call returns a Dictionary with code, name,
message, operation, context, ok. Async calls return
EOS_RequestInProgress immediately and deliver the final result
through operation_completed plus a per-interface signal. Await the
signal:

var result: Dictionary = EOS.auth.login(1, id, token)
if int(result.get("code", 0)) == 39:
    result = await EOS.auth.login_completed

Identity strings: Epic Account IDs and Product User IDs travel as
strings. Session search results travel as integer session_ref
values owned by a native registry.

Auth: EOS.auth.login(login_type, id, token), logout(local_user_id),
verify(local_user_id). Types follow EOS_ELoginCredentialType.

Connect: EOS.connect_iface.login(credential_type, token,
display_name), logout(product_user_id), create_user(token).

Social graph: EOS.friends.get_friends, get_friend_count,
send_invite. EOS.presence.set_presence, query_presence,
has_presence. EOS.userinfo.query, get_display_name.

Multiplayer: EOS.sessions.create, search, join, EOS.lobbies.create,
join, leave, send_invite. EOS.p2p.send_packet, receive_packet,
set_relay.

Progression: EOS.stats.query, ingest. EOS.achievements.unlock,
query. EOS.leaderboards.query_definitions, query_ranks,
query_user_scores. Scores are submitted through stats ingest, which
mirrors the EOS design where leaderboards are read models.

Services: EOS.storage write, read, title read, EOS.ecom offers and
checkout, EOS.ui overlay, EOS.social reports, sanctions, rtc,
anticheat, metrics.

The C# story: the GDExtension classes are visible to Godot.NET
through the standard interop once the native library loads. No
separate binding is shipped in 0.1.0.
