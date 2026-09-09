class_name YugenEOSSessionsFacade
extends "res://addons/yugen_eos/facades/eos_facade_base.gd"

signal session_created(result: Dictionary)
signal session_joined(result: Dictionary)

func create(local_user_id: String = "", session_name: String = "", max_players: int = 8, is_public: bool = true, attributes: Array = []) -> Dictionary:
	var result := _call_core("sessions_create", [local_user_id, session_name, max_players, is_public, attributes])
	session_created.emit(result)
	operation_completed.emit(result)
	return result

func join(local_user_id: String = "", session_handle: String = "") -> Dictionary:
	var result := _call_core("sessions_join", [local_user_id, session_handle])
	session_joined.emit(result)
	operation_completed.emit(result)
	return result

func search(options: Dictionary = {}) -> Dictionary:
	return _call_core("sessions_search", [options])

func get_search_count() -> int:
	var result := _call_core("sessions_get_search_count", [])
	return int(result.get("count", 0))

func get_search_result(index: int = 0) -> Dictionary:
	return _call_core("sessions_get_search_result", [index])

func get_attributes(session_ref: int = 0) -> Dictionary:
	return _call_core("sessions_get_attributes", [session_ref])

func set_attributes(session_name: String = "", attributes: Array = []) -> Dictionary:
	return _call_core("sessions_set_attributes", [session_name, attributes])

func destroy(session_name: String = "") -> Dictionary:
	return _call_core("sessions_destroy", [session_name])

func send_invite(session_name: String = "", local_user_id: String = "", target_user_id: String = "") -> Dictionary:
	return _call_core("sessions_send_invite", [session_name, local_user_id, target_user_id])

func query_invites(local_user_id: String = "") -> Dictionary:
	return _call_core("sessions_query_invites", [local_user_id])

func reject_invite(local_user_id: String = "", invite_id: String = "") -> Dictionary:
	return _call_core("sessions_reject_invite", [local_user_id, invite_id])
