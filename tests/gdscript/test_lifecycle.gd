extends Node

var _failures: Array[String] = []

func _ready() -> void:
	run_lifecycle()
	run_errors()
	if _failures.is_empty():
		print("YugenEOS GDScript tests: PASS")
	else:
		push_error("YugenEOS GDScript tests: FAIL")
		for failure in _failures:
			push_error(failure)
	get_tree().quit(1 if not _failures.is_empty() else 0)

func check(condition: bool, name: String) -> void:
	if not condition:
		_failures.append(name)

func run_lifecycle() -> void:
	var eos := get_node_or_null("/root/EOS")
	check(eos != null, "autoload EOS exists")
	if eos == null:
		return
	check(eos.has_method("initialize"), "EOS.initialize exists")
	check(eos.has_method("tick"), "EOS.tick exists")
	check(eos.has_method("get_diagnostics"), "EOS.get_diagnostics exists")
	var diag: Dictionary = eos.get_diagnostics()
	check(diag.has("sdk_version"), "diagnostics expose sdk_version")
	var result: Dictionary = eos.initialize()
	check(result.has("code"), "initialize returns result dict")

func run_errors() -> void:
	var eos := get_node_or_null("/root/EOS")
	if eos == null:
		return
	var result: Dictionary = eos.auth.login(0, "", "")
	check(result.has("code"), "auth.login returns result dict")
	check(int(result.get("code", 0)) != 0 or eos.is_initialized(), "auth without init reports error")
