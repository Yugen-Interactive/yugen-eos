class_name YugenEOSUIFacade
extends "res://addons/yugen_eos/facades/eos_facade_base.gd"

func show_friends(local_user_id: String = "") -> Dictionary:
	return _call_core("ui_show_friends", [local_user_id])

func hide_friends() -> Dictionary:
	return _call_core("ui_hide_friends", [])

func set_visibility(visible: bool = true) -> Dictionary:
	return _call_core("ui_set_visibility", [visible])

func pause_social_overlay(paused: bool = true) -> Dictionary:
	return _call_core("ui_pause_overlay", [paused])

func is_social_overlay_paused() -> bool:
	var result := _call_core("ui_is_overlay_paused", [])
	return bool(result.get("paused", false))

func get_friends_visible(local_user_id: String = "") -> bool:
	var result := _call_core("ui_get_friends_visible", [local_user_id])
	return bool(result.get("visible", false))

func set_notification_location(location: int = 0) -> Dictionary:
	return _call_core("ui_set_notification_location", [location])
