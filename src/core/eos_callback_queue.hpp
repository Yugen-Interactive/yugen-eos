#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/templates/vector.hpp>
#include <godot_cpp/variant/callable.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/string.hpp>

#include <mutex>
#include <queue>

namespace yugen {

struct PendingCallback {
    uint64_t id = 0;
    godot::String operation;
    godot::Dictionary payload;
    godot::Callable target;
};

class EOSCallbackQueue : public godot::RefCounted {
    GDCLASS(EOSCallbackQueue, godot::RefCounted);

    std::mutex mutex;
    std::queue<PendingCallback> pending;
    uint64_t next_id = 1;

protected:
    static void _bind_methods();

public:
    uint64_t enqueue(const godot::String &operation, const godot::Dictionary &payload);
    uint64_t enqueue_callable(const godot::String &operation, const godot::Dictionary &payload, const godot::Callable &target);
    godot::Array drain(int max_items);
    int pending_count();
    void clear();
};

}
