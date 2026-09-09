#include "core/eos_loader.hpp"

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace yugen {

void EOSLoader::_bind_methods() {
    godot::ClassDB::bind_method(godot::D_METHOD("load"), &EOSLoader::load);
    godot::ClassDB::bind_method(godot::D_METHOD("unload"), &EOSLoader::unload);
    godot::ClassDB::bind_method(godot::D_METHOD("is_loaded"), &EOSLoader::is_loaded);
    godot::ClassDB::bind_method(godot::D_METHOD("get_loaded_path"), &EOSLoader::get_loaded_path);
    godot::ClassDB::bind_method(godot::D_METHOD("get_last_error"), &EOSLoader::get_last_error);
}

EOSLoader::~EOSLoader() {
    unload();
}

godot::String EOSLoader::default_library_name() {
#ifdef _WIN32
    return "EOSSDK-Win64-Shipping.dll";
#elif __APPLE__
#if TARGET_OS_IPHONE
    return "EOSSDK.framework/EOSSDK";
#else
    return "libEOSSDK-Mac-Shipping.dylib";
#endif
#elif __ANDROID__
    return "libEOSSDK.so";
#else
    return "libEOSSDK-Linux-Shipping.so";
#endif
}

bool EOSLoader::load() {
    if (library != nullptr) {
        return true;
    }
    godot::String name = default_library_name();
#ifdef _WIN32
    HMODULE handle = LoadLibraryW((LPCWSTR)name.wide_string().get_data());
    if (!handle) {
        last_error = "LoadLibrary failed for " + name;
        return false;
    }
    library = (void *)handle;
#else
    void *handle = dlopen(name.utf8().get_data(), RTLD_NOW | RTLD_LOCAL);
    if (!handle) {
        const char *err = dlerror();
        last_error = godot::String("dlopen failed for {0}: {1}").format(godot::Array::make(name, err ? err : "unknown"));
        return false;
    }
    library = handle;
#endif
    loaded_path = name;
    last_error = "";
    return true;
}

void EOSLoader::unload() {
    if (!library) {
        return;
    }
#ifdef _WIN32
    FreeLibrary((HMODULE)library);
#else
    dlclose(library);
#endif
    library = nullptr;
    loaded_path = "";
}

bool EOSLoader::is_loaded() const { return library != nullptr; }
godot::String EOSLoader::get_loaded_path() const { return loaded_path; }
godot::String EOSLoader::get_last_error() const { return last_error; }

void *EOSLoader::find_symbol(const char *name) {
    if (!library) {
        return nullptr;
    }
#ifdef _WIN32
    return (void *)GetProcAddress((HMODULE)library, name);
#else
    return dlsym(library, name);
#endif
}

}
