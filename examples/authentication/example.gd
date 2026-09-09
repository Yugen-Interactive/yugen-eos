extends Node

func _ready() -> void:
	var boot: Dictionary = EOS.initialize()
	if not bool(boot.get("ok", false)):
		push_error("EOS init failed")
		return
	var login: Dictionary = EOS.auth.login(1, "", "")
	if int(login.get("code", 0)) == 39:
		login = await EOS.auth.login_completed
	if bool(login.get("ok", false)):
		print("logged in as ", login.get("local_user_id", ""))
	else:
		push_error("login failed: %s" % login.get("message", ""))
