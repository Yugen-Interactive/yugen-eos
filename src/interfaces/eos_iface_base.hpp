#pragma once

#include "core/eos_bindings.gen.hpp"
#include "core/eos_callback_queue.hpp"
#include "core/eos_platform_manager.hpp"
#include "core/eos_result.hpp"

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/string.hpp>

namespace yugen {

struct RequestContext {
    EOSCallbackQueue *queue = nullptr;
    EOSApiTable *table = nullptr;
    EOSPlatformManager *platform = nullptr;
    void *handle = nullptr;
    godot::String operation;
    godot::Dictionary context;
};

class EOSInterfaceBase : public godot::RefCounted {
    GDCLASS(EOSInterfaceBase, godot::RefCounted);

protected:
    EOSPlatformManager *platform = nullptr;
    static void _bind_methods();
    godot::Dictionary require_ready(const godot::String &operation) const;
    godot::Dictionary not_implemented(const godot::String &operation, const godot::String &reason) const;
    static RequestContext *make_context(EOSCallbackQueue *queue, const godot::String &operation, const godot::Dictionary &context);
    RequestContext *make_request(const godot::String &operation, const godot::Dictionary &context);
    EOSApiTable *bindings() const;

public:
    void set_platform(EOSPlatformManager *p_platform);
    bool is_available() const;
};

}
