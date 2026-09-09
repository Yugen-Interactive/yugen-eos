extends Node

@export var local_user_id: String = ""

func _ready() -> void:
	var result: Dictionary = EOS.social.anticheat_start(local_user_id)
	print("anticheat: ", result.get("name", ""), " - ", result.get("message", ""))
