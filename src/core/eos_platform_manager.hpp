#pragma once

#include "core/eos_bindings.gen.hpp"
#include "core/eos_callback_queue.hpp"
#include "core/eos_loader.hpp"
#include "core/eos_logger.hpp"

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/dictionary.hpp>

namespace yugen {

class EOSPlatformManager : public godot::RefCounted {
    GDCLASS(EOSPlatformManager, godot::RefCounted);

    bool sdk_initialized = false;
    bool platform_created = false;
    void *platform_handle = nullptr;
    EOSApiTable api;
    godot::Ref<EOSLoader> loader;
    godot::Ref<EOSLogger> logger;
    godot::Ref<EOSCallbackQueue> queue;
    godot::Dictionary last_config;
    godot::Array recent_errors;

protected:
    static void _bind_methods();

public:
    EOSPlatformManager();
    godot::Dictionary initialize(const godot::Dictionary &config);
    void shutdown();
    void tick();
    bool is_initialized() const;
    bool has_library() const;
    bool has_platform() const;
    godot::String sdk_version() const;
    godot::String platform_name() const;
    godot::Dictionary get_diagnostics() const;
    godot::Ref<EOSLogger> get_logger() const;
    godot::Ref<EOSCallbackQueue> get_queue() const;
    void *get_platform_handle() const;
    EOSApiTable *api_table();
    void record_error(const godot::Dictionary &error);
    godot::Array get_recent_errors() const;
};

}
