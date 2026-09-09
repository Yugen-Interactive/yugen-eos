#include "interfaces/eos_multiplayer.hpp"

#include <cstring>
#include <map>

#if YUGEN_EOS_HAS_SDK
#include "eos_common.h"
#include "eos_sdk.h"
#include "eos_sessions.h"
#include "eos_lobby.h"
#include "eos_p2p.h"
#include "eos_p2p_types.h"
#endif

namespace yugen {

void EOSSessionsInterface::_bind_methods() {
    godot::ClassDB::bind_method(godot::D_METHOD("sessions_create"), &EOSSessionsInterface::sessions_create);
    godot::ClassDB::bind_method(godot::D_METHOD("sessions_search"), &EOSSessionsInterface::sessions_search);
    godot::ClassDB::bind_method(godot::D_METHOD("sessions_get_search_count"), &EOSSessionsInterface::sessions_get_search_count);
    godot::ClassDB::bind_method(godot::D_METHOD("sessions_get_search_result"), &EOSSessionsInterface::sessions_get_search_result);
    godot::ClassDB::bind_method(godot::D_METHOD("sessions_join"), &EOSSessionsInterface::sessions_join);
    godot::ClassDB::bind_method(godot::D_METHOD("sessions_destroy"), &EOSSessionsInterface::sessions_destroy);
    ADD_SIGNAL(godot::MethodInfo("session_created", godot::PropertyInfo(godot::Variant::DICTIONARY, "result")));
    ADD_SIGNAL(godot::MethodInfo("session_joined", godot::PropertyInfo(godot::Variant::DICTIONARY, "result")));
    ADD_SIGNAL(godot::MethodInfo("search_completed", godot::PropertyInfo(godot::Variant::DICTIONARY, "result")));
}

void EOSLobbiesInterface::_bind_methods() {
    godot::ClassDB::bind_method(godot::D_METHOD("lobbies_create"), &EOSLobbiesInterface::lobbies_create);
    godot::ClassDB::bind_method(godot::D_METHOD("lobbies_join"), &EOSLobbiesInterface::lobbies_join);
    godot::ClassDB::bind_method(godot::D_METHOD("lobbies_leave"), &EOSLobbiesInterface::lobbies_leave);
    godot::ClassDB::bind_method(godot::D_METHOD("lobbies_send_invite"), &EOSLobbiesInterface::lobbies_send_invite);
    ADD_SIGNAL(godot::MethodInfo("lobby_created", godot::PropertyInfo(godot::Variant::DICTIONARY, "result")));
    ADD_SIGNAL(godot::MethodInfo("lobby_joined", godot::PropertyInfo(godot::Variant::DICTIONARY, "result")));
}

void EOSP2PInterface::_bind_methods() {
    godot::ClassDB::bind_method(godot::D_METHOD("p2p_send"), &EOSP2PInterface::p2p_send);
    godot::ClassDB::bind_method(godot::D_METHOD("p2p_receive"), &EOSP2PInterface::p2p_receive);
    godot::ClassDB::bind_method(godot::D_METHOD("p2p_set_relay"), &EOSP2PInterface::p2p_set_relay);
    godot::ClassDB::bind_method(godot::D_METHOD("p2p_query_nat"), &EOSP2PInterface::p2p_query_nat);
    ADD_SIGNAL(godot::MethodInfo("packet_received", godot::PropertyInfo(godot::Variant::DICTIONARY, "result")));
}

EOSSessionsInterface::~EOSSessionsInterface() {
#if YUGEN_EOS_HAS_SDK
    for (auto &entry : session_registry) {
        bindings()->EOS_SessionDetails_Release((EOS_HSessionDetails)entry.second);
    }
    session_registry.clear();
    if (active_search != nullptr) {
        bindings()->EOS_SessionSearch_Release((EOS_HSessionSearch)active_search);
        active_search = nullptr;
    }
#endif
}

EOSLobbiesInterface::~EOSLobbiesInterface() {
#if YUGEN_EOS_HAS_SDK
    if (active_search != nullptr) {
        bindings()->EOS_LobbySearch_Release((EOS_HLobbySearch)active_search);
        active_search = nullptr;
    }
#endif
}

#if YUGEN_EOS_HAS_SDK
static void sessions_create_cb(const EOS_Sessions_UpdateSessionCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}

static void sessions_find_cb(const EOS_SessionSearch_FindCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}

static void sessions_join_cb(const EOS_Sessions_JoinSessionCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}

static void sessions_destroy_cb(const EOS_Sessions_DestroySessionCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}

static void lobby_create_cb(const EOS_Lobby_CreateLobbyCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    if (data->LobbyId != nullptr) {
        payload["lobby_id"] = godot::String(data->LobbyId);
    }
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}

static void lobby_join_cb(const EOS_Lobby_JoinLobbyByIdCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    if (data->LobbyId != nullptr) {
        payload["lobby_id"] = godot::String(data->LobbyId);
    }
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}

static void lobby_leave_cb(const EOS_Lobby_LeaveLobbyCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}

static void lobby_invite_cb(const EOS_Lobby_SendInviteCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}
#endif

godot::Dictionary EOSSessionsInterface::sessions_create(const godot::String &local_user_id, const godot::String &session_name, int64_t max_players, bool is_public) {
    godot::Dictionary ready = require_ready("sessions.create");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("sessions.create", "Platform handle not created yet");
    }
    EOS_HSessions sessions = bindings()->EOS_Platform_GetSessionsInterface(h);
    if (sessions == nullptr) {
        return not_implemented("sessions.create", "Sessions interface unavailable on this platform");
    }
    EOS_ProductUserId user = bindings()->EOS_ProductUserId_FromString(local_user_id.utf8().get_data());
    if (bindings()->EOS_ProductUserId_IsValid(user) == EOS_FALSE) {
        godot::Dictionary context;
        context["reason"] = "Invalid Product User ID string";
        return EOSResult::make_result(10, "sessions.create", context);
    }
    godot::CharString name_utf = session_name.utf8();
    EOS_Sessions_CreateSessionModificationOptions mod_options = {};
    mod_options.ApiVersion = EOS_SESSIONS_CREATESESSIONMODIFICATION_API_LATEST;
    mod_options.SessionName = name_utf.get_data();
    mod_options.BucketId = "yugen_game";
    mod_options.MaxPlayers = (uint32_t)max_players;
    mod_options.LocalUserId = user;
    mod_options.bPresenceEnabled = EOS_TRUE;
    EOS_HSessionModification mod = nullptr;
    EOS_EResult mod_result = bindings()->EOS_Sessions_CreateSessionModification(sessions, &mod_options, &mod);
    if (mod_result != EOS_EResult::EOS_Success || mod == nullptr) {
        godot::Dictionary context;
        context["native_code"] = (int64_t)mod_result;
        return EOSResult::make_result((int64_t)mod_result, "sessions.create", context);
    }
    EOS_SessionModification_SetPermissionLevelOptions perm_options = {};
    perm_options.ApiVersion = EOS_SESSIONMODIFICATION_SETPERMISSIONLEVEL_API_LATEST;
    perm_options.PermissionLevel = is_public ? EOS_EOnlineSessionPermissionLevel::EOS_OSPF_PublicAdvertised : EOS_EOnlineSessionPermissionLevel::EOS_OSPF_JoinViaPresence;
    bindings()->EOS_SessionModification_SetPermissionLevel(mod, &perm_options);
    godot::Dictionary context;
    context["session_name"] = session_name;
    RequestContext *ctx = make_request("sessions.create", context);
    EOS_Sessions_UpdateSessionOptions update_options = {};
    update_options.ApiVersion = EOS_SESSIONS_UPDATESESSION_API_LATEST;
    update_options.SessionModificationHandle = mod;
    bindings()->EOS_Sessions_UpdateSession(sessions, &update_options, ctx, sessions_create_cb);
    bindings()->EOS_SessionModification_Release(mod);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "sessions.create";
    return pending;
#else
    return not_implemented("sessions.create", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSSessionsInterface::sessions_search(const godot::Dictionary &options) {
    godot::Dictionary ready = require_ready("sessions.search");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("sessions.search", "Platform handle not created yet");
    }
    EOS_HSessions sessions = bindings()->EOS_Platform_GetSessionsInterface(h);
    if (sessions == nullptr) {
        return not_implemented("sessions.search", "Sessions interface unavailable on this platform");
    }
    if (active_search != nullptr) {
        bindings()->EOS_SessionSearch_Release((EOS_HSessionSearch)active_search);
        active_search = nullptr;
    }
    EOS_Sessions_CreateSessionSearchOptions create_options = {};
    create_options.ApiVersion = EOS_SESSIONS_CREATESESSIONSEARCH_API_LATEST;
    create_options.MaxSearchResults = (uint32_t)(int64_t)options.get("max_results", 10);
    EOS_HSessionSearch search = nullptr;
    EOS_EResult create_result = bindings()->EOS_Sessions_CreateSessionSearch(sessions, &create_options, &search);
    if (create_result != EOS_EResult::EOS_Success || search == nullptr) {
        godot::Dictionary context;
        context["native_code"] = (int64_t)create_result;
        return EOSResult::make_result((int64_t)create_result, "sessions.search", context);
    }
    active_search = search;
    godot::Dictionary context;
    RequestContext *ctx = make_request("sessions.search", context);
    EOS_SessionSearch_FindOptions find_options = {};
    find_options.ApiVersion = EOS_SESSIONSEARCH_FIND_API_LATEST;
    godot::String local_id = options.get("local_user_id", "");
    EOS_ProductUserId user = nullptr;
    godot::CharString local_utf;
    if (!local_id.is_empty()) {
        local_utf = local_id.utf8();
        user = bindings()->EOS_ProductUserId_FromString(local_utf.get_data());
    }
    find_options.LocalUserId = user;
    bindings()->EOS_SessionSearch_Find(search, &find_options, ctx, sessions_find_cb);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "sessions.search";
    return pending;
#else
    return not_implemented("sessions.search", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSSessionsInterface::sessions_get_search_count() {
#if YUGEN_EOS_HAS_SDK
    godot::Dictionary ready = require_ready("sessions.get_search_count");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
    if (active_search == nullptr) {
        godot::Dictionary context;
        context["reason"] = "No active search. Call sessions.search first.";
        return EOSResult::make_result(38, "sessions.get_search_count", context);
    }
    EOS_SessionSearch_GetSearchResultCountOptions options = {};
    options.ApiVersion = EOS_SESSIONSEARCH_GETSEARCHRESULTCOUNT_API_LATEST;
    int32_t count = bindings()->EOS_SessionSearch_GetSearchResultCount((EOS_HSessionSearch)active_search, &options);
    godot::Dictionary out = EOSResult::make_result(0, "sessions.get_search_count", godot::Dictionary());
    out["count"] = (int64_t)count;
    return out;
#else
    return not_implemented("sessions.get_search_count", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSSessionsInterface::sessions_get_search_result(int64_t index) {
#if YUGEN_EOS_HAS_SDK
    godot::Dictionary ready = require_ready("sessions.get_search_result");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
    if (active_search == nullptr) {
        godot::Dictionary context;
        context["reason"] = "No active search. Call sessions.search first.";
        return EOSResult::make_result(38, "sessions.get_search_result", context);
    }
    EOS_SessionSearch_CopySearchResultByIndexOptions options = {};
    options.ApiVersion = EOS_SESSIONSEARCH_COPYSEARCHRESULTBYINDEX_API_LATEST;
    options.SessionIndex = (uint32_t)index;
    EOS_HSessionDetails details = nullptr;
    EOS_EResult copy_result = bindings()->EOS_SessionSearch_CopySearchResultByIndex((EOS_HSessionSearch)active_search, &options, &details);
    if (copy_result != EOS_EResult::EOS_Success || details == nullptr) {
        godot::Dictionary context;
        context["native_code"] = (int64_t)copy_result;
        return EOSResult::make_result((int64_t)copy_result, "sessions.get_search_result", context);
    }
    int64_t ref = next_registry_id++;
    session_registry[ref] = details;
    godot::Dictionary out = EOSResult::make_result(0, "sessions.get_search_result", godot::Dictionary());
    out["session_ref"] = ref;
    return out;
#else
    return not_implemented("sessions.get_search_result", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSSessionsInterface::sessions_join(const godot::String &local_user_id, int64_t session_ref) {
    godot::Dictionary ready = require_ready("sessions.join");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("sessions.join", "Platform handle not created yet");
    }
    EOS_HSessions sessions = bindings()->EOS_Platform_GetSessionsInterface(h);
    if (sessions == nullptr) {
        return not_implemented("sessions.join", "Sessions interface unavailable on this platform");
    }
    auto it = session_registry.find(session_ref);
    if (it == session_registry.end()) {
        godot::Dictionary context;
        context["reason"] = "Unknown session_ref. Use sessions.get_search_result first.";
        return EOSResult::make_result(10, "sessions.join", context);
    }
    EOS_ProductUserId user = bindings()->EOS_ProductUserId_FromString(local_user_id.utf8().get_data());
    if (bindings()->EOS_ProductUserId_IsValid(user) == EOS_FALSE) {
        godot::Dictionary context;
        context["reason"] = "Invalid Product User ID string";
        return EOSResult::make_result(10, "sessions.join", context);
    }
    godot::Dictionary context;
    context["session_ref"] = session_ref;
    RequestContext *ctx = make_request("sessions.join", context);
    EOS_Sessions_JoinSessionOptions options = {};
    options.ApiVersion = EOS_SESSIONS_JOINSESSION_API_LATEST;
    options.SessionName = "yugen_session";
    options.SessionHandle = (EOS_HSessionDetails)it->second;
    options.LocalUserId = user;
    options.bPresenceEnabled = EOS_TRUE;
    bindings()->EOS_Sessions_JoinSession(sessions, &options, ctx, sessions_join_cb);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "sessions.join";
    return pending;
#else
    return not_implemented("sessions.join", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSSessionsInterface::sessions_destroy(const godot::String &session_name) {
    godot::Dictionary ready = require_ready("sessions.destroy");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("sessions.destroy", "Platform handle not created yet");
    }
    EOS_HSessions sessions = bindings()->EOS_Platform_GetSessionsInterface(h);
    if (sessions == nullptr) {
        return not_implemented("sessions.destroy", "Sessions interface unavailable on this platform");
    }
    godot::Dictionary context;
    context["session_name"] = session_name;
    RequestContext *ctx = make_request("sessions.destroy", context);
    godot::CharString name_utf = session_name.utf8();
    EOS_Sessions_DestroySessionOptions options = {};
    options.ApiVersion = EOS_SESSIONS_DESTROYSESSION_API_LATEST;
    options.SessionName = name_utf.get_data();
    bindings()->EOS_Sessions_DestroySession(sessions, &options, ctx, sessions_destroy_cb);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "sessions.destroy";
    return pending;
#else
    return not_implemented("sessions.destroy", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSLobbiesInterface::lobbies_create(const godot::String &local_user_id, int64_t max_members, bool is_public) {
    godot::Dictionary ready = require_ready("lobbies.create");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("lobbies.create", "Platform handle not created yet");
    }
    EOS_HLobby lobby = bindings()->EOS_Platform_GetLobbyInterface(h);
    if (lobby == nullptr) {
        return not_implemented("lobbies.create", "Lobby interface unavailable on this platform");
    }
    EOS_ProductUserId user = bindings()->EOS_ProductUserId_FromString(local_user_id.utf8().get_data());
    if (bindings()->EOS_ProductUserId_IsValid(user) == EOS_FALSE) {
        godot::Dictionary context;
        context["reason"] = "Invalid Product User ID string";
        return EOSResult::make_result(10, "lobbies.create", context);
    }
    godot::Dictionary context;
    RequestContext *ctx = make_request("lobbies.create", context);
    EOS_Lobby_CreateLobbyOptions options = {};
    options.ApiVersion = EOS_LOBBY_CREATELOBBY_API_LATEST;
    options.LocalUserId = user;
    options.MaxLobbyMembers = (uint32_t)max_members;
    options.PermissionLevel = is_public ? EOS_ELobbyPermissionLevel::EOS_LPL_PUBLICADVERTISED : EOS_ELobbyPermissionLevel::EOS_LPL_JOINVIAPRESENCE;
    options.bPresenceEnabled = EOS_TRUE;
    options.bAllowInvites = EOS_TRUE;
    bindings()->EOS_Lobby_CreateLobby(lobby, &options, ctx, lobby_create_cb);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "lobbies.create";
    return pending;
#else
    return not_implemented("lobbies.create", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSLobbiesInterface::lobbies_join(const godot::String &lobby_id, const godot::String &local_user_id) {
    godot::Dictionary ready = require_ready("lobbies.join");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("lobbies.join", "Platform handle not created yet");
    }
    EOS_HLobby lobby = bindings()->EOS_Platform_GetLobbyInterface(h);
    if (lobby == nullptr) {
        return not_implemented("lobbies.join", "Lobby interface unavailable on this platform");
    }
    EOS_ProductUserId user = bindings()->EOS_ProductUserId_FromString(local_user_id.utf8().get_data());
    if (bindings()->EOS_ProductUserId_IsValid(user) == EOS_FALSE) {
        godot::Dictionary context;
        context["reason"] = "Invalid Product User ID string";
        return EOSResult::make_result(10, "lobbies.join", context);
    }
    godot::Dictionary context;
    context["lobby_id"] = lobby_id;
    RequestContext *ctx = make_request("lobbies.join", context);
    godot::CharString lobby_utf = lobby_id.utf8();
    EOS_Lobby_JoinLobbyByIdOptions options = {};
    options.ApiVersion = EOS_LOBBY_JOINLOBBYBYID_API_LATEST;
    options.LobbyId = lobby_utf.get_data();
    options.LocalUserId = user;
    options.bPresenceEnabled = EOS_TRUE;
    bindings()->EOS_Lobby_JoinLobbyById(lobby, &options, ctx, lobby_join_cb);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "lobbies.join";
    return pending;
#else
    return not_implemented("lobbies.join", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSLobbiesInterface::lobbies_leave(const godot::String &lobby_id, const godot::String &local_user_id) {
    godot::Dictionary ready = require_ready("lobbies.leave");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("lobbies.leave", "Platform handle not created yet");
    }
    EOS_HLobby lobby = bindings()->EOS_Platform_GetLobbyInterface(h);
    if (lobby == nullptr) {
        return not_implemented("lobbies.leave", "Lobby interface unavailable on this platform");
    }
    EOS_ProductUserId user = bindings()->EOS_ProductUserId_FromString(local_user_id.utf8().get_data());
    godot::Dictionary context;
    context["lobby_id"] = lobby_id;
    RequestContext *ctx = make_request("lobbies.leave", context);
    godot::CharString lobby_utf = lobby_id.utf8();
    EOS_Lobby_LeaveLobbyOptions options = {};
    options.ApiVersion = EOS_LOBBY_LEAVELOBBY_API_LATEST;
    options.LobbyId = lobby_utf.get_data();
    options.LocalUserId = user;
    bindings()->EOS_Lobby_LeaveLobby(lobby, &options, ctx, lobby_leave_cb);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "lobbies.leave";
    return pending;
#else
    return not_implemented("lobbies.leave", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSLobbiesInterface::lobbies_send_invite(const godot::String &lobby_id, const godot::String &local_user_id, const godot::String &target_user_id) {
    godot::Dictionary ready = require_ready("lobbies.send_invite");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("lobbies.send_invite", "Platform handle not created yet");
    }
    EOS_HLobby lobby = bindings()->EOS_Platform_GetLobbyInterface(h);
    if (lobby == nullptr) {
        return not_implemented("lobbies.send_invite", "Lobby interface unavailable on this platform");
    }
    EOS_ProductUserId local = bindings()->EOS_ProductUserId_FromString(local_user_id.utf8().get_data());
    EOS_ProductUserId target = bindings()->EOS_ProductUserId_FromString(target_user_id.utf8().get_data());
    if (bindings()->EOS_ProductUserId_IsValid(local) == EOS_FALSE || bindings()->EOS_ProductUserId_IsValid(target) == EOS_FALSE) {
        godot::Dictionary context;
        context["reason"] = "Invalid Product User ID string";
        return EOSResult::make_result(10, "lobbies.send_invite", context);
    }
    godot::Dictionary context;
    context["lobby_id"] = lobby_id;
    RequestContext *ctx = make_request("lobbies.send_invite", context);
    godot::CharString lobby_utf = lobby_id.utf8();
    EOS_Lobby_SendInviteOptions options = {};
    options.ApiVersion = EOS_LOBBY_SENDINVITE_API_LATEST;
    options.LobbyId = lobby_utf.get_data();
    options.LocalUserId = local;
    options.TargetUserId = target;
    bindings()->EOS_Lobby_SendInvite(lobby, &options, ctx, lobby_invite_cb);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "lobbies.send_invite";
    return pending;
#else
    return not_implemented("lobbies.send_invite", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSP2PInterface::p2p_send(const godot::String &local_user_id, const godot::String &remote_user_id, const godot::String &socket_id, const godot::PackedByteArray &data, bool reliable) {
    godot::Dictionary ready = require_ready("p2p.send");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("p2p.send", "Platform handle not created yet");
    }
    EOS_HP2P p2p = bindings()->EOS_Platform_GetP2PInterface(h);
    if (p2p == nullptr) {
        return not_implemented("p2p.send", "P2P interface unavailable on this platform");
    }
    EOS_ProductUserId local = bindings()->EOS_ProductUserId_FromString(local_user_id.utf8().get_data());
    EOS_ProductUserId remote = bindings()->EOS_ProductUserId_FromString(remote_user_id.utf8().get_data());
    if (bindings()->EOS_ProductUserId_IsValid(local) == EOS_FALSE || bindings()->EOS_ProductUserId_IsValid(remote) == EOS_FALSE) {
        godot::Dictionary context;
        context["reason"] = "Invalid Product User ID string";
        return EOSResult::make_result(10, "p2p.send", context);
    }
    godot::CharString socket_utf = socket_id.utf8();
    EOS_P2P_SocketId socket = {};
    socket.ApiVersion = EOS_P2P_SOCKETID_API_LATEST;
    socket.SocketName[0] = '\0';
    strncpy(socket.SocketName, socket_utf.get_data(), EOS_P2P_SOCKETID_SOCKETNAME_SIZE - 1);
    EOS_P2P_SendPacketOptions options = {};
    options.ApiVersion = EOS_P2P_SENDPACKET_API_LATEST;
    options.LocalUserId = local;
    options.RemoteUserId = remote;
    options.SocketId = &socket;
    options.Channel = 0;
    options.DataLengthBytes = (uint32_t)data.size();
    options.Data = data.ptr();
    options.bAllowDelayedDelivery = EOS_TRUE;
    options.Reliability = reliable ? EOS_EPacketReliability::EOS_PR_ReliableUnordered : EOS_EPacketReliability::EOS_PR_UnreliableUnordered;
    options.bDisableAutoAcceptConnection = EOS_FALSE;
    EOS_EResult send_result = bindings()->EOS_P2P_SendPacket(p2p, &options);
    godot::Dictionary context;
    context["bytes"] = (int64_t)data.size();
    return EOSResult::make_result((int64_t)send_result, "p2p.send", context);
#else
    return not_implemented("p2p.send", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSP2PInterface::p2p_receive(const godot::String &local_user_id, const godot::String &socket_id) {
    godot::Dictionary ready = require_ready("p2p.receive");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("p2p.receive", "Platform handle not created yet");
    }
    EOS_HP2P p2p = bindings()->EOS_Platform_GetP2PInterface(h);
    if (p2p == nullptr) {
        return not_implemented("p2p.receive", "P2P interface unavailable on this platform");
    }
    EOS_ProductUserId local = bindings()->EOS_ProductUserId_FromString(local_user_id.utf8().get_data());
    if (bindings()->EOS_ProductUserId_IsValid(local) == EOS_FALSE) {
        godot::Dictionary context;
        context["reason"] = "Invalid Product User ID string";
        return EOSResult::make_result(10, "p2p.receive", context);
    }
    EOS_P2P_GetNextReceivedPacketSizeOptions size_options = {};
    size_options.ApiVersion = EOS_P2P_GETNEXTRECEIVEDPACKETSIZE_API_LATEST;
    size_options.LocalUserId = local;
    uint32_t next_size = 0;
    EOS_EResult size_result = bindings()->EOS_P2P_GetNextReceivedPacketSize(p2p, &size_options, &next_size);
    godot::Dictionary context;
    if (size_result != EOS_EResult::EOS_Success || next_size == 0) {
        context["reason"] = "No packet available";
        godot::Dictionary out = EOSResult::make_result((int64_t)size_result, "p2p.receive", context);
        out["has_packet"] = false;
        return out;
    }
    godot::PackedByteArray buffer;
    buffer.resize(next_size);
    EOS_P2P_ReceivePacketOptions recv_options = {};
    recv_options.ApiVersion = EOS_P2P_RECEIVEPACKET_API_LATEST;
    recv_options.LocalUserId = local;
    recv_options.MaxDataSizeBytes = next_size;
    EOS_ProductUserId peer = nullptr;
    EOS_P2P_SocketId out_socket = {};
    out_socket.ApiVersion = EOS_P2P_SOCKETID_API_LATEST;
    uint8_t channel = 0;
    uint32_t bytes_written = 0;
    EOS_EResult recv_result = bindings()->EOS_P2P_ReceivePacket(p2p, &recv_options, &peer, &out_socket, &channel, buffer.ptrw(), &bytes_written);
    godot::Dictionary out = EOSResult::make_result((int64_t)recv_result, "p2p.receive", context);
    if (recv_result == EOS_EResult::EOS_Success) {
        buffer.resize(bytes_written);
        out["has_packet"] = true;
        out["data"] = buffer;
        out["channel"] = (int64_t)channel;
        out["socket"] = godot::String(out_socket.SocketName);
        if (peer != nullptr) {
            char buf[64] = {};
            int32_t len = (int32_t)sizeof(buf);
            if (bindings()->EOS_ProductUserId_ToString(peer, buf, &len) == EOS_EResult::EOS_Success) {
                out["peer_id"] = godot::String(buf);
            }
        }
    } else {
        out["has_packet"] = false;
    }
    return out;
#else
    return not_implemented("p2p.receive", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSP2PInterface::p2p_set_relay(const godot::String &local_user_id, bool allow_relays) {
    godot::Dictionary ready = require_ready("p2p.set_relay");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("p2p.set_relay", "Platform handle not created yet");
    }
    EOS_HP2P p2p = bindings()->EOS_Platform_GetP2PInterface(h);
    if (p2p == nullptr) {
        return not_implemented("p2p.set_relay", "P2P interface unavailable on this platform");
    }
    EOS_P2P_SetRelayControlOptions options = {};
    options.ApiVersion = EOS_P2P_SETRELAYCONTROL_API_LATEST;
    options.RelayControl = allow_relays ? EOS_ERelayControl::EOS_RC_AllowRelays : EOS_ERelayControl::EOS_RC_NoRelays;
    EOS_EResult result = bindings()->EOS_P2P_SetRelayControl(p2p, &options);
    return EOSResult::make_result((int64_t)result, "p2p.set_relay", godot::Dictionary());
#else
    return not_implemented("p2p.set_relay", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSP2PInterface::p2p_query_nat() {
    godot::Dictionary ready = require_ready("p2p.query_nat");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("p2p.query_nat", "Platform handle not created yet");
    }
    EOS_HP2P p2p = bindings()->EOS_Platform_GetP2PInterface(h);
    if (p2p == nullptr) {
        return not_implemented("p2p.query_nat", "P2P interface unavailable on this platform");
    }
    EOS_P2P_GetNATTypeOptions options = {};
    options.ApiVersion = EOS_P2P_GETNATTYPE_API_LATEST;
    EOS_ENATType nat = EOS_ENATType::EOS_NAT_Unknown;
    EOS_EResult nat_result = bindings()->EOS_P2P_GetNATType(p2p, &options, &nat);
    godot::Dictionary out = EOSResult::make_result((int64_t)nat_result, "p2p.query_nat", godot::Dictionary());
    out["nat_type"] = (int64_t)nat;
    return out;
#else
    return not_implemented("p2p.query_nat", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

}
