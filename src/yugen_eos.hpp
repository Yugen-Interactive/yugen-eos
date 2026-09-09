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
    godot::Dictionary connect_create_user(int64_t continuance_ref);
    godot::Dictionary auth_link_account(int64_t continuance_ref, int64_t link_flags);
    godot::Dictionary friends_query(const godot::String &local_user_id);
    godot::Dictionary friends_get_count(const godot::String &local_user_id);
    godot::Dictionary friends_send_invite(const godot::String &local_user_id, const godot::String &target_user_id);
    godot::Dictionary friends_accept_invite(const godot::String &local_user_id, const godot::String &target_user_id);
    godot::Dictionary friends_reject_invite(const godot::String &local_user_id, const godot::String &target_user_id);
    godot::Dictionary presence_set(const godot::String &local_user_id, int64_t status, const godot::String &details);
    godot::Dictionary presence_query(const godot::String &local_user_id, const godot::String &target_user_id);
    godot::Dictionary presence_has(const godot::String &local_user_id, const godot::String &target_user_id);
    godot::Dictionary userinfo_query(const godot::String &local_user_id, const godot::String &target_user_id);
    godot::Dictionary userinfo_get(const godot::String &target_user_id);
    godot::Dictionary sessions_create(const godot::String &local_user_id, const godot::String &session_name, int64_t max_players, bool is_public, const godot::Array &attributes);
    godot::Dictionary sessions_search(const godot::Dictionary &options);
    godot::Dictionary sessions_get_search_count();
    godot::Dictionary sessions_get_search_result(int64_t index);
    godot::Dictionary sessions_join(const godot::String &local_user_id, const godot::String &session_handle);
    godot::Dictionary sessions_destroy(const godot::String &session_name);
    godot::Dictionary sessions_send_invite(const godot::String &session_name, const godot::String &local_user_id, const godot::String &target_user_id);
    godot::Dictionary sessions_query_invites(const godot::String &local_user_id);
    godot::Dictionary sessions_reject_invite(const godot::String &local_user_id, const godot::String &invite_id);
    godot::Dictionary sessions_set_attributes(const godot::String &session_name, const godot::Array &attributes);
    godot::Dictionary sessions_get_attributes(int64_t session_ref);
    godot::Dictionary lobbies_create(const godot::String &local_user_id, int64_t max_members, bool is_public);
    godot::Dictionary lobbies_join(const godot::String &lobby_id, const godot::String &local_user_id);
    godot::Dictionary lobbies_join_details(int64_t lobby_ref, const godot::String &local_user_id);
    godot::Dictionary lobbies_search(const godot::String &local_user_id, int64_t max_results);
    godot::Dictionary lobbies_get_search_count();
    godot::Dictionary lobbies_get_search_result(int64_t index);
    godot::Dictionary lobbies_get_details(const godot::String &lobby_id, const godot::String &local_user_id);
    godot::Dictionary lobbies_leave(const godot::String &lobby_id, const godot::String &local_user_id);
    godot::Dictionary lobbies_send_invite(const godot::String &lobby_id, const godot::String &local_user_id, const godot::String &target_user_id);
    godot::Dictionary p2p_send(const godot::String &local_user_id, const godot::String &remote_user_id, const godot::String &socket_id, const godot::PackedByteArray &data, bool reliable);
    godot::Dictionary p2p_receive(const godot::String &local_user_id, const godot::String &socket_id);
    godot::Dictionary p2p_set_relay(const godot::String &local_user_id, bool allow_relays);
    godot::Dictionary p2p_query_nat();
    godot::Dictionary stats_query(const godot::String &local_user_id, const godot::Array &stat_names);
    godot::Dictionary stats_ingest(const godot::String &local_user_id, const godot::Array &stats);
    godot::Dictionary achievements_unlock(const godot::String &local_user_id, const godot::String &achievement_id);
    godot::Dictionary achievements_query(const godot::String &local_user_id, const godot::Array &achievement_ids);
    godot::Dictionary achievements_query_definitions(const godot::String &local_user_id);
    godot::Dictionary achievements_get_definition_count();
    godot::Dictionary achievements_get_definition(int64_t index);
    godot::Dictionary leaderboards_query_definitions();
    godot::Dictionary leaderboards_query_ranks(const godot::String &leaderboard_id, const godot::String &local_user_id);
    godot::Dictionary leaderboards_query_scores(const godot::String &local_user_id, const godot::String &stat_name, const godot::Array &target_user_ids);
    godot::Dictionary leaderboards_query(const godot::String &leaderboard_id, const godot::String &local_user_id);
    godot::Dictionary storage_write(const godot::String &local_user_id, const godot::String &filename, const godot::PackedByteArray &data);
    godot::Dictionary storage_read(const godot::String &local_user_id, const godot::String &filename);
    godot::Dictionary storage_query_list(const godot::String &local_user_id);
    godot::Dictionary titlestorage_read(const godot::String &filename);
    godot::Dictionary ecom_query_offers(const godot::String &local_user_id);
    godot::Dictionary ecom_query_entitlements(const godot::String &local_user_id);
    godot::Dictionary ecom_checkout(const godot::String &local_user_id, const godot::String &offer_id);
    godot::Dictionary ui_show_friends(const godot::String &local_user_id);
    godot::Dictionary ui_hide_friends();
    godot::Dictionary ui_set_visibility(bool visible);
    godot::Dictionary ui_pause_overlay(bool paused);
    godot::Dictionary ui_is_overlay_paused();
    godot::Dictionary ui_get_friends_visible(const godot::String &local_user_id);
    godot::Dictionary ui_set_notification_location(int64_t location);
    godot::Dictionary anticheat_start(const godot::String &local_user_id);
    godot::Dictionary metrics_send(const godot::String &event_name, const godot::Dictionary &attributes);
    godot::Dictionary metrics_begin_session(const godot::String &account_id);
    godot::Dictionary metrics_end_session(const godot::String &account_id);
    godot::Dictionary reports_send(const godot::String &reporter_id, const godot::String &reported_id, int64_t category, const godot::String &message);
    godot::Dictionary sanctions_query(const godot::String &player_id);
    godot::Dictionary sanctions_appeal(const godot::String &local_user_id, int64_t reason, const godot::String &reference_id);
    godot::Dictionary rtc_join(const godot::String &local_user_id, const godot::String &room);
    godot::Dictionary rtc_leave(const godot::String &local_user_id, const godot::String &room);
    godot::Dictionary rtc_set_sending_volume(const godot::String &local_user_id, const godot::String &room, double volume);
    godot::Dictionary rtc_set_receiving_volume(const godot::String &local_user_id, const godot::String &room, double volume);
    godot::Dictionary mods_enumerate(const godot::String &local_user_id, int64_t mod_type);
    godot::Dictionary mods_get_info(const godot::String &local_user_id, int64_t mod_type);
    godot::Dictionary kws_query_age_gate();
    godot::Dictionary custom_invites_set(const godot::String &local_user_id, const godot::String &payload);
    godot::Dictionary custom_invites_send(const godot::String &local_user_id, const godot::Array &target_user_ids);
    godot::Dictionary snapshot_begin(const godot::String &local_user_id);
    godot::Dictionary snapshot_add(int64_t snapshot_id, const godot::String &key, const godot::String &value);
    godot::Dictionary snapshot_submit(int64_t snapshot_id);
    godot::Dictionary snapshot_end(int64_t snapshot_id);
};

}
