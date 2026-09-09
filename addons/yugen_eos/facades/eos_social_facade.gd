class_name YugenEOSSocialFacade
extends "res://addons/yugen_eos/facades/eos_facade_base.gd"

func report_player(reporter_id: String = "", reported_id: String = "", category: int = 0, message: String = "") -> Dictionary:
	return _call_core("reports_send", {"reporter_id": reporter_id, "reported_id": reported_id, "category": category, "message": message})

func query_sanctions(player_id: String = "") -> Dictionary:
	return _call_core("sanctions_query", {"player_id": player_id})

func rtc_join(local_user_id: String = "", room: String = "") -> Dictionary:
	return _call_core("rtc_join", {"local_user_id": local_user_id, "room": room})

func rtc_leave(local_user_id: String = "", room: String = "") -> Dictionary:
	return _call_core("rtc_leave", {"local_user_id": local_user_id, "room": room})

func anticheat_start(local_user_id: String = "") -> Dictionary:
	return _call_core("anticheat_start", {"local_user_id": local_user_id})

func send_event(event_name: String = "", attributes: Dictionary = {}) -> Dictionary:
	return _call_core("metrics_send", {"event_name": event_name, "attributes": attributes})
