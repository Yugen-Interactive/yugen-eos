extends Node

@export var local_user_id: String = ""
@export var remote_user_id: String = ""

func _ready() -> void:
	var payload := PackedByteArray([1, 2, 3, 4])
	var sent: Dictionary = EOS.p2p.send_packet(local_user_id, remote_user_id, "game", payload, true)
	print("sent: ", bool(sent.get("ok", false)))
	var received: Dictionary = EOS.p2p.receive_packet(local_user_id, "game")
	if bool(received.get("has_packet", false)):
		print("got ", (received.get("data") as PackedByteArray).size(), " bytes")
