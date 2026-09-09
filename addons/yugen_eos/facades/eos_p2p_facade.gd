class_name YugenEOSP2PFacade
extends "res://addons/yugen_eos/facades/eos_facade_base.gd"

signal packet_received(result: Dictionary)

func send_packet(local_user_id: String = "", remote_user_id: String = "", socket_id: String = "game", data: PackedByteArray = PackedByteArray(), reliable: bool = true) -> Dictionary:
	return _call_core("p2p_send", {"local_user_id": local_user_id, "remote_user_id": remote_user_id, "socket_id": socket_id, "data": data, "reliable": reliable})

func receive_packet(local_user_id: String = "", socket_id: String = "game") -> Dictionary:
	return _call_core("p2p_receive", {"local_user_id": local_user_id, "socket_id": socket_id})

func set_relay(local_user_id: String = "", allow_relays: bool = true) -> Dictionary:
	return _call_core("p2p_set_relay", {"local_user_id": local_user_id, "allow_relays": allow_relays})
