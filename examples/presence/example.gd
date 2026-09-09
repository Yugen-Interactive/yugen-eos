extends Node

@export var local_user_id: String = ""

func _ready() -> void:
	var result: Dictionary = EOS.presence.set_presence(local_user_id, 1, "In hub")
	if int(result.get("code", 0)) == 39:
		result = await EOS.presence.operation_completed
	print("presence set: ", bool(result.get("ok", false)))
