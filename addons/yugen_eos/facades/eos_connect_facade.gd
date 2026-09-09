class_name YugenEOSConnectFacade
extends "res://addons/yugen_eos/facades/eos_facade_base.gd"

signal login_completed(result: Dictionary)

func login(credential_type: int = 0, token: String = "", display_name: String = "") -> Dictionary:
	var result := _call_core("connect_login", {"credential_type": credential_type, "token": token, "display_name": display_name})
	login_completed.emit(result)
	operation_completed.emit(result)
	return result

func logout(product_user_id: String = "") -> Dictionary:
	return _call_core("connect_logout", {"product_user_id": product_user_id})

func create_user(product_user_id: String = "") -> Dictionary:
	return _call_core("connect_create_user", {"product_user_id": product_user_id})
