extends EditorPlugin

const AUTOLOAD_NAME := "EOS"
const AUTOLOAD_PATH := "res://addons/yugen_eos/eos_autoload.gd"

const PROJECT_SETTINGS := [
	["yugen_eos/product_id", TYPE_STRING, ""],
	["yugen_eos/sandbox_id", TYPE_STRING, ""],
	["yugen_eos/deployment_id", TYPE_STRING, ""],
	["yugen_eos/client_id", TYPE_STRING, ""],
	["yugen_eos/product_name", TYPE_STRING, "Yugen Game"],
	["yugen_eos/product_version", TYPE_STRING, "1.0.0"],
	["yugen_eos/tick_interval_ms", TYPE_INT, 100],
	["yugen_eos/log_level", TYPE_INT, 1],
]

func _enter_tree() -> void:
	for entry in PROJECT_SETTINGS:
		var key: String = entry[0]
		if not ProjectSettings.has_setting(key):
			ProjectSettings.set_setting(key, entry[2])
		ProjectSettings.set_initial_value(key, entry[2])
		ProjectSettings.add_property_info({
			"name": key,
			"type": entry[1],
		})
	if not has_autoload(AUTOLOAD_NAME):
		add_autoload_singleton(AUTOLOAD_NAME, AUTOLOAD_PATH)
	add_tool_menu_item("Yugen EOS: Diagnostics", _on_diagnostics)


func _exit_tree() -> void:
	remove_tool_menu_item("Yugen EOS: Diagnostics")


func has_autoload(name: String) -> bool:
	return ProjectSettings.has_setting("autoload/" + name)


func _on_diagnostics() -> void:
	var eos := get_node_or_null("/root/EOS")
	if eos == null:
		push_warning("YugenEOS: EOS autoload not active in editor.")
		return
	if eos.has_method("get_diagnostics"):
		print("YugenEOS diagnostics: ", eos.get_diagnostics())
