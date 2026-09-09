#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/string.hpp>

namespace yugen {

class EOSLogger : public godot::RefCounted {
    GDCLASS(EOSLogger, godot::RefCounted);

public:
    enum Level {
        Off = 0,
        Error = 1,
        Warning = 2,
        Info = 3,
        Verbose = 4,
    };

private:
    int level = 1;
    godot::Array recent;
    static const int MAX_RECENT = 200;

protected:
    static void _bind_methods();

public:
    void set_level(int p_level);
    int get_level() const;
    void log_message(int p_level, const godot::String &category, const godot::String &message);
    godot::Array get_recent() const;
    void clear_recent();
    static godot::String sanitize(const godot::String &message);
};

}

VARIANT_ENUM_CAST(yugen::EOSLogger::Level);
