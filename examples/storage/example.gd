extends Node

@export var local_user_id: String = ""
@export var filename: String = "savegame.bin"

func _ready() -> void:
	var payload := PackedByteArray([7, 8, 9])
	var written: Dictionary = EOS.storage.write_file(local_user_id, filename, payload)
	if int(written.get("code", 0)) == 39:
		written = await EOS.storage.operation_completed
	print("written: ", bool(written.get("ok", false)))
	var read: Dictionary = EOS.storage.read_file(local_user_id, filename)
	if int(read.get("code", 0)) == 39:
		read = await EOS.storage.operation_completed
	if bool(read.get("ok", false)):
		print("bytes: ", (read.get("data") as PackedByteArray).size())
