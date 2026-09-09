#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/string.hpp>

namespace yugen {

class EOSError : public godot::RefCounted {
    GDCLASS(EOSError, godot::RefCounted);

    int64_t code = 16;
    godot::String name = "EOS_NotImplemented";
    godot::String message;
    godot::String operation;
    godot::Dictionary context;

protected:
    static void _bind_methods();

public:
    void setup(int64_t p_code, const godot::String &p_operation, const godot::Dictionary &p_context);
    int64_t get_code() const;
    godot::String get_name() const;
    godot::String get_message() const;
    godot::String get_operation() const;
    godot::Dictionary get_context() const;
    godot::Dictionary to_dict() const;
    godot::String to_string() const;
};

}
