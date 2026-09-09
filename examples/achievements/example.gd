extends Node

@export var local_user_id: String = ""
@export var achievement_id: String = "first_win"

func _ready() -> void:
	var result: Dictionary = EOS.achievements.unlock(local_user_id, achievement_id)
	if int(result.get("code", 0)) == 39:
		result = await EOS.achievements.unlocked
	print("unlocked: ", bool(result.get("ok", false)))
