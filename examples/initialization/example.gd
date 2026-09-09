extends Node

func _ready() -> void:
	var result: Dictionary = EOS.initialize()
	if not bool(result.get("ok", false)):
		push_error("EOS init failed: %s" % result.get("message", ""))
		return
	print("EOS ready, SDK ", EOS.sdk_version())
