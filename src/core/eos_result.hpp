#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/string.hpp>

namespace yugen {

class EOSResult : public godot::RefCounted {
    GDCLASS(EOSResult, godot::RefCounted);

public:
    enum Code {
        Success = 0,
        NoConnection = 1,
        InvalidCredentials = 2,
        InvalidUser = 3,
        InvalidAuth = 4,
        AccessDenied = 5,
        MissingPermissions = 6,
        TooManyRequests = 8,
        AlreadyPending = 9,
        InvalidParameters = 10,
        InvalidRequest = 11,
        IncompatibleVersion = 13,
        NotConfigured = 14,
        AlreadyConfigured = 15,
        NotImplemented = 16,
        Canceled = 17,
        NotFound = 18,
        OperationWillRetry = 19,
        NoChange = 20,
        VersionMismatch = 21,
        LimitExceeded = 22,
        Disabled = 23,
        TimedOut = 27,
        PartialResult = 28,
        InvalidState = 38,
        RequestInProgress = 39,
        ApplicationSuspended = 40,
        NetworkDisconnected = 41,
        Auth_AccountLocked = 1001,
        Auth_InvalidRefreshToken = 1003,
        Auth_InvalidToken = 1004,
        Auth_AuthenticationFailure = 1005,
        Auth_WrongAccount = 1007,
        Auth_WrongClient = 1008,
        Friends_InviteAlreadySent = 2001,
        Friends_NoInvitation = 2002,
        Sessions_SessionInProgress = 3001,
        Sessions_TooManyPlayers = 3003,
        Sessions_NoPermission = 3006,
        Ecom_CatalogOfferNotFound = 4002,
        Ecom_CheckoutLoadError = 4012,
    };

    static void _bind_methods();
    static godot::String code_to_name(int64_t code);
    static godot::String code_to_message(int64_t code);
    static bool is_success(int64_t code);
    static bool is_operation_complete(int64_t code);
    static godot::Dictionary make_result(int64_t code, const godot::String &operation, const godot::Dictionary &context);
};

}

VARIANT_ENUM_CAST(yugen::EOSResult::Code);
