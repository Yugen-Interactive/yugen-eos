#include "yugen_eos.hpp"

#include "core/eos_result.hpp"

namespace yugen {

YugenEOS *YugenEOS::singleton = nullptr;

void YugenEOS::_bind_methods() {
    godot::ClassDB::bind_method(godot::D_METHOD("initialize"), &YugenEOS::initialize);
    godot::ClassDB::bind_method(godot::D_METHOD("shutdown"), &YugenEOS::shutdown);
    godot::ClassDB::bind_method(godot::D_METHOD("tick"), &YugenEOS::tick);
    godot::ClassDB::bind_method(godot::D_METHOD("is_initialized"), &YugenEOS::is_initialized);
    godot::ClassDB::bind_method(godot::D_METHOD("sdk_version"), &YugenEOS::sdk_version);
    godot::ClassDB::bind_method(godot::D_METHOD("get_diagnostics"), &YugenEOS::get_diagnostics);
    godot::ClassDB::bind_method(godot::D_METHOD("auth_login"), &YugenEOS::auth_login);
    godot::ClassDB::bind_method(godot::D_METHOD("auth_logout"), &YugenEOS::auth_logout);
    godot::ClassDB::bind_method(godot::D_METHOD("auth_verify"), &YugenEOS::auth_verify);
    godot::ClassDB::bind_method(godot::D_METHOD("auth_link_account"), &YugenEOS::auth_link_account);
    godot::ClassDB::bind_method(godot::D_METHOD("connect_login"), &YugenEOS::connect_login);
    godot::ClassDB::bind_method(godot::D_METHOD("connect_logout"), &YugenEOS::connect_logout);
    godot::ClassDB::bind_method(godot::D_METHOD("connect_create_user"), &YugenEOS::connect_create_user);
    godot::ClassDB::bind_method(godot::D_METHOD("friends_query"), &YugenEOS::friends_query);
    godot::ClassDB::bind_method(godot::D_METHOD("friends_get_count"), &YugenEOS::friends_get_count);
    godot::ClassDB::bind_method(godot::D_METHOD("friends_send_invite"), &YugenEOS::friends_send_invite);
    godot::ClassDB::bind_method(godot::D_METHOD("friends_accept_invite"), &YugenEOS::friends_accept_invite);
    godot::ClassDB::bind_method(godot::D_METHOD("friends_reject_invite"), &YugenEOS::friends_reject_invite);
    godot::ClassDB::bind_method(godot::D_METHOD("presence_set"), &YugenEOS::presence_set);
    godot::ClassDB::bind_method(godot::D_METHOD("presence_query"), &YugenEOS::presence_query);
    godot::ClassDB::bind_method(godot::D_METHOD("presence_has"), &YugenEOS::presence_has);
    godot::ClassDB::bind_method(godot::D_METHOD("userinfo_query"), &YugenEOS::userinfo_query);
    godot::ClassDB::bind_method(godot::D_METHOD("userinfo_get"), &YugenEOS::userinfo_get);
    godot::ClassDB::bind_method(godot::D_METHOD("sessions_create"), &YugenEOS::sessions_create);
    godot::ClassDB::bind_method(godot::D_METHOD("sessions_search"), &YugenEOS::sessions_search);
    godot::ClassDB::bind_method(godot::D_METHOD("sessions_get_search_count"), &YugenEOS::sessions_get_search_count);
    godot::ClassDB::bind_method(godot::D_METHOD("sessions_get_search_result"), &YugenEOS::sessions_get_search_result);
    godot::ClassDB::bind_method(godot::D_METHOD("sessions_join"), &YugenEOS::sessions_join);
    godot::ClassDB::bind_method(godot::D_METHOD("sessions_destroy"), &YugenEOS::sessions_destroy);
    godot::ClassDB::bind_method(godot::D_METHOD("sessions_send_invite"), &YugenEOS::sessions_send_invite);
    godot::ClassDB::bind_method(godot::D_METHOD("sessions_query_invites"), &YugenEOS::sessions_query_invites);
    godot::ClassDB::bind_method(godot::D_METHOD("sessions_reject_invite"), &YugenEOS::sessions_reject_invite);
    godot::ClassDB::bind_method(godot::D_METHOD("sessions_set_attributes"), &YugenEOS::sessions_set_attributes);
    godot::ClassDB::bind_method(godot::D_METHOD("sessions_get_attributes"), &YugenEOS::sessions_get_attributes);
    godot::ClassDB::bind_method(godot::D_METHOD("lobbies_create"), &YugenEOS::lobbies_create);
    godot::ClassDB::bind_method(godot::D_METHOD("lobbies_join"), &YugenEOS::lobbies_join);
    godot::ClassDB::bind_method(godot::D_METHOD("lobbies_join_details"), &YugenEOS::lobbies_join_details);
    godot::ClassDB::bind_method(godot::D_METHOD("lobbies_search"), &YugenEOS::lobbies_search);
    godot::ClassDB::bind_method(godot::D_METHOD("lobbies_get_search_count"), &YugenEOS::lobbies_get_search_count);
    godot::ClassDB::bind_method(godot::D_METHOD("lobbies_get_search_result"), &YugenEOS::lobbies_get_search_result);
    godot::ClassDB::bind_method(godot::D_METHOD("lobbies_get_details"), &YugenEOS::lobbies_get_details);
    godot::ClassDB::bind_method(godot::D_METHOD("lobbies_leave"), &YugenEOS::lobbies_leave);
    godot::ClassDB::bind_method(godot::D_METHOD("lobbies_send_invite"), &YugenEOS::lobbies_send_invite);
    godot::ClassDB::bind_method(godot::D_METHOD("p2p_send"), &YugenEOS::p2p_send);
    godot::ClassDB::bind_method(godot::D_METHOD("p2p_receive"), &YugenEOS::p2p_receive);
    godot::ClassDB::bind_method(godot::D_METHOD("p2p_set_relay"), &YugenEOS::p2p_set_relay);
    godot::ClassDB::bind_method(godot::D_METHOD("p2p_query_nat"), &YugenEOS::p2p_query_nat);
    godot::ClassDB::bind_method(godot::D_METHOD("stats_query"), &YugenEOS::stats_query);
    godot::ClassDB::bind_method(godot::D_METHOD("stats_ingest"), &YugenEOS::stats_ingest);
    godot::ClassDB::bind_method(godot::D_METHOD("achievements_unlock"), &YugenEOS::achievements_unlock);
    godot::ClassDB::bind_method(godot::D_METHOD("achievements_query"), &YugenEOS::achievements_query);
    godot::ClassDB::bind_method(godot::D_METHOD("achievements_query_definitions"), &YugenEOS::achievements_query_definitions);
    godot::ClassDB::bind_method(godot::D_METHOD("achievements_get_definition_count"), &YugenEOS::achievements_get_definition_count);
    godot::ClassDB::bind_method(godot::D_METHOD("achievements_get_definition"), &YugenEOS::achievements_get_definition);
    godot::ClassDB::bind_method(godot::D_METHOD("leaderboards_query_definitions"), &YugenEOS::leaderboards_query_definitions);
    godot::ClassDB::bind_method(godot::D_METHOD("leaderboards_query_ranks"), &YugenEOS::leaderboards_query_ranks);
    godot::ClassDB::bind_method(godot::D_METHOD("leaderboards_query_scores"), &YugenEOS::leaderboards_query_scores);
    godot::ClassDB::bind_method(godot::D_METHOD("leaderboards_query"), &YugenEOS::leaderboards_query);
    godot::ClassDB::bind_method(godot::D_METHOD("storage_write"), &YugenEOS::storage_write);
    godot::ClassDB::bind_method(godot::D_METHOD("storage_read"), &YugenEOS::storage_read);
    godot::ClassDB::bind_method(godot::D_METHOD("storage_query_list"), &YugenEOS::storage_query_list);
    godot::ClassDB::bind_method(godot::D_METHOD("titlestorage_read"), &YugenEOS::titlestorage_read);
    godot::ClassDB::bind_method(godot::D_METHOD("ecom_query_offers"), &YugenEOS::ecom_query_offers);
    godot::ClassDB::bind_method(godot::D_METHOD("ecom_query_entitlements"), &YugenEOS::ecom_query_entitlements);
    godot::ClassDB::bind_method(godot::D_METHOD("ecom_checkout"), &YugenEOS::ecom_checkout);
    godot::ClassDB::bind_method(godot::D_METHOD("ui_show_friends"), &YugenEOS::ui_show_friends);
    godot::ClassDB::bind_method(godot::D_METHOD("ui_hide_friends"), &YugenEOS::ui_hide_friends);
    godot::ClassDB::bind_method(godot::D_METHOD("ui_set_visibility"), &YugenEOS::ui_set_visibility);
    godot::ClassDB::bind_method(godot::D_METHOD("ui_pause_overlay"), &YugenEOS::ui_pause_overlay);
    godot::ClassDB::bind_method(godot::D_METHOD("ui_is_overlay_paused"), &YugenEOS::ui_is_overlay_paused);
    godot::ClassDB::bind_method(godot::D_METHOD("ui_get_friends_visible"), &YugenEOS::ui_get_friends_visible);
    godot::ClassDB::bind_method(godot::D_METHOD("ui_set_notification_location"), &YugenEOS::ui_set_notification_location);
    godot::ClassDB::bind_method(godot::D_METHOD("anticheat_start"), &YugenEOS::anticheat_start);
    godot::ClassDB::bind_method(godot::D_METHOD("metrics_send"), &YugenEOS::metrics_send);
    godot::ClassDB::bind_method(godot::D_METHOD("metrics_begin_session"), &YugenEOS::metrics_begin_session);
    godot::ClassDB::bind_method(godot::D_METHOD("metrics_end_session"), &YugenEOS::metrics_end_session);
    godot::ClassDB::bind_method(godot::D_METHOD("reports_send"), &YugenEOS::reports_send);
    godot::ClassDB::bind_method(godot::D_METHOD("sanctions_query"), &YugenEOS::sanctions_query);
    godot::ClassDB::bind_method(godot::D_METHOD("sanctions_appeal"), &YugenEOS::sanctions_appeal);
    godot::ClassDB::bind_method(godot::D_METHOD("rtc_join"), &YugenEOS::rtc_join);
    godot::ClassDB::bind_method(godot::D_METHOD("rtc_leave"), &YugenEOS::rtc_leave);
    godot::ClassDB::bind_method(godot::D_METHOD("rtc_set_sending_volume"), &YugenEOS::rtc_set_sending_volume);
    godot::ClassDB::bind_method(godot::D_METHOD("rtc_set_receiving_volume"), &YugenEOS::rtc_set_receiving_volume);
    godot::ClassDB::bind_method(godot::D_METHOD("mods_enumerate"), &YugenEOS::mods_enumerate);
    godot::ClassDB::bind_method(godot::D_METHOD("mods_get_info"), &YugenEOS::mods_get_info);
    godot::ClassDB::bind_method(godot::D_METHOD("kws_query_age_gate"), &YugenEOS::kws_query_age_gate);
    godot::ClassDB::bind_method(godot::D_METHOD("custom_invites_set"), &YugenEOS::custom_invites_set);
    godot::ClassDB::bind_method(godot::D_METHOD("custom_invites_send"), &YugenEOS::custom_invites_send);
    godot::ClassDB::bind_method(godot::D_METHOD("snapshot_begin"), &YugenEOS::snapshot_begin);
    godot::ClassDB::bind_method(godot::D_METHOD("snapshot_add"), &YugenEOS::snapshot_add);
    godot::ClassDB::bind_method(godot::D_METHOD("snapshot_submit"), &YugenEOS::snapshot_submit);
    godot::ClassDB::bind_method(godot::D_METHOD("snapshot_end"), &YugenEOS::snapshot_end);
    ADD_SIGNAL(godot::MethodInfo("operation_completed", godot::PropertyInfo(godot::Variant::DICTIONARY, "result")));
    ADD_SIGNAL(godot::MethodInfo("initialized", godot::PropertyInfo(godot::Variant::DICTIONARY, "result")));
}

YugenEOS::YugenEOS() {
    singleton = this;
    platform.instantiate();
    auth.instantiate();
    connect.instantiate();
    friends.instantiate();
    presence.instantiate();
    userinfo.instantiate();
    sessions.instantiate();
    lobbies.instantiate();
    p2p.instantiate();
    stats.instantiate();
    achievements.instantiate();
    leaderboards.instantiate();
    storage.instantiate();
    ecom.instantiate();
    ui.instantiate();
    social.instantiate();
    EOSInterfaceBase *ifaces[] = { auth.ptr(), connect.ptr(), friends.ptr(), presence.ptr(), userinfo.ptr(), sessions.ptr(), lobbies.ptr(), p2p.ptr(), stats.ptr(), achievements.ptr(), leaderboards.ptr(), storage.ptr(), ecom.ptr(), ui.ptr(), social.ptr() };
    for (EOSInterfaceBase *iface : ifaces) {
        iface->set_platform(platform.ptr());
    }
}

YugenEOS::~YugenEOS() {
    if (platform.is_valid()) {
        platform->shutdown();
    }
    singleton = nullptr;
}

YugenEOS *YugenEOS::get_singleton() {
    return singleton;
}

godot::Dictionary YugenEOS::initialize(const godot::Dictionary &config) {
    godot::Dictionary result = platform->initialize(config);
    emit_signal("initialized", result);
    return result;
}

void YugenEOS::shutdown() {
    platform->shutdown();
}

void YugenEOS::tick() {
    if (!platform.is_valid()) {
        return;
    }
    godot::Array drained = platform->get_queue()->drain(64);
    for (int i = 0; i < drained.size(); i++) {
        godot::Dictionary entry = drained[i];
        godot::Dictionary payload = entry.get("payload", godot::Dictionary());
        emit_signal("operation_completed", payload);
    }
#if YUGEN_EOS_HAS_SDK
    platform->tick();
#else
    platform->get_queue()->drain(0);
#endif
}

bool YugenEOS::is_initialized() const {
    return platform.is_valid() && platform->is_initialized();
}

godot::String YugenEOS::sdk_version() const {
    if (platform.is_valid()) {
        return platform->sdk_version();
    }
    return "1.19.1";
}

godot::Dictionary YugenEOS::get_diagnostics() const {
    if (platform.is_valid()) {
        return platform->get_diagnostics();
    }
    return godot::Dictionary();
}

godot::Dictionary YugenEOS::auth_login(int64_t login_type, const godot::String &id, const godot::String &token) { return auth->auth_login(login_type, id, token); }
godot::Dictionary YugenEOS::auth_logout(const godot::String &local_user_id) { return auth->auth_logout(local_user_id); }
godot::Dictionary YugenEOS::auth_verify(const godot::String &local_user_id) { return auth->auth_verify(local_user_id); }
godot::Dictionary YugenEOS::connect_login(int64_t credential_type, const godot::String &token, const godot::String &display_name) { return connect->connect_login(credential_type, token, display_name); }
godot::Dictionary YugenEOS::connect_logout(const godot::String &product_user_id) { return connect->connect_logout(product_user_id); }
godot::Dictionary YugenEOS::connect_create_user(int64_t continuance_ref) { return connect->connect_create_user(continuance_ref); }
godot::Dictionary YugenEOS::auth_link_account(int64_t continuance_ref, int64_t link_flags) { return auth->auth_link_account(continuance_ref, link_flags); }
godot::Dictionary YugenEOS::friends_query(const godot::String &local_user_id) { return friends->friends_query(local_user_id); }
godot::Dictionary YugenEOS::friends_get_count(const godot::String &local_user_id) { return friends->friends_get_count(local_user_id); }
godot::Dictionary YugenEOS::friends_send_invite(const godot::String &local_user_id, const godot::String &target_user_id) { return friends->friends_send_invite(local_user_id, target_user_id); }
godot::Dictionary YugenEOS::friends_accept_invite(const godot::String &local_user_id, const godot::String &target_user_id) { return friends->friends_accept_invite(local_user_id, target_user_id); }
godot::Dictionary YugenEOS::friends_reject_invite(const godot::String &local_user_id, const godot::String &target_user_id) { return friends->friends_reject_invite(local_user_id, target_user_id); }
godot::Dictionary YugenEOS::presence_set(const godot::String &local_user_id, int64_t status, const godot::String &details) { return presence->presence_set(local_user_id, status, details); }
godot::Dictionary YugenEOS::presence_query(const godot::String &local_user_id, const godot::String &target_user_id) { return presence->presence_query(local_user_id, target_user_id); }
godot::Dictionary YugenEOS::presence_has(const godot::String &local_user_id, const godot::String &target_user_id) { return presence->presence_has(local_user_id, target_user_id); }
godot::Dictionary YugenEOS::userinfo_query(const godot::String &local_user_id, const godot::String &target_user_id) { return userinfo->userinfo_query(local_user_id, target_user_id); }
godot::Dictionary YugenEOS::userinfo_get(const godot::String &target_user_id) { return userinfo->userinfo_get(target_user_id); }
godot::Dictionary YugenEOS::sessions_create(const godot::String &local_user_id, const godot::String &session_name, int64_t max_players, bool is_public, const godot::Array &attributes) { return sessions->sessions_create(local_user_id, session_name, max_players, is_public, attributes); }
godot::Dictionary YugenEOS::sessions_search(const godot::Dictionary &options) { return sessions->sessions_search(options); }
godot::Dictionary YugenEOS::sessions_get_search_count() { return sessions->sessions_get_search_count(); }
godot::Dictionary YugenEOS::sessions_get_search_result(int64_t index) { return sessions->sessions_get_search_result(index); }
godot::Dictionary YugenEOS::sessions_join(const godot::String &local_user_id, const godot::String &session_handle) { return sessions->sessions_join(local_user_id, session_handle.to_int()); }
godot::Dictionary YugenEOS::sessions_destroy(const godot::String &session_name) { return sessions->sessions_destroy(session_name); }
godot::Dictionary YugenEOS::sessions_send_invite(const godot::String &session_name, const godot::String &local_user_id, const godot::String &target_user_id) { return sessions->sessions_send_invite(session_name, local_user_id, target_user_id); }
godot::Dictionary YugenEOS::sessions_query_invites(const godot::String &local_user_id) { return sessions->sessions_query_invites(local_user_id); }
godot::Dictionary YugenEOS::sessions_reject_invite(const godot::String &local_user_id, const godot::String &invite_id) { return sessions->sessions_reject_invite(local_user_id, invite_id); }
godot::Dictionary YugenEOS::sessions_set_attributes(const godot::String &session_name, const godot::Array &attributes) { return sessions->sessions_set_attributes(session_name, attributes); }
godot::Dictionary YugenEOS::sessions_get_attributes(int64_t session_ref) { return sessions->sessions_get_attributes(session_ref); }
godot::Dictionary YugenEOS::lobbies_create(const godot::String &local_user_id, int64_t max_members, bool is_public) { return lobbies->lobbies_create(local_user_id, max_members, is_public); }
godot::Dictionary YugenEOS::lobbies_join(const godot::String &lobby_id, const godot::String &local_user_id) { return lobbies->lobbies_join(lobby_id, local_user_id); }
godot::Dictionary YugenEOS::lobbies_join_details(int64_t lobby_ref, const godot::String &local_user_id) { return lobbies->lobbies_join_details(lobby_ref, local_user_id); }
godot::Dictionary YugenEOS::lobbies_search(const godot::String &local_user_id, int64_t max_results) { return lobbies->lobbies_search(local_user_id, max_results); }
godot::Dictionary YugenEOS::lobbies_get_search_count() { return lobbies->lobbies_get_search_count(); }
godot::Dictionary YugenEOS::lobbies_get_search_result(int64_t index) { return lobbies->lobbies_get_search_result(index); }
godot::Dictionary YugenEOS::lobbies_get_details(const godot::String &lobby_id, const godot::String &local_user_id) { return lobbies->lobbies_get_details(lobby_id, local_user_id); }
godot::Dictionary YugenEOS::lobbies_leave(const godot::String &lobby_id, const godot::String &local_user_id) { return lobbies->lobbies_leave(lobby_id, local_user_id); }
godot::Dictionary YugenEOS::lobbies_send_invite(const godot::String &lobby_id, const godot::String &local_user_id, const godot::String &target_user_id) { return lobbies->lobbies_send_invite(lobby_id, local_user_id, target_user_id); }
godot::Dictionary YugenEOS::p2p_send(const godot::String &local_user_id, const godot::String &remote_user_id, const godot::String &socket_id, const godot::PackedByteArray &data, bool reliable) { return p2p->p2p_send(local_user_id, remote_user_id, socket_id, data, reliable); }
godot::Dictionary YugenEOS::p2p_receive(const godot::String &local_user_id, const godot::String &socket_id) { return p2p->p2p_receive(local_user_id, socket_id); }
godot::Dictionary YugenEOS::p2p_set_relay(const godot::String &local_user_id, bool allow_relays) { return p2p->p2p_set_relay(local_user_id, allow_relays); }
godot::Dictionary YugenEOS::p2p_query_nat() { return p2p->p2p_query_nat(); }
godot::Dictionary YugenEOS::stats_query(const godot::String &local_user_id, const godot::Array &stat_names) { return stats->stats_query(local_user_id, stat_names); }
godot::Dictionary YugenEOS::stats_ingest(const godot::String &local_user_id, const godot::Array &stats_data) { return stats->stats_ingest(local_user_id, stats_data); }
godot::Dictionary YugenEOS::achievements_unlock(const godot::String &local_user_id, const godot::String &achievement_id) { return achievements->achievements_unlock(local_user_id, achievement_id); }
godot::Dictionary YugenEOS::achievements_query(const godot::String &local_user_id, const godot::Array &achievement_ids) { return achievements->achievements_query(local_user_id, achievement_ids); }
godot::Dictionary YugenEOS::achievements_query_definitions(const godot::String &local_user_id) { return achievements->achievements_query_definitions(local_user_id); }
godot::Dictionary YugenEOS::achievements_get_definition_count() { return achievements->achievements_get_definition_count(); }
godot::Dictionary YugenEOS::achievements_get_definition(int64_t index) { return achievements->achievements_get_definition(index); }
godot::Dictionary YugenEOS::leaderboards_query_definitions() { return leaderboards->leaderboards_query_definitions(); }
godot::Dictionary YugenEOS::leaderboards_query_ranks(const godot::String &leaderboard_id, const godot::String &local_user_id) { return leaderboards->leaderboards_query_ranks(leaderboard_id, local_user_id); }
godot::Dictionary YugenEOS::leaderboards_query_scores(const godot::String &local_user_id, const godot::String &stat_name, const godot::Array &target_user_ids) { return leaderboards->leaderboards_query_scores(local_user_id, stat_name, target_user_ids); }
godot::Dictionary YugenEOS::leaderboards_query(const godot::String &leaderboard_id, const godot::String &local_user_id) { return leaderboards->leaderboards_query_ranks(leaderboard_id, local_user_id); }
godot::Dictionary YugenEOS::storage_write(const godot::String &local_user_id, const godot::String &filename, const godot::PackedByteArray &data) { return storage->storage_write(local_user_id, filename, data); }
godot::Dictionary YugenEOS::storage_read(const godot::String &local_user_id, const godot::String &filename) { return storage->storage_read(local_user_id, filename); }
godot::Dictionary YugenEOS::storage_query_list(const godot::String &local_user_id) { return storage->storage_query_list(local_user_id); }
godot::Dictionary YugenEOS::titlestorage_read(const godot::String &filename) { return storage->titlestorage_read(filename); }
godot::Dictionary YugenEOS::ecom_query_offers(const godot::String &local_user_id) { return ecom->ecom_query_offers(local_user_id); }
godot::Dictionary YugenEOS::ecom_query_entitlements(const godot::String &local_user_id) { return ecom->ecom_query_entitlements(local_user_id); }
godot::Dictionary YugenEOS::ecom_checkout(const godot::String &local_user_id, const godot::String &offer_id) { return ecom->ecom_checkout(local_user_id, offer_id); }
godot::Dictionary YugenEOS::ui_show_friends(const godot::String &local_user_id) { return ui->ui_show_friends(local_user_id); }
godot::Dictionary YugenEOS::ui_hide_friends() { return ui->ui_hide_friends(); }
godot::Dictionary YugenEOS::ui_set_visibility(bool visible) { return ui->ui_set_visibility(visible); }
godot::Dictionary YugenEOS::ui_pause_overlay(bool paused) { return ui->ui_pause_overlay(paused); }
godot::Dictionary YugenEOS::ui_is_overlay_paused() { return ui->ui_is_overlay_paused(); }
godot::Dictionary YugenEOS::ui_get_friends_visible(const godot::String &local_user_id) { return ui->ui_get_friends_visible(local_user_id); }
godot::Dictionary YugenEOS::ui_set_notification_location(int64_t location) { return ui->ui_set_notification_location(location); }
godot::Dictionary YugenEOS::anticheat_start(const godot::String &local_user_id) { return social->anticheat_start(local_user_id); }
godot::Dictionary YugenEOS::metrics_send(const godot::String &event_name, const godot::Dictionary &attributes) { return social->metrics_send(event_name, attributes); }
godot::Dictionary YugenEOS::metrics_begin_session(const godot::String &account_id) { return social->metrics_begin_session(account_id); }
godot::Dictionary YugenEOS::metrics_end_session(const godot::String &account_id) { return social->metrics_end_session(account_id); }
godot::Dictionary YugenEOS::reports_send(const godot::String &reporter_id, const godot::String &reported_id, int64_t category, const godot::String &message) { return social->reports_send(reporter_id, reported_id, category, message); }
godot::Dictionary YugenEOS::sanctions_query(const godot::String &player_id) { return social->sanctions_query(player_id); }
godot::Dictionary YugenEOS::sanctions_appeal(const godot::String &local_user_id, int64_t reason, const godot::String &reference_id) { return social->sanctions_appeal(local_user_id, reason, reference_id); }
godot::Dictionary YugenEOS::rtc_join(const godot::String &local_user_id, const godot::String &room) { return social->rtc_join(local_user_id, room); }
godot::Dictionary YugenEOS::rtc_leave(const godot::String &local_user_id, const godot::String &room) { return social->rtc_leave(local_user_id, room); }
godot::Dictionary YugenEOS::rtc_set_sending_volume(const godot::String &local_user_id, const godot::String &room, double volume) { return social->rtc_set_sending_volume(local_user_id, room, volume); }
godot::Dictionary YugenEOS::rtc_set_receiving_volume(const godot::String &local_user_id, const godot::String &room, double volume) { return social->rtc_set_receiving_volume(local_user_id, room, volume); }
godot::Dictionary YugenEOS::mods_enumerate(const godot::String &local_user_id, int64_t mod_type) { return social->mods_enumerate(local_user_id, mod_type); }
godot::Dictionary YugenEOS::mods_get_info(const godot::String &local_user_id, int64_t mod_type) { return social->mods_get_info(local_user_id, mod_type); }
godot::Dictionary YugenEOS::kws_query_age_gate() { return social->kws_query_age_gate(); }
godot::Dictionary YugenEOS::custom_invites_set(const godot::String &local_user_id, const godot::String &payload) { return social->custom_invites_set(local_user_id, payload); }
godot::Dictionary YugenEOS::custom_invites_send(const godot::String &local_user_id, const godot::Array &target_user_ids) { return social->custom_invites_send(local_user_id, target_user_ids); }
godot::Dictionary YugenEOS::snapshot_begin(const godot::String &local_user_id) { return social->snapshot_begin(local_user_id); }
godot::Dictionary YugenEOS::snapshot_add(int64_t snapshot_id, const godot::String &key, const godot::String &value) { return social->snapshot_add(snapshot_id, key, value); }
godot::Dictionary YugenEOS::snapshot_submit(int64_t snapshot_id) { return social->snapshot_submit(snapshot_id); }
godot::Dictionary YugenEOS::snapshot_end(int64_t snapshot_id) { return social->snapshot_end(snapshot_id); }

}
