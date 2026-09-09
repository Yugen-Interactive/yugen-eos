class_name YugenEOSPresenceFacade
extends "res://addons/yugen_eos/facades/eos_facade_base.gd"

func set_presence(local_user_id: String = "", status: int = 0, details: String = "") -> Dictionary:
	return _call_core("presence_set", [local_user_id, status, details])

func query_presence(local_user_id: String = "", target_user_id: String = "") -> Dictionary:
	return _call_core("presence_query", [local_user_id, target_user_id])

func has_presence(local_user_id: String = "", target_user_id: String = "") -> bool:
	var result := _call_core("presence_has", [local_user_id, target_user_id])
	return bool(result.get("has_presence", false))
