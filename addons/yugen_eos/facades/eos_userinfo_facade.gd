class_name YugenEOSUserInfoFacade
extends "res://addons/yugen_eos/facades/eos_facade_base.gd"

func query(local_user_id: String = "", target_user_id: String = "") -> Dictionary:
	return _call_core("userinfo_query", {"local_user_id": local_user_id, "target_user_id": target_user_id})

func get_display_name(target_user_id: String = "") -> String:
	var result := _call_core("userinfo_get", {"target_user_id": target_user_id})
	return str(result.get("display_name", ""))
