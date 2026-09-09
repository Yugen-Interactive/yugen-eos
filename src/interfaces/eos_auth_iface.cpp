#include "interfaces/eos_auth_iface.hpp"

#if YUGEN_EOS_HAS_SDK
#include "eos_common.h"
#include "eos_sdk.h"
#include "eos_auth.h"
#endif

namespace yugen {

void EOSAuthInterface::_bind_methods() {
    godot::ClassDB::bind_method(godot::D_METHOD("auth_login"), &EOSAuthInterface::auth_login);
    godot::ClassDB::bind_method(godot::D_METHOD("auth_logout"), &EOSAuthInterface::auth_logout);
    godot::ClassDB::bind_method(godot::D_METHOD("auth_verify"), &EOSAuthInterface::auth_verify);
    godot::ClassDB::bind_method(godot::D_METHOD("get_logged_in_count"), &EOSAuthInterface::get_logged_in_count);
    ADD_SIGNAL(godot::MethodInfo("login_completed", godot::PropertyInfo(godot::Variant::DICTIONARY, "result")));
    ADD_SIGNAL(godot::MethodInfo("logout_completed", godot::PropertyInfo(godot::Variant::DICTIONARY, "result")));
}

#if YUGEN_EOS_HAS_SDK
static void auth_login_cb(const EOS_Auth_LoginCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    if (data->LocalUserId != nullptr) {
        char buf[64] = {};
        int32_t len = (int32_t)sizeof(buf);
        if (ctx->table->EOS_EpicAccountId_ToString(data->LocalUserId, buf, &len) == EOS_EResult::EOS_Success) {
            payload["local_user_id"] = godot::String(buf);
        }
    }
    if (ctx->table->EOS_EResult_IsOperationComplete(data->ResultCode) == EOS_TRUE) {
        ctx->queue->enqueue(ctx->operation, payload);
        delete ctx;
    }
}

static void auth_logout_cb(const EOS_Auth_LogoutCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}
#endif

godot::Dictionary EOSAuthInterface::auth_login(int64_t login_type, const godot::String &id, const godot::String &token) {
    godot::Dictionary ready = require_ready("auth.login");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("auth.login", "Platform handle not created yet");
    }
    EOS_HAuth auth = bindings()->EOS_Platform_GetAuthInterface(h);
    if (auth == nullptr) {
        return not_implemented("auth.login", "Auth interface unavailable on this platform");
    }
    godot::Dictionary context;
    RequestContext *ctx = make_request("auth.login", context);
    EOS_Auth_Credentials creds = {};
    creds.ApiVersion = EOS_AUTH_CREDENTIALS_API_LATEST;
    godot::CharString id_utf = id.utf8();
    godot::CharString token_utf = token.utf8();
    creds.Id = id.is_empty() ? nullptr : id_utf.get_data();
    creds.Token = token.is_empty() ? nullptr : token_utf.get_data();
    creds.Type = (EOS_ELoginCredentialType)login_type;
    EOS_Auth_LoginOptions options = {};
    options.ApiVersion = EOS_AUTH_LOGIN_API_LATEST;
    options.Credentials = &creds;
    options.ScopeFlags = EOS_EAuthScopeFlags::EOS_AS_BasicProfile | EOS_EAuthScopeFlags::EOS_AS_FriendsList | EOS_EAuthScopeFlags::EOS_AS_Presence;
    bindings()->EOS_Auth_Login(auth, &options, ctx, auth_login_cb);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "auth.login";
    return pending;
#else
    return not_implemented("auth.login", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSAuthInterface::auth_logout(const godot::String &local_user_id) {
    godot::Dictionary ready = require_ready("auth.logout");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("auth.logout", "Platform handle not created yet");
    }
    EOS_HAuth auth = bindings()->EOS_Platform_GetAuthInterface(h);
    if (auth == nullptr) {
        return not_implemented("auth.logout", "Auth interface unavailable on this platform");
    }
    EOS_EpicAccountId account = bindings()->EOS_EpicAccountId_FromString(local_user_id.utf8().get_data());
    if (bindings()->EOS_EpicAccountId_IsValid(account) == EOS_FALSE) {
        godot::Dictionary context;
        context["reason"] = "Invalid Epic Account ID string";
        return EOSResult::make_result(10, "auth.logout", context);
    }
    godot::Dictionary context;
    context["local_user_id"] = local_user_id;
    RequestContext *ctx = make_request("auth.logout", context);
    EOS_Auth_LogoutOptions options = {};
    options.ApiVersion = EOS_AUTH_LOGOUT_API_LATEST;
    options.LocalUserId = account;
    bindings()->EOS_Auth_Logout(auth, &options, ctx, auth_logout_cb);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "auth.logout";
    return pending;
#else
    return not_implemented("auth.logout", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSAuthInterface::auth_verify(const godot::String &local_user_id) {
    godot::Dictionary ready = require_ready("auth.verify");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_EpicAccountId account = bindings()->EOS_EpicAccountId_FromString(local_user_id.utf8().get_data());
    godot::Dictionary context;
    context["local_user_id"] = local_user_id;
    bool valid = bindings()->EOS_EpicAccountId_IsValid(account) == EOS_TRUE;
    godot::Dictionary out = EOSResult::make_result(valid ? 0 : 3, "auth.verify", context);
    out["valid"] = valid;
    return out;
#else
    return not_implemented("auth.verify", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

int64_t EOSAuthInterface::get_logged_in_count() {
#if YUGEN_EOS_HAS_SDK
    if (!is_available() || platform->get_platform_handle() == nullptr) {
        return 0;
    }
    EOS_HAuth auth = bindings()->EOS_Platform_GetAuthInterface((EOS_HPlatform)platform->get_platform_handle());
    if (auth == nullptr) {
        return 0;
    }
    return (int64_t)bindings()->EOS_Auth_GetLoggedInAccountsCount(auth);
#else
    return 0;
#endif
}

}
