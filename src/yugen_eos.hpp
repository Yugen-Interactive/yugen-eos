#pragma once

#include "core/eos_platform_manager.hpp"
#include "interfaces/eos_auth_iface.hpp"
#include "interfaces/eos_connect_iface.hpp"
#include "interfaces/eos_multiplayer.hpp"
#include "interfaces/eos_platform_services.hpp"
#include "interfaces/eos_progression.hpp"
#include "interfaces/eos_social_graph.hpp"

#include <godot_cpp/classes/object.hpp>

namespace yugen {

class YugenEOS : public godot::Object {
    GDCLASS(YugenEOS, godot::Object);

    static YugenEOS *singleton;
    godot::Ref<EOSPlatformManager> platform;
    godot::Ref<EOSAuthInterface> auth;
    godot::Ref<EOSConnectInterface> connect;
    godot::Ref<EOSFriendsInterface> friends;
    godot::Ref<EOSPresenceInterface> presence;
    godot::Ref<EOSUserInfoInterface> userinfo;
    godot::Ref<EOSSessionsInterface> sessions;
    godot::Ref<EOSLobbiesInterface> lobbies;
    godot::Ref<EOSP2PInterface> p2p;
    godot::Ref<EOSStatsInterface> stats;
    godot::Ref<EOSAchievementsInterface> achievements;
    godot::Ref<EOSLeaderboardsInterface> leaderboards;
    godot::Ref<EOSStorageInterface> storage;
    godot::Ref<EOSEcomInterface> ecom;
    godot::Ref<EOSUIInterface> ui;
    godot::Ref<EOSSocialInterface> social;

protected:
    static void _bind_methods();

public:
    YugenEOS();
    ~YugenEOS();
    static YugenEOS *get_singleton();
    godot::Dictionary initialize(const godot::Dictionary &config);
    void shutdown();
    void tick();
    bool is_initialized() const;
    godot::String sdk_version() const;
    godot::Dictionary get_diagnostics() const;
    godot::Dictionary auth_login(int64_t login_type, const godot::String &id, const godot::String &token);
    godot::Dictionary auth_logout(const godot::String &local_user_id);
    godot::Dictionary auth_verify(const godot::String &local_user_id);
    godot::Dictionary connect_login(int64_t credential_type, const godot::String &token, const godot::String &display_name);
    godot::Dictionary connect_logout(const godot::String &product_user_id);
    godot::Dictionary connect_create_user(const godot::String &continuance_token);
    godot::Dictionary friends_query(const godot::String &local_user_id);
    godot::Dictionary friends_get_count(const godot::String &local_user_id);
    godot::Dictionary friends_send_invite(const godot::String &local_user_id, const godot::String &target_user_id);
    godot::Dictionary presence_set(const godot::String &local_user_id, int64_t status, const godot::String &details);
    godot::Dictionary presence_query(const godot::String &local_user_id, const godot::String &target_user_id);
    godot::Dictionary presence_has(const godot::String &local_user_id, const godot::String &target_user_id);
    godot::Dictionary userinfo_query(const godot::String &local_user_id, const godot::String &target_user_id);
    godot::Dictionary userinfo_get(const godot::String &target_user_id);
    godot::Dictionary sessions_create(const godot::String &local_user_id, const godot::String &session_name, int64_t max_players, bool is_public);
    godot::Dictionary sessions_search(const godot::Dictionary &options);
    godot::Dictionary sessions_join(const godot::String &local_user_id, const godot::String &session_handle);
    godot::Dictionary lobbies_create(const godot::String &local_user_id, int64_t max_members, bool is_public);
    godot::Dictionary lobbies_join(const godot::String &lobby_id, const godot::String &local_user_id);
    godot::Dictionary lobbies_leave(const godot::String &lobby_id, const godot::String &local_user_id);
    godot::Dictionary lobbies_send_invite(const godot::String &lobby_id, const godot::String &local_user_id, const godot::String &target_user_id);
    godot::Dictionary p2p_send(const godot::String &local_user_id, const godot::String &remote_user_id, const godot::String &socket_id, const godot::PackedByteArray &data, bool reliable);
    godot::Dictionary p2p_receive(const godot::String &local_user_id, const godot::String &socket_id);
    godot::Dictionary p2p_set_relay(const godot::String &local_user_id, bool allow_relays);
    godot::Dictionary stats_query(const godot::String &local_user_id, const godot::Array &stat_names);
    godot::Dictionary stats_ingest(const godot::String &local_user_id, const godot::Array &stats);
    godot::Dictionary achievements_unlock(const godot::String &local_user_id, const godot::String &achievement_id);
    godot::Dictionary achievements_query(const godot::String &local_user_id, const godot::Array &achievement_ids);
    godot::Dictionary leaderboards_query_definitions();
    godot::Dictionary leaderboards_query_ranks(const godot::String &leaderboard_id, const godot::String &local_user_id);
    godot::Dictionary leaderboards_query_scores(const godot::String &local_user_id, const godot::String &stat_name, const godot::Array &target_user_ids);
    godot::Dictionary leaderboards_query(const godot::String &leaderboard_id, const godot::String &local_user_id);
    godot::Dictionary storage_write(const godot::String &local_user_id, const godot::String &filename, const godot::PackedByteArray &data);
    godot::Dictionary storage_read(const godot::String &local_user_id, const godot::String &filename);
    godot::Dictionary titlestorage_read(const godot::String &filename);
    godot::Dictionary ecom_query_offers(const godot::String &local_user_id);
    godot::Dictionary ecom_checkout(const godot::String &local_user_id, const godot::String &offer_id);
    godot::Dictionary ui_show_friends(const godot::String &local_user_id);
    godot::Dictionary ui_hide_friends();
    godot::Dictionary ui_set_visibility(bool visible);
    godot::Dictionary anticheat_start(const godot::String &local_user_id);
    godot::Dictionary metrics_send(const godot::String &event_name, const godot::Dictionary &attributes);
    godot::Dictionary reports_send(const godot::String &reporter_id, const godot::String &reported_id, int64_t category, const godot::String &message);
    godot::Dictionary sanctions_query(const godot::String &player_id);
    godot::Dictionary rtc_join(const godot::String &local_user_id, const godot::String &room);
    godot::Dictionary rtc_leave(const godot::String &local_user_id, const godot::String &room);
};

}
