class_name YugenEOSFacadeBase
extends RefCounted

signal operation_completed(result: Dictionary)

var _core: Object

func _init(core: Object) -> void:
	_core = core

func _call_core(method: String, args: Dictionary) -> Dictionary:
	if _core != null and _core.has_method(method):
		var out = _core.call(method, args)
		if out is Dictionary:
			return out
	return {"code": 16, "name": "EOS_NotImplemented", "message": "Native core unavailable for " + method}

func _available() -> bool:
	return _core != null and not (_core.has_method("is_stub") and bool(_core.call("is_stub")))
