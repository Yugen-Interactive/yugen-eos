#include "yugen_eos.hpp"

#include "core/eos_error.hpp"
#include "core/eos_logger.hpp"
#include "core/eos_callback_queue.hpp"
#include "core/eos_loader.hpp"
#include "core/eos_platform_manager.hpp"
#include "core/eos_result.hpp"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/godot.hpp>

using namespace godot;

static yugen::YugenEOS *yugen_eos_singleton = nullptr;

extern "C" GDExtensionBool GDE_EXPORT yugen_eos_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
    GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);
    init_obj.register_initializer([](ModuleInitializationLevel p_level) {
        if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
            return;
        }
        ClassDB::register_class<yugen::EOSResult>();
        ClassDB::register_class<yugen::EOSError>();
        ClassDB::register_class<yugen::EOSLogger>();
        ClassDB::register_class<yugen::EOSCallbackQueue>();
        ClassDB::register_class<yugen::EOSLoader>();
        ClassDB::register_class<yugen::EOSPlatformManager>();
        ClassDB::register_class<yugen::EOSInterfaceBase>();
        ClassDB::register_class<yugen::EOSAuthInterface>();
        ClassDB::register_class<yugen::EOSConnectInterface>();
        ClassDB::register_class<yugen::EOSFriendsInterface>();
        ClassDB::register_class<yugen::EOSPresenceInterface>();
        ClassDB::register_class<yugen::EOSUserInfoInterface>();
        ClassDB::register_class<yugen::EOSSessionsInterface>();
        ClassDB::register_class<yugen::EOSLobbiesInterface>();
        ClassDB::register_class<yugen::EOSP2PInterface>();
        ClassDB::register_class<yugen::EOSStatsInterface>();
        ClassDB::register_class<yugen::EOSAchievementsInterface>();
        ClassDB::register_class<yugen::EOSLeaderboardsInterface>();
        ClassDB::register_class<yugen::EOSStorageInterface>();
        ClassDB::register_class<yugen::EOSEcomInterface>();
        ClassDB::register_class<yugen::EOSUIInterface>();
        ClassDB::register_class<yugen::EOSSocialInterface>();
        ClassDB::register_class<yugen::YugenEOS>();
        yugen_eos_singleton = memnew(yugen::YugenEOS);
        Engine::get_singleton()->register_singleton("YugenEOS", yugen::YugenEOS::get_singleton());
    });
    init_obj.register_terminator([](ModuleInitializationLevel p_level) {
        if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
            return;
        }
        Engine::get_singleton()->unregister_singleton("YugenEOS");
        memdelete(yugen_eos_singleton);
        yugen_eos_singleton = nullptr;
    });
    init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);
    return init_obj.init();
}
