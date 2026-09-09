#pragma once

#include "interfaces/eos_iface_base.hpp"

namespace yugen {

class EOSFriendsInterface : public EOSInterfaceBase {
    GDCLASS(EOSFriendsInterface, EOSInterfaceBase);

protected:
    static void _bind_methods();

public:
    godot::Dictionary friends_query(const godot::String &local_user_id);
    godot::Dictionary friends_get_count(const godot::String &local_user_id);
    godot::Dictionary friends_send_invite(const godot::String &local_user_id, const godot::String &target_user_id);
    godot::Dictionary friends_accept_invite(const godot::String &local_user_id, const godot::String &target_user_id);
    godot::Dictionary friends_reject_invite(const godot::String &local_user_id, const godot::String &target_user_id);
};

class EOSPresenceInterface : public EOSInterfaceBase {
    GDCLASS(EOSPresenceInterface, EOSInterfaceBase);

protected:
    static void _bind_methods();

public:
    godot::Dictionary presence_set(const godot::String &local_user_id, int64_t status, const godot::String &details);
    godot::Dictionary presence_query(const godot::String &local_user_id, const godot::String &target_user_id);
    godot::Dictionary presence_has(const godot::String &local_user_id, const godot::String &target_user_id);
};

class EOSUserInfoInterface : public EOSInterfaceBase {
    GDCLASS(EOSUserInfoInterface, EOSInterfaceBase);

protected:
    static void _bind_methods();

public:
    godot::Dictionary userinfo_query(const godot::String &local_user_id, const godot::String &target_user_id);
    godot::Dictionary userinfo_get(const godot::String &target_user_id);
};

}
