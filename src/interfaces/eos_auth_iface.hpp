#pragma once

#include "interfaces/eos_iface_base.hpp"

namespace yugen {

class EOSAuthInterface : public EOSInterfaceBase {
    GDCLASS(EOSAuthInterface, EOSInterfaceBase);

protected:
    static void _bind_methods();

public:
    godot::Dictionary auth_login(int64_t login_type, const godot::String &id, const godot::String &token);
    godot::Dictionary auth_logout(const godot::String &local_user_id);
    godot::Dictionary auth_verify(const godot::String &local_user_id);
    int64_t get_logged_in_count();
};

}
