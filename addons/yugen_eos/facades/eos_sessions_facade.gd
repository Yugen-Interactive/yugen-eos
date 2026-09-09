class_name YugenEOSSessionsFacade
extends "res://addons/yugen_eos/facades/eos_facade_base.gd"

signal session_created(result: Dictionary)
signal session_joined(result: Dictionary)

func create(local_user_id: String = "", session_name: String = "", max_players: int = 8, is_public: bool = true) -> Dictionary:
	var result := _call_core("sessions_create", {"local_user_id": local_user_id, "session_name": session_name, "max_players": max_players, "is_public": is_public})
	session_created.emit(result)
	operation_completed.emit(result)
	return result

func join(local_user_id: String = "", session_handle: String = "") -> Dictionary:
	var result := _call_core("sessions_join", {"local_user_id": local_user_id, "session_handle": session_handle})
	session_joined.emit(result)
	operation_completed.emit(result)
	return result

func search(options: Dictionary = {}) -> Dictionary:
	return _call_core("sessions_search", options)
