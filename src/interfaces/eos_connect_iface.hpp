#pragma once

#include "interfaces/eos_iface_base.hpp"

namespace yugen {

class EOSConnectInterface : public EOSInterfaceBase {
    GDCLASS(EOSConnectInterface, EOSInterfaceBase);

protected:
    static void _bind_methods();

public:
    godot::Dictionary connect_login(int64_t credential_type, const godot::String &token, const godot::String &display_name);
    godot::Dictionary connect_logout(const godot::String &product_user_id);
    godot::Dictionary connect_create_user(const godot::String &continuance_token);
    int64_t get_logged_in_count();
};

}
