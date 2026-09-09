extends Node

@export var local_user_id: String = ""

func _ready() -> void:
	var created: Dictionary = EOS.lobbies.create(local_user_id, 8, true)
	if int(created.get("code", 0)) == 39:
		created = await EOS.lobbies.lobby_created
	if not bool(created.get("ok", false)):
		push_error("lobby create failed")
		return
	var lobby_id: String = str(created.get("lobby_id", ""))
	print("lobby: ", lobby_id)
	var left: Dictionary = EOS.lobbies.leave(lobby_id, local_user_id)
	if int(left.get("code", 0)) == 39:
		left = await EOS.lobbies.operation_completed
	print("left: ", bool(left.get("ok", false)))
