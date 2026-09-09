extends Node

@export var local_user_id: String = ""

func _ready() -> void:
	var created: Dictionary = EOS.sessions.create(local_user_id, "my_session", 8, true)
	if int(created.get("code", 0)) == 39:
		created = await EOS.sessions.session_created
	print("session created: ", bool(created.get("ok", false)))
	var search: Dictionary = EOS.sessions.search({"local_user_id": local_user_id, "max_results": 10})
	if int(search.get("code", 0)) == 39:
		search = await EOS.sessions.operation_completed
	print("search done: ", bool(search.get("ok", false)))
