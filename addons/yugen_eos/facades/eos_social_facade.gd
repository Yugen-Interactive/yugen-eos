class_name YugenEOSSocialFacade
extends "res://addons/yugen_eos/facades/eos_facade_base.gd"

func report_player(reporter_id: String = "", reported_id: String = "", category: int = 0, message: String = "") -> Dictionary:
	return _call_core("reports_send", [reporter_id, reported_id, category, message])

func query_sanctions(player_id: String = "") -> Dictionary:
	return _call_core("sanctions_query", [player_id])

func appeal_sanction(player_id: String = "", reason: int = 0, reference_id: String = "") -> Dictionary:
	return _call_core("sanctions_appeal", [player_id, reason, reference_id])

func rtc_join(local_user_id: String = "", room: String = "") -> Dictionary:
	return _call_core("rtc_join", [local_user_id, room])

func rtc_leave(local_user_id: String = "", room: String = "") -> Dictionary:
	return _call_core("rtc_leave", [local_user_id, room])

func rtc_set_sending_volume(local_user_id: String = "", room: String = "", volume: float = 50.0) -> Dictionary:
	return _call_core("rtc_set_sending_volume", [local_user_id, room, volume])

func rtc_set_receiving_volume(local_user_id: String = "", room: String = "", volume: float = 50.0) -> Dictionary:
	return _call_core("rtc_set_receiving_volume", [local_user_id, room, volume])

func anticheat_start(local_user_id: String = "") -> Dictionary:
	return _call_core("anticheat_start", [local_user_id])

func send_event(event_name: String = "", attributes: Dictionary = {}) -> Dictionary:
	return _call_core("metrics_send", [event_name, attributes])

func begin_player_session(account_id: String = "") -> Dictionary:
	return _call_core("metrics_begin_session", [account_id])

func end_player_session(account_id: String = "") -> Dictionary:
	return _call_core("metrics_end_session", [account_id])

func enumerate_mods(local_user_id: String = "", mod_type: int = 0) -> Dictionary:
	return _call_core("mods_enumerate", [local_user_id, mod_type])

func query_age_gate() -> Dictionary:
	return _call_core("kws_query_age_gate", [])

func set_custom_invite(local_user_id: String = "", payload: String = "") -> Dictionary:
	return _call_core("custom_invites_set", [local_user_id, payload])

func send_custom_invite(local_user_id: String = "", target_user_ids: Array = []) -> Dictionary:
	return _call_core("custom_invites_send", [local_user_id, target_user_ids])

func snapshot_begin(local_user_id: String = "") -> Dictionary:
	return _call_core("snapshot_begin", [local_user_id])

func snapshot_add(snapshot_id: int = 0, key: String = "", value: String = "") -> Dictionary:
	return _call_core("snapshot_add", [snapshot_id, key, value])

func snapshot_submit(snapshot_id: int = 0) -> Dictionary:
	return _call_core("snapshot_submit", [snapshot_id])

func snapshot_end(snapshot_id: int = 0) -> Dictionary:
	return _call_core("snapshot_end", [snapshot_id])
