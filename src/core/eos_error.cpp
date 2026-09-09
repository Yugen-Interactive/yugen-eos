#include "core/eos_error.hpp"
#include "core/eos_result.hpp"

namespace yugen {

void EOSError::_bind_methods() {
    godot::ClassDB::bind_method(godot::D_METHOD("get_code"), &EOSError::get_code);
    godot::ClassDB::bind_method(godot::D_METHOD("get_name"), &EOSError::get_name);
    godot::ClassDB::bind_method(godot::D_METHOD("get_message"), &EOSError::get_message);
    godot::ClassDB::bind_method(godot::D_METHOD("get_operation"), &EOSError::get_operation);
    godot::ClassDB::bind_method(godot::D_METHOD("get_context"), &EOSError::get_context);
    godot::ClassDB::bind_method(godot::D_METHOD("to_dict"), &EOSError::to_dict);
}

void EOSError::setup(int64_t p_code, const godot::String &p_operation, const godot::Dictionary &p_context) {
    code = p_code;
    name = EOSResult::code_to_name(p_code);
    message = EOSResult::code_to_message(p_code);
    operation = p_operation;
    context = p_context;
}

int64_t EOSError::get_code() const { return code; }
godot::String EOSError::get_name() const { return name; }
godot::String EOSError::get_message() const { return message; }
godot::String EOSError::get_operation() const { return operation; }
godot::Dictionary EOSError::get_context() const { return context; }

godot::Dictionary EOSError::to_dict() const {
    return EOSResult::make_result(code, operation, context);
}

godot::String EOSError::to_string() const {
    return name + godot::String(" (") + godot::String::num_int64(code) + godot::String(") in ") + operation + godot::String(": ") + message;
}

}
