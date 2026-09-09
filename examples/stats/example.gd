extends Node

@export var local_user_id: String = ""

func _ready() -> void:
	var ingested: Dictionary = EOS.stats.ingest(local_user_id, [{"name": "kills", "value": 3}])
	if int(ingested.get("code", 0)) == 39:
		ingested = await EOS.stats.operation_completed
	print("ingested: ", bool(ingested.get("ok", false)))
	var queried: Dictionary = EOS.stats.query(local_user_id, ["kills"])
	if int(queried.get("code", 0)) == 39:
		queried = await EOS.stats.operation_completed
	print("queried: ", bool(queried.get("ok", false)))
