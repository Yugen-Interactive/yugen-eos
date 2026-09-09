class_name YugenEOSStatsFacade
extends "res://addons/yugen_eos/facades/eos_facade_base.gd"

func query(local_user_id: String = "", stat_names: Array = []) -> Dictionary:
	return _call_core("stats_query", {"local_user_id": local_user_id, "stat_names": stat_names})

func ingest(local_user_id: String = "", stats: Array = []) -> Dictionary:
	return _call_core("stats_ingest", {"local_user_id": local_user_id, "stats": stats})
