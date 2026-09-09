class_name YugenEOSConfig
extends Resource

@export var product_id: String = ""
@export var sandbox_id: String = ""
@export var deployment_id: String = ""
@export var client_id: String = ""
@export var product_name: String = "Yugen Game"
@export var product_version: String = "1.0.0"
@export var tick_interval_ms: int = 100
@export var log_level: int = 1
@export var cache_directory: String = "user://eos_cache"

var client_secret: String = ""
var encryption_key: String = ""

func load_from_project_settings() -> void:
	product_id = ProjectSettings.get_setting("yugen_eos/product_id", product_id)
	sandbox_id = ProjectSettings.get_setting("yugen_eos/sandbox_id", sandbox_id)
	deployment_id = ProjectSettings.get_setting("yugen_eos/deployment_id", deployment_id)
	client_id = ProjectSettings.get_setting("yugen_eos/client_id", client_id)
	product_name = ProjectSettings.get_setting("yugen_eos/product_name", product_name)
	product_version = ProjectSettings.get_setting("yugen_eos/product_version", product_version)
	tick_interval_ms = ProjectSettings.get_setting("yugen_eos/tick_interval_ms", tick_interval_ms)
	log_level = ProjectSettings.get_setting("yugen_eos/log_level", log_level)
	client_secret = OS.get_environment("EOS_CLIENT_SECRET")
	encryption_key = OS.get_environment("EOS_ENCRYPTION_KEY")

func is_presentable() -> Dictionary:
	return {
		"product_id_set": product_id != "",
		"sandbox_id_set": sandbox_id != "",
		"deployment_id_set": deployment_id != "",
		"client_id_set": client_id != "",
		"product_name": product_name,
		"product_version": product_version,
	}

func validate() -> Dictionary:
	var missing: Array[String] = []
	if product_id == "":
		missing.append("product_id")
	if sandbox_id == "":
		missing.append("sandbox_id")
	if deployment_id == "":
		missing.append("deployment_id")
	return {"ok": missing.is_empty(), "missing": missing}
