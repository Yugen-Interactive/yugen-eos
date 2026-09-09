class_name YugenEOSEcomFacade
extends "res://addons/yugen_eos/facades/eos_facade_base.gd"

func query_offers(local_user_id: String = "") -> Dictionary:
	return _call_core("ecom_query_offers", [local_user_id])

func query_entitlements(local_user_id: String = "") -> Dictionary:
	return _call_core("ecom_query_entitlements", [local_user_id])

func checkout(local_user_id: String = "", offer_id: String = "") -> Dictionary:
	return _call_core("ecom_checkout", [local_user_id, offer_id])
