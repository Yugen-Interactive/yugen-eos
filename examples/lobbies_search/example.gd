extends Node

@export var local_user_id: String = ""

func _ready() -> void:
	var search: Dictionary = EOS.lobbies.search(local_user_id, 10)
	if int(search.get("code", 0)) == 39:
		search = await EOS.lobbies.operation_completed
	if not bool(search.get("ok", false)):
		push_error("lobby search failed")
		return
	var count: int = EOS.lobbies.get_search_count()
	print("lobbies found: ", count)
	if count > 0:
		var ref: Dictionary = EOS.lobbies.get_search_result(0)
		var joined: Dictionary = EOS.lobbies.join_details(int(ref.get("lobby_ref", 0)), local_user_id)
		if int(joined.get("code", 0)) == 39:
			joined = await EOS.lobbies.lobby_joined
		print("joined: ", joined.get("lobby_id", ""))
