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
    godot::ClassDB::bind_method(godot::D_METHOD("sessions_send_invite"), &EOSSessionsInterface::sessions_send_invite);
    godot::ClassDB::bind_method(godot::D_METHOD("sessions_query_invites"), &EOSSessionsInterface::sessions_query_invites);
    godot::ClassDB::bind_method(godot::D_METHOD("sessions_reject_invite"), &EOSSessionsInterface::sessions_reject_invite);
    godot::ClassDB::bind_method(godot::D_METHOD("sessions_set_attributes"), &EOSSessionsInterface::sessions_set_attributes);
    godot::ClassDB::bind_method(godot::D_METHOD("sessions_get_attributes"), &EOSSessionsInterface::sessions_get_attributes);
    ADD_SIGNAL(godot::MethodInfo("session_created", godot::PropertyInfo(godot::Variant::DICTIONARY, "result")));
    ADD_SIGNAL(godot::MethodInfo("session_joined", godot::PropertyInfo(godot::Variant::DICTIONARY, "result")));
    ADD_SIGNAL(godot::MethodInfo("search_completed", godot::PropertyInfo(godot::Variant::DICTIONARY, "result")));
}

void EOSLobbiesInterface::_bind_methods() {
    godot::ClassDB::bind_method(godot::D_METHOD("lobbies_create"), &EOSLobbiesInterface::lobbies_create);
    godot::ClassDB::bind_method(godot::D_METHOD("lobbies_join"), &EOSLobbiesInterface::lobbies_join);
    godot::ClassDB::bind_method(godot::D_METHOD("lobbies_join_details"), &EOSLobbiesInterface::lobbies_join_details);
    godot::ClassDB::bind_method(godot::D_METHOD("lobbies_search"), &EOSLobbiesInterface::lobbies_search);
    godot::ClassDB::bind_method(godot::D_METHOD("lobbies_get_search_count"), &EOSLobbiesInterface::lobbies_get_search_count);
    godot::ClassDB::bind_method(godot::D_METHOD("lobbies_get_search_result"), &EOSLobbiesInterface::lobbies_get_search_result);
    godot::ClassDB::bind_method(godot::D_METHOD("lobbies_get_details"), &EOSLobbiesInterface::lobbies_get_details);
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
    for (auto &entry : lobby_registry) {
        bindings()->EOS_LobbyDetails_Release((EOS_HLobbyDetails)entry.second);
    }
    lobby_registry.clear();
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

static void sessions_invite_cb(const EOS_Sessions_SendInviteCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}

static void sessions_query_invites_cb(const EOS_Sessions_QueryInvitesCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}

static void sessions_reject_invite_cb(const EOS_Sessions_RejectInviteCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}

static void lobby_find_cb(const EOS_LobbySearch_FindCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}

static void lobby_join_details_cb(const EOS_Lobby_JoinLobbyCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    if (data->LobbyId != nullptr) {
        payload["lobby_id"] = godot::String(data->LobbyId);
    }
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}

static bool fill_session_attribute(const godot::Dictionary &entry, godot::CharString &key_keep, godot::CharString &value_keep, EOS_Sessions_AttributeData &out, EOS_ESessionAttributeAdvertisementType &advert) {
    godot::String key = entry.get("key", "");
    if (key.is_empty()) {
        return false;
    }
    key_keep = key.utf8();
    godot::String type = entry.get("value_type", "string");
    advert = bool(entry.get("advertise", true)) ? EOS_ESessionAttributeAdvertisementType::EOS_SAAT_Advertise : EOS_ESessionAttributeAdvertisementType::EOS_SAAT_DontAdvertise;
    out.ApiVersion = EOS_SESSIONS_ATTRIBUTEDATA_API_LATEST;
    out.Key = key_keep.get_data();
    if (type == "int") {
        out.ValueType = EOS_EAttributeType::EOS_AT_INT64;
        out.Value.AsInt64 = (int64_t)entry.get("value", 0);
    } else if (type == "double") {
        out.ValueType = EOS_EAttributeType::EOS_AT_DOUBLE;
        out.Value.AsDouble = (double)entry.get("value", 0.0);
    } else if (type == "bool") {
        out.ValueType = EOS_EAttributeType::EOS_AT_BOOLEAN;
        out.Value.AsBool = bool(entry.get("value", false)) ? EOS_TRUE : EOS_FALSE;
    } else {
        out.ValueType = EOS_EAttributeType::EOS_AT_STRING;
        value_keep = godot::String(entry.get("value", "")).utf8();
        out.Value.AsUtf8 = value_keep.get_data();
    }
    return true;
}

static godot::Dictionary session_attribute_to_dict(const EOS_Sessions_AttributeData *attr, EOS_ESessionAttributeAdvertisementType advert) {
    godot::Dictionary entry;
    entry["key"] = godot::String(attr->Key ? attr->Key : "");
    entry["advertise"] = advert == EOS_ESessionAttributeAdvertisementType::EOS_SAAT_Advertise;
    switch (attr->ValueType) {
        case EOS_EAttributeType::EOS_AT_INT64:
            entry["value_type"] = "int";
            entry["value"] = attr->Value.AsInt64;
            break;
        case EOS_EAttributeType::EOS_AT_DOUBLE:
            entry["value_type"] = "double";
            entry["value"] = attr->Value.AsDouble;
            break;
        case EOS_EAttributeType::EOS_AT_BOOLEAN:
            entry["value_type"] = "bool";
            entry["value"] = attr->Value.AsBool == EOS_TRUE;
            break;
        default:
            entry["value_type"] = "string";
            entry["value"] = godot::String(attr->Value.AsUtf8 ? attr->Value.AsUtf8 : "");
            break;
    }
    return entry;
}
#endif

godot::Dictionary EOSSessionsInterface::sessions_create(const godot::String &local_user_id, const godot::String &session_name, int64_t max_players, bool is_public, const godot::Array &attributes) {
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
    std::vector<godot::CharString> attr_keys;
    std::vector<godot::CharString> attr_values;
    attr_keys.reserve(attributes.size());
    attr_values.reserve(attributes.size());
    for (int i = 0; i < attributes.size(); i++) {
        godot::Dictionary entry = attributes[i];
        attr_keys.push_back(godot::CharString());
        attr_values.push_back(godot::CharString());
        EOS_Sessions_AttributeData data = {};
        EOS_ESessionAttributeAdvertisementType advert = EOS_ESessionAttributeAdvertisementType::EOS_SAAT_Advertise;
        if (!fill_session_attribute(entry, attr_keys.back(), attr_values.back(), data, advert)) {
            continue;
        }
        EOS_SessionModification_AddAttributeOptions attr_options = {};
        attr_options.ApiVersion = EOS_SESSIONMODIFICATION_ADDATTRIBUTE_API_LATEST;
        attr_options.SessionAttribute = &data;
        attr_options.AdvertisementType = advert;
        bindings()->EOS_SessionModification_AddAttribute(mod, &attr_options);
    }
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

godot::Dictionary EOSSessionsInterface::sessions_send_invite(const godot::String &session_name, const godot::String &local_user_id, const godot::String &target_user_id) {
    godot::Dictionary ready = require_ready("sessions.send_invite");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("sessions.send_invite", "Platform handle not created yet");
    }
    EOS_HSessions sessions = bindings()->EOS_Platform_GetSessionsInterface(h);
    if (sessions == nullptr) {
        return not_implemented("sessions.send_invite", "Sessions interface unavailable on this platform");
    }
    EOS_ProductUserId local = bindings()->EOS_ProductUserId_FromString(local_user_id.utf8().get_data());
    EOS_ProductUserId target = bindings()->EOS_ProductUserId_FromString(target_user_id.utf8().get_data());
    if (bindings()->EOS_ProductUserId_IsValid(local) == EOS_FALSE || bindings()->EOS_ProductUserId_IsValid(target) == EOS_FALSE) {
        godot::Dictionary context;
        context["reason"] = "Invalid Product User ID string";
        return EOSResult::make_result(10, "sessions.send_invite", context);
    }
    godot::Dictionary context;
    context["session_name"] = session_name;
    RequestContext *ctx = make_request("sessions.send_invite", context);
    godot::CharString name_utf = session_name.utf8();
    EOS_Sessions_SendInviteOptions options = {};
    options.ApiVersion = EOS_SESSIONS_SENDINVITE_API_LATEST;
    options.SessionName = name_utf.get_data();
    options.LocalUserId = local;
    options.TargetUserId = target;
    bindings()->EOS_Sessions_SendInvite(sessions, &options, ctx, sessions_invite_cb);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "sessions.send_invite";
    return pending;
#else
    return not_implemented("sessions.send_invite", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSSessionsInterface::sessions_query_invites(const godot::String &local_user_id) {
    godot::Dictionary ready = require_ready("sessions.query_invites");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("sessions.query_invites", "Platform handle not created yet");
    }
    EOS_HSessions sessions = bindings()->EOS_Platform_GetSessionsInterface(h);
    if (sessions == nullptr) {
        return not_implemented("sessions.query_invites", "Sessions interface unavailable on this platform");
    }
    EOS_ProductUserId user = bindings()->EOS_ProductUserId_FromString(local_user_id.utf8().get_data());
    if (bindings()->EOS_ProductUserId_IsValid(user) == EOS_FALSE) {
        godot::Dictionary context;
        context["reason"] = "Invalid Product User ID string";
        return EOSResult::make_result(10, "sessions.query_invites", context);
    }
    godot::Dictionary context;
    RequestContext *ctx = make_request("sessions.query_invites", context);
    EOS_Sessions_QueryInvitesOptions options = {};
    options.ApiVersion = EOS_SESSIONS_QUERYINVITES_API_LATEST;
    options.LocalUserId = user;
    bindings()->EOS_Sessions_QueryInvites(sessions, &options, ctx, sessions_query_invites_cb);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "sessions.query_invites";
    return pending;
#else
    return not_implemented("sessions.query_invites", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSSessionsInterface::sessions_reject_invite(const godot::String &local_user_id, const godot::String &invite_id) {
    godot::Dictionary ready = require_ready("sessions.reject_invite");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("sessions.reject_invite", "Platform handle not created yet");
    }
    EOS_HSessions sessions = bindings()->EOS_Platform_GetSessionsInterface(h);
    if (sessions == nullptr) {
        return not_implemented("sessions.reject_invite", "Sessions interface unavailable on this platform");
    }
    EOS_ProductUserId user = bindings()->EOS_ProductUserId_FromString(local_user_id.utf8().get_data());
    if (bindings()->EOS_ProductUserId_IsValid(user) == EOS_FALSE || invite_id.is_empty()) {
        godot::Dictionary context;
        context["reason"] = "Valid user id and invite_id are required";
        return EOSResult::make_result(10, "sessions.reject_invite", context);
    }
    godot::Dictionary context;
    RequestContext *ctx = make_request("sessions.reject_invite", context);
    godot::CharString invite_utf = invite_id.utf8();
    EOS_Sessions_RejectInviteOptions options = {};
    options.ApiVersion = EOS_SESSIONS_REJECTINVITE_API_LATEST;
    options.LocalUserId = user;
    options.InviteId = invite_utf.get_data();
    bindings()->EOS_Sessions_RejectInvite(sessions, &options, ctx, sessions_reject_invite_cb);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "sessions.reject_invite";
    return pending;
#else
    return not_implemented("sessions.reject_invite", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSSessionsInterface::sessions_set_attributes(const godot::String &session_name, const godot::Array &attributes) {
    godot::Dictionary ready = require_ready("sessions.set_attributes");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("sessions.set_attributes", "Platform handle not created yet");
    }
    EOS_HSessions sessions = bindings()->EOS_Platform_GetSessionsInterface(h);
    if (sessions == nullptr) {
        return not_implemented("sessions.set_attributes", "Sessions interface unavailable on this platform");
    }
    if (session_name.is_empty() || attributes.is_empty()) {
        godot::Dictionary context;
        context["reason"] = "session_name and a non-empty attributes array are required";
        return EOSResult::make_result(10, "sessions.set_attributes", context);
    }
    godot::CharString name_utf = session_name.utf8();
    EOS_Sessions_UpdateSessionModificationOptions mod_options = {};
    mod_options.ApiVersion = EOS_SESSIONS_UPDATESESSIONMODIFICATION_API_LATEST;
    mod_options.SessionName = name_utf.get_data();
    EOS_HSessionModification mod = nullptr;
    EOS_EResult mod_result = bindings()->EOS_Sessions_UpdateSessionModification(sessions, &mod_options, &mod);
    if (mod_result != EOS_EResult::EOS_Success || mod == nullptr) {
        godot::Dictionary context;
        context["native_code"] = (int64_t)mod_result;
        return EOSResult::make_result((int64_t)mod_result, "sessions.set_attributes", context);
    }
    std::vector<godot::CharString> attr_keys;
    std::vector<godot::CharString> attr_values;
    attr_keys.reserve(attributes.size());
    attr_values.reserve(attributes.size());
    for (int i = 0; i < attributes.size(); i++) {
        godot::Dictionary entry = attributes[i];
        attr_keys.push_back(godot::CharString());
        attr_values.push_back(godot::CharString());
        EOS_Sessions_AttributeData data = {};
        EOS_ESessionAttributeAdvertisementType advert = EOS_ESessionAttributeAdvertisementType::EOS_SAAT_Advertise;
        if (!fill_session_attribute(entry, attr_keys.back(), attr_values.back(), data, advert)) {
            continue;
        }
        EOS_SessionModification_AddAttributeOptions attr_options = {};
        attr_options.ApiVersion = EOS_SESSIONMODIFICATION_ADDATTRIBUTE_API_LATEST;
        attr_options.SessionAttribute = &data;
        attr_options.AdvertisementType = advert;
        bindings()->EOS_SessionModification_AddAttribute(mod, &attr_options);
    }
    godot::Dictionary context;
    context["session_name"] = session_name;
    RequestContext *ctx = make_request("sessions.set_attributes", context);
    EOS_Sessions_UpdateSessionOptions update_options = {};
    update_options.ApiVersion = EOS_SESSIONS_UPDATESESSION_API_LATEST;
    update_options.SessionModificationHandle = mod;
    bindings()->EOS_Sessions_UpdateSession(sessions, &update_options, ctx, sessions_create_cb);
    bindings()->EOS_SessionModification_Release(mod);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "sessions.set_attributes";
    return pending;
#else
    return not_implemented("sessions.set_attributes", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSSessionsInterface::sessions_get_attributes(int64_t session_ref) {
    godot::Dictionary ready = require_ready("sessions.get_attributes");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    auto it = session_registry.find(session_ref);
    if (it == session_registry.end()) {
        godot::Dictionary context;
        context["reason"] = "Unknown session_ref. Use sessions.get_search_result first.";
        return EOSResult::make_result(10, "sessions.get_attributes", context);
    }
    EOS_SessionDetails_GetSessionAttributeCountOptions count_options = {};
    count_options.ApiVersion = EOS_SESSIONDETAILS_GETSESSIONATTRIBUTECOUNT_API_LATEST;
    uint32_t count = bindings()->EOS_SessionDetails_GetSessionAttributeCount((EOS_HSessionDetails)it->second, &count_options);
    godot::Array attrs;
    for (uint32_t i = 0; i < count; i++) {
        EOS_SessionDetails_CopySessionAttributeByIndexOptions copy_options = {};
        copy_options.ApiVersion = EOS_SESSIONDETAILS_COPYSESSIONATTRIBUTEBYINDEX_API_LATEST;
        copy_options.AttrIndex = i;
        EOS_SessionDetails_Attribute *attr = nullptr;
        if (bindings()->EOS_SessionDetails_CopySessionAttributeByIndex((EOS_HSessionDetails)it->second, &copy_options, &attr) != EOS_EResult::EOS_Success || attr == nullptr || attr->Data == nullptr) {
            continue;
        }
        attrs.push_back(session_attribute_to_dict(attr->Data, attr->AdvertisementType));
        bindings()->EOS_SessionDetails_Attribute_Release(attr);
    }
    godot::Dictionary out = EOSResult::make_result(0, "sessions.get_attributes", godot::Dictionary());
    out["attributes"] = attrs;
    return out;
#else
    return not_implemented("sessions.get_attributes", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
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

godot::Dictionary EOSLobbiesInterface::lobbies_search(const godot::String &local_user_id, int64_t max_results) {
    godot::Dictionary ready = require_ready("lobbies.search");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("lobbies.search", "Platform handle not created yet");
    }
    EOS_HLobby lobby = bindings()->EOS_Platform_GetLobbyInterface(h);
    if (lobby == nullptr) {
        return not_implemented("lobbies.search", "Lobby interface unavailable on this platform");
    }
    EOS_ProductUserId user = nullptr;
    godot::CharString local_utf;
    if (!local_user_id.is_empty()) {
        local_utf = local_user_id.utf8();
        user = bindings()->EOS_ProductUserId_FromString(local_utf.get_data());
    }
    if (active_search != nullptr) {
        bindings()->EOS_LobbySearch_Release((EOS_HLobbySearch)active_search);
        active_search = nullptr;
    }
    EOS_Lobby_CreateLobbySearchOptions create_options = {};
    create_options.ApiVersion = EOS_LOBBY_CREATELOBBYSEARCH_API_LATEST;
    create_options.MaxResults = max_results > 0 ? (uint32_t)max_results : 10;
    EOS_HLobbySearch search = nullptr;
    EOS_EResult create_result = bindings()->EOS_Lobby_CreateLobbySearch(lobby, &create_options, &search);
    if (create_result != EOS_EResult::EOS_Success || search == nullptr) {
        godot::Dictionary context;
        context["native_code"] = (int64_t)create_result;
        return EOSResult::make_result((int64_t)create_result, "lobbies.search", context);
    }
    active_search = search;
    godot::Dictionary context;
    RequestContext *ctx = make_request("lobbies.search", context);
    EOS_LobbySearch_FindOptions find_options = {};
    find_options.ApiVersion = EOS_LOBBYSEARCH_FIND_API_LATEST;
    find_options.LocalUserId = user;
    bindings()->EOS_LobbySearch_Find(search, &find_options, ctx, lobby_find_cb);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "lobbies.search";
    return pending;
#else
    return not_implemented("lobbies.search", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSLobbiesInterface::lobbies_get_search_count() {
    godot::Dictionary ready = require_ready("lobbies.get_search_count");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    if (active_search == nullptr) {
        godot::Dictionary context;
        context["reason"] = "No active search. Call lobbies.search first.";
        return EOSResult::make_result(38, "lobbies.get_search_count", context);
    }
    EOS_LobbySearch_GetSearchResultCountOptions options = {};
    options.ApiVersion = EOS_LOBBYSEARCH_GETSEARCHRESULTCOUNT_API_LATEST;
    int32_t count = bindings()->EOS_LobbySearch_GetSearchResultCount((EOS_HLobbySearch)active_search, &options);
    godot::Dictionary out = EOSResult::make_result(0, "lobbies.get_search_count", godot::Dictionary());
    out["count"] = (int64_t)count;
    return out;
#else
    return not_implemented("lobbies.get_search_count", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSLobbiesInterface::lobbies_get_search_result(int64_t index) {
    godot::Dictionary ready = require_ready("lobbies.get_search_result");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    if (active_search == nullptr) {
        godot::Dictionary context;
        context["reason"] = "No active search. Call lobbies.search first.";
        return EOSResult::make_result(38, "lobbies.get_search_result", context);
    }
    EOS_LobbySearch_CopySearchResultByIndexOptions options = {};
    options.ApiVersion = EOS_LOBBYSEARCH_COPYSEARCHRESULTBYINDEX_API_LATEST;
    options.LobbyIndex = (uint32_t)index;
    EOS_HLobbyDetails details = nullptr;
    EOS_EResult copy_result = bindings()->EOS_LobbySearch_CopySearchResultByIndex((EOS_HLobbySearch)active_search, &options, &details);
    if (copy_result != EOS_EResult::EOS_Success || details == nullptr) {
        godot::Dictionary context;
        context["native_code"] = (int64_t)copy_result;
        return EOSResult::make_result((int64_t)copy_result, "lobbies.get_search_result", context);
    }
    int64_t ref = next_registry_id++;
    lobby_registry[ref] = details;
    godot::Dictionary out = EOSResult::make_result(0, "lobbies.get_search_result", godot::Dictionary());
    out["lobby_ref"] = ref;
    return out;
#else
    return not_implemented("lobbies.get_search_result", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSLobbiesInterface::lobbies_join_details(int64_t lobby_ref, const godot::String &local_user_id) {
    godot::Dictionary ready = require_ready("lobbies.join_details");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("lobbies.join_details", "Platform handle not created yet");
    }
    EOS_HLobby lobby = bindings()->EOS_Platform_GetLobbyInterface(h);
    if (lobby == nullptr) {
        return not_implemented("lobbies.join_details", "Lobby interface unavailable on this platform");
    }
    auto it = lobby_registry.find(lobby_ref);
    if (it == lobby_registry.end()) {
        godot::Dictionary context;
        context["reason"] = "Unknown lobby_ref. Use lobbies.get_search_result first.";
        return EOSResult::make_result(10, "lobbies.join_details", context);
    }
    EOS_ProductUserId user = bindings()->EOS_ProductUserId_FromString(local_user_id.utf8().get_data());
    if (bindings()->EOS_ProductUserId_IsValid(user) == EOS_FALSE) {
        godot::Dictionary context;
        context["reason"] = "Invalid Product User ID string";
        return EOSResult::make_result(10, "lobbies.join_details", context);
    }
    godot::Dictionary context;
    context["lobby_ref"] = lobby_ref;
    RequestContext *ctx = make_request("lobbies.join_details", context);
    EOS_Lobby_JoinLobbyOptions options = {};
    options.ApiVersion = EOS_LOBBY_JOINLOBBY_API_LATEST;
    options.LobbyDetailsHandle = (EOS_HLobbyDetails)it->second;
    options.LocalUserId = user;
    options.bPresenceEnabled = EOS_TRUE;
    bindings()->EOS_Lobby_JoinLobby(lobby, &options, ctx, lobby_join_details_cb);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "lobbies.join_details";
    return pending;
#else
    return not_implemented("lobbies.join_details", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSLobbiesInterface::lobbies_get_details(const godot::String &lobby_id, const godot::String &local_user_id) {
    godot::Dictionary ready = require_ready("lobbies.get_details");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("lobbies.get_details", "Platform handle not created yet");
    }
    EOS_HLobby lobby = bindings()->EOS_Platform_GetLobbyInterface(h);
    if (lobby == nullptr) {
        return not_implemented("lobbies.get_details", "Lobby interface unavailable on this platform");
    }
    EOS_ProductUserId user = nullptr;
    godot::CharString local_utf;
    if (!local_user_id.is_empty()) {
        local_utf = local_user_id.utf8();
        user = bindings()->EOS_ProductUserId_FromString(local_utf.get_data());
    }
    godot::CharString lobby_utf = lobby_id.utf8();
    EOS_Lobby_CopyLobbyDetailsHandleOptions copy_options = {};
    copy_options.ApiVersion = EOS_LOBBY_COPYLOBBYDETAILSHANDLE_API_LATEST;
    copy_options.LobbyId = lobby_utf.get_data();
    copy_options.LocalUserId = user;
    EOS_HLobbyDetails details = nullptr;
    EOS_EResult copy_result = bindings()->EOS_Lobby_CopyLobbyDetailsHandle(lobby, &copy_options, &details);
    if (copy_result != EOS_EResult::EOS_Success || details == nullptr) {
        godot::Dictionary context;
        context["native_code"] = (int64_t)copy_result;
        context["reason"] = "Lobby details unavailable. Join the lobby first for full data.";
        return EOSResult::make_result((int64_t)copy_result, "lobbies.get_details", context);
    }
    godot::Dictionary context;
    context["lobby_id"] = lobby_id;
    godot::Dictionary out = EOSResult::make_result(0, "lobbies.get_details", context);
    EOS_LobbyDetails_CopyInfoOptions info_options = {};
    info_options.ApiVersion = EOS_LOBBYDETAILS_COPYINFO_API_LATEST;
    EOS_LobbyDetails_Info *info = nullptr;
    if (bindings()->EOS_LobbyDetails_CopyInfo(details, &info_options, &info) == EOS_EResult::EOS_Success && info != nullptr) {
        out["max_members"] = (int64_t)info->MaxMembers;
        out["available_slots"] = (int64_t)info->AvailableSlots;
        out["permission_level"] = (int64_t)info->PermissionLevel;
        out["bucket_id"] = godot::String(info->BucketId ? info->BucketId : "");
        if (info->LobbyOwnerUserId != nullptr) {
            char owner_buf[64] = {};
            int32_t owner_len = (int32_t)sizeof(owner_buf);
            if (bindings()->EOS_ProductUserId_ToString(info->LobbyOwnerUserId, owner_buf, &owner_len) == EOS_EResult::EOS_Success) {
                out["owner_id"] = godot::String(owner_buf);
            }
        }
        bindings()->EOS_LobbyDetails_Info_Release(info);
    }
    EOS_LobbyDetails_GetMemberCountOptions member_count_options = {};
    member_count_options.ApiVersion = EOS_LOBBYDETAILS_GETMEMBERCOUNT_API_LATEST;
    int32_t member_count = bindings()->EOS_LobbyDetails_GetMemberCount(details, &member_count_options);
    godot::Array members;
    for (int32_t i = 0; i < member_count; i++) {
        EOS_LobbyDetails_GetMemberByIndexOptions member_options = {};
        member_options.ApiVersion = EOS_LOBBYDETAILS_GETMEMBERBYINDEX_API_LATEST;
        member_options.MemberIndex = (uint32_t)i;
        EOS_ProductUserId member = bindings()->EOS_LobbyDetails_GetMemberByIndex(details, &member_options);
        if (member != nullptr) {
            char member_buf[64] = {};
            int32_t member_len = (int32_t)sizeof(member_buf);
            if (bindings()->EOS_ProductUserId_ToString(member, member_buf, &member_len) == EOS_EResult::EOS_Success) {
                members.push_back(godot::String(member_buf));
            }
        }
    }
    out["member_count"] = (int64_t)member_count;
    out["members"] = members;
    EOS_LobbyDetails_GetAttributeCountOptions attr_count_options = {};
    attr_count_options.ApiVersion = EOS_LOBBYDETAILS_GETATTRIBUTECOUNT_API_LATEST;
    uint32_t attr_count = bindings()->EOS_LobbyDetails_GetAttributeCount(details, &attr_count_options);
    godot::Array attrs;
    for (uint32_t i = 0; i < attr_count; i++) {
        EOS_LobbyDetails_CopyAttributeByIndexOptions attr_options = {};
        attr_options.ApiVersion = EOS_LOBBYDETAILS_COPYATTRIBUTEBYINDEX_API_LATEST;
        attr_options.AttrIndex = i;
        EOS_Lobby_Attribute *attr = nullptr;
        if (bindings()->EOS_LobbyDetails_CopyAttributeByIndex(details, &attr_options, &attr) != EOS_EResult::EOS_Success || attr == nullptr || attr->Data == nullptr) {
            continue;
        }
        godot::Dictionary entry;
        entry["key"] = godot::String(attr->Data->Key ? attr->Data->Key : "");
        switch (attr->Data->ValueType) {
            case EOS_EAttributeType::EOS_AT_INT64:
                entry["value_type"] = "int";
                entry["value"] = attr->Data->Value.AsInt64;
                break;
            case EOS_EAttributeType::EOS_AT_DOUBLE:
                entry["value_type"] = "double";
                entry["value"] = attr->Data->Value.AsDouble;
                break;
            case EOS_EAttributeType::EOS_AT_BOOLEAN:
                entry["value_type"] = "bool";
                entry["value"] = attr->Data->Value.AsBool == EOS_TRUE;
                break;
            default:
                entry["value_type"] = "string";
                entry["value"] = godot::String(attr->Data->Value.AsUtf8 ? attr->Data->Value.AsUtf8 : "");
                break;
        }
        attrs.push_back(entry);
        bindings()->EOS_Lobby_Attribute_Release(attr);
    }
    out["attributes"] = attrs;
    bindings()->EOS_LobbyDetails_Release(details);
    return out;
#else
    return not_implemented("lobbies.get_details", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
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
