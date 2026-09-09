#include "core/eos_result.hpp"

#include <godot_cpp/variant/utility_functions.hpp>

namespace yugen {

void EOSResult::_bind_methods() {
    godot::ClassDB::bind_static_method("EOSResult", godot::D_METHOD("code_to_name"), &EOSResult::code_to_name);
    godot::ClassDB::bind_static_method("EOSResult", godot::D_METHOD("code_to_message"), &EOSResult::code_to_message);
    godot::ClassDB::bind_static_method("EOSResult", godot::D_METHOD("is_success"), &EOSResult::is_success);
    godot::ClassDB::bind_static_method("EOSResult", godot::D_METHOD("is_operation_complete"), &EOSResult::is_operation_complete);
    BIND_ENUM_CONSTANT(Success);
    BIND_ENUM_CONSTANT(NoConnection);
    BIND_ENUM_CONSTANT(InvalidUser);
    BIND_ENUM_CONSTANT(InvalidAuth);
    BIND_ENUM_CONSTANT(NotConfigured);
    BIND_ENUM_CONSTANT(NotImplemented);
    BIND_ENUM_CONSTANT(NotFound);
    BIND_ENUM_CONSTANT(TimedOut);
    BIND_ENUM_CONSTANT(InvalidState);
}

godot::String EOSResult::code_to_name(int64_t code) {
    switch (code) {
        case 0: return "EOS_EResult::EOS_Success";
        case 1: return "EOS_NoConnection";
        case 2: return "EOS_InvalidCredentials";
        case 3: return "EOS_InvalidUser";
        case 4: return "EOS_InvalidAuth";
        case 5: return "EOS_AccessDenied";
        case 6: return "EOS_MissingPermissions";
        case 8: return "EOS_TooManyRequests";
        case 9: return "EOS_AlreadyPending";
        case 10: return "EOS_InvalidParameters";
        case 11: return "EOS_InvalidRequest";
        case 13: return "EOS_IncompatibleVersion";
        case 14: return "EOS_NotConfigured";
        case 15: return "EOS_AlreadyConfigured";
        case 16: return "EOS_NotImplemented";
        case 17: return "EOS_Canceled";
        case 18: return "EOS_NotFound";
        case 19: return "EOS_OperationWillRetry";
        case 20: return "EOS_NoChange";
        case 21: return "EOS_VersionMismatch";
        case 22: return "EOS_LimitExceeded";
        case 23: return "EOS_Disabled";
        case 27: return "EOS_TimedOut";
        case 28: return "EOS_PartialResult";
        case 38: return "EOS_InvalidState";
        case 39: return "EOS_RequestInProgress";
        case 40: return "EOS_ApplicationSuspended";
        case 41: return "EOS_NetworkDisconnected";
        default: break;
    }
    if (code >= 1000 && code < 2000) return "EOS_Auth_Error";
    if (code >= 2000 && code < 3000) return "EOS_Friends_Error";
    if (code >= 3000 && code < 4000) return "EOS_Sessions_Error";
    if (code >= 4000 && code < 5000) return "EOS_Ecom_Error";
    return "EOS_Unknown";
}

godot::String EOSResult::code_to_message(int64_t code) {
    switch (code) {
        case 0: return "Operation completed successfully.";
        case 1: return "No connection to backend services.";
        case 2: return "Invalid credentials provided.";
        case 3: return "Invalid or missing user.";
        case 4: return "Invalid or expired authentication token.";
        case 10: return "Invalid parameters specified for request.";
        case 14: return "EOS platform is not configured. Call initialize first.";
        case 16: return "Feature not available on this implementation or platform.";
        case 18: return "Requested information was not found.";
        case 27: return "Request timed out.";
        case 38: return "Resource was in an invalid state for the request.";
        case 41: return "Network is disconnected.";
        default: break;
    }
    return "EOS operation returned code " + godot::String::num_int64(code) + ".";
}

bool EOSResult::is_success(int64_t code) {
    return code == 0 || code == 20 || code == 28;
}

bool EOSResult::is_operation_complete(int64_t code) {
    return code != 19 && code != 39;
}

godot::Dictionary EOSResult::make_result(int64_t code, const godot::String &operation, const godot::Dictionary &context) {
    godot::Dictionary out;
    out["code"] = code;
    out["name"] = code_to_name(code);
    out["message"] = code_to_message(code);
    out["operation"] = operation;
    out["context"] = context;
    out["ok"] = is_success(code);
    return out;
}

}
