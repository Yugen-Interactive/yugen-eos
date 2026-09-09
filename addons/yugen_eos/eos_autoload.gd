extends Node

signal initialized(result: Dictionary)
signal shutdown_completed()

const EOSConfigResource = preload("res://addons/yugen_eos/eos_config_resource.gd")
const EOSStub = preload("res://addons/yugen_eos/eos_stub.gd")
const AuthFacade = preload("res://addons/yugen_eos/facades/eos_auth_facade.gd")
const ConnectFacade = preload("res://addons/yugen_eos/facades/eos_connect_facade.gd")
const FriendsFacade = preload("res://addons/yugen_eos/facades/eos_friends_facade.gd")
const PresenceFacade = preload("res://addons/yugen_eos/facades/eos_presence_facade.gd")
const SessionsFacade = preload("res://addons/yugen_eos/facades/eos_sessions_facade.gd")
const LobbiesFacade = preload("res://addons/yugen_eos/facades/eos_lobbies_facade.gd")
const P2PFacade = preload("res://addons/yugen_eos/facades/eos_p2p_facade.gd")
const StatsFacade = preload("res://addons/yugen_eos/facades/eos_stats_facade.gd")
const AchievementsFacade = preload("res://addons/yugen_eos/facades/eos_achievements_facade.gd")
const LeaderboardsFacade = preload("res://addons/yugen_eos/facades/eos_leaderboards_facade.gd")
const UserInfoFacade = preload("res://addons/yugen_eos/facades/eos_userinfo_facade.gd")
const StorageFacade = preload("res://addons/yugen_eos/facades/eos_storage_facade.gd")
const EcomFacade = preload("res://addons/yugen_eos/facades/eos_ecom_facade.gd")
const UIFacade = preload("res://addons/yugen_eos/facades/eos_ui_facade.gd")
const SocialFacade = preload("res://addons/yugen_eos/facades/eos_social_facade.gd")

var config: EOSConfigResource
var auth: AuthFacade
var connect_iface: ConnectFacade
var friends: FriendsFacade
var presence: PresenceFacade
var sessions: SessionsFacade
var lobbies: LobbiesFacade
var p2p: P2PFacade
var stats: StatsFacade
var achievements: AchievementsFacade
var leaderboards: LeaderboardsFacade
var userinfo: UserInfoFacade
var storage: StorageFacade
var ecom: EcomFacade
var ui: UIFacade
var social: SocialFacade

var _core: Object = null
var _tick_timer: Timer

func _ready() -> void:
	config = EOSConfigResource.new()
	config.load_from_project_settings()
	_core = _load_core()
	auth = AuthFacade.new(_core)
	connect_iface = ConnectFacade.new(_core)
	friends = FriendsFacade.new(_core)
	presence = PresenceFacade.new(_core)
	sessions = SessionsFacade.new(_core)
	lobbies = LobbiesFacade.new(_core)
	p2p = P2PFacade.new(_core)
	stats = StatsFacade.new(_core)
	achievements = AchievementsFacade.new(_core)
	leaderboards = LeaderboardsFacade.new(_core)
	userinfo = UserInfoFacade.new(_core)
	storage = StorageFacade.new(_core)
	ecom = EcomFacade.new(_core)
	ui = UIFacade.new(_core)
	social = SocialFacade.new(_core)
	_tick_timer = Timer.new()
	_tick_timer.wait_time = maxf(0.05, float(config.tick_interval_ms) / 1000.0)
	_tick_timer.timeout.connect(_on_tick)
	add_child(_tick_timer)
	_tick_timer.start()

func _load_core() -> Object:
	if ClassDB.class_exists("YugenEOS"):
		var singleton = Engine.get_singleton("YugenEOS")
		if singleton != null:
			return singleton
	return EOSStub.new()

func initialize() -> Dictionary:
	var result: Dictionary = {"code": 14, "name": "EOS_NotConfigured", "message": "Core not available"}
	if _core != null and _core.has_method("initialize"):
		result = _core.initialize(config_to_dict())
	initialized.emit(result)
	return result

func shutdown() -> void:
	if _core != null and _core.has_method("shutdown"):
		_core.shutdown()
	shutdown_completed.emit()

func is_initialized() -> bool:
	if _core != null and _core.has_method("is_initialized"):
		return _core.is_initialized()
	return false

func tick() -> void:
	if _core != null and _core.has_method("tick"):
		_core.tick()

func get_diagnostics() -> Dictionary:
	var diag := {
		"initialized": is_initialized(),
		"sdk_version": sdk_version(),
		"platform": OS.get_name(),
		"config": config.is_presentable(),
	}
	if _core != null and _core.has_method("get_diagnostics"):
		var extra: Dictionary = _core.get_diagnostics()
		for key in extra:
			diag[key] = extra[key]
	return diag

func sdk_version() -> String:
	if _core != null and _core.has_method("sdk_version"):
		return str(_core.sdk_version())
	return "1.19.1"

func config_to_dict() -> Dictionary:
	return {
		"product_id": config.product_id,
		"product_name": config.product_name,
		"product_version": config.product_version,
		"sandbox_id": config.sandbox_id,
		"deployment_id": config.deployment_id,
		"client_id": config.client_id,
		"client_secret": config.client_secret,
		"encryption_key": config.encryption_key,
		"cache_directory": ProjectSettings.globalize_path(config.cache_directory),
		"log_level": config.log_level,
	}

func _on_tick() -> void:
	tick()
