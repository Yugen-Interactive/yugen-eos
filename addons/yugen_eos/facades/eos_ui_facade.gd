class_name YugenEOSUIFacade
extends "res://addons/yugen_eos/facades/eos_facade_base.gd"

func show_friends(local_user_id: String = "") -> Dictionary:
	return _call_core("ui_show_friends", {"local_user_id": local_user_id})

func hide_friends() -> Dictionary:
	return _call_core("ui_hide_friends", {})

func set_visibility(visible: bool = true) -> Dictionary:
	return _call_core("ui_set_visibility", {"visible": visible})
