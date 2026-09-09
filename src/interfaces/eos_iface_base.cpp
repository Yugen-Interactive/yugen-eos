#include "interfaces/eos_iface_base.hpp"

namespace yugen {

void EOSInterfaceBase::_bind_methods() {
    godot::ClassDB::bind_method(godot::D_METHOD("is_available"), &EOSInterfaceBase::is_available);
}

void EOSInterfaceBase::set_platform(EOSPlatformManager *p_platform) {
    platform = p_platform;
}

bool EOSInterfaceBase::is_available() const {
    return platform != nullptr && platform->is_initialized();
}

godot::Dictionary EOSInterfaceBase::require_ready(const godot::String &operation) const {
    godot::Dictionary context;
    if (platform == nullptr || !platform->is_initialized()) {
        context["reason"] = "EOS platform is not initialized";
        return EOSResult::make_result(14, operation, context);
    }
    godot::Dictionary ok;
    ok["ok"] = true;
    return ok;
}

godot::Dictionary EOSInterfaceBase::not_implemented(const godot::String &operation, const godot::String &reason) const {
    godot::Dictionary context;
    context["reason"] = reason;
    return EOSResult::make_result(16, operation, context);
}

RequestContext *EOSInterfaceBase::make_context(EOSCallbackQueue *queue, const godot::String &operation, const godot::Dictionary &context) {
    RequestContext *ctx = new RequestContext();
    ctx->queue = queue;
    ctx->operation = operation;
    ctx->context = context;
    return ctx;
}

RequestContext *EOSInterfaceBase::make_request(const godot::String &operation, const godot::Dictionary &context) {
    RequestContext *ctx = new RequestContext();
    ctx->queue = platform->get_queue().ptr();
    ctx->table = platform->api_table();
    ctx->platform = platform;
    ctx->operation = operation;
    ctx->context = context;
    return ctx;
}

EOSApiTable *EOSInterfaceBase::bindings() const {
    if (platform == nullptr) {
        return nullptr;
    }
    return platform->api_table();
}

}
