class_name YugenEOSLobbiesFacade
extends "res://addons/yugen_eos/facades/eos_facade_base.gd"

signal lobby_created(result: Dictionary)
signal lobby_joined(result: Dictionary)

func create(local_user_id: String = "", max_members: int = 8, is_public: bool = true) -> Dictionary:
	var result := _call_core("lobbies_create", [local_user_id, max_members, is_public])
	lobby_created.emit(result)
	operation_completed.emit(result)
	return result

func join(lobby_id: String = "", local_user_id: String = "") -> Dictionary:
	var result := _call_core("lobbies_join", [lobby_id, local_user_id])
	lobby_joined.emit(result)
	operation_completed.emit(result)
	return result

func join_details(lobby_ref: int = 0, local_user_id: String = "") -> Dictionary:
	var result := _call_core("lobbies_join_details", [lobby_ref, local_user_id])
	lobby_joined.emit(result)
	operation_completed.emit(result)
	return result

func search(local_user_id: String = "", max_results: int = 10) -> Dictionary:
	return _call_core("lobbies_search", [local_user_id, max_results])

func get_search_count() -> int:
	var result := _call_core("lobbies_get_search_count", [])
	return int(result.get("count", 0))

func get_search_result(index: int = 0) -> Dictionary:
	return _call_core("lobbies_get_search_result", [index])

func get_details(lobby_id: String = "", local_user_id: String = "") -> Dictionary:
	return _call_core("lobbies_get_details", [lobby_id, local_user_id])

func leave(lobby_id: String = "", local_user_id: String = "") -> Dictionary:
	return _call_core("lobbies_leave", [lobby_id, local_user_id])

func send_invite(lobby_id: String = "", local_user_id: String = "", target_user_id: String = "") -> Dictionary:
	return _call_core("lobbies_send_invite", [lobby_id, local_user_id, target_user_id])
