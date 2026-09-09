class_name YugenEOSStub
extends RefCounted

var _initialized: bool = false

func initialize(_config: Dictionary) -> Dictionary:
	_initialized = false
	return {"code": 16, "name": "EOS_NotImplemented", "message": "Native core not loaded. Build the GDExtension and place EOS SDK binaries."}

func shutdown() -> void:
	_initialized = false

func is_initialized() -> bool:
	return _initialized

func is_stub() -> bool:
	return true

func tick() -> void:
	pass

func sdk_version() -> String:
	return "1.19.1"

func get_diagnostics() -> Dictionary:
	return {"native_loaded": false}
