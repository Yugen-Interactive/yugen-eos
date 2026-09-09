#pragma once

#include "interfaces/eos_iface_base.hpp"

#include <map>

namespace yugen {

class EOSSessionsInterface : public EOSInterfaceBase {
    GDCLASS(EOSSessionsInterface, EOSInterfaceBase);

    void *active_search = nullptr;
    std::map<int64_t, void *> session_registry;
    int64_t next_registry_id = 1;

protected:
    static void _bind_methods();

public:
    ~EOSSessionsInterface();
    godot::Dictionary sessions_create(const godot::String &local_user_id, const godot::String &session_name, int64_t max_players, bool is_public);
    godot::Dictionary sessions_search(const godot::Dictionary &options);
    godot::Dictionary sessions_get_search_count();
    godot::Dictionary sessions_get_search_result(int64_t index);
    godot::Dictionary sessions_join(const godot::String &local_user_id, int64_t session_ref);
    godot::Dictionary sessions_destroy(const godot::String &session_name);
};

class EOSLobbiesInterface : public EOSInterfaceBase {
    GDCLASS(EOSLobbiesInterface, EOSInterfaceBase);

    void *active_search = nullptr;

protected:
    static void _bind_methods();

public:
    ~EOSLobbiesInterface();
    godot::Dictionary lobbies_create(const godot::String &local_user_id, int64_t max_members, bool is_public);
    godot::Dictionary lobbies_join(const godot::String &lobby_id, const godot::String &local_user_id);
    godot::Dictionary lobbies_leave(const godot::String &lobby_id, const godot::String &local_user_id);
    godot::Dictionary lobbies_send_invite(const godot::String &lobby_id, const godot::String &local_user_id, const godot::String &target_user_id);
};

class EOSP2PInterface : public EOSInterfaceBase {
    GDCLASS(EOSP2PInterface, EOSInterfaceBase);

protected:
    static void _bind_methods();

public:
    godot::Dictionary p2p_send(const godot::String &local_user_id, const godot::String &remote_user_id, const godot::String &socket_id, const godot::PackedByteArray &data, bool reliable);
    godot::Dictionary p2p_receive(const godot::String &local_user_id, const godot::String &socket_id);
    godot::Dictionary p2p_set_relay(const godot::String &local_user_id, bool allow_relays);
    godot::Dictionary p2p_query_nat();
};

}
