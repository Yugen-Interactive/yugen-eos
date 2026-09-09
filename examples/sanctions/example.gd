extends Node

@export var player_id: String = ""

func _ready() -> void:
	var result: Dictionary = EOS.social.query_sanctions(player_id)
	if int(result.get("code", 0)) == 39:
		result = await EOS.social.operation_completed
	print("sanctions: ", int(result.get("sanction_count", 0)))
	for entry in result.get("sanctions", []):
		print(entry.get("action", ""), " ", entry.get("reference_id", ""))
