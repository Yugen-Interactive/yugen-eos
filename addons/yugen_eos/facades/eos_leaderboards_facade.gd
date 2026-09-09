class_name YugenEOSLeaderboardsFacade
extends "res://addons/yugen_eos/facades/eos_facade_base.gd"

func query_definitions() -> Dictionary:
	return _call_core("leaderboards_query_definitions", [])

func query_ranks(leaderboard_id: String = "", local_user_id: String = "") -> Dictionary:
	return _call_core("leaderboards_query_ranks", [leaderboard_id, local_user_id])

func query_user_scores(local_user_id: String = "", stat_name: String = "", target_user_ids: Array = []) -> Dictionary:
	return _call_core("leaderboards_query_scores", [local_user_id, stat_name, target_user_ids])
