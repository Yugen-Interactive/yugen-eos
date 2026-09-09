#include "core/eos_platform_manager.hpp"
#include "core/eos_result.hpp"
#include "eos_version.hpp"

#include <cstring>

#if YUGEN_EOS_HAS_SDK
#include "eos_common.h"
#include "eos_sdk.h"
#include "eos_version.h"
#endif

namespace yugen {

void EOSPlatformManager::_bind_methods() {
    godot::ClassDB::bind_method(godot::D_METHOD("initialize"), &EOSPlatformManager::initialize);
    godot::ClassDB::bind_method(godot::D_METHOD("shutdown"), &EOSPlatformManager::shutdown);
    godot::ClassDB::bind_method(godot::D_METHOD("tick"), &EOSPlatformManager::tick);
    godot::ClassDB::bind_method(godot::D_METHOD("is_initialized"), &EOSPlatformManager::is_initialized);
    godot::ClassDB::bind_method(godot::D_METHOD("has_library"), &EOSPlatformManager::has_library);
    godot::ClassDB::bind_method(godot::D_METHOD("sdk_version"), &EOSPlatformManager::sdk_version);
    godot::ClassDB::bind_method(godot::D_METHOD("get_diagnostics"), &EOSPlatformManager::get_diagnostics);
    godot::ClassDB::bind_method(godot::D_METHOD("get_recent_errors"), &EOSPlatformManager::get_recent_errors);
    ADD_SIGNAL(godot::MethodInfo("platform_ticked"));
}

EOSPlatformManager::EOSPlatformManager() {
    loader.instantiate();
    logger.instantiate();
    queue.instantiate();
    logger->set_level(1);
}

godot::Dictionary EOSPlatformManager::initialize(const godot::Dictionary &config) {
    godot::Dictionary context;
    context["platform"] = platform_name();
    if (sdk_initialized) {
        return EOSResult::make_result(15, "platform.initialize", context);
    }
    godot::String product_name = config.get("product_name", "Yugen Game");
    godot::String product_version = config.get("product_version", "1.0.0");
    if (product_name.is_empty() || product_version.is_empty()) {
        context["reason"] = "product_name and product_version are required";
        return EOSResult::make_result(10, "platform.initialize", context);
    }
    last_config = config;
    int log_level = (int)config.get("log_level", 1);
    logger->set_level(log_level);
#if YUGEN_EOS_HAS_SDK
    bool library_loaded = loader->load() && api.load(loader.ptr());
    if (!library_loaded) {
        context["reason"] = "EOS native library is not available. Place it beside the game binary and retry.";
        context["loader_error"] = loader->get_last_error();
        context["missing_symbols"] = (int64_t)api.missing_count;
        godot::Dictionary out = EOSResult::make_result(14, "platform.initialize", context);
        record_error(out);
        return out;
    }
    EOS_InitializeOptions init_options = {};
    init_options.ApiVersion = EOS_INITIALIZE_API_LATEST;
    char pname[128] = {};
    char pver[128] = {};
    strncpy(pname, product_name.utf8().get_data(), sizeof(pname) - 1);
    strncpy(pver, product_version.utf8().get_data(), sizeof(pver) - 1);
    init_options.ProductName = pname;
    init_options.ProductVersion = pver;
    EOS_EResult init_result = api.EOS_Initialize(&init_options);
    if (init_result != EOS_EResult::EOS_Success) {
        context["native_code"] = (int64_t)init_result;
        godot::Dictionary out = EOSResult::make_result((int64_t)init_result, "platform.initialize", context);
        record_error(out);
        return out;
    }
    sdk_initialized = true;
    logger->log_message(3, "platform", "EOS_Initialize succeeded");
    return EOSResult::make_result(0, "platform.initialize", context);
#else
    bool loaded = loader->load();
    if (loaded) {
        sdk_initialized = true;
        logger->log_message(3, "platform", "EOS library loaded dynamically");
        return EOSResult::make_result(0, "platform.initialize", context);
    }
    context["reason"] = "EOS SDK not linked. Provide SDK binaries and rebuild with EOS_SDK_DIR, or place native library beside the game.";
    context["loader_error"] = loader->get_last_error();
    godot::Dictionary out = EOSResult::make_result(14, "platform.initialize", context);
    record_error(out);
    return out;
#endif
}

void EOSPlatformManager::shutdown() {
    queue->clear();
#if YUGEN_EOS_HAS_SDK
    if (sdk_initialized && api.EOS_Shutdown != nullptr) {
        api.EOS_Shutdown();
    }
    api = EOSApiTable();
#endif
    loader->unload();
    sdk_initialized = false;
    platform_created = false;
    platform_handle = nullptr;
}

void EOSPlatformManager::tick() {
#if YUGEN_EOS_HAS_SDK
    if (sdk_initialized && platform_handle != nullptr && api.EOS_Platform_Tick != nullptr) {
        api.EOS_Platform_Tick((EOS_HPlatform)platform_handle);
    }
#endif
    queue->drain(64);
    emit_signal("platform_ticked");
}

bool EOSPlatformManager::is_initialized() const { return sdk_initialized; }
bool EOSPlatformManager::has_library() const { return loader->is_loaded(); }
bool EOSPlatformManager::has_platform() const { return platform_created && platform_handle != nullptr; }

godot::String EOSPlatformManager::sdk_version() const {
#if YUGEN_EOS_HAS_SDK
    if (api.EOS_GetVersion != nullptr) {
        const char *v = api.EOS_GetVersion();
        if (v) {
            return godot::String(v);
        }
    }
#endif
    return godot::String("1.19.1");
}

godot::String EOSPlatformManager::platform_name() const {
#ifdef _WIN32
    return "windows";
#elif __ANDROID__
    return "android";
#elif __APPLE__
#include <TargetConditionals.h>
#if TARGET_OS_IPHONE
    return "ios";
#else
    return "macos";
#endif
#elif __linux__
    return "linux";
#else
    return "unknown";
#endif
}

godot::Dictionary EOSPlatformManager::get_diagnostics() const {
    godot::Dictionary out;
    out["initialized"] = sdk_initialized;
    out["platform_created"] = platform_created;
    out["native_loaded"] = loader->is_loaded();
    out["sdk_linked"] = (int)YUGEN_EOS_HAS_SDK;
    out["sdk_version"] = sdk_version();
    out["platform"] = platform_name();
    out["pending_callbacks"] = queue->pending_count();
    out["recent_errors"] = recent_errors;
    return out;
}

godot::Ref<EOSLogger> EOSPlatformManager::get_logger() const { return logger; }
godot::Ref<EOSCallbackQueue> EOSPlatformManager::get_queue() const { return queue; }
void *EOSPlatformManager::get_platform_handle() const { return platform_handle; }

EOSApiTable *EOSPlatformManager::api_table() {
#if YUGEN_EOS_HAS_SDK
    return &api;
#else
    return nullptr;
#endif
}

void EOSPlatformManager::record_error(const godot::Dictionary &error) {
    recent_errors.push_back(error);
    while (recent_errors.size() > 50) {
        recent_errors.remove_at(0);
    }
}

godot::Array EOSPlatformManager::get_recent_errors() const { return recent_errors; }

}
