#pragma once

#include "interfaces/eos_iface_base.hpp"

namespace yugen {

class EOSStorageInterface : public EOSInterfaceBase {
    GDCLASS(EOSStorageInterface, EOSInterfaceBase);

protected:
    static void _bind_methods();

public:
    godot::Dictionary storage_write(const godot::String &local_user_id, const godot::String &filename, const godot::PackedByteArray &data);
    godot::Dictionary storage_read(const godot::String &local_user_id, const godot::String &filename);
    godot::Dictionary titlestorage_read(const godot::String &filename);
    godot::Dictionary storage_query_list(const godot::String &local_user_id);
};

class EOSEcomInterface : public EOSInterfaceBase {
    GDCLASS(EOSEcomInterface, EOSInterfaceBase);

protected:
    static void _bind_methods();

public:
    godot::Dictionary ecom_query_offers(const godot::String &local_user_id);
    godot::Dictionary ecom_query_entitlements(const godot::String &local_user_id);
    godot::Dictionary ecom_checkout(const godot::String &local_user_id, const godot::String &offer_id);
};

class EOSUIInterface : public EOSInterfaceBase {
    GDCLASS(EOSUIInterface, EOSInterfaceBase);

protected:
    static void _bind_methods();

public:
    godot::Dictionary ui_show_friends(const godot::String &local_user_id);
    godot::Dictionary ui_hide_friends();
    godot::Dictionary ui_set_visibility(bool visible);
    godot::Dictionary ui_pause_overlay(bool paused);
    godot::Dictionary ui_is_overlay_paused();
    godot::Dictionary ui_get_friends_visible(const godot::String &local_user_id);
    godot::Dictionary ui_set_notification_location(int64_t location);
};

class EOSSocialInterface : public EOSInterfaceBase {
    GDCLASS(EOSSocialInterface, EOSInterfaceBase);

protected:
    static void _bind_methods();

public:
    godot::Dictionary reports_send(const godot::String &reporter_id, const godot::String &reported_id, int64_t category, const godot::String &message);
    godot::Dictionary sanctions_query(const godot::String &player_id);
    godot::Dictionary sanctions_appeal(const godot::String &local_user_id, int64_t reason, const godot::String &reference_id);
    godot::Dictionary rtc_join(const godot::String &local_user_id, const godot::String &room);
    godot::Dictionary rtc_leave(const godot::String &local_user_id, const godot::String &room);
    godot::Dictionary rtc_set_sending_volume(const godot::String &local_user_id, const godot::String &room, double volume);
    godot::Dictionary rtc_set_receiving_volume(const godot::String &local_user_id, const godot::String &room, double volume);
    godot::Dictionary anticheat_start(const godot::String &local_user_id);
    godot::Dictionary metrics_send(const godot::String &event_name, const godot::Dictionary &attributes);
    godot::Dictionary metrics_begin_session(const godot::String &account_id);
    godot::Dictionary metrics_end_session(const godot::String &account_id);
    godot::Dictionary application_set_status(int64_t status);
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
