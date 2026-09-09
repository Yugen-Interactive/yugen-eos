extends Node

func _ready() -> void:
	var diag: Dictionary = EOS.get_diagnostics()
	print("initialized: ", diag.get("initialized", false))
	print("sdk: ", diag.get("sdk_version", ""))
	print("platform: ", diag.get("platform", ""))
	print("native: ", diag.get("native_loaded", false))
