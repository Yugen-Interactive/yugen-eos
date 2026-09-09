#pragma once

#include "interfaces/eos_iface_base.hpp"

namespace yugen {

class EOSStatsInterface : public EOSInterfaceBase {
    GDCLASS(EOSStatsInterface, EOSInterfaceBase);

protected:
    static void _bind_methods();

public:
    godot::Dictionary stats_query(const godot::String &local_user_id, const godot::Array &stat_names);
    godot::Dictionary stats_ingest(const godot::String &local_user_id, const godot::Array &stats);
};

class EOSAchievementsInterface : public EOSInterfaceBase {
    GDCLASS(EOSAchievementsInterface, EOSInterfaceBase);

protected:
    static void _bind_methods();

public:
    godot::Dictionary achievements_unlock(const godot::String &local_user_id, const godot::String &achievement_id);
    godot::Dictionary achievements_query(const godot::String &local_user_id, const godot::Array &achievement_ids);
};

class EOSLeaderboardsInterface : public EOSInterfaceBase {
    GDCLASS(EOSLeaderboardsInterface, EOSInterfaceBase);

protected:
    static void _bind_methods();

public:
    godot::Dictionary leaderboards_query_definitions();
    godot::Dictionary leaderboards_query_ranks(const godot::String &leaderboard_id, const godot::String &local_user_id);
    godot::Dictionary leaderboards_query_scores(const godot::String &local_user_id, const godot::String &stat_name, const godot::Array &target_user_ids);
};

}
