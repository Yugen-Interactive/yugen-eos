class_name YugenEOSAuthFacade
extends "res://addons/yugen_eos/facades/eos_facade_base.gd"

signal login_completed(result: Dictionary)
signal logout_completed(result: Dictionary)

func login(login_type: int = 0, id: String = "", token: String = "") -> Dictionary:
	var result := _call_core("auth_login", [login_type, id, token])
	login_completed.emit(result)
	operation_completed.emit(result)
	return result

func logout(local_user_id: String = "") -> Dictionary:
	var result := _call_core("auth_logout", [local_user_id])
	logout_completed.emit(result)
	operation_completed.emit(result)
	return result

func verify_auth(local_user_id: String = "") -> Dictionary:
	return _call_core("auth_verify", [local_user_id])

func link_account(continuance_ref: int = 0, link_flags: int = 0) -> Dictionary:
	var result := _call_core("auth_link_account", [continuance_ref, link_flags])
	login_completed.emit(result)
	operation_completed.emit(result)
	return result
