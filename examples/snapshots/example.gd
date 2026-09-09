extends Node

@export var local_user_id: String = ""

func _ready() -> void:
	var begun: Dictionary = EOS.social.snapshot_begin(local_user_id)
	if not bool(begun.get("ok", false)):
		push_error("snapshot begin failed")
		return
	var snapshot_id: int = int(begun.get("snapshot_id", 0))
	EOS.social.snapshot_add(snapshot_id, "level", "12")
	var submitted: Dictionary = EOS.social.snapshot_submit(snapshot_id)
	if int(submitted.get("code", 0)) == 39:
		submitted = await EOS.social.operation_completed
	print("submitted: ", bool(submitted.get("ok", false)))
	EOS.social.snapshot_end(snapshot_id)
