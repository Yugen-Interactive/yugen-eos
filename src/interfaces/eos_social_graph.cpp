#include "interfaces/eos_social_graph.hpp"

#if YUGEN_EOS_HAS_SDK
#include "eos_common.h"
#include "eos_sdk.h"
#include "eos_friends.h"
#include "eos_presence.h"
#include "eos_userinfo.h"
#endif

namespace yugen {

void EOSFriendsInterface::_bind_methods() {
    godot::ClassDB::bind_method(godot::D_METHOD("friends_query"), &EOSFriendsInterface::friends_query);
    godot::ClassDB::bind_method(godot::D_METHOD("friends_get_count"), &EOSFriendsInterface::friends_get_count);
    godot::ClassDB::bind_method(godot::D_METHOD("friends_send_invite"), &EOSFriendsInterface::friends_send_invite);
    ADD_SIGNAL(godot::MethodInfo("query_completed", godot::PropertyInfo(godot::Variant::DICTIONARY, "result")));
}

void EOSPresenceInterface::_bind_methods() {
    godot::ClassDB::bind_method(godot::D_METHOD("presence_set"), &EOSPresenceInterface::presence_set);
    godot::ClassDB::bind_method(godot::D_METHOD("presence_query"), &EOSPresenceInterface::presence_query);
    godot::ClassDB::bind_method(godot::D_METHOD("presence_has"), &EOSPresenceInterface::presence_has);
    ADD_SIGNAL(godot::MethodInfo("query_completed", godot::PropertyInfo(godot::Variant::DICTIONARY, "result")));
}

void EOSUserInfoInterface::_bind_methods() {
    godot::ClassDB::bind_method(godot::D_METHOD("userinfo_query"), &EOSUserInfoInterface::userinfo_query);
    godot::ClassDB::bind_method(godot::D_METHOD("userinfo_get"), &EOSUserInfoInterface::userinfo_get);
    ADD_SIGNAL(godot::MethodInfo("query_completed", godot::PropertyInfo(godot::Variant::DICTIONARY, "result")));
}

#if YUGEN_EOS_HAS_SDK
static void friends_query_cb(const EOS_Friends_QueryFriendsCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}

static void friends_invite_cb(const EOS_Friends_SendInviteCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}

static void presence_query_cb(const EOS_Presence_QueryPresenceCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}

static void presence_set_cb(const EOS_Presence_SetPresenceCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}

static void userinfo_query_cb(const EOS_UserInfo_QueryUserInfoCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}
#endif

godot::Dictionary EOSFriendsInterface::friends_query(const godot::String &local_user_id) {
    godot::Dictionary ready = require_ready("friends.query");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("friends.query", "Platform handle not created yet");
    }
    EOS_HFriends friends = bindings()->EOS_Platform_GetFriendsInterface(h);
    if (friends == nullptr) {
        return not_implemented("friends.query", "Friends interface unavailable on this platform");
    }
    EOS_EpicAccountId user = bindings()->EOS_EpicAccountId_FromString(local_user_id.utf8().get_data());
    if (bindings()->EOS_EpicAccountId_IsValid(user) == EOS_FALSE) {
        godot::Dictionary context;
        context["reason"] = "Invalid Epic Account ID string";
        return EOSResult::make_result(10, "friends.query", context);
    }
    godot::Dictionary context;
    context["local_user_id"] = local_user_id;
    RequestContext *ctx = make_request("friends.query", context);
    EOS_Friends_QueryFriendsOptions options = {};
    options.ApiVersion = EOS_FRIENDS_QUERYFRIENDS_API_LATEST;
    options.LocalUserId = user;
    bindings()->EOS_Friends_QueryFriends(friends, &options, ctx, friends_query_cb);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "friends.query";
    return pending;
#else
    return not_implemented("friends.query", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSFriendsInterface::friends_get_count(const godot::String &local_user_id) {
    godot::Dictionary ready = require_ready("friends.get_count");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("friends.get_count", "Platform handle not created yet");
    }
    EOS_HFriends friends = bindings()->EOS_Platform_GetFriendsInterface(h);
    if (friends == nullptr) {
        return not_implemented("friends.get_count", "Friends interface unavailable on this platform");
    }
    EOS_EpicAccountId user = bindings()->EOS_EpicAccountId_FromString(local_user_id.utf8().get_data());
    EOS_Friends_GetFriendsCountOptions count_options = {};
    count_options.ApiVersion = EOS_FRIENDS_GETFRIENDSCOUNT_API_LATEST;
    count_options.LocalUserId = user;
    int32_t count = bindings()->EOS_Friends_GetFriendsCount(friends, &count_options);
    godot::Dictionary context;
    context["local_user_id"] = local_user_id;
    godot::Dictionary out = EOSResult::make_result(0, "friends.get_count", context);
    out["count"] = (int64_t)count;
    return out;
#else
    return not_implemented("friends.get_count", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSFriendsInterface::friends_send_invite(const godot::String &local_user_id, const godot::String &target_user_id) {
    godot::Dictionary ready = require_ready("friends.send_invite");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("friends.send_invite", "Platform handle not created yet");
    }
    EOS_HFriends friends = bindings()->EOS_Platform_GetFriendsInterface(h);
    if (friends == nullptr) {
        return not_implemented("friends.send_invite", "Friends interface unavailable on this platform");
    }
    EOS_EpicAccountId local = bindings()->EOS_EpicAccountId_FromString(local_user_id.utf8().get_data());
    EOS_EpicAccountId target = bindings()->EOS_EpicAccountId_FromString(target_user_id.utf8().get_data());
    if (bindings()->EOS_EpicAccountId_IsValid(local) == EOS_FALSE || bindings()->EOS_EpicAccountId_IsValid(target) == EOS_FALSE) {
        godot::Dictionary context;
        context["reason"] = "Invalid Epic Account ID string";
        return EOSResult::make_result(10, "friends.send_invite", context);
    }
    godot::Dictionary context;
    context["local_user_id"] = local_user_id;
    context["target_user_id"] = target_user_id;
    RequestContext *ctx = make_request("friends.send_invite", context);
    EOS_Friends_SendInviteOptions options = {};
    options.ApiVersion = EOS_FRIENDS_SENDINVITE_API_LATEST;
    options.LocalUserId = local;
    options.TargetUserId = target;
    bindings()->EOS_Friends_SendInvite(friends, &options, ctx, friends_invite_cb);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "friends.send_invite";
    return pending;
#else
    return not_implemented("friends.send_invite", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSPresenceInterface::presence_set(const godot::String &local_user_id, int64_t status, const godot::String &details) {
    godot::Dictionary ready = require_ready("presence.set");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("presence.set", "Platform handle not created yet");
    }
    EOS_HPresence presence = bindings()->EOS_Platform_GetPresenceInterface(h);
    if (presence == nullptr) {
        return not_implemented("presence.set", "Presence interface unavailable on this platform");
    }
    EOS_EpicAccountId user = bindings()->EOS_EpicAccountId_FromString(local_user_id.utf8().get_data());
    if (bindings()->EOS_EpicAccountId_IsValid(user) == EOS_FALSE) {
        godot::Dictionary context;
        context["reason"] = "Invalid Epic Account ID string";
        return EOSResult::make_result(10, "presence.set", context);
    }
    EOS_Presence_CreatePresenceModificationOptions create_options = {};
    create_options.ApiVersion = EOS_PRESENCE_CREATEPRESENCEMODIFICATION_API_LATEST;
    create_options.LocalUserId = user;
    EOS_HPresenceModification mod = nullptr;
    EOS_EResult create_result = bindings()->EOS_Presence_CreatePresenceModification(presence, &create_options, &mod);
    if (create_result != EOS_EResult::EOS_Success || mod == nullptr) {
        godot::Dictionary context;
        context["native_code"] = (int64_t)create_result;
        return EOSResult::make_result((int64_t)create_result, "presence.set", context);
    }
    EOS_PresenceModification_SetStatusOptions status_options = {};
    status_options.ApiVersion = EOS_PRESENCEMODIFICATION_SETSTATUS_API_LATEST;
    status_options.Status = (EOS_Presence_EStatus)status;
    EOS_EResult status_result = bindings()->EOS_PresenceModification_SetStatus(mod, &status_options);
    if (status_result == EOS_EResult::EOS_Success && !details.is_empty()) {
        EOS_Presence_DataRecord record = {};
        record.ApiVersion = EOS_PRESENCE_DATARECORD_API_LATEST;
        godot::CharString key_utf = godot::String("details").utf8();
        godot::CharString value_utf = details.utf8();
        record.Key = key_utf.get_data();
        record.Value = value_utf.get_data();
        EOS_PresenceModification_SetDataOptions data_options = {};
        data_options.ApiVersion = EOS_PRESENCEMODIFICATION_SETDATA_API_LATEST;
        data_options.Records = &record;
        data_options.RecordsCount = 1;
        status_result = bindings()->EOS_PresenceModification_SetData(mod, &data_options);
    }
    godot::Dictionary out;
    if (status_result != EOS_EResult::EOS_Success) {
        bindings()->EOS_PresenceModification_Release(mod);
        godot::Dictionary context;
        context["native_code"] = (int64_t)status_result;
        return EOSResult::make_result((int64_t)status_result, "presence.set", context);
    }
    godot::Dictionary context;
    context["local_user_id"] = local_user_id;
    RequestContext *ctx = make_request("presence.set", context);
    EOS_Presence_SetPresenceOptions set_options = {};
    set_options.ApiVersion = EOS_PRESENCE_SETPRESENCE_API_LATEST;
    set_options.LocalUserId = user;
    set_options.PresenceModificationHandle = mod;
    bindings()->EOS_Presence_SetPresence(presence, &set_options, ctx, presence_set_cb);
    bindings()->EOS_PresenceModification_Release(mod);
    out["ok"] = true;
    out["code"] = (int64_t)39;
    out["name"] = "EOS_RequestInProgress";
    out["operation"] = "presence.set";
    return out;
#else
    return not_implemented("presence.set", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSPresenceInterface::presence_query(const godot::String &local_user_id, const godot::String &target_user_id) {
    godot::Dictionary ready = require_ready("presence.query");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("presence.query", "Platform handle not created yet");
    }
    EOS_HPresence presence = bindings()->EOS_Platform_GetPresenceInterface(h);
    if (presence == nullptr) {
        return not_implemented("presence.query", "Presence interface unavailable on this platform");
    }
    EOS_EpicAccountId local = bindings()->EOS_EpicAccountId_FromString(local_user_id.utf8().get_data());
    EOS_EpicAccountId target = bindings()->EOS_EpicAccountId_FromString(target_user_id.utf8().get_data());
    if (bindings()->EOS_EpicAccountId_IsValid(local) == EOS_FALSE || bindings()->EOS_EpicAccountId_IsValid(target) == EOS_FALSE) {
        godot::Dictionary context;
        context["reason"] = "Invalid Epic Account ID string";
        return EOSResult::make_result(10, "presence.query", context);
    }
    godot::Dictionary context;
    context["local_user_id"] = local_user_id;
    context["target_user_id"] = target_user_id;
    RequestContext *ctx = make_request("presence.query", context);
    EOS_Presence_QueryPresenceOptions options = {};
    options.ApiVersion = EOS_PRESENCE_QUERYPRESENCE_API_LATEST;
    options.LocalUserId = local;
    options.TargetUserId = target;
    bindings()->EOS_Presence_QueryPresence(presence, &options, ctx, presence_query_cb);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "presence.query";
    return pending;
#else
    return not_implemented("presence.query", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSPresenceInterface::presence_has(const godot::String &local_user_id, const godot::String &target_user_id) {
    godot::Dictionary ready = require_ready("presence.has");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("presence.has", "Platform handle not created yet");
    }
    EOS_HPresence presence = bindings()->EOS_Platform_GetPresenceInterface(h);
    if (presence == nullptr) {
        return not_implemented("presence.has", "Presence interface unavailable on this platform");
    }
    EOS_EpicAccountId local = bindings()->EOS_EpicAccountId_FromString(local_user_id.utf8().get_data());
    EOS_EpicAccountId target = bindings()->EOS_EpicAccountId_FromString(target_user_id.utf8().get_data());
    EOS_Presence_HasPresenceOptions options = {};
    options.ApiVersion = EOS_PRESENCE_HASPRESENCE_API_LATEST;
    options.LocalUserId = local;
    options.TargetUserId = target;
    EOS_Bool has = bindings()->EOS_Presence_HasPresence(presence, &options);
    godot::Dictionary context;
    context["local_user_id"] = local_user_id;
    context["target_user_id"] = target_user_id;
    godot::Dictionary out = EOSResult::make_result(0, "presence.has", context);
    out["has_presence"] = has == EOS_TRUE;
    return out;
#else
    return not_implemented("presence.has", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSUserInfoInterface::userinfo_query(const godot::String &local_user_id, const godot::String &target_user_id) {
    godot::Dictionary ready = require_ready("userinfo.query");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("userinfo.query", "Platform handle not created yet");
    }
    EOS_HUserInfo info = bindings()->EOS_Platform_GetUserInfoInterface(h);
    if (info == nullptr) {
        return not_implemented("userinfo.query", "UserInfo interface unavailable on this platform");
    }
    EOS_EpicAccountId local = bindings()->EOS_EpicAccountId_FromString(local_user_id.utf8().get_data());
    EOS_EpicAccountId target = bindings()->EOS_EpicAccountId_FromString(target_user_id.utf8().get_data());
    if (bindings()->EOS_EpicAccountId_IsValid(local) == EOS_FALSE || bindings()->EOS_EpicAccountId_IsValid(target) == EOS_FALSE) {
        godot::Dictionary context;
        context["reason"] = "Invalid Epic Account ID string";
        return EOSResult::make_result(10, "userinfo.query", context);
    }
    godot::Dictionary context;
    context["local_user_id"] = local_user_id;
    context["target_user_id"] = target_user_id;
    RequestContext *ctx = make_request("userinfo.query", context);
    EOS_UserInfo_QueryUserInfoOptions options = {};
    options.ApiVersion = EOS_USERINFO_QUERYUSERINFO_API_LATEST;
    options.LocalUserId = local;
    options.TargetUserId = target;
    bindings()->EOS_UserInfo_QueryUserInfo(info, &options, ctx, userinfo_query_cb);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "userinfo.query";
    return pending;
#else
    return not_implemented("userinfo.query", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSUserInfoInterface::userinfo_get(const godot::String &target_user_id) {
    godot::Dictionary ready = require_ready("userinfo.get");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("userinfo.get", "Platform handle not created yet");
    }
    EOS_HUserInfo info = bindings()->EOS_Platform_GetUserInfoInterface(h);
    if (info == nullptr) {
        return not_implemented("userinfo.get", "UserInfo interface unavailable on this platform");
    }
    EOS_EpicAccountId target = bindings()->EOS_EpicAccountId_FromString(target_user_id.utf8().get_data());
    EOS_UserInfo_CopyUserInfoOptions options = {};
    options.ApiVersion = EOS_USERINFO_COPYUSERINFO_API_LATEST;
    options.LocalUserId = target;
    options.TargetUserId = target;
    EOS_UserInfo *out_info = nullptr;
    EOS_EResult copy_result = bindings()->EOS_UserInfo_CopyUserInfo(info, &options, &out_info);
    godot::Dictionary context;
    context["target_user_id"] = target_user_id;
    if (copy_result != EOS_EResult::EOS_Success || out_info == nullptr) {
        context["reason"] = "No cached user info. Call userinfo.query first.";
        return EOSResult::make_result((int64_t)copy_result, "userinfo.get", context);
    }
    godot::Dictionary out = EOSResult::make_result(0, "userinfo.get", context);
    out["display_name"] = godot::String(out_info->DisplayName ? out_info->DisplayName : "");
    out["country"] = godot::String(out_info->Country ? out_info->Country : "");
    out["preferred_language"] = godot::String(out_info->PreferredLanguage ? out_info->PreferredLanguage : "");
    bindings()->EOS_UserInfo_Release(out_info);
    return out;
#else
    return not_implemented("userinfo.get", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

}
