#include "core/eos_callback_queue.hpp"

namespace yugen {

void EOSCallbackQueue::_bind_methods() {
    godot::ClassDB::bind_method(godot::D_METHOD("drain"), &EOSCallbackQueue::drain);
    godot::ClassDB::bind_method(godot::D_METHOD("pending_count"), &EOSCallbackQueue::pending_count);
    godot::ClassDB::bind_method(godot::D_METHOD("clear"), &EOSCallbackQueue::clear);
}

uint64_t EOSCallbackQueue::enqueue(const godot::String &operation, const godot::Dictionary &payload) {
    return enqueue_callable(operation, payload, godot::Callable());
}

uint64_t EOSCallbackQueue::enqueue_callable(const godot::String &operation, const godot::Dictionary &payload, const godot::Callable &target) {
    std::lock_guard<std::mutex> lock(mutex);
    PendingCallback item;
    item.id = next_id++;
    item.operation = operation;
    item.payload = payload;
    item.target = target;
    pending.push(item);
    return item.id;
}

godot::Array EOSCallbackQueue::drain(int max_items) {
    godot::Array out;
    std::lock_guard<std::mutex> lock(mutex);
    int count = 0;
    while (!pending.empty() && (max_items <= 0 || count < max_items)) {
        PendingCallback item = pending.front();
        pending.pop();
        godot::Dictionary entry;
        entry["id"] = (int64_t)item.id;
        entry["operation"] = item.operation;
        entry["payload"] = item.payload;
        out.push_back(entry);
        if (item.target.is_valid()) {
            item.target.call(item.payload);
        }
        count++;
    }
    return out;
}

int EOSCallbackQueue::pending_count() {
    std::lock_guard<std::mutex> lock(mutex);
    return (int)pending.size();
}

void EOSCallbackQueue::clear() {
    std::lock_guard<std::mutex> lock(mutex);
    std::queue<PendingCallback> empty;
    std::swap(pending, empty);
}

}
