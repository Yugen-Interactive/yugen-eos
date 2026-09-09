#include "core/eos_logger.hpp"

#include <godot_cpp/variant/utility_functions.hpp>

namespace yugen {

void EOSLogger::_bind_methods() {
    godot::ClassDB::bind_method(godot::D_METHOD("set_level"), &EOSLogger::set_level);
    godot::ClassDB::bind_method(godot::D_METHOD("get_level"), &EOSLogger::get_level);
    godot::ClassDB::bind_method(godot::D_METHOD("log_message"), &EOSLogger::log_message);
    godot::ClassDB::bind_method(godot::D_METHOD("get_recent"), &EOSLogger::get_recent);
    godot::ClassDB::bind_method(godot::D_METHOD("clear_recent"), &EOSLogger::clear_recent);
    BIND_ENUM_CONSTANT(Off);
    BIND_ENUM_CONSTANT(Error);
    BIND_ENUM_CONSTANT(Warning);
    BIND_ENUM_CONSTANT(Info);
    BIND_ENUM_CONSTANT(Verbose);
}

void EOSLogger::set_level(int p_level) {
    level = p_level < 0 ? 0 : (p_level > 4 ? 4 : p_level);
}

int EOSLogger::get_level() const { return level; }

void EOSLogger::log_message(int p_level, const godot::String &category, const godot::String &message) {
    if (p_level > level || p_level <= 0) {
        return;
    }
    godot::String clean = sanitize(message);
    godot::String line = "[YugenEOS][" + category + "] " + clean;
    if (p_level == 1) {
        godot::UtilityFunctions::push_error(line);
    } else if (p_level == 2) {
        godot::UtilityFunctions::push_warning(line);
    } else {
        godot::UtilityFunctions::print(line);
    }
    godot::Dictionary entry;
    entry["level"] = p_level;
    entry["category"] = category;
    entry["message"] = clean;
    recent.push_back(entry);
    while (recent.size() > MAX_RECENT) {
        recent.remove_at(0);
    }
}

godot::Array EOSLogger::get_recent() const { return recent; }

void EOSLogger::clear_recent() { recent.clear(); }

godot::String EOSLogger::sanitize(const godot::String &message) {
    godot::String out = message;
    const char *keys[] = { "secret", "token", "password", "encryption", "key", "credential", "auth" };
    godot::String lower = out.to_lower();
    for (const char *k : keys) {
        godot::String needle = godot::String(k) + "=";
        int pos = lower.find(needle);
        while (pos >= 0) {
            int end = out.find(" ", pos);
            if (end < 0) {
                end = out.length();
            }
            out = out.substr(0, pos + needle.length()) + "***" + out.substr(end);
            lower = out.to_lower();
            pos = lower.find(needle);
        }
    }
    return out;
}

}
