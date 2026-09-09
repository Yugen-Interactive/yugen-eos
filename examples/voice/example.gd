extends Node

@export var local_user_id: String = ""
@export var room: String = "squad"

func _ready() -> void:
	var joined: Dictionary = EOS.social.rtc_join(local_user_id, room)
	if int(joined.get("code", 0)) == 39:
		joined = await EOS.social.operation_completed
	print("rtc joined: ", bool(joined.get("ok", false)))
