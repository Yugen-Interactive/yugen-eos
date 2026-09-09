class_name YugenEOSFriendsFacade
extends "res://addons/yugen_eos/facades/eos_facade_base.gd"

func get_friends(local_user_id: String = "") -> Dictionary:
	return _call_core("friends_query", [local_user_id])

func get_friend_count(local_user_id: String = "") -> int:
	var result := _call_core("friends_get_count", [local_user_id])
	return int(result.get("count", 0))

func send_invite(local_user_id: String = "", target_user_id: String = "") -> Dictionary:
	return _call_core("friends_send_invite", [local_user_id, target_user_id])

func accept_invite(local_user_id: String = "", target_user_id: String = "") -> Dictionary:
	return _call_core("friends_accept_invite", [local_user_id, target_user_id])

func reject_invite(local_user_id: String = "", target_user_id: String = "") -> Dictionary:
	return _call_core("friends_reject_invite", [local_user_id, target_user_id])
