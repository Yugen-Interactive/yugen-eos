#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/string.hpp>

namespace yugen {

class EOSLoader : public godot::RefCounted {
    GDCLASS(EOSLoader, godot::RefCounted);

    void *library = nullptr;
    godot::String loaded_path;
    godot::String last_error;

protected:
    static void _bind_methods();

public:
    ~EOSLoader();
    bool load();
    void unload();
    bool is_loaded() const;
    godot::String get_loaded_path() const;
    godot::String get_last_error() const;
    void *find_symbol(const char *name);
    static godot::String default_library_name();
};

}
