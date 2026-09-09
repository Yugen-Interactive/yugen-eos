class_name YugenEOSAchievementsFacade
extends "res://addons/yugen_eos/facades/eos_facade_base.gd"

signal unlocked(result: Dictionary)

func unlock(local_user_id: String = "", achievement_id: String = "") -> Dictionary:
	var result := _call_core("achievements_unlock", [local_user_id, achievement_id])
	unlocked.emit(result)
	operation_completed.emit(result)
	return result

func query(local_user_id: String = "", achievement_ids: Array = []) -> Dictionary:
	return _call_core("achievements_query", [local_user_id, achievement_ids])

func query_definitions(local_user_id: String = "") -> Dictionary:
	return _call_core("achievements_query_definitions", [local_user_id])

func get_definition_count() -> int:
	var result := _call_core("achievements_get_definition_count", [])
	return int(result.get("count", 0))

func get_definition(index: int = 0) -> Dictionary:
	return _call_core("achievements_get_definition", [index])
