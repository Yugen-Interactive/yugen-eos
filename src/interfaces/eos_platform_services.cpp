#include "interfaces/eos_platform_services.hpp"

#if YUGEN_EOS_HAS_SDK
#include "eos_common.h"
#include "eos_sdk.h"
#include "eos_playerdatastorage.h"
#include "eos_titlestorage.h"
#include "eos_ecom.h"
#include "eos_ui.h"
#include "eos_metrics.h"
#include "eos_reports.h"
#include "eos_sanctions.h"
#include "eos_rtc.h"
#endif

namespace yugen {

void EOSStorageInterface::_bind_methods() {
    godot::ClassDB::bind_method(godot::D_METHOD("storage_write"), &EOSStorageInterface::storage_write);
    godot::ClassDB::bind_method(godot::D_METHOD("storage_read"), &EOSStorageInterface::storage_read);
    godot::ClassDB::bind_method(godot::D_METHOD("titlestorage_read"), &EOSStorageInterface::titlestorage_read);
    godot::ClassDB::bind_method(godot::D_METHOD("storage_query_list"), &EOSStorageInterface::storage_query_list);
    ADD_SIGNAL(godot::MethodInfo("operation_completed", godot::PropertyInfo(godot::Variant::DICTIONARY, "result")));
}

void EOSEcomInterface::_bind_methods() {
    godot::ClassDB::bind_method(godot::D_METHOD("ecom_query_offers"), &EOSEcomInterface::ecom_query_offers);
    godot::ClassDB::bind_method(godot::D_METHOD("ecom_query_entitlements"), &EOSEcomInterface::ecom_query_entitlements);
    godot::ClassDB::bind_method(godot::D_METHOD("ecom_checkout"), &EOSEcomInterface::ecom_checkout);
    ADD_SIGNAL(godot::MethodInfo("operation_completed", godot::PropertyInfo(godot::Variant::DICTIONARY, "result")));
}

void EOSUIInterface::_bind_methods() {
    godot::ClassDB::bind_method(godot::D_METHOD("ui_show_friends"), &EOSUIInterface::ui_show_friends);
    godot::ClassDB::bind_method(godot::D_METHOD("ui_hide_friends"), &EOSUIInterface::ui_hide_friends);
    godot::ClassDB::bind_method(godot::D_METHOD("ui_set_visibility"), &EOSUIInterface::ui_set_visibility);
}

void EOSSocialInterface::_bind_methods() {
    godot::ClassDB::bind_method(godot::D_METHOD("reports_send"), &EOSSocialInterface::reports_send);
    godot::ClassDB::bind_method(godot::D_METHOD("sanctions_query"), &EOSSocialInterface::sanctions_query);
    godot::ClassDB::bind_method(godot::D_METHOD("rtc_join"), &EOSSocialInterface::rtc_join);
    godot::ClassDB::bind_method(godot::D_METHOD("rtc_leave"), &EOSSocialInterface::rtc_leave);
    godot::ClassDB::bind_method(godot::D_METHOD("anticheat_start"), &EOSSocialInterface::anticheat_start);
    godot::ClassDB::bind_method(godot::D_METHOD("metrics_send"), &EOSSocialInterface::metrics_send);
    godot::ClassDB::bind_method(godot::D_METHOD("metrics_begin_session"), &EOSSocialInterface::metrics_begin_session);
    godot::ClassDB::bind_method(godot::D_METHOD("application_set_status"), &EOSSocialInterface::application_set_status);
    ADD_SIGNAL(godot::MethodInfo("operation_completed", godot::PropertyInfo(godot::Variant::DICTIONARY, "result")));
}

#if YUGEN_EOS_HAS_SDK
static void ui_show_friends_cb(const EOS_UI_ShowFriendsCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}

static void ui_hide_friends_cb(const EOS_UI_HideFriendsCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}

static void ecom_offers_cb(const EOS_Ecom_QueryOffersCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}

static void ecom_entitlements_cb(const EOS_Ecom_QueryEntitlementsCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}

static void ecom_checkout_cb(const EOS_Ecom_CheckoutCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}

static void sanctions_query_cb(const EOS_Sanctions_QueryActivePlayerSanctionsCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}

static void reports_send_cb(const EOS_Reports_SendPlayerBehaviorReportCompleteCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}

static void rtc_join_cb(const EOS_RTC_JoinRoomCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}

static void rtc_leave_cb(const EOS_RTC_LeaveRoomCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}

static void playerdata_list_cb(const EOS_PlayerDataStorage_QueryFileListCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}
#endif

godot::Dictionary EOSStorageInterface::storage_write(const godot::String &local_user_id, const godot::String &filename, const godot::PackedByteArray &data) {
    godot::Dictionary ready = require_ready("storage.write");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
    if (local_user_id.is_empty() || filename.is_empty() || data.is_empty()) {
        godot::Dictionary context;
        context["reason"] = "local_user_id, filename and non-empty data are required";
        return EOSResult::make_result(10, "storage.write", context);
    }
    return not_implemented("storage.write", "FileTransferRequest chunk pipeline pending. See docs/status.md.");
}

godot::Dictionary EOSStorageInterface::storage_read(const godot::String &local_user_id, const godot::String &filename) {
    godot::Dictionary ready = require_ready("storage.read");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
    if (local_user_id.is_empty() || filename.is_empty()) {
        godot::Dictionary context;
        context["reason"] = "local_user_id and filename are required";
        return EOSResult::make_result(10, "storage.read", context);
    }
    return not_implemented("storage.read", "FileTransferRequest chunk pipeline pending. See docs/status.md.");
}

godot::Dictionary EOSStorageInterface::titlestorage_read(const godot::String &filename) {
    godot::Dictionary ready = require_ready("titlestorage.read");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
    if (filename.is_empty()) {
        godot::Dictionary context;
        context["reason"] = "filename is required";
        return EOSResult::make_result(10, "titlestorage.read", context);
    }
    return not_implemented("titlestorage.read", "FileTransferRequest chunk pipeline pending. See docs/status.md.");
}

godot::Dictionary EOSStorageInterface::storage_query_list(const godot::String &local_user_id) {
    godot::Dictionary ready = require_ready("storage.query_list");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("storage.query_list", "Platform handle not created yet");
    }
    EOS_HPlayerDataStorage storage = bindings()->EOS_Platform_GetPlayerDataStorageInterface(h);
    if (storage == nullptr) {
        return not_implemented("storage.query_list", "PlayerDataStorage interface unavailable on this platform");
    }
    EOS_ProductUserId user = bindings()->EOS_ProductUserId_FromString(local_user_id.utf8().get_data());
    if (bindings()->EOS_ProductUserId_IsValid(user) == EOS_FALSE) {
        godot::Dictionary context;
        context["reason"] = "Invalid Product User ID string";
        return EOSResult::make_result(10, "storage.query_list", context);
    }
    godot::Dictionary context;
    RequestContext *ctx = make_request("storage.query_list", context);
    EOS_PlayerDataStorage_QueryFileListOptions options = {};
    options.ApiVersion = EOS_PLAYERDATASTORAGE_QUERYFILELIST_API_LATEST;
    options.LocalUserId = user;
    bindings()->EOS_PlayerDataStorage_QueryFileList(storage, &options, ctx, playerdata_list_cb);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "storage.query_list";
    return pending;
#else
    return not_implemented("storage.query_list", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSEcomInterface::ecom_query_offers(const godot::String &local_user_id) {
    godot::Dictionary ready = require_ready("ecom.query_offers");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("ecom.query_offers", "Platform handle not created yet");
    }
    EOS_HEcom ecom = bindings()->EOS_Platform_GetEcomInterface(h);
    if (ecom == nullptr) {
        return not_implemented("ecom.query_offers", "Ecom interface unavailable on this platform");
    }
    EOS_EpicAccountId user = bindings()->EOS_EpicAccountId_FromString(local_user_id.utf8().get_data());
    if (bindings()->EOS_EpicAccountId_IsValid(user) == EOS_FALSE) {
        godot::Dictionary context;
        context["reason"] = "Invalid Epic Account ID string";
        return EOSResult::make_result(10, "ecom.query_offers", context);
    }
    godot::Dictionary context;
    RequestContext *ctx = make_request("ecom.query_offers", context);
    EOS_Ecom_QueryOffersOptions options = {};
    options.ApiVersion = EOS_ECOM_QUERYOFFERS_API_LATEST;
    options.LocalUserId = user;
    bindings()->EOS_Ecom_QueryOffers(ecom, &options, ctx, ecom_offers_cb);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "ecom.query_offers";
    return pending;
#else
    return not_implemented("ecom.query_offers", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSEcomInterface::ecom_query_entitlements(const godot::String &local_user_id) {
    godot::Dictionary ready = require_ready("ecom.query_entitlements");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("ecom.query_entitlements", "Platform handle not created yet");
    }
    EOS_HEcom ecom = bindings()->EOS_Platform_GetEcomInterface(h);
    if (ecom == nullptr) {
        return not_implemented("ecom.query_entitlements", "Ecom interface unavailable on this platform");
    }
    EOS_EpicAccountId user = bindings()->EOS_EpicAccountId_FromString(local_user_id.utf8().get_data());
    if (bindings()->EOS_EpicAccountId_IsValid(user) == EOS_FALSE) {
        godot::Dictionary context;
        context["reason"] = "Invalid Epic Account ID string";
        return EOSResult::make_result(10, "ecom.query_entitlements", context);
    }
    godot::Dictionary context;
    RequestContext *ctx = make_request("ecom.query_entitlements", context);
    EOS_Ecom_QueryEntitlementsOptions options = {};
    options.ApiVersion = EOS_ECOM_QUERYENTITLEMENTS_API_LATEST;
    options.LocalUserId = user;
    bindings()->EOS_Ecom_QueryEntitlements(ecom, &options, ctx, ecom_entitlements_cb);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "ecom.query_entitlements";
    return pending;
#else
    return not_implemented("ecom.query_entitlements", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSEcomInterface::ecom_checkout(const godot::String &local_user_id, const godot::String &offer_id) {
    godot::Dictionary ready = require_ready("ecom.checkout");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("ecom.checkout", "Platform handle not created yet");
    }
    EOS_HEcom ecom = bindings()->EOS_Platform_GetEcomInterface(h);
    if (ecom == nullptr) {
        return not_implemented("ecom.checkout", "Ecom interface unavailable on this platform");
    }
    EOS_EpicAccountId user = bindings()->EOS_EpicAccountId_FromString(local_user_id.utf8().get_data());
    if (bindings()->EOS_EpicAccountId_IsValid(user) == EOS_FALSE || offer_id.is_empty()) {
        godot::Dictionary context;
        context["reason"] = "Valid user id and offer_id are required";
        return EOSResult::make_result(10, "ecom.checkout", context);
    }
    godot::Dictionary context;
    context["offer_id"] = offer_id;
    RequestContext *ctx = make_request("ecom.checkout", context);
    godot::CharString offer_utf = offer_id.utf8();
    EOS_Ecom_CheckoutEntry entry = {};
    entry.ApiVersion = EOS_ECOM_CHECKOUTENTRY_API_LATEST;
    entry.OfferId = offer_utf.get_data();
    EOS_Ecom_CheckoutOptions options = {};
    options.ApiVersion = EOS_ECOM_CHECKOUT_API_LATEST;
    options.LocalUserId = user;
    options.EntryCount = 1;
    options.Entries = &entry;
    bindings()->EOS_Ecom_Checkout(ecom, &options, ctx, ecom_checkout_cb);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "ecom.checkout";
    return pending;
#else
    return not_implemented("ecom.checkout", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSUIInterface::ui_show_friends(const godot::String &local_user_id) {
    godot::Dictionary ready = require_ready("ui.show_friends");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("ui.show_friends", "Platform handle not created yet");
    }
    EOS_HUI ui = bindings()->EOS_Platform_GetUIInterface(h);
    if (ui == nullptr) {
        return not_implemented("ui.show_friends", "UI interface unavailable on this platform");
    }
    EOS_EpicAccountId user = bindings()->EOS_EpicAccountId_FromString(local_user_id.utf8().get_data());
    godot::Dictionary context;
    context["local_user_id"] = local_user_id;
    RequestContext *ctx = make_request("ui.show_friends", context);
    EOS_UI_ShowFriendsOptions options = {};
    options.ApiVersion = EOS_UI_SHOWFRIENDS_API_LATEST;
    options.LocalUserId = bindings()->EOS_EpicAccountId_IsValid(user) == EOS_TRUE ? user : nullptr;
    bindings()->EOS_UI_ShowFriends(ui, &options, ctx, ui_show_friends_cb);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "ui.show_friends";
    return pending;
#else
    return not_implemented("ui.show_friends", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSUIInterface::ui_hide_friends() {
    godot::Dictionary ready = require_ready("ui.hide_friends");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("ui.hide_friends", "Platform handle not created yet");
    }
    EOS_HUI ui = bindings()->EOS_Platform_GetUIInterface(h);
    if (ui == nullptr) {
        return not_implemented("ui.hide_friends", "UI interface unavailable on this platform");
    }
    EOS_UI_HideFriendsOptions options = {};
    options.ApiVersion = EOS_UI_HIDEFRIENDS_API_LATEST;
    options.LocalUserId = nullptr;
    godot::Dictionary context;
    RequestContext *ctx = make_request("ui.hide_friends", context);
    bindings()->EOS_UI_HideFriends(ui, &options, ctx, ui_hide_friends_cb);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "ui.hide_friends";
    return pending;
#else
    return not_implemented("ui.hide_friends", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSUIInterface::ui_set_visibility(bool visible) {
    godot::Dictionary ready = require_ready("ui.set_visibility");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("ui.set_visibility", "Platform handle not created yet");
    }
    EOS_HUI ui = bindings()->EOS_Platform_GetUIInterface(h);
    if (ui == nullptr) {
        return not_implemented("ui.set_visibility", "UI interface unavailable on this platform");
    }
    if (visible) {
        return ui_show_friends("");
    }
    return ui_hide_friends();
#else
    return not_implemented("ui.set_visibility", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSSocialInterface::reports_send(const godot::String &reporter_id, const godot::String &reported_id, int64_t category, const godot::String &message) {
    godot::Dictionary ready = require_ready("reports.send");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("reports.send", "Platform handle not created yet");
    }
    EOS_HReports reports = bindings()->EOS_Platform_GetReportsInterface(h);
    if (reports == nullptr) {
        return not_implemented("reports.send", "Reports interface unavailable on this platform");
    }
    EOS_ProductUserId reporter = bindings()->EOS_ProductUserId_FromString(reporter_id.utf8().get_data());
    EOS_ProductUserId reported = bindings()->EOS_ProductUserId_FromString(reported_id.utf8().get_data());
    if (bindings()->EOS_ProductUserId_IsValid(reporter) == EOS_FALSE || bindings()->EOS_ProductUserId_IsValid(reported) == EOS_FALSE) {
        godot::Dictionary context;
        context["reason"] = "Invalid Product User ID string";
        return EOSResult::make_result(10, "reports.send", context);
    }
    godot::Dictionary context;
    RequestContext *ctx = make_request("reports.send", context);
    godot::CharString msg_utf = message.utf8();
    EOS_Reports_SendPlayerBehaviorReportOptions options = {};
    options.ApiVersion = EOS_REPORTS_SENDPLAYERBEHAVIORREPORT_API_LATEST;
    options.ReporterUserId = reporter;
    options.ReportedUserId = reported;
    options.Category = (EOS_EPlayerReportsCategory)category;
    options.Message = msg_utf.get_data();
    bindings()->EOS_Reports_SendPlayerBehaviorReport(reports, &options, ctx, reports_send_cb);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "reports.send";
    return pending;
#else
    return not_implemented("reports.send", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSSocialInterface::sanctions_query(const godot::String &player_id) {
    godot::Dictionary ready = require_ready("sanctions.query");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("sanctions.query", "Platform handle not created yet");
    }
    EOS_HSanctions sanctions = bindings()->EOS_Platform_GetSanctionsInterface(h);
    if (sanctions == nullptr) {
        return not_implemented("sanctions.query", "Sanctions interface unavailable on this platform");
    }
    EOS_ProductUserId player = bindings()->EOS_ProductUserId_FromString(player_id.utf8().get_data());
    if (bindings()->EOS_ProductUserId_IsValid(player) == EOS_FALSE) {
        godot::Dictionary context;
        context["reason"] = "Invalid Product User ID string";
        return EOSResult::make_result(10, "sanctions.query", context);
    }
    godot::Dictionary context;
    RequestContext *ctx = make_request("sanctions.query", context);
    EOS_Sanctions_QueryActivePlayerSanctionsOptions options = {};
    options.ApiVersion = EOS_SANCTIONS_QUERYACTIVEPLAYERSANCTIONS_API_LATEST;
    options.TargetUserId = player;
    options.LocalUserId = player;
    bindings()->EOS_Sanctions_QueryActivePlayerSanctions(sanctions, &options, ctx, sanctions_query_cb);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "sanctions.query";
    return pending;
#else
    return not_implemented("sanctions.query", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSSocialInterface::rtc_join(const godot::String &local_user_id, const godot::String &room) {
    godot::Dictionary ready = require_ready("rtc.join");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("rtc.join", "Platform handle not created yet");
    }
    EOS_HRTC rtc = bindings()->EOS_Platform_GetRTCInterface(h);
    if (rtc == nullptr) {
        return not_implemented("rtc.join", "RTC interface unavailable on this platform");
    }
    EOS_ProductUserId user = bindings()->EOS_ProductUserId_FromString(local_user_id.utf8().get_data());
    if (bindings()->EOS_ProductUserId_IsValid(user) == EOS_FALSE || room.is_empty()) {
        godot::Dictionary context;
        context["reason"] = "Valid user id and room name are required";
        return EOSResult::make_result(10, "rtc.join", context);
    }
    godot::Dictionary context;
    context["room"] = room;
    RequestContext *ctx = make_request("rtc.join", context);
    godot::CharString room_utf = room.utf8();
    EOS_RTC_JoinRoomOptions options = {};
    options.ApiVersion = EOS_RTC_JOINROOM_API_LATEST;
    options.LocalUserId = user;
    options.RoomName = room_utf.get_data();
    options.ClientBaseUrl = nullptr;
    options.ParticipantToken = nullptr;
    options.ParticipantId = nullptr;
    options.Flags = 0;
    options.bManualAudioInputEnabled = EOS_FALSE;
    bindings()->EOS_RTC_JoinRoom(rtc, &options, ctx, rtc_join_cb);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "rtc.join";
    return pending;
#else
    return not_implemented("rtc.join", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSSocialInterface::rtc_leave(const godot::String &local_user_id, const godot::String &room) {
    godot::Dictionary ready = require_ready("rtc.leave");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("rtc.leave", "Platform handle not created yet");
    }
    EOS_HRTC rtc = bindings()->EOS_Platform_GetRTCInterface(h);
    if (rtc == nullptr) {
        return not_implemented("rtc.leave", "RTC interface unavailable on this platform");
    }
    EOS_ProductUserId user = bindings()->EOS_ProductUserId_FromString(local_user_id.utf8().get_data());
    godot::Dictionary context;
    context["room"] = room;
    RequestContext *ctx = make_request("rtc.leave", context);
    godot::CharString room_utf = room.utf8();
    EOS_RTC_LeaveRoomOptions options = {};
    options.ApiVersion = EOS_RTC_LEAVEROOM_API_LATEST;
    options.LocalUserId = user;
    options.RoomName = room_utf.get_data();
    bindings()->EOS_RTC_LeaveRoom(rtc, &options, ctx, rtc_leave_cb);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "rtc.leave";
    return pending;
#else
    return not_implemented("rtc.leave", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSSocialInterface::anticheat_start(const godot::String &local_user_id) {
    godot::Dictionary ready = require_ready("anticheat.start");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
    godot::Dictionary context;
    context["local_user_id"] = local_user_id;
    context["reason"] = "AntiCheat requires game-server registration and the AntiCheatServer interface. Client game builds use EOS_Platform_GetAntiCheatClientInterface with a registered session.";
    return EOSResult::make_result(16, "anticheat.start", context);
}

godot::Dictionary EOSSocialInterface::metrics_send(const godot::String &event_name, const godot::Dictionary &attributes) {
    godot::Dictionary ready = require_ready("metrics.send");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
    if (event_name.is_empty()) {
        godot::Dictionary context;
        context["reason"] = "event_name is required";
        return EOSResult::make_result(10, "metrics.send", context);
    }
    godot::Dictionary context;
    context["event_name"] = event_name;
    context["attribute_count"] = (int64_t)attributes.size();
    return not_implemented("metrics.send", "Custom event metrics funnel through BeginPlayerSession account tracking. See metrics_begin_session.");
}

godot::Dictionary EOSSocialInterface::metrics_begin_session(const godot::String &account_id) {
    godot::Dictionary ready = require_ready("metrics.begin_session");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("metrics.begin_session", "Platform handle not created yet");
    }
    EOS_HMetrics metrics = bindings()->EOS_Platform_GetMetricsInterface(h);
    if (metrics == nullptr) {
        return not_implemented("metrics.begin_session", "Metrics interface unavailable on this platform");
    }
    EOS_EpicAccountId account = bindings()->EOS_EpicAccountId_FromString(account_id.utf8().get_data());
    if (bindings()->EOS_EpicAccountId_IsValid(account) == EOS_FALSE) {
        godot::Dictionary context;
        context["reason"] = "Invalid Epic Account ID string";
        return EOSResult::make_result(10, "metrics.begin_session", context);
    }
    EOS_Metrics_BeginPlayerSessionOptions options = {};
    options.ApiVersion = EOS_METRICS_BEGINPLAYERSESSION_API_LATEST;
    options.AccountIdType = EOS_EMetricsAccountIdType::EOS_MAIT_Epic;
    options.AccountId.Epic = account;
    options.DisplayName = nullptr;
    options.ControllerType = EOS_EUserControllerType::EOS_UCT_Unknown;
    options.ServerIp = nullptr;
    EOS_EResult result = bindings()->EOS_Metrics_BeginPlayerSession(metrics, &options);
    return EOSResult::make_result((int64_t)result, "metrics.begin_session", godot::Dictionary());
#else
    return not_implemented("metrics.begin_session", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSSocialInterface::application_set_status(int64_t status) {
    godot::Dictionary ready = require_ready("application.set_status");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("application.set_status", "Platform handle not created yet");
    }
    EOS_EResult result = bindings()->EOS_Platform_SetApplicationStatus(h, (EOS_EApplicationStatus)status);
    return EOSResult::make_result((int64_t)result, "application.set_status", godot::Dictionary());
#else
    return not_implemented("application.set_status", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

}
