class_name YugenEOSStorageFacade
extends "res://addons/yugen_eos/facades/eos_facade_base.gd"

func write_file(local_user_id: String = "", filename: String = "", data: PackedByteArray = PackedByteArray()) -> Dictionary:
	return _call_core("storage_write", {"local_user_id": local_user_id, "filename": filename, "data": data})

func read_file(local_user_id: String = "", filename: String = "") -> Dictionary:
	return _call_core("storage_read", {"local_user_id": local_user_id, "filename": filename})

func read_title_file(filename: String = "") -> Dictionary:
	return _call_core("titlestorage_read", {"filename": filename})
