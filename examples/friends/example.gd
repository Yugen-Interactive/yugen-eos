extends Node

@export var local_user_id: String = ""

func _ready() -> void:
	var query: Dictionary = EOS.friends.get_friends(local_user_id)
	if int(query.get("code", 0)) == 39:
		query = await EOS.friends.operation_completed
	if not bool(query.get("ok", false)):
		push_error("friends query failed")
		return
	var count: int = EOS.friends.get_friend_count(local_user_id)
	print("friends: ", count)
