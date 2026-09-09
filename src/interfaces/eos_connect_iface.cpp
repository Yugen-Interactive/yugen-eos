#include "interfaces/eos_connect_iface.hpp"

#if YUGEN_EOS_HAS_SDK
#include "eos_common.h"
#include "eos_sdk.h"
#include "eos_connect.h"
#endif

namespace yugen {

void EOSConnectInterface::_bind_methods() {
    godot::ClassDB::bind_method(godot::D_METHOD("connect_login"), &EOSConnectInterface::connect_login);
    godot::ClassDB::bind_method(godot::D_METHOD("connect_logout"), &EOSConnectInterface::connect_logout);
    godot::ClassDB::bind_method(godot::D_METHOD("connect_create_user"), &EOSConnectInterface::connect_create_user);
    godot::ClassDB::bind_method(godot::D_METHOD("get_logged_in_count"), &EOSConnectInterface::get_logged_in_count);
    ADD_SIGNAL(godot::MethodInfo("login_completed", godot::PropertyInfo(godot::Variant::DICTIONARY, "result")));
}

#if YUGEN_EOS_HAS_SDK
static void connect_login_cb(const EOS_Connect_LoginCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    if (data->LocalUserId != nullptr) {
        char buf[64] = {};
        int32_t len = (int32_t)sizeof(buf);
        if (ctx->table->EOS_ProductUserId_ToString(data->LocalUserId, buf, &len) == EOS_EResult::EOS_Success) {
            payload["product_user_id"] = godot::String(buf);
        }
    }
    if (ctx->table->EOS_EResult_IsOperationComplete(data->ResultCode) == EOS_TRUE) {
        if (data->ContinuanceToken != nullptr && ctx->platform != nullptr) {
            payload["continuance_ref"] = ctx->platform->store_continuance((void *)data->ContinuanceToken);
        }
        ctx->queue->enqueue(ctx->operation, payload);
        delete ctx;
    }
}

static void connect_logout_cb(const EOS_Connect_LogoutCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}

static void connect_create_user_cb(const EOS_Connect_CreateUserCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    if (data->LocalUserId != nullptr) {
        char buf[64] = {};
        int32_t len = (int32_t)sizeof(buf);
        if (ctx->table->EOS_ProductUserId_ToString(data->LocalUserId, buf, &len) == EOS_EResult::EOS_Success) {
            payload["product_user_id"] = godot::String(buf);
        }
    }
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}
#endif

godot::Dictionary EOSConnectInterface::connect_login(int64_t credential_type, const godot::String &token, const godot::String &display_name) {
    godot::Dictionary ready = require_ready("connect.login");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("connect.login", "Platform handle not created yet");
    }
    EOS_HConnect connect = bindings()->EOS_Platform_GetConnectInterface(h);
    if (connect == nullptr) {
        return not_implemented("connect.login", "Connect interface unavailable on this platform");
    }
    godot::Dictionary context;
    RequestContext *ctx = make_request("connect.login", context);
    EOS_Connect_Credentials creds = {};
    creds.ApiVersion = EOS_CONNECT_CREDENTIALS_API_LATEST;
    godot::CharString token_utf = token.utf8();
    creds.Token = token.is_empty() ? nullptr : token_utf.get_data();
    creds.Type = (EOS_EExternalCredentialType)credential_type;
    EOS_Connect_UserLoginInfo login_info = {};
    login_info.ApiVersion = EOS_CONNECT_USERLOGININFO_API_LATEST;
    godot::CharString name_utf = display_name.utf8();
    login_info.DisplayName = display_name.is_empty() ? nullptr : name_utf.get_data();
    EOS_Connect_LoginOptions options = {};
    options.ApiVersion = EOS_CONNECT_LOGIN_API_LATEST;
    options.Credentials = &creds;
    options.UserLoginInfo = display_name.is_empty() ? nullptr : &login_info;
    bindings()->EOS_Connect_Login(connect, &options, ctx, connect_login_cb);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "connect.login";
    return pending;
#else
    return not_implemented("connect.login", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSConnectInterface::connect_logout(const godot::String &product_user_id) {
    godot::Dictionary ready = require_ready("connect.logout");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("connect.logout", "Platform handle not created yet");
    }
    EOS_HConnect connect = bindings()->EOS_Platform_GetConnectInterface(h);
    if (connect == nullptr) {
        return not_implemented("connect.logout", "Connect interface unavailable on this platform");
    }
    EOS_ProductUserId user = bindings()->EOS_ProductUserId_FromString(product_user_id.utf8().get_data());
    if (bindings()->EOS_ProductUserId_IsValid(user) == EOS_FALSE) {
        godot::Dictionary context;
        context["reason"] = "Invalid Product User ID string";
        return EOSResult::make_result(10, "connect.logout", context);
    }
    godot::Dictionary context;
    context["product_user_id"] = product_user_id;
    RequestContext *ctx = make_request("connect.logout", context);
    EOS_Connect_LogoutOptions options = {};
    options.ApiVersion = EOS_CONNECT_LOGOUT_API_LATEST;
    options.LocalUserId = user;
    bindings()->EOS_Connect_Logout(connect, &options, ctx, connect_logout_cb);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "connect.logout";
    return pending;
#else
    return not_implemented("connect.logout", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSConnectInterface::connect_create_user(int64_t continuance_ref) {
    godot::Dictionary ready = require_ready("connect.create_user");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("connect.create_user", "Platform handle not created yet");
    }
    EOS_HConnect connect = bindings()->EOS_Platform_GetConnectInterface(h);
    if (connect == nullptr) {
        return not_implemented("connect.create_user", "Connect interface unavailable on this platform");
    }
    void *token = platform->take_continuance(continuance_ref);
    if (token == nullptr) {
        godot::Dictionary context;
        context["reason"] = "Unknown continuance_ref. Use the continuance_ref from a login result.";
        return EOSResult::make_result(10, "connect.create_user", context);
    }
    godot::Dictionary context;
    RequestContext *ctx = make_request("connect.create_user", context);
    EOS_Connect_CreateUserOptions options = {};
    options.ApiVersion = EOS_CONNECT_CREATEUSER_API_LATEST;
    options.ContinuanceToken = (EOS_ContinuanceToken)token;
    bindings()->EOS_Connect_CreateUser(connect, &options, ctx, connect_create_user_cb);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "connect.create_user";
    return pending;
#else
    return not_implemented("connect.create_user", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

int64_t EOSConnectInterface::get_logged_in_count() {
#if YUGEN_EOS_HAS_SDK
    if (!is_available() || platform->get_platform_handle() == nullptr) {
        return 0;
    }
    EOS_HConnect connect = bindings()->EOS_Platform_GetConnectInterface((EOS_HPlatform)platform->get_platform_handle());
    if (connect == nullptr) {
        return 0;
    }
    return (int64_t)bindings()->EOS_Connect_GetLoggedInUsersCount(connect);
#else
    return 0;
#endif
}

}
