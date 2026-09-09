#include "interfaces/eos_progression.hpp"

#include <vector>

#if YUGEN_EOS_HAS_SDK
#include "eos_common.h"
#include "eos_sdk.h"
#include "eos_stats.h"
#include "eos_achievements.h"
#include "eos_leaderboards.h"
#endif

namespace yugen {

void EOSStatsInterface::_bind_methods() {
    godot::ClassDB::bind_method(godot::D_METHOD("stats_query"), &EOSStatsInterface::stats_query);
    godot::ClassDB::bind_method(godot::D_METHOD("stats_ingest"), &EOSStatsInterface::stats_ingest);
    ADD_SIGNAL(godot::MethodInfo("query_completed", godot::PropertyInfo(godot::Variant::DICTIONARY, "result")));
}

void EOSAchievementsInterface::_bind_methods() {
    godot::ClassDB::bind_method(godot::D_METHOD("achievements_unlock"), &EOSAchievementsInterface::achievements_unlock);
    godot::ClassDB::bind_method(godot::D_METHOD("achievements_query"), &EOSAchievementsInterface::achievements_query);
    ADD_SIGNAL(godot::MethodInfo("unlocked", godot::PropertyInfo(godot::Variant::DICTIONARY, "result")));
}

void EOSLeaderboardsInterface::_bind_methods() {
    godot::ClassDB::bind_method(godot::D_METHOD("leaderboards_query_definitions"), &EOSLeaderboardsInterface::leaderboards_query_definitions);
    godot::ClassDB::bind_method(godot::D_METHOD("leaderboards_query_ranks"), &EOSLeaderboardsInterface::leaderboards_query_ranks);
    godot::ClassDB::bind_method(godot::D_METHOD("leaderboards_query_scores"), &EOSLeaderboardsInterface::leaderboards_query_scores);
    ADD_SIGNAL(godot::MethodInfo("query_completed", godot::PropertyInfo(godot::Variant::DICTIONARY, "result")));
}

#if YUGEN_EOS_HAS_SDK
static void stats_query_cb(const EOS_Stats_OnQueryStatsCompleteCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}

static void stats_ingest_cb(const EOS_Stats_IngestStatCompleteCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}

static void achievements_unlock_cb(const EOS_Achievements_OnUnlockAchievementsCompleteCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}

static void achievements_query_cb(const EOS_Achievements_OnQueryPlayerAchievementsCompleteCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}

static void leaderboards_ranks_cb(const EOS_Leaderboards_OnQueryLeaderboardRanksCompleteCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}

static void leaderboards_defs_cb(const EOS_Leaderboards_OnQueryLeaderboardDefinitionsCompleteCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}

static void leaderboards_scores_cb(const EOS_Leaderboards_OnQueryLeaderboardUserScoresCompleteCallbackInfo *data) {
    RequestContext *ctx = (RequestContext *)data->ClientData;
    godot::Dictionary payload = EOSResult::make_result((int64_t)data->ResultCode, ctx->operation, ctx->context);
    ctx->queue->enqueue(ctx->operation, payload);
    delete ctx;
}
#endif

godot::Dictionary EOSStatsInterface::stats_query(const godot::String &local_user_id, const godot::Array &stat_names) {
    godot::Dictionary ready = require_ready("stats.query");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("stats.query", "Platform handle not created yet");
    }
    EOS_HStats stats = bindings()->EOS_Platform_GetStatsInterface(h);
    if (stats == nullptr) {
        return not_implemented("stats.query", "Stats interface unavailable on this platform");
    }
    EOS_ProductUserId local = nullptr;
    EOS_ProductUserId target = nullptr;
    godot::CharString local_utf;
    if (!local_user_id.is_empty()) {
        local_utf = local_user_id.utf8();
        local = bindings()->EOS_ProductUserId_FromString(local_utf.get_data());
        target = local;
    }
    std::vector<godot::CharString> keepalive;
    std::vector<const char *> names;
    keepalive.reserve(stat_names.size());
    names.reserve(stat_names.size());
    for (int i = 0; i < stat_names.size(); i++) {
        keepalive.push_back(godot::String(stat_names[i]).utf8());
        names.push_back(keepalive.back().get_data());
    }
    godot::Dictionary context;
    RequestContext *ctx = make_request("stats.query", context);
    EOS_Stats_QueryStatsOptions options = {};
    options.ApiVersion = EOS_STATS_QUERYSTATS_API_LATEST;
    options.LocalUserId = local;
    options.StartTime = EOS_STATS_TIME_UNDEFINED;
    options.EndTime = EOS_STATS_TIME_UNDEFINED;
    options.StatNames = names.empty() ? nullptr : names.data();
    options.StatNamesCount = (uint32_t)names.size();
    options.TargetUserId = target;
    bindings()->EOS_Stats_QueryStats(stats, &options, ctx, stats_query_cb);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "stats.query";
    return pending;
#else
    return not_implemented("stats.query", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSStatsInterface::stats_ingest(const godot::String &local_user_id, const godot::Array &stats) {
    godot::Dictionary ready = require_ready("stats.ingest");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("stats.ingest", "Platform handle not created yet");
    }
    EOS_HStats handle = bindings()->EOS_Platform_GetStatsInterface(h);
    if (handle == nullptr) {
        return not_implemented("stats.ingest", "Stats interface unavailable on this platform");
    }
    EOS_ProductUserId local = nullptr;
    godot::CharString local_utf;
    if (!local_user_id.is_empty()) {
        local_utf = local_user_id.utf8();
        local = bindings()->EOS_ProductUserId_FromString(local_utf.get_data());
    }
    if (stats.is_empty()) {
        godot::Dictionary context;
        context["reason"] = "stats array is empty. Each entry needs name and value.";
        return EOSResult::make_result(10, "stats.ingest", context);
    }
    std::vector<EOS_Stats_IngestData> entries;
    std::vector<godot::CharString> name_keepalive;
    entries.reserve(stats.size());
    name_keepalive.reserve(stats.size());
    for (int i = 0; i < stats.size(); i++) {
        godot::Dictionary entry = stats[i];
        name_keepalive.push_back(godot::String(entry.get("name", "")).utf8());
        EOS_Stats_IngestData data = {};
        data.ApiVersion = EOS_STATS_INGESTDATA_API_LATEST;
        data.StatName = name_keepalive.back().get_data();
        data.IngestAmount = (int32_t)(int64_t)entry.get("value", 0);
        entries.push_back(data);
    }
    godot::Dictionary context;
    RequestContext *ctx = make_request("stats.ingest", context);
    EOS_Stats_IngestStatOptions options = {};
    options.ApiVersion = EOS_STATS_INGESTSTAT_API_LATEST;
    options.LocalUserId = local;
    options.Stats = entries.data();
    options.StatsCount = (uint32_t)entries.size();
    options.TargetUserId = local;
    bindings()->EOS_Stats_IngestStat(handle, &options, ctx, stats_ingest_cb);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "stats.ingest";
    return pending;
#else
    return not_implemented("stats.ingest", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSAchievementsInterface::achievements_unlock(const godot::String &local_user_id, const godot::String &achievement_id) {
    godot::Dictionary ready = require_ready("achievements.unlock");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("achievements.unlock", "Platform handle not created yet");
    }
    EOS_HAchievements achievements = bindings()->EOS_Platform_GetAchievementsInterface(h);
    if (achievements == nullptr) {
        return not_implemented("achievements.unlock", "Achievements interface unavailable on this platform");
    }
    EOS_ProductUserId user = bindings()->EOS_ProductUserId_FromString(local_user_id.utf8().get_data());
    if (bindings()->EOS_ProductUserId_IsValid(user) == EOS_FALSE || achievement_id.is_empty()) {
        godot::Dictionary context;
        context["reason"] = "Valid user id and achievement_id are required";
        return EOSResult::make_result(10, "achievements.unlock", context);
    }
    godot::Dictionary context;
    context["achievement_id"] = achievement_id;
    RequestContext *ctx = make_request("achievements.unlock", context);
    godot::CharString ach_utf = achievement_id.utf8();
    const char *ids[1] = { ach_utf.get_data() };
    EOS_Achievements_UnlockAchievementsOptions options = {};
    options.ApiVersion = EOS_ACHIEVEMENTS_UNLOCKACHIEVEMENTS_API_LATEST;
    options.UserId = user;
    options.AchievementIds = ids;
    options.AchievementsCount = 1;
    bindings()->EOS_Achievements_UnlockAchievements(achievements, &options, ctx, achievements_unlock_cb);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "achievements.unlock";
    return pending;
#else
    return not_implemented("achievements.unlock", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSAchievementsInterface::achievements_query(const godot::String &local_user_id, const godot::Array &achievement_ids) {
    godot::Dictionary ready = require_ready("achievements.query");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("achievements.query", "Platform handle not created yet");
    }
    EOS_HAchievements achievements = bindings()->EOS_Platform_GetAchievementsInterface(h);
    if (achievements == nullptr) {
        return not_implemented("achievements.query", "Achievements interface unavailable on this platform");
    }
    EOS_ProductUserId user = bindings()->EOS_ProductUserId_FromString(local_user_id.utf8().get_data());
    if (bindings()->EOS_ProductUserId_IsValid(user) == EOS_FALSE) {
        godot::Dictionary context;
        context["reason"] = "Invalid Product User ID string";
        return EOSResult::make_result(10, "achievements.query", context);
    }
    godot::Dictionary context;
    RequestContext *ctx = make_request("achievements.query", context);
    EOS_Achievements_QueryPlayerAchievementsOptions options = {};
    options.ApiVersion = EOS_ACHIEVEMENTS_QUERYPLAYERACHIEVEMENTS_API_LATEST;
    options.TargetUserId = user;
    options.LocalUserId = user;
    bindings()->EOS_Achievements_QueryPlayerAchievements(achievements, &options, ctx, achievements_query_cb);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "achievements.query";
    return pending;
#else
    return not_implemented("achievements.query", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSLeaderboardsInterface::leaderboards_query_definitions() {
    godot::Dictionary ready = require_ready("leaderboards.query_definitions");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("leaderboards.query_definitions", "Platform handle not created yet");
    }
    EOS_HLeaderboards boards = bindings()->EOS_Platform_GetLeaderboardsInterface(h);
    if (boards == nullptr) {
        return not_implemented("leaderboards.query_definitions", "Leaderboards interface unavailable on this platform");
    }
    godot::Dictionary context;
    RequestContext *ctx = make_request("leaderboards.query_definitions", context);
    EOS_Leaderboards_QueryLeaderboardDefinitionsOptions options = {};
    options.ApiVersion = EOS_LEADERBOARDS_QUERYLEADERBOARDDEFINITIONS_API_LATEST;
    bindings()->EOS_Leaderboards_QueryLeaderboardDefinitions(boards, &options, ctx, leaderboards_defs_cb);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "leaderboards.query_definitions";
    return pending;
#else
    return not_implemented("leaderboards.query_definitions", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSLeaderboardsInterface::leaderboards_query_ranks(const godot::String &leaderboard_id, const godot::String &local_user_id) {
    godot::Dictionary ready = require_ready("leaderboards.query_ranks");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("leaderboards.query_ranks", "Platform handle not created yet");
    }
    EOS_HLeaderboards boards = bindings()->EOS_Platform_GetLeaderboardsInterface(h);
    if (boards == nullptr) {
        return not_implemented("leaderboards.query_ranks", "Leaderboards interface unavailable on this platform");
    }
    if (leaderboard_id.is_empty()) {
        godot::Dictionary context;
        context["reason"] = "leaderboard_id is required";
        return EOSResult::make_result(10, "leaderboards.query_ranks", context);
    }
    EOS_ProductUserId local = nullptr;
    godot::CharString local_utf;
    if (!local_user_id.is_empty()) {
        local_utf = local_user_id.utf8();
        local = bindings()->EOS_ProductUserId_FromString(local_utf.get_data());
    }
    godot::Dictionary context;
    context["leaderboard_id"] = leaderboard_id;
    RequestContext *ctx = make_request("leaderboards.query_ranks", context);
    godot::CharString board_utf = leaderboard_id.utf8();
    EOS_Leaderboards_QueryLeaderboardRanksOptions options = {};
    options.ApiVersion = EOS_LEADERBOARDS_QUERYLEADERBOARDRANKS_API_LATEST;
    options.LeaderboardId = board_utf.get_data();
    options.LocalUserId = local;
    bindings()->EOS_Leaderboards_QueryLeaderboardRanks(boards, &options, ctx, leaderboards_ranks_cb);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "leaderboards.query_ranks";
    return pending;
#else
    return not_implemented("leaderboards.query_ranks", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

godot::Dictionary EOSLeaderboardsInterface::leaderboards_query_scores(const godot::String &local_user_id, const godot::String &stat_name, const godot::Array &target_user_ids) {
    godot::Dictionary ready = require_ready("leaderboards.query_scores");
    if (!bool(ready.get("ok", false))) {
        return ready;
    }
#if YUGEN_EOS_HAS_SDK
    EOS_HPlatform h = (EOS_HPlatform)platform->get_platform_handle();
    if (h == nullptr) {
        return not_implemented("leaderboards.query_scores", "Platform handle not created yet");
    }
    EOS_HLeaderboards boards = bindings()->EOS_Platform_GetLeaderboardsInterface(h);
    if (boards == nullptr) {
        return not_implemented("leaderboards.query_scores", "Leaderboards interface unavailable on this platform");
    }
    if (stat_name.is_empty() || target_user_ids.is_empty()) {
        godot::Dictionary context;
        context["reason"] = "stat_name and at least one target user id are required";
        return EOSResult::make_result(10, "leaderboards.query_scores", context);
    }
    EOS_ProductUserId local = nullptr;
    godot::CharString local_utf;
    if (!local_user_id.is_empty()) {
        local_utf = local_user_id.utf8();
        local = bindings()->EOS_ProductUserId_FromString(local_utf.get_data());
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
    context["stat_name"] = stat_name;
    RequestContext *ctx = make_request("leaderboards.query_scores", context);
    godot::CharString stat_utf = stat_name.utf8();
    EOS_Leaderboards_UserScoresQueryStatInfo stat_info = {};
    stat_info.ApiVersion = EOS_LEADERBOARDS_USERSCORESQUERYSTATINFO_API_LATEST;
    stat_info.StatName = stat_utf.get_data();
    stat_info.Aggregation = EOS_ELeaderboardAggregation::EOS_LA_Max;
    EOS_Leaderboards_QueryLeaderboardUserScoresOptions options = {};
    options.ApiVersion = EOS_LEADERBOARDS_QUERYLEADERBOARDUSERSCORES_API_LATEST;
    options.UserIds = targets.data();
    options.UserIdsCount = (uint32_t)targets.size();
    options.StatInfo = &stat_info;
    options.StatInfoCount = 1;
    options.StartTime = EOS_LEADERBOARDS_TIME_UNDEFINED;
    options.EndTime = EOS_LEADERBOARDS_TIME_UNDEFINED;
    options.LocalUserId = local;
    bindings()->EOS_Leaderboards_QueryLeaderboardUserScores(boards, &options, ctx, leaderboards_scores_cb);
    godot::Dictionary pending;
    pending["ok"] = true;
    pending["code"] = (int64_t)39;
    pending["name"] = "EOS_RequestInProgress";
    pending["operation"] = "leaderboards.query_scores";
    return pending;
#else
    return not_implemented("leaderboards.query_scores", "EOS SDK not linked. Rebuild with EOS_SDK_DIR pointing at SDK 1.19.1.");
#endif
}

}
