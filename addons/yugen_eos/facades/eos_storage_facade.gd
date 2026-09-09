class_name YugenEOSStorageFacade
extends "res://addons/yugen_eos/facades/eos_facade_base.gd"

func query_list(local_user_id: String = "") -> Dictionary:
	return _call_core("storage_query_list", [local_user_id])

func write_file(local_user_id: String = "", filename: String = "", data: PackedByteArray = PackedByteArray()) -> Dictionary:
	return _call_core("storage_write", [local_user_id, filename, data])

func read_file(local_user_id: String = "", filename: String = "") -> Dictionary:
	return _call_core("storage_read", [local_user_id, filename])

func read_title_file(filename: String = "") -> Dictionary:
	return _call_core("titlestorage_read", [filename])
