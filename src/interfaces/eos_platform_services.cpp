#include "interfaces/eos_platform_services.hpp"

#include <cstring>
#include <vector>

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
#include "eos_rtc_audio.h"
#include "eos_mods.h"
#include "eos_kws.h"
#include "eos_custominvites.h"
#include "eos_progressionsnapshot.h"
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
    godot::ClassDB::bind_method(godot::D_METHOD("ui_pause_overlay"), &EOSUIInterface::ui_pause_overlay);
    godot::ClassDB::bind_method(godot::D_METHOD("ui_is_overlay_paused"), &EOSUIInterface::ui_is_overlay_paused);
    godot::ClassDB::bind_method(godot::D_METHOD("ui_get_friends_visible"), &EOSUIInterface::ui_get_friends_visible);
    godot::ClassDB::bind_method(godot::D_METHOD("ui_set_notification_location"), &EOSUIInterface::ui_set_notification_location);
}

void EOSSocialInterface::_bind_methods() {
    godot::ClassDB::bind_method(godot::D_METHOD("reports_send"), &EOSSocialInterface::reports_send);
    godot::ClassDB::bind_method(godot::D_METHOD("sanctions_query"), &EOSSocialInterface::sanctions_query);
    godot::ClassDB::bind_method(godot::D_METHOD("sanctions_appeal"), &EOSSocialInterface::sanctions_appeal);
    godot::ClassDB::bind_method(godot::D_METHOD("rtc_join"), &EOSSocialInterface::rtc_join);
    godot::ClassDB::bind_method(godot::D_METHOD("rtc_leave"), &EOSSocialInterface::rtc_leave);
    godot::ClassDB::bind_method(godot::D_METHOD("rtc_set_sending_volume"), &EOSSocialInterface::rtc_set_sending_volume);
    godot::ClassDB::bind_method(godot::D_METHOD("rtc_set_receiving_volume"), &EOSSocialInterface::rtc_set_receiving_volume);
    godot::ClassDB::bind_method(godot::D_METHOD("anticheat_start"), &EOSSocialInterface::anticheat_start);
    godot::ClassDB::bind_method(godot::D_METHOD("metrics_send"), &EOSSocialInterface::metrics_send);
    godot::ClassDB::bind_method(godot::D_METHOD("metrics_begin_session"), &EOSSocialInterface::metrics_begin_session);
    godot::ClassDB::bind_method(godot::D_METHOD("metrics_end_session"), &EOSSocialInterface::metrics_end_session);
    godot::ClassDB::bind_method(godot::D_METHOD("application_set_status"), &EOSSocialInterface::application_set_status);
    godot::ClassDB::bind_method(godot::D_METHOD("mods_enumerate"), &EOSSocialInterface::mods_enumerate);
    godot::ClassDB::bind_method(godot::D_METHOD("mods_get_info"), &EOSSocialInterface::mods_get_info);
    godot::ClassDB::bind_method(godot::D_METHOD("kws_query_age_gate"), &EOSSocialInterface::kws_query_age_gate);
    godot::ClassDB::bind_method(godot::D_METHOD("custom_invites_set"), &EOSSocialInterface::custom_invites_set);
    godot::ClassDB::bind_method(godot::D_METHOD("custom_invites_send"), &EOSSocialInterface::custom_invites_send);
    godot::ClassDB::bind_method(godot::D_METHOD("snapshot_begin"), &EOSSocialInterface::snapshot_begin);
    godot::ClassDB::bind_method(godot::D_METHOD("snapshot_add"), &EOSSocialInterface::snapshot_add);
    godot::ClassDB::bind_method(godot::D_METHOD("snapshot_submit"), &EOSSocialInterface::snapshot_submit);
    godot::ClassDB::bind_method(godot::D_METHOD("snapshot_end"), &EOSSocialInterface::snapshot_end);
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
    if (data->ResultCode == EOS_EResult::EOS_Success && ctx->handle != nullptr && data->TargetUserId != nullptr) {
        EOS_Sanctions_GetPlayerSanctionCountOptions count_options = {};
        count_options.ApiVersion = EOS_SANCTIONS_GETPLAYERSANCTIONCOUNT_API_LATEST;
        count_options.TargetUserId = data->TargetUserId;
        uint32_t count = ctx->table->EOS_Sanctions_GetPlayerSanctionCount((EOS_HSanctions)ctx->handle, &count_options);
        godot::Array sanctions;
        for (uint32_t i = 0; i < count; i++) {
            EOS_Sanctions_CopyPlayerSanctionByIndexOptions copy_options = {};
            copy_options.ApiVersion = EOS_SANCTIONS_COPYPLAYERSANCTIONBYINDEX_API_LATEST;
            copy_options.TargetUserId = data->TargetUserId;
            copy_options.SanctionIndex = i;
            EOS_Sanctions_PlayerSanction *sanction = nullptr;
            if (ctx->table->EOS_Sanctions_CopyPlayerSanctionByIndex((EOS_HSanctions)ctx->handle, &copy_options, &sanction) != EOS_EResult::EOS_Success || sanction == nullptr) {
                continue;
            }
            godot::Dictionary entry;
            entry["action"] = godot::String(sanction->Action ? sanction->Action : "");
            entry["reference_id"] = godot::String(sanction->ReferenceId ? sanction->ReferenceId : "");
            entry["time_placed"] = sanction->TimePlaced;
            entry["time_expires"] = sanction->TimeExpires;
            sanctions.push_back(entry);
            ctx->table->EOS_Sanctions_PlayerSanction_Release(sanction);
        }
        payload["sanction_count"] = (int64_t)count;
        payload["sanctions"] = sanctions;
    }
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}

static void sanctions_appeal_cb(const EOS_Sanctions_CreatePlayerSanctionAppealCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}

static void rtc_sending_volume_cb(const EOS_RTCAudio_UpdateSendingVolumeCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}

static void rtc_receiving_volume_cb(const EOS_RTCAudio_UpdateReceivingVolumeCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}

static void mods_enumerate_cb(const EOS_Mods_EnumerateModsCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}

static void kws_age_gate_cb(const EOS_KWS_QueryAgeGateCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    if (data->ResultCode == EOS_EResult::EOS_Success) {
        payload["country_code"] = godot::String(data->CountryCode ? data->CountryCode : "");
        payload["age_of_consent"] = (int64_t)data->AgeOfConsent;
    }
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}

static void custom_invites_send_cb(const EOS_CustomInvites_SendCustomInviteCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}

static void snapshot_submit_cb(const EOS_ProgressionSnapshot_SubmitSnapshotCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    payload["snapshot_id"] = (int64_t)data->SnapshotId;
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

struct StorageWriteContext : RequestContext {
    godot::PackedByteArray data;
    int32_t offset = 0;
    void *request = nullptr;
};

struct StorageReadContext : RequestContext {
    godot::PackedByteArray data;
    void *request = nullptr;
};

struct TitleReadContext : RequestContext {
    godot::PackedByteArray data;
    void *request = nullptr;
};

static EOS_PlayerDataStorage_EWriteResult storage_write_data_cb(const EOS_PlayerDataStorage_WriteFileDataCallbackInfo *info, void *out_buffer, uint32_t *out_written) {
    StorageWriteContext *ctx = (StorageWriteContext *)info->ClientData;
    int32_t remaining = ctx->data.size() - ctx->offset;
    int32_t chunk = remaining < (int32_t)info->DataBufferLengthBytes ? remaining : (int32_t)info->DataBufferLengthBytes;
    if (chunk > 0) {
        memcpy(out_buffer, ctx->data.ptr() + ctx->offset, (size_t)chunk);
        ctx->offset += chunk;
    }
    *out_written = (uint32_t)(chunk > 0 ? chunk : 0);
    if (ctx->offset >= ctx->data.size()) {
        return EOS_PlayerDataStorage_EWriteResult::EOS_WR_CompleteRequest;
    }
    return EOS_PlayerDataStorage_EWriteResult::EOS_WR_ContinueWriting;
}

static void storage_write_complete_cb(const EOS_PlayerDataStorage_WriteFileCallbackInfo *info) {
    StorageWriteContext *ctx = (StorageWriteContext *)info->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)info->ResultCode, ctx->operation, ctx->context);
    if (ctx->request != nullptr) {
        ctx->table->EOS_PlayerDataStorageFileTransferRequest_Release((EOS_HPlayerDataStorageFileTransferRequest)ctx->request);
    }
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}

static EOS_PlayerDataStorage_EReadResult storage_read_data_cb(const EOS_PlayerDataStorage_ReadFileDataCallbackInfo *info) {
    StorageReadContext *ctx = (StorageReadContext *)info->ClientData;
    int32_t old_size = ctx->data.size();
    ctx->data.resize(old_size + (int32_t)info->DataChunkLengthBytes);
    memcpy(ctx->data.ptrw() + old_size, info->DataChunk, (size_t)info->DataChunkLengthBytes);
    return EOS_PlayerDataStorage_EReadResult::EOS_RR_ContinueReading;
}

static void storage_read_complete_cb(const EOS_PlayerDataStorage_ReadFileCallbackInfo *info) {
    StorageReadContext *ctx = (StorageReadContext *)info->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)info->ResultCode, ctx->operation, ctx->context);
    payload["data"] = ctx->data;
    if (ctx->request != nullptr) {
        ctx->table->EOS_PlayerDataStorageFileTransferRequest_Release((EOS_HPlayerDataStorageFileTransferRequest)ctx->request);
    }
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}

static EOS_TitleStorage_EReadResult title_read_data_cb(const EOS_TitleStorage_ReadFileDataCallbackInfo *info) {
    TitleReadContext *ctx = (TitleReadContext *)info->ClientData;
    int32_t old_size = ctx->data.size();
    ctx->data.resize(old_size + (int32_t)info->DataChunkLengthBytes);
    memcpy(ctx->data.ptrw() + old_size, info->DataChunk, (size_t)info->DataChunkLengthBytes);
    return EOS_TitleStorage_EReadResult::EOS_TS_RR_ContinueReading;
}

static void title_read_complete_cb(const EOS_TitleStorage_ReadFileCallbackInfo *info) {
    TitleReadContext *ctx = (TitleReadContext *)info->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)info->ResultCode, ctx->operation, ctx->context);
    payload["data"] = ctx->data;
    if (ctx->request != nullptr) {
        ctx->table->EOS_TitleStorageFileTransferRequest_Release((EOS_HTitleStorageFileTransferRequest)ctx->request);
    }
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
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("storage.write", "Platform handle not created yet");
    }
    EOS_HPlayerDataStorage storage = bindings()->EOS_Platform_GetPlayerDataStorageInterface(h);
    if (storage == nullptr) {
        return not_implemented("storage.write", "PlayerDataStorage interface unavailable on this platform");
    }
    EOS_ProductUserId user = bindings()->EOS_ProductUserId_FromString(local_user_id.utf8().get_data());
    if (bindings()->EOS_ProductUserId_IsValid(user) == EOS_FALSE) {
        godot::Dictionary context;
        context["reason"] = "Invalid Product User ID string";
        return EOSResult::make_result(10, "storage.write", context);
    }
    godot::Dictionary context;
    context["filename"] = filename;
    StorageWriteContext *ctx = new StorageWriteContext();
    ctx->queue = platform->get_queue().ptr();
    ctx->table = platform->api_table();
    ctx->platform = platform;
    ctx->operation = "storage.write";
    ctx->context = context;
    ctx->data = data;
    godot::CharString filename_utf = filename.utf8();
    EOS_PlayerDataStorage_WriteFileOptions options = {};
    options.ApiVersion = EOS_PLAYERDATASTORAGE_WRITEFILE_API_LATEST;
    options.LocalUserId = user;
    options.Filename = filename_utf.get_data();
    options.ChunkLengthBytes = 262144;
    options.WriteFileDataCallback = storage_write_data_cb;
    options.FileTransferProgressCallback = nullptr;
    EOS_HPlayerDataStorageFileTransferRequest request = bindings()->EOS_PlayerDataStorage_WriteFile(storage, &options, ctx, storage_write_complete_cb);
    if (request == nullptr) {
        delete ctx;
        context["reason"] = "WriteFile rejected the request";
        return EOSResult::make_result(10, "storage.write", context);
    }
    ctx->request = request;
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "storage.write";
    return pending;
#else
    return not_implemented("storage.write", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
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
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("storage.read", "Platform handle not created yet");
    }
    EOS_HPlayerDataStorage storage = bindings()->EOS_Platform_GetPlayerDataStorageInterface(h);
    if (storage == nullptr) {
        return not_implemented("storage.read", "PlayerDataStorage interface unavailable on this platform");
    }
    EOS_ProductUserId user = bindings()->EOS_ProductUserId_FromString(local_user_id.utf8().get_data());
    if (bindings()->EOS_ProductUserId_IsValid(user) == EOS_FALSE) {
        godot::Dictionary context;
        context["reason"] = "Invalid Product User ID string";
        return EOSResult::make_result(10, "storage.read", context);
    }
    godot::Dictionary context;
    context["filename"] = filename;
    StorageReadContext *ctx = new StorageReadContext();
    ctx->queue = platform->get_queue().ptr();
    ctx->table = platform->api_table();
    ctx->platform = platform;
    ctx->operation = "storage.read";
    ctx->context = context;
    godot::CharString filename_utf = filename.utf8();
    EOS_PlayerDataStorage_ReadFileOptions options = {};
    options.ApiVersion = EOS_PLAYERDATASTORAGE_READFILE_API_LATEST;
    options.LocalUserId = user;
    options.Filename = filename_utf.get_data();
    options.ReadChunkLengthBytes = 262144;
    options.ReadFileDataCallback = storage_read_data_cb;
    options.FileTransferProgressCallback = nullptr;
    EOS_HPlayerDataStorageFileTransferRequest request = bindings()->EOS_PlayerDataStorage_ReadFile(storage, &options, ctx, storage_read_complete_cb);
    if (request == nullptr) {
        delete ctx;
        context["reason"] = "ReadFile rejected the request";
        return EOSResult::make_result(10, "storage.read", context);
    }
    ctx->request = request;
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "storage.read";
    return pending;
#else
    return not_implemented("storage.read", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
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
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("titlestorage.read", "Platform handle not created yet");
    }
    EOS_HTitleStorage titlestorage = bindings()->EOS_Platform_GetTitleStorageInterface(h);
    if (titlestorage == nullptr) {
        return not_implemented("titlestorage.read", "TitleStorage interface unavailable on this platform");
    }
    godot::Dictionary context;
    context["filename"] = filename;
    TitleReadContext *ctx = new TitleReadContext();
    ctx->queue = platform->get_queue().ptr();
    ctx->table = platform->api_table();
    ctx->platform = platform;
    ctx->operation = "titlestorage.read";
    ctx->context = context;
    godot::CharString filename_utf = filename.utf8();
    EOS_TitleStorage_ReadFileOptions options = {};
    options.ApiVersion = EOS_TITLESTORAGE_READFILE_API_LATEST;
    options.LocalUserId = nullptr;
    options.Filename = filename_utf.get_data();
    options.ReadChunkLengthBytes = 262144;
    options.ReadFileDataCallback = title_read_data_cb;
    options.FileTransferProgressCallback = nullptr;
    EOS_HTitleStorageFileTransferRequest request = bindings()->EOS_TitleStorage_ReadFile(titlestorage, &options, ctx, title_read_complete_cb);
    if (request == nullptr) {
        delete ctx;
        context["reason"] = "ReadFile rejected the request";
        return EOSResult::make_result(10, "titlestorage.read", context);
    }
    ctx->request = request;
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "titlestorage.read";
    return pending;
#else
    return not_implemented("titlestorage.read", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
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
    ctx->handle = sanctions;
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

godot::Dictionary EOSUIInterface::ui_pause_overlay(bool paused) {
    godot::Dictionary ready = require_ready("ui.pause_overlay");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("ui.pause_overlay", "Platform handle not created yet");
    }
    EOS_HUI ui = bindings()->EOS_Platform_GetUIInterface(h);
    if (ui == nullptr) {
        return not_implemented("ui.pause_overlay", "UI interface unavailable on this platform");
    }
    EOS_UI_PauseSocialOverlayOptions options = {};
    options.ApiVersion = EOS_UI_PAUSESOCIALOVERLAY_API_LATEST;
    options.bIsPaused = paused ? EOS_TRUE : EOS_FALSE;
    EOS_EResult result = bindings()->EOS_UI_PauseSocialOverlay(ui, &options);
    return EOSResult::make_result((int64_t)result, "ui.pause_overlay", godot::Dictionary());
#else
    return not_implemented("ui.pause_overlay", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSUIInterface::ui_is_overlay_paused() {
    godot::Dictionary ready = require_ready("ui.is_overlay_paused");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("ui.is_overlay_paused", "Platform handle not created yet");
    }
    EOS_HUI ui = bindings()->EOS_Platform_GetUIInterface(h);
    if (ui == nullptr) {
        return not_implemented("ui.is_overlay_paused", "UI interface unavailable on this platform");
    }
    EOS_UI_IsSocialOverlayPausedOptions options = {};
    options.ApiVersion = EOS_UI_ISSOCIALOVERLAYPAUSED_API_LATEST;
    EOS_Bool paused = bindings()->EOS_UI_IsSocialOverlayPaused(ui, &options);
    godot::Dictionary out = EOSResult::make_result(0, "ui.is_overlay_paused", godot::Dictionary());
    out["paused"] = paused == EOS_TRUE;
    return out;
#else
    return not_implemented("ui.is_overlay_paused", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSUIInterface::ui_get_friends_visible(const godot::String &local_user_id) {
    godot::Dictionary ready = require_ready("ui.get_friends_visible");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("ui.get_friends_visible", "Platform handle not created yet");
    }
    EOS_HUI ui = bindings()->EOS_Platform_GetUIInterface(h);
    if (ui == nullptr) {
        return not_implemented("ui.get_friends_visible", "UI interface unavailable on this platform");
    }
    EOS_EpicAccountId user = bindings()->EOS_EpicAccountId_FromString(local_user_id.utf8().get_data());
    EOS_UI_GetFriendsVisibleOptions options = {};
    options.ApiVersion = EOS_UI_GETFRIENDSVISIBLE_API_LATEST;
    options.LocalUserId = bindings()->EOS_EpicAccountId_IsValid(user) == EOS_TRUE ? user : nullptr;
    EOS_Bool visible = bindings()->EOS_UI_GetFriendsVisible(ui, &options);
    godot::Dictionary out = EOSResult::make_result(0, "ui.get_friends_visible", godot::Dictionary());
    out["visible"] = visible == EOS_TRUE;
    return out;
#else
    return not_implemented("ui.get_friends_visible", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSUIInterface::ui_set_notification_location(int64_t location) {
    godot::Dictionary ready = require_ready("ui.set_notification_location");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("ui.set_notification_location", "Platform handle not created yet");
    }
    EOS_HUI ui = bindings()->EOS_Platform_GetUIInterface(h);
    if (ui == nullptr) {
        return not_implemented("ui.set_notification_location", "UI interface unavailable on this platform");
    }
    EOS_UI_SetDisplayPreferenceOptions options = {};
    options.ApiVersion = EOS_UI_SETDISPLAYPREFERENCE_API_LATEST;
    options.NotificationLocation = (EOS_UI_ENotificationLocation)location;
    EOS_EResult result = bindings()->EOS_UI_SetDisplayPreference(ui, &options);
    return EOSResult::make_result((int64_t)result, "ui.set_notification_location", godot::Dictionary());
#else
    return not_implemented("ui.set_notification_location", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSSocialInterface::sanctions_appeal(const godot::String &local_user_id, int64_t reason, const godot::String &reference_id) {
    godot::Dictionary ready = require_ready("sanctions.appeal");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("sanctions.appeal", "Platform handle not created yet");
    }
    EOS_HSanctions sanctions = bindings()->EOS_Platform_GetSanctionsInterface(h);
    if (sanctions == nullptr) {
        return not_implemented("sanctions.appeal", "Sanctions interface unavailable on this platform");
    }
    EOS_ProductUserId user = bindings()->EOS_ProductUserId_FromString(local_user_id.utf8().get_data());
    if (bindings()->EOS_ProductUserId_IsValid(user) == EOS_FALSE || reference_id.is_empty()) {
        godot::Dictionary context;
        context["reason"] = "Valid user id and sanction reference_id are required";
        return EOSResult::make_result(10, "sanctions.appeal", context);
    }
    godot::Dictionary context;
    context["reference_id"] = reference_id;
    RequestContext *ctx = make_request("sanctions.appeal", context);
    godot::CharString ref_utf = reference_id.utf8();
    EOS_Sanctions_CreatePlayerSanctionAppealOptions options = {};
    options.ApiVersion = EOS_SANCTIONS_CREATEPLAYERSANCTIONAPPEAL_API_LATEST;
    options.LocalUserId = user;
    options.Reason = (EOS_ESanctionAppealReason)reason;
    options.ReferenceId = ref_utf.get_data();
    bindings()->EOS_Sanctions_CreatePlayerSanctionAppeal(sanctions, &options, ctx, sanctions_appeal_cb);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "sanctions.appeal";
    return pending;
#else
    return not_implemented("sanctions.appeal", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSSocialInterface::rtc_set_sending_volume(const godot::String &local_user_id, const godot::String &room, double volume) {
    godot::Dictionary ready = require_ready("rtc.set_sending_volume");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("rtc.set_sending_volume", "Platform handle not created yet");
    }
    EOS_HRTC rtc = bindings()->EOS_Platform_GetRTCInterface(h);
    if (rtc == nullptr) {
        return not_implemented("rtc.set_sending_volume", "RTC interface unavailable on this platform");
    }
    EOS_HRTCAudio audio = bindings()->EOS_RTC_GetAudioInterface(rtc);
    if (audio == nullptr) {
        return not_implemented("rtc.set_sending_volume", "RTC audio interface unavailable");
    }
    EOS_ProductUserId user = bindings()->EOS_ProductUserId_FromString(local_user_id.utf8().get_data());
    if (bindings()->EOS_ProductUserId_IsValid(user) == EOS_FALSE || room.is_empty()) {
        godot::Dictionary context;
        context["reason"] = "Valid user id and room name are required";
        return EOSResult::make_result(10, "rtc.set_sending_volume", context);
    }
    godot::Dictionary context;
    context["room"] = room;
    RequestContext *ctx = make_request("rtc.set_sending_volume", context);
    godot::CharString room_utf = room.utf8();
    EOS_RTCAudio_UpdateSendingVolumeOptions options = {};
    options.ApiVersion = EOS_RTCAUDIO_UPDATESENDINGVOLUME_API_LATEST;
    options.LocalUserId = user;
    options.RoomName = room_utf.get_data();
    options.Volume = (float)volume;
    bindings()->EOS_RTCAudio_UpdateSendingVolume(audio, &options, ctx, rtc_sending_volume_cb);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "rtc.set_sending_volume";
    return pending;
#else
    return not_implemented("rtc.set_sending_volume", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSSocialInterface::rtc_set_receiving_volume(const godot::String &local_user_id, const godot::String &room, double volume) {
    godot::Dictionary ready = require_ready("rtc.set_receiving_volume");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("rtc.set_receiving_volume", "Platform handle not created yet");
    }
    EOS_HRTC rtc = bindings()->EOS_Platform_GetRTCInterface(h);
    if (rtc == nullptr) {
        return not_implemented("rtc.set_receiving_volume", "RTC interface unavailable on this platform");
    }
    EOS_HRTCAudio audio = bindings()->EOS_RTC_GetAudioInterface(rtc);
    if (audio == nullptr) {
        return not_implemented("rtc.set_receiving_volume", "RTC audio interface unavailable");
    }
    EOS_ProductUserId user = bindings()->EOS_ProductUserId_FromString(local_user_id.utf8().get_data());
    if (bindings()->EOS_ProductUserId_IsValid(user) == EOS_FALSE || room.is_empty()) {
        godot::Dictionary context;
        context["reason"] = "Valid user id and room name are required";
        return EOSResult::make_result(10, "rtc.set_receiving_volume", context);
    }
    godot::Dictionary context;
    context["room"] = room;
    RequestContext *ctx = make_request("rtc.set_receiving_volume", context);
    godot::CharString room_utf = room.utf8();
    EOS_RTCAudio_UpdateReceivingVolumeOptions options = {};
    options.ApiVersion = EOS_RTCAUDIO_UPDATERECEIVINGVOLUME_API_LATEST;
    options.LocalUserId = user;
    options.RoomName = room_utf.get_data();
    options.Volume = (float)volume;
    bindings()->EOS_RTCAudio_UpdateReceivingVolume(audio, &options, ctx, rtc_receiving_volume_cb);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "rtc.set_receiving_volume";
    return pending;
#else
    return not_implemented("rtc.set_receiving_volume", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSSocialInterface::metrics_end_session(const godot::String &account_id) {
    godot::Dictionary ready = require_ready("metrics.end_session");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("metrics.end_session", "Platform handle not created yet");
    }
    EOS_HMetrics metrics = bindings()->EOS_Platform_GetMetricsInterface(h);
    if (metrics == nullptr) {
        return not_implemented("metrics.end_session", "Metrics interface unavailable on this platform");
    }
    EOS_EpicAccountId account = bindings()->EOS_EpicAccountId_FromString(account_id.utf8().get_data());
    if (bindings()->EOS_EpicAccountId_IsValid(account) == EOS_FALSE) {
        godot::Dictionary context;
        context["reason"] = "Invalid Epic Account ID string";
        return EOSResult::make_result(10, "metrics.end_session", context);
    }
    EOS_Metrics_EndPlayerSessionOptions options = {};
    options.ApiVersion = EOS_METRICS_ENDPLAYERSESSION_API_LATEST;
    options.AccountIdType = EOS_EMetricsAccountIdType::EOS_MAIT_Epic;
    options.AccountId.Epic = account;
    EOS_EResult result = bindings()->EOS_Metrics_EndPlayerSession(metrics, &options);
    return EOSResult::make_result((int64_t)result, "metrics.end_session", godot::Dictionary());
#else
    return not_implemented("metrics.end_session", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSSocialInterface::mods_enumerate(const godot::String &local_user_id, int64_t mod_type) {
    godot::Dictionary ready = require_ready("mods.enumerate");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("mods.enumerate", "Platform handle not created yet");
    }
    EOS_HMods mods = bindings()->EOS_Platform_GetModsInterface(h);
    if (mods == nullptr) {
        return not_implemented("mods.enumerate", "Mods interface unavailable on this platform");
    }
    EOS_EpicAccountId user = bindings()->EOS_EpicAccountId_FromString(local_user_id.utf8().get_data());
    if (bindings()->EOS_EpicAccountId_IsValid(user) == EOS_FALSE) {
        godot::Dictionary context;
        context["reason"] = "Invalid Epic Account ID string";
        return EOSResult::make_result(10, "mods.enumerate", context);
    }
    godot::Dictionary context;
    RequestContext *ctx = make_request("mods.enumerate", context);
    EOS_Mods_EnumerateModsOptions options = {};
    options.ApiVersion = EOS_MODS_ENUMERATEMODS_API_LATEST;
    options.LocalUserId = user;
    options.Type = (EOS_EModEnumerationType)mod_type;
    bindings()->EOS_Mods_EnumerateMods(mods, &options, ctx, mods_enumerate_cb);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "mods.enumerate";
    return pending;
#else
    return not_implemented("mods.enumerate", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSSocialInterface::mods_get_info(const godot::String &local_user_id, int64_t mod_type) {
    godot::Dictionary ready = require_ready("mods.get_info");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("mods.get_info", "Platform handle not created yet");
    }
    EOS_HMods mods = bindings()->EOS_Platform_GetModsInterface(h);
    if (mods == nullptr) {
        return not_implemented("mods.get_info", "Mods interface unavailable on this platform");
    }
    EOS_EpicAccountId user = bindings()->EOS_EpicAccountId_FromString(local_user_id.utf8().get_data());
    EOS_Mods_CopyModInfoOptions options = {};
    options.ApiVersion = EOS_MODS_COPYMODINFO_API_LATEST;
    options.LocalUserId = user;
    options.Type = (EOS_EModEnumerationType)mod_type;
    EOS_Mods_ModInfo *info = nullptr;
    EOS_EResult copy_result = bindings()->EOS_Mods_CopyModInfo(mods, &options, &info);
    if (copy_result != EOS_EResult::EOS_Success || info == nullptr) {
        godot::Dictionary context;
        context["reason"] = "No cached mod info. Call mods.enumerate first.";
        return EOSResult::make_result((int64_t)copy_result, "mods.get_info", context);
    }
    godot::Dictionary out = EOSResult::make_result(0, "mods.get_info", godot::Dictionary());
    out["mods_count"] = (int64_t)info->ModsCount;
    godot::Array items;
    for (int32_t i = 0; i < info->ModsCount && info->Mods != nullptr; i++) {
        godot::Dictionary item;
        item["namespace_id"] = godot::String(info->Mods[i].NamespaceId ? info->Mods[i].NamespaceId : "");
        item["item_id"] = godot::String(info->Mods[i].ItemId ? info->Mods[i].ItemId : "");
        item["artifact_id"] = godot::String(info->Mods[i].ArtifactId ? info->Mods[i].ArtifactId : "");
        item["title"] = godot::String(info->Mods[i].Title ? info->Mods[i].Title : "");
        item["version"] = godot::String(info->Mods[i].Version ? info->Mods[i].Version : "");
        items.push_back(item);
    }
    out["mods"] = items;
    bindings()->EOS_Mods_ModInfo_Release(info);
    return out;
#else
    return not_implemented("mods.get_info", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSSocialInterface::kws_query_age_gate() {
    godot::Dictionary ready = require_ready("kws.query_age_gate");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("kws.query_age_gate", "Platform handle not created yet");
    }
    EOS_HKWS kws = bindings()->EOS_Platform_GetKWSInterface(h);
    if (kws == nullptr) {
        return not_implemented("kws.query_age_gate", "KWS interface unavailable on this platform");
    }
    godot::Dictionary context;
    RequestContext *ctx = make_request("kws.query_age_gate", context);
    EOS_KWS_QueryAgeGateOptions options = {};
    options.ApiVersion = EOS_KWS_QUERYAGEGATE_API_LATEST;
    bindings()->EOS_KWS_QueryAgeGate(kws, &options, ctx, kws_age_gate_cb);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "kws.query_age_gate";
    return pending;
#else
    return not_implemented("kws.query_age_gate", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSSocialInterface::custom_invites_set(const godot::String &local_user_id, const godot::String &payload) {
    godot::Dictionary ready = require_ready("custom_invites.set");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("custom_invites.set", "Platform handle not created yet");
    }
    EOS_HCustomInvites invites = bindings()->EOS_Platform_GetCustomInvitesInterface(h);
    if (invites == nullptr) {
        return not_implemented("custom_invites.set", "CustomInvites interface unavailable on this platform");
    }
    EOS_ProductUserId user = bindings()->EOS_ProductUserId_FromString(local_user_id.utf8().get_data());
    if (bindings()->EOS_ProductUserId_IsValid(user) == EOS_FALSE || payload.is_empty()) {
        godot::Dictionary context;
        context["reason"] = "Valid user id and non-empty payload are required";
        return EOSResult::make_result(10, "custom_invites.set", context);
    }
    godot::CharString payload_utf = payload.utf8();
    EOS_CustomInvites_SetCustomInviteOptions options = {};
    options.ApiVersion = EOS_CUSTOMINVITES_SETCUSTOMINVITE_API_LATEST;
    options.LocalUserId = user;
    options.Payload = payload_utf.get_data();
    EOS_EResult result = bindings()->EOS_CustomInvites_SetCustomInvite(invites, &options);
    return EOSResult::make_result((int64_t)result, "custom_invites.set", godot::Dictionary());
#else
    return not_implemented("custom_invites.set", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSSocialInterface::custom_invites_send(const godot::String &local_user_id, const godot::Array &target_user_ids) {
    godot::Dictionary ready = require_ready("custom_invites.send");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("custom_invites.send", "Platform handle not created yet");
    }
    EOS_HCustomInvites invites = bindings()->EOS_Platform_GetCustomInvitesInterface(h);
    if (invites == nullptr) {
        return not_implemented("custom_invites.send", "CustomInvites interface unavailable on this platform");
    }
    EOS_ProductUserId local = bindings()->EOS_ProductUserId_FromString(local_user_id.utf8().get_data());
    if (bindings()->EOS_ProductUserId_IsValid(local) == EOS_FALSE || target_user_ids.is_empty()) {
        godot::Dictionary context;
        context["reason"] = "Valid user id and at least one target user id are required";
        return EOSResult::make_result(10, "custom_invites.send", context);
    }
    std::vector<godot::CharString> id_keepalive;
    std::vector<EOS_ProductUserId> targets;
    id_keepalive.reserve(target_user_ids.size());
    targets.reserve(target_user_ids.size());
    for (int i = 0; i < target_user_ids.size(); i++) {
        id_keepalive.push_back(godot::String(target_user_ids[i]).utf8());
        targets.push_back(bindings()->EOS_ProductUserId_FromString(id_keepalive.back().get_data()));
    }
    godot::Dictionary context;
    RequestContext *ctx = make_request("custom_invites.send", context);
    EOS_CustomInvites_SendCustomInviteOptions options = {};
    options.ApiVersion = EOS_CUSTOMINVITES_SENDCUSTOMINVITE_API_LATEST;
    options.LocalUserId = local;
    options.TargetUserIds = targets.data();
    options.TargetUserIdsCount = (uint32_t)targets.size();
    bindings()->EOS_CustomInvites_SendCustomInvite(invites, &options, ctx, custom_invites_send_cb);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "custom_invites.send";
    return pending;
#else
    return not_implemented("custom_invites.send", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSSocialInterface::snapshot_begin(const godot::String &local_user_id) {
    godot::Dictionary ready = require_ready("snapshot.begin");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("snapshot.begin", "Platform handle not created yet");
    }
    EOS_HProgressionSnapshot snapshot = bindings()->EOS_Platform_GetProgressionSnapshotInterface(h);
    if (snapshot == nullptr) {
        return not_implemented("snapshot.begin", "ProgressionSnapshot interface unavailable on this platform");
    }
    EOS_ProductUserId user = bindings()->EOS_ProductUserId_FromString(local_user_id.utf8().get_data());
    if (bindings()->EOS_ProductUserId_IsValid(user) == EOS_FALSE) {
        godot::Dictionary context;
        context["reason"] = "Invalid Product User ID string";
        return EOSResult::make_result(10, "snapshot.begin", context);
    }
    EOS_ProgressionSnapshot_BeginSnapshotOptions options = {};
    options.ApiVersion = EOS_PROGRESSIONSNAPSHOT_BEGINSNAPSHOT_API_LATEST;
    options.LocalUserId = user;
    uint32_t snapshot_id = 0;
    EOS_EResult result = bindings()->EOS_ProgressionSnapshot_BeginSnapshot(snapshot, &options, &snapshot_id);
    godot::Dictionary out = EOSResult::make_result((int64_t)result, "snapshot.begin", godot::Dictionary());
    out["snapshot_id"] = (int64_t)snapshot_id;
    return out;
#else
    return not_implemented("snapshot.begin", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSSocialInterface::snapshot_add(int64_t snapshot_id, const godot::String &key, const godot::String &value) {
    godot::Dictionary ready = require_ready("snapshot.add");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("snapshot.add", "Platform handle not created yet");
    }
    EOS_HProgressionSnapshot snapshot = bindings()->EOS_Platform_GetProgressionSnapshotInterface(h);
    if (snapshot == nullptr) {
        return not_implemented("snapshot.add", "ProgressionSnapshot interface unavailable on this platform");
    }
    if (key.is_empty()) {
        godot::Dictionary context;
        context["reason"] = "key is required";
        return EOSResult::make_result(10, "snapshot.add", context);
    }
    godot::CharString key_utf = key.utf8();
    godot::CharString value_utf = value.utf8();
    EOS_ProgressionSnapshot_AddProgressionOptions options = {};
    options.ApiVersion = EOS_PROGRESSIONSNAPSHOT_ADDPROGRESSION_API_LATEST;
    options.SnapshotId = (uint32_t)snapshot_id;
    options.Key = key_utf.get_data();
    options.Value = value_utf.get_data();
    EOS_EResult result = bindings()->EOS_ProgressionSnapshot_AddProgression(snapshot, &options);
    return EOSResult::make_result((int64_t)result, "snapshot.add", godot::Dictionary());
#else
    return not_implemented("snapshot.add", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSSocialInterface::snapshot_submit(int64_t snapshot_id) {
    godot::Dictionary ready = require_ready("snapshot.submit");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("snapshot.submit", "Platform handle not created yet");
    }
    EOS_HProgressionSnapshot snapshot = bindings()->EOS_Platform_GetProgressionSnapshotInterface(h);
    if (snapshot == nullptr) {
        return not_implemented("snapshot.submit", "ProgressionSnapshot interface unavailable on this platform");
    }
    godot::Dictionary context;
    context["snapshot_id"] = snapshot_id;
    RequestContext *ctx = make_request("snapshot.submit", context);
    EOS_ProgressionSnapshot_SubmitSnapshotOptions options = {};
    options.ApiVersion = EOS_PROGRESSIONSNAPSHOT_SUBMITSNAPSHOT_API_LATEST;
    options.SnapshotId = (uint32_t)snapshot_id;
    bindings()->EOS_ProgressionSnapshot_SubmitSnapshot(snapshot, &options, ctx, snapshot_submit_cb);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "snapshot.submit";
    return pending;
#else
    return not_implemented("snapshot.submit", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSSocialInterface::snapshot_end(int64_t snapshot_id) {
    godot::Dictionary ready = require_ready("snapshot.end");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("snapshot.end", "Platform handle not created yet");
    }
    EOS_HProgressionSnapshot snapshot = bindings()->EOS_Platform_GetProgressionSnapshotInterface(h);
    if (snapshot == nullptr) {
        return not_implemented("snapshot.end", "ProgressionSnapshot interface unavailable on this platform");
    }
    EOS_ProgressionSnapshot_EndSnapshotOptions options = {};
    options.ApiVersion = EOS_PROGRESSIONSNAPSHOT_ENDSNAPSHOT_API_LATEST;
    options.SnapshotId = (uint32_t)snapshot_id;
    EOS_EResult result = bindings()->EOS_ProgressionSnapshot_EndSnapshot(snapshot, &options);
    return EOSResult::make_result((int64_t)result, "snapshot.end", godot::Dictionary());
#else
    return not_implemented("snapshot.end", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

}
