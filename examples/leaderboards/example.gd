extends Node

@export var local_user_id: String = ""

func _ready() -> void:
	var defs: Dictionary = EOS.leaderboards.query_definitions()
	if int(defs.get("code", 0)) == 39:
		defs = await EOS.leaderboards.operation_completed
	print("definitions: ", bool(defs.get("ok", false)))
	var ranks: Dictionary = EOS.leaderboards.query_ranks("top_players", local_user_id)
	if int(ranks.get("code", 0)) == 39:
		ranks = await EOS.leaderboards.operation_completed
	print("ranks: ", bool(ranks.get("ok", false)))
