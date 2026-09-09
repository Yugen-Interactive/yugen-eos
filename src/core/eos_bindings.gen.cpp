#include "core/eos_bindings.gen.hpp"

#if YUGEN_EOS_HAS_SDK

#include "core/eos_loader.hpp"

namespace yugen {

bool EOSApiTable::load(EOSLoader *loader) {
    missing_count = 0;
    if (loader == nullptr || !loader->is_loaded()) {
        return false;
    }
    EOS_Achievements_CopyAchievementDefinitionV2ByIndex = (EOS_Achievements_CopyAchievementDefinitionV2ByIndex_fn)loader->find_symbol("EOS_Achievements_CopyAchievementDefinitionV2ByIndex");
    if (EOS_Achievements_CopyAchievementDefinitionV2ByIndex == nullptr) { missing_count++; }
    EOS_Achievements_DefinitionV2_Release = (EOS_Achievements_DefinitionV2_Release_fn)loader->find_symbol("EOS_Achievements_DefinitionV2_Release");
    if (EOS_Achievements_DefinitionV2_Release == nullptr) { missing_count++; }
    EOS_Achievements_GetAchievementDefinitionCount = (EOS_Achievements_GetAchievementDefinitionCount_fn)loader->find_symbol("EOS_Achievements_GetAchievementDefinitionCount");
    if (EOS_Achievements_GetAchievementDefinitionCount == nullptr) { missing_count++; }
    EOS_Achievements_QueryDefinitions = (EOS_Achievements_QueryDefinitions_fn)loader->find_symbol("EOS_Achievements_QueryDefinitions");
    if (EOS_Achievements_QueryDefinitions == nullptr) { missing_count++; }
    EOS_Achievements_QueryPlayerAchievements = (EOS_Achievements_QueryPlayerAchievements_fn)loader->find_symbol("EOS_Achievements_QueryPlayerAchievements");
    if (EOS_Achievements_QueryPlayerAchievements == nullptr) { missing_count++; }
    EOS_Achievements_UnlockAchievements = (EOS_Achievements_UnlockAchievements_fn)loader->find_symbol("EOS_Achievements_UnlockAchievements");
    if (EOS_Achievements_UnlockAchievements == nullptr) { missing_count++; }
    EOS_Auth_GetLoggedInAccountsCount = (EOS_Auth_GetLoggedInAccountsCount_fn)loader->find_symbol("EOS_Auth_GetLoggedInAccountsCount");
    if (EOS_Auth_GetLoggedInAccountsCount == nullptr) { missing_count++; }
    EOS_Auth_LinkAccount = (EOS_Auth_LinkAccount_fn)loader->find_symbol("EOS_Auth_LinkAccount");
    if (EOS_Auth_LinkAccount == nullptr) { missing_count++; }
    EOS_Auth_Login = (EOS_Auth_Login_fn)loader->find_symbol("EOS_Auth_Login");
    if (EOS_Auth_Login == nullptr) { missing_count++; }
    EOS_Auth_Logout = (EOS_Auth_Logout_fn)loader->find_symbol("EOS_Auth_Logout");
    if (EOS_Auth_Logout == nullptr) { missing_count++; }
    EOS_Connect_CreateUser = (EOS_Connect_CreateUser_fn)loader->find_symbol("EOS_Connect_CreateUser");
    if (EOS_Connect_CreateUser == nullptr) { missing_count++; }
    EOS_Connect_GetLoggedInUsersCount = (EOS_Connect_GetLoggedInUsersCount_fn)loader->find_symbol("EOS_Connect_GetLoggedInUsersCount");
    if (EOS_Connect_GetLoggedInUsersCount == nullptr) { missing_count++; }
    EOS_Connect_Login = (EOS_Connect_Login_fn)loader->find_symbol("EOS_Connect_Login");
    if (EOS_Connect_Login == nullptr) { missing_count++; }
    EOS_Connect_Logout = (EOS_Connect_Logout_fn)loader->find_symbol("EOS_Connect_Logout");
    if (EOS_Connect_Logout == nullptr) { missing_count++; }
    EOS_CustomInvites_SendCustomInvite = (EOS_CustomInvites_SendCustomInvite_fn)loader->find_symbol("EOS_CustomInvites_SendCustomInvite");
    if (EOS_CustomInvites_SendCustomInvite == nullptr) { missing_count++; }
    EOS_CustomInvites_SetCustomInvite = (EOS_CustomInvites_SetCustomInvite_fn)loader->find_symbol("EOS_CustomInvites_SetCustomInvite");
    if (EOS_CustomInvites_SetCustomInvite == nullptr) { missing_count++; }
    EOS_EResult_IsOperationComplete = (EOS_EResult_IsOperationComplete_fn)loader->find_symbol("EOS_EResult_IsOperationComplete");
    if (EOS_EResult_IsOperationComplete == nullptr) { missing_count++; }
    EOS_Ecom_Checkout = (EOS_Ecom_Checkout_fn)loader->find_symbol("EOS_Ecom_Checkout");
    if (EOS_Ecom_Checkout == nullptr) { missing_count++; }
    EOS_Ecom_QueryEntitlements = (EOS_Ecom_QueryEntitlements_fn)loader->find_symbol("EOS_Ecom_QueryEntitlements");
    if (EOS_Ecom_QueryEntitlements == nullptr) { missing_count++; }
    EOS_Ecom_QueryOffers = (EOS_Ecom_QueryOffers_fn)loader->find_symbol("EOS_Ecom_QueryOffers");
    if (EOS_Ecom_QueryOffers == nullptr) { missing_count++; }
    EOS_EpicAccountId_FromString = (EOS_EpicAccountId_FromString_fn)loader->find_symbol("EOS_EpicAccountId_FromString");
    if (EOS_EpicAccountId_FromString == nullptr) { missing_count++; }
    EOS_EpicAccountId_IsValid = (EOS_EpicAccountId_IsValid_fn)loader->find_symbol("EOS_EpicAccountId_IsValid");
    if (EOS_EpicAccountId_IsValid == nullptr) { missing_count++; }
    EOS_EpicAccountId_ToString = (EOS_EpicAccountId_ToString_fn)loader->find_symbol("EOS_EpicAccountId_ToString");
    if (EOS_EpicAccountId_ToString == nullptr) { missing_count++; }
    EOS_Friends_AcceptInvite = (EOS_Friends_AcceptInvite_fn)loader->find_symbol("EOS_Friends_AcceptInvite");
    if (EOS_Friends_AcceptInvite == nullptr) { missing_count++; }
    EOS_Friends_GetFriendsCount = (EOS_Friends_GetFriendsCount_fn)loader->find_symbol("EOS_Friends_GetFriendsCount");
    if (EOS_Friends_GetFriendsCount == nullptr) { missing_count++; }
    EOS_Friends_QueryFriends = (EOS_Friends_QueryFriends_fn)loader->find_symbol("EOS_Friends_QueryFriends");
    if (EOS_Friends_QueryFriends == nullptr) { missing_count++; }
    EOS_Friends_RejectInvite = (EOS_Friends_RejectInvite_fn)loader->find_symbol("EOS_Friends_RejectInvite");
    if (EOS_Friends_RejectInvite == nullptr) { missing_count++; }
    EOS_Friends_SendInvite = (EOS_Friends_SendInvite_fn)loader->find_symbol("EOS_Friends_SendInvite");
    if (EOS_Friends_SendInvite == nullptr) { missing_count++; }
    EOS_GetVersion = (EOS_GetVersion_fn)loader->find_symbol("EOS_GetVersion");
    if (EOS_GetVersion == nullptr) { missing_count++; }
    EOS_Initialize = (EOS_Initialize_fn)loader->find_symbol("EOS_Initialize");
    if (EOS_Initialize == nullptr) { missing_count++; }
    EOS_KWS_QueryAgeGate = (EOS_KWS_QueryAgeGate_fn)loader->find_symbol("EOS_KWS_QueryAgeGate");
    if (EOS_KWS_QueryAgeGate == nullptr) { missing_count++; }
    EOS_Leaderboards_QueryLeaderboardDefinitions = (EOS_Leaderboards_QueryLeaderboardDefinitions_fn)loader->find_symbol("EOS_Leaderboards_QueryLeaderboardDefinitions");
    if (EOS_Leaderboards_QueryLeaderboardDefinitions == nullptr) { missing_count++; }
    EOS_Leaderboards_QueryLeaderboardRanks = (EOS_Leaderboards_QueryLeaderboardRanks_fn)loader->find_symbol("EOS_Leaderboards_QueryLeaderboardRanks");
    if (EOS_Leaderboards_QueryLeaderboardRanks == nullptr) { missing_count++; }
    EOS_Leaderboards_QueryLeaderboardUserScores = (EOS_Leaderboards_QueryLeaderboardUserScores_fn)loader->find_symbol("EOS_Leaderboards_QueryLeaderboardUserScores");
    if (EOS_Leaderboards_QueryLeaderboardUserScores == nullptr) { missing_count++; }
    EOS_LobbyDetails_CopyAttributeByIndex = (EOS_LobbyDetails_CopyAttributeByIndex_fn)loader->find_symbol("EOS_LobbyDetails_CopyAttributeByIndex");
    if (EOS_LobbyDetails_CopyAttributeByIndex == nullptr) { missing_count++; }
    EOS_LobbyDetails_CopyInfo = (EOS_LobbyDetails_CopyInfo_fn)loader->find_symbol("EOS_LobbyDetails_CopyInfo");
    if (EOS_LobbyDetails_CopyInfo == nullptr) { missing_count++; }
    EOS_LobbyDetails_GetAttributeCount = (EOS_LobbyDetails_GetAttributeCount_fn)loader->find_symbol("EOS_LobbyDetails_GetAttributeCount");
    if (EOS_LobbyDetails_GetAttributeCount == nullptr) { missing_count++; }
    EOS_LobbyDetails_GetMemberByIndex = (EOS_LobbyDetails_GetMemberByIndex_fn)loader->find_symbol("EOS_LobbyDetails_GetMemberByIndex");
    if (EOS_LobbyDetails_GetMemberByIndex == nullptr) { missing_count++; }
    EOS_LobbyDetails_GetMemberCount = (EOS_LobbyDetails_GetMemberCount_fn)loader->find_symbol("EOS_LobbyDetails_GetMemberCount");
    if (EOS_LobbyDetails_GetMemberCount == nullptr) { missing_count++; }
    EOS_LobbyDetails_Info_Release = (EOS_LobbyDetails_Info_Release_fn)loader->find_symbol("EOS_LobbyDetails_Info_Release");
    if (EOS_LobbyDetails_Info_Release == nullptr) { missing_count++; }
    EOS_LobbyDetails_Release = (EOS_LobbyDetails_Release_fn)loader->find_symbol("EOS_LobbyDetails_Release");
    if (EOS_LobbyDetails_Release == nullptr) { missing_count++; }
    EOS_LobbySearch_CopySearchResultByIndex = (EOS_LobbySearch_CopySearchResultByIndex_fn)loader->find_symbol("EOS_LobbySearch_CopySearchResultByIndex");
    if (EOS_LobbySearch_CopySearchResultByIndex == nullptr) { missing_count++; }
    EOS_LobbySearch_Find = (EOS_LobbySearch_Find_fn)loader->find_symbol("EOS_LobbySearch_Find");
    if (EOS_LobbySearch_Find == nullptr) { missing_count++; }
    EOS_LobbySearch_GetSearchResultCount = (EOS_LobbySearch_GetSearchResultCount_fn)loader->find_symbol("EOS_LobbySearch_GetSearchResultCount");
    if (EOS_LobbySearch_GetSearchResultCount == nullptr) { missing_count++; }
    EOS_LobbySearch_Release = (EOS_LobbySearch_Release_fn)loader->find_symbol("EOS_LobbySearch_Release");
    if (EOS_LobbySearch_Release == nullptr) { missing_count++; }
    EOS_Lobby_Attribute_Release = (EOS_Lobby_Attribute_Release_fn)loader->find_symbol("EOS_Lobby_Attribute_Release");
    if (EOS_Lobby_Attribute_Release == nullptr) { missing_count++; }
    EOS_Lobby_CopyLobbyDetailsHandle = (EOS_Lobby_CopyLobbyDetailsHandle_fn)loader->find_symbol("EOS_Lobby_CopyLobbyDetailsHandle");
    if (EOS_Lobby_CopyLobbyDetailsHandle == nullptr) { missing_count++; }
    EOS_Lobby_CreateLobby = (EOS_Lobby_CreateLobby_fn)loader->find_symbol("EOS_Lobby_CreateLobby");
    if (EOS_Lobby_CreateLobby == nullptr) { missing_count++; }
    EOS_Lobby_CreateLobbySearch = (EOS_Lobby_CreateLobbySearch_fn)loader->find_symbol("EOS_Lobby_CreateLobbySearch");
    if (EOS_Lobby_CreateLobbySearch == nullptr) { missing_count++; }
    EOS_Lobby_JoinLobby = (EOS_Lobby_JoinLobby_fn)loader->find_symbol("EOS_Lobby_JoinLobby");
    if (EOS_Lobby_JoinLobby == nullptr) { missing_count++; }
    EOS_Lobby_JoinLobbyById = (EOS_Lobby_JoinLobbyById_fn)loader->find_symbol("EOS_Lobby_JoinLobbyById");
    if (EOS_Lobby_JoinLobbyById == nullptr) { missing_count++; }
    EOS_Lobby_LeaveLobby = (EOS_Lobby_LeaveLobby_fn)loader->find_symbol("EOS_Lobby_LeaveLobby");
    if (EOS_Lobby_LeaveLobby == nullptr) { missing_count++; }
    EOS_Lobby_SendInvite = (EOS_Lobby_SendInvite_fn)loader->find_symbol("EOS_Lobby_SendInvite");
    if (EOS_Lobby_SendInvite == nullptr) { missing_count++; }
    EOS_Metrics_BeginPlayerSession = (EOS_Metrics_BeginPlayerSession_fn)loader->find_symbol("EOS_Metrics_BeginPlayerSession");
    if (EOS_Metrics_BeginPlayerSession == nullptr) { missing_count++; }
    EOS_Metrics_EndPlayerSession = (EOS_Metrics_EndPlayerSession_fn)loader->find_symbol("EOS_Metrics_EndPlayerSession");
    if (EOS_Metrics_EndPlayerSession == nullptr) { missing_count++; }
    EOS_Mods_CopyModInfo = (EOS_Mods_CopyModInfo_fn)loader->find_symbol("EOS_Mods_CopyModInfo");
    if (EOS_Mods_CopyModInfo == nullptr) { missing_count++; }
    EOS_Mods_EnumerateMods = (EOS_Mods_EnumerateMods_fn)loader->find_symbol("EOS_Mods_EnumerateMods");
    if (EOS_Mods_EnumerateMods == nullptr) { missing_count++; }
    EOS_Mods_ModInfo_Release = (EOS_Mods_ModInfo_Release_fn)loader->find_symbol("EOS_Mods_ModInfo_Release");
    if (EOS_Mods_ModInfo_Release == nullptr) { missing_count++; }
    EOS_P2P_GetNATType = (EOS_P2P_GetNATType_fn)loader->find_symbol("EOS_P2P_GetNATType");
    if (EOS_P2P_GetNATType == nullptr) { missing_count++; }
    EOS_P2P_GetNextReceivedPacketSize = (EOS_P2P_GetNextReceivedPacketSize_fn)loader->find_symbol("EOS_P2P_GetNextReceivedPacketSize");
    if (EOS_P2P_GetNextReceivedPacketSize == nullptr) { missing_count++; }
    EOS_P2P_ReceivePacket = (EOS_P2P_ReceivePacket_fn)loader->find_symbol("EOS_P2P_ReceivePacket");
    if (EOS_P2P_ReceivePacket == nullptr) { missing_count++; }
    EOS_P2P_SendPacket = (EOS_P2P_SendPacket_fn)loader->find_symbol("EOS_P2P_SendPacket");
    if (EOS_P2P_SendPacket == nullptr) { missing_count++; }
    EOS_P2P_SetRelayControl = (EOS_P2P_SetRelayControl_fn)loader->find_symbol("EOS_P2P_SetRelayControl");
    if (EOS_P2P_SetRelayControl == nullptr) { missing_count++; }
    EOS_Platform_Create = (EOS_Platform_Create_fn)loader->find_symbol("EOS_Platform_Create");
    if (EOS_Platform_Create == nullptr) { missing_count++; }
    EOS_Platform_GetAchievementsInterface = (EOS_Platform_GetAchievementsInterface_fn)loader->find_symbol("EOS_Platform_GetAchievementsInterface");
    if (EOS_Platform_GetAchievementsInterface == nullptr) { missing_count++; }
    EOS_Platform_GetAuthInterface = (EOS_Platform_GetAuthInterface_fn)loader->find_symbol("EOS_Platform_GetAuthInterface");
    if (EOS_Platform_GetAuthInterface == nullptr) { missing_count++; }
    EOS_Platform_GetConnectInterface = (EOS_Platform_GetConnectInterface_fn)loader->find_symbol("EOS_Platform_GetConnectInterface");
    if (EOS_Platform_GetConnectInterface == nullptr) { missing_count++; }
    EOS_Platform_GetCustomInvitesInterface = (EOS_Platform_GetCustomInvitesInterface_fn)loader->find_symbol("EOS_Platform_GetCustomInvitesInterface");
    if (EOS_Platform_GetCustomInvitesInterface == nullptr) { missing_count++; }
    EOS_Platform_GetEcomInterface = (EOS_Platform_GetEcomInterface_fn)loader->find_symbol("EOS_Platform_GetEcomInterface");
    if (EOS_Platform_GetEcomInterface == nullptr) { missing_count++; }
    EOS_Platform_GetFriendsInterface = (EOS_Platform_GetFriendsInterface_fn)loader->find_symbol("EOS_Platform_GetFriendsInterface");
    if (EOS_Platform_GetFriendsInterface == nullptr) { missing_count++; }
    EOS_Platform_GetKWSInterface = (EOS_Platform_GetKWSInterface_fn)loader->find_symbol("EOS_Platform_GetKWSInterface");
    if (EOS_Platform_GetKWSInterface == nullptr) { missing_count++; }
    EOS_Platform_GetLeaderboardsInterface = (EOS_Platform_GetLeaderboardsInterface_fn)loader->find_symbol("EOS_Platform_GetLeaderboardsInterface");
    if (EOS_Platform_GetLeaderboardsInterface == nullptr) { missing_count++; }
    EOS_Platform_GetLobbyInterface = (EOS_Platform_GetLobbyInterface_fn)loader->find_symbol("EOS_Platform_GetLobbyInterface");
    if (EOS_Platform_GetLobbyInterface == nullptr) { missing_count++; }
    EOS_Platform_GetMetricsInterface = (EOS_Platform_GetMetricsInterface_fn)loader->find_symbol("EOS_Platform_GetMetricsInterface");
    if (EOS_Platform_GetMetricsInterface == nullptr) { missing_count++; }
    EOS_Platform_GetModsInterface = (EOS_Platform_GetModsInterface_fn)loader->find_symbol("EOS_Platform_GetModsInterface");
    if (EOS_Platform_GetModsInterface == nullptr) { missing_count++; }
    EOS_Platform_GetP2PInterface = (EOS_Platform_GetP2PInterface_fn)loader->find_symbol("EOS_Platform_GetP2PInterface");
    if (EOS_Platform_GetP2PInterface == nullptr) { missing_count++; }
    EOS_Platform_GetPlayerDataStorageInterface = (EOS_Platform_GetPlayerDataStorageInterface_fn)loader->find_symbol("EOS_Platform_GetPlayerDataStorageInterface");
    if (EOS_Platform_GetPlayerDataStorageInterface == nullptr) { missing_count++; }
    EOS_Platform_GetPresenceInterface = (EOS_Platform_GetPresenceInterface_fn)loader->find_symbol("EOS_Platform_GetPresenceInterface");
    if (EOS_Platform_GetPresenceInterface == nullptr) { missing_count++; }
    EOS_Platform_GetProgressionSnapshotInterface = (EOS_Platform_GetProgressionSnapshotInterface_fn)loader->find_symbol("EOS_Platform_GetProgressionSnapshotInterface");
    if (EOS_Platform_GetProgressionSnapshotInterface == nullptr) { missing_count++; }
    EOS_Platform_GetRTCInterface = (EOS_Platform_GetRTCInterface_fn)loader->find_symbol("EOS_Platform_GetRTCInterface");
    if (EOS_Platform_GetRTCInterface == nullptr) { missing_count++; }
    EOS_Platform_GetReportsInterface = (EOS_Platform_GetReportsInterface_fn)loader->find_symbol("EOS_Platform_GetReportsInterface");
    if (EOS_Platform_GetReportsInterface == nullptr) { missing_count++; }
    EOS_Platform_GetSanctionsInterface = (EOS_Platform_GetSanctionsInterface_fn)loader->find_symbol("EOS_Platform_GetSanctionsInterface");
    if (EOS_Platform_GetSanctionsInterface == nullptr) { missing_count++; }
    EOS_Platform_GetSessionsInterface = (EOS_Platform_GetSessionsInterface_fn)loader->find_symbol("EOS_Platform_GetSessionsInterface");
    if (EOS_Platform_GetSessionsInterface == nullptr) { missing_count++; }
    EOS_Platform_GetStatsInterface = (EOS_Platform_GetStatsInterface_fn)loader->find_symbol("EOS_Platform_GetStatsInterface");
    if (EOS_Platform_GetStatsInterface == nullptr) { missing_count++; }
    EOS_Platform_GetTitleStorageInterface = (EOS_Platform_GetTitleStorageInterface_fn)loader->find_symbol("EOS_Platform_GetTitleStorageInterface");
    if (EOS_Platform_GetTitleStorageInterface == nullptr) { missing_count++; }
    EOS_Platform_GetUIInterface = (EOS_Platform_GetUIInterface_fn)loader->find_symbol("EOS_Platform_GetUIInterface");
    if (EOS_Platform_GetUIInterface == nullptr) { missing_count++; }
    EOS_Platform_GetUserInfoInterface = (EOS_Platform_GetUserInfoInterface_fn)loader->find_symbol("EOS_Platform_GetUserInfoInterface");
    if (EOS_Platform_GetUserInfoInterface == nullptr) { missing_count++; }
    EOS_Platform_Release = (EOS_Platform_Release_fn)loader->find_symbol("EOS_Platform_Release");
    if (EOS_Platform_Release == nullptr) { missing_count++; }
    EOS_Platform_SetApplicationStatus = (EOS_Platform_SetApplicationStatus_fn)loader->find_symbol("EOS_Platform_SetApplicationStatus");
    if (EOS_Platform_SetApplicationStatus == nullptr) { missing_count++; }
    EOS_Platform_Tick = (EOS_Platform_Tick_fn)loader->find_symbol("EOS_Platform_Tick");
    if (EOS_Platform_Tick == nullptr) { missing_count++; }
    EOS_PlayerDataStorageFileTransferRequest_Release = (EOS_PlayerDataStorageFileTransferRequest_Release_fn)loader->find_symbol("EOS_PlayerDataStorageFileTransferRequest_Release");
    if (EOS_PlayerDataStorageFileTransferRequest_Release == nullptr) { missing_count++; }
    EOS_PlayerDataStorage_QueryFileList = (EOS_PlayerDataStorage_QueryFileList_fn)loader->find_symbol("EOS_PlayerDataStorage_QueryFileList");
    if (EOS_PlayerDataStorage_QueryFileList == nullptr) { missing_count++; }
    EOS_PlayerDataStorage_ReadFile = (EOS_PlayerDataStorage_ReadFile_fn)loader->find_symbol("EOS_PlayerDataStorage_ReadFile");
    if (EOS_PlayerDataStorage_ReadFile == nullptr) { missing_count++; }
    EOS_PlayerDataStorage_WriteFile = (EOS_PlayerDataStorage_WriteFile_fn)loader->find_symbol("EOS_PlayerDataStorage_WriteFile");
    if (EOS_PlayerDataStorage_WriteFile == nullptr) { missing_count++; }
    EOS_PresenceModification_Release = (EOS_PresenceModification_Release_fn)loader->find_symbol("EOS_PresenceModification_Release");
    if (EOS_PresenceModification_Release == nullptr) { missing_count++; }
    EOS_PresenceModification_SetData = (EOS_PresenceModification_SetData_fn)loader->find_symbol("EOS_PresenceModification_SetData");
    if (EOS_PresenceModification_SetData == nullptr) { missing_count++; }
    EOS_PresenceModification_SetStatus = (EOS_PresenceModification_SetStatus_fn)loader->find_symbol("EOS_PresenceModification_SetStatus");
    if (EOS_PresenceModification_SetStatus == nullptr) { missing_count++; }
    EOS_Presence_CreatePresenceModification = (EOS_Presence_CreatePresenceModification_fn)loader->find_symbol("EOS_Presence_CreatePresenceModification");
    if (EOS_Presence_CreatePresenceModification == nullptr) { missing_count++; }
    EOS_Presence_HasPresence = (EOS_Presence_HasPresence_fn)loader->find_symbol("EOS_Presence_HasPresence");
    if (EOS_Presence_HasPresence == nullptr) { missing_count++; }
    EOS_Presence_QueryPresence = (EOS_Presence_QueryPresence_fn)loader->find_symbol("EOS_Presence_QueryPresence");
    if (EOS_Presence_QueryPresence == nullptr) { missing_count++; }
    EOS_Presence_SetPresence = (EOS_Presence_SetPresence_fn)loader->find_symbol("EOS_Presence_SetPresence");
    if (EOS_Presence_SetPresence == nullptr) { missing_count++; }
    EOS_ProductUserId_FromString = (EOS_ProductUserId_FromString_fn)loader->find_symbol("EOS_ProductUserId_FromString");
    if (EOS_ProductUserId_FromString == nullptr) { missing_count++; }
    EOS_ProductUserId_IsValid = (EOS_ProductUserId_IsValid_fn)loader->find_symbol("EOS_ProductUserId_IsValid");
    if (EOS_ProductUserId_IsValid == nullptr) { missing_count++; }
    EOS_ProductUserId_ToString = (EOS_ProductUserId_ToString_fn)loader->find_symbol("EOS_ProductUserId_ToString");
    if (EOS_ProductUserId_ToString == nullptr) { missing_count++; }
    EOS_ProgressionSnapshot_AddProgression = (EOS_ProgressionSnapshot_AddProgression_fn)loader->find_symbol("EOS_ProgressionSnapshot_AddProgression");
    if (EOS_ProgressionSnapshot_AddProgression == nullptr) { missing_count++; }
    EOS_ProgressionSnapshot_BeginSnapshot = (EOS_ProgressionSnapshot_BeginSnapshot_fn)loader->find_symbol("EOS_ProgressionSnapshot_BeginSnapshot");
    if (EOS_ProgressionSnapshot_BeginSnapshot == nullptr) { missing_count++; }
    EOS_ProgressionSnapshot_EndSnapshot = (EOS_ProgressionSnapshot_EndSnapshot_fn)loader->find_symbol("EOS_ProgressionSnapshot_EndSnapshot");
    if (EOS_ProgressionSnapshot_EndSnapshot == nullptr) { missing_count++; }
    EOS_ProgressionSnapshot_SubmitSnapshot = (EOS_ProgressionSnapshot_SubmitSnapshot_fn)loader->find_symbol("EOS_ProgressionSnapshot_SubmitSnapshot");
    if (EOS_ProgressionSnapshot_SubmitSnapshot == nullptr) { missing_count++; }
    EOS_RTCAudio_UpdateReceivingVolume = (EOS_RTCAudio_UpdateReceivingVolume_fn)loader->find_symbol("EOS_RTCAudio_UpdateReceivingVolume");
    if (EOS_RTCAudio_UpdateReceivingVolume == nullptr) { missing_count++; }
    EOS_RTCAudio_UpdateSendingVolume = (EOS_RTCAudio_UpdateSendingVolume_fn)loader->find_symbol("EOS_RTCAudio_UpdateSendingVolume");
    if (EOS_RTCAudio_UpdateSendingVolume == nullptr) { missing_count++; }
    EOS_RTC_GetAudioInterface = (EOS_RTC_GetAudioInterface_fn)loader->find_symbol("EOS_RTC_GetAudioInterface");
    if (EOS_RTC_GetAudioInterface == nullptr) { missing_count++; }
    EOS_RTC_JoinRoom = (EOS_RTC_JoinRoom_fn)loader->find_symbol("EOS_RTC_JoinRoom");
    if (EOS_RTC_JoinRoom == nullptr) { missing_count++; }
    EOS_RTC_LeaveRoom = (EOS_RTC_LeaveRoom_fn)loader->find_symbol("EOS_RTC_LeaveRoom");
    if (EOS_RTC_LeaveRoom == nullptr) { missing_count++; }
    EOS_Reports_SendPlayerBehaviorReport = (EOS_Reports_SendPlayerBehaviorReport_fn)loader->find_symbol("EOS_Reports_SendPlayerBehaviorReport");
    if (EOS_Reports_SendPlayerBehaviorReport == nullptr) { missing_count++; }
    EOS_Sanctions_CopyPlayerSanctionByIndex = (EOS_Sanctions_CopyPlayerSanctionByIndex_fn)loader->find_symbol("EOS_Sanctions_CopyPlayerSanctionByIndex");
    if (EOS_Sanctions_CopyPlayerSanctionByIndex == nullptr) { missing_count++; }
    EOS_Sanctions_CreatePlayerSanctionAppeal = (EOS_Sanctions_CreatePlayerSanctionAppeal_fn)loader->find_symbol("EOS_Sanctions_CreatePlayerSanctionAppeal");
    if (EOS_Sanctions_CreatePlayerSanctionAppeal == nullptr) { missing_count++; }
    EOS_Sanctions_GetPlayerSanctionCount = (EOS_Sanctions_GetPlayerSanctionCount_fn)loader->find_symbol("EOS_Sanctions_GetPlayerSanctionCount");
    if (EOS_Sanctions_GetPlayerSanctionCount == nullptr) { missing_count++; }
    EOS_Sanctions_PlayerSanction_Release = (EOS_Sanctions_PlayerSanction_Release_fn)loader->find_symbol("EOS_Sanctions_PlayerSanction_Release");
    if (EOS_Sanctions_PlayerSanction_Release == nullptr) { missing_count++; }
    EOS_Sanctions_QueryActivePlayerSanctions = (EOS_Sanctions_QueryActivePlayerSanctions_fn)loader->find_symbol("EOS_Sanctions_QueryActivePlayerSanctions");
    if (EOS_Sanctions_QueryActivePlayerSanctions == nullptr) { missing_count++; }
    EOS_SessionDetails_Attribute_Release = (EOS_SessionDetails_Attribute_Release_fn)loader->find_symbol("EOS_SessionDetails_Attribute_Release");
    if (EOS_SessionDetails_Attribute_Release == nullptr) { missing_count++; }
    EOS_SessionDetails_CopySessionAttributeByIndex = (EOS_SessionDetails_CopySessionAttributeByIndex_fn)loader->find_symbol("EOS_SessionDetails_CopySessionAttributeByIndex");
    if (EOS_SessionDetails_CopySessionAttributeByIndex == nullptr) { missing_count++; }
    EOS_SessionDetails_GetSessionAttributeCount = (EOS_SessionDetails_GetSessionAttributeCount_fn)loader->find_symbol("EOS_SessionDetails_GetSessionAttributeCount");
    if (EOS_SessionDetails_GetSessionAttributeCount == nullptr) { missing_count++; }
    EOS_SessionDetails_Release = (EOS_SessionDetails_Release_fn)loader->find_symbol("EOS_SessionDetails_Release");
    if (EOS_SessionDetails_Release == nullptr) { missing_count++; }
    EOS_SessionModification_AddAttribute = (EOS_SessionModification_AddAttribute_fn)loader->find_symbol("EOS_SessionModification_AddAttribute");
    if (EOS_SessionModification_AddAttribute == nullptr) { missing_count++; }
    EOS_SessionModification_Release = (EOS_SessionModification_Release_fn)loader->find_symbol("EOS_SessionModification_Release");
    if (EOS_SessionModification_Release == nullptr) { missing_count++; }
    EOS_SessionModification_SetPermissionLevel = (EOS_SessionModification_SetPermissionLevel_fn)loader->find_symbol("EOS_SessionModification_SetPermissionLevel");
    if (EOS_SessionModification_SetPermissionLevel == nullptr) { missing_count++; }
    EOS_SessionSearch_CopySearchResultByIndex = (EOS_SessionSearch_CopySearchResultByIndex_fn)loader->find_symbol("EOS_SessionSearch_CopySearchResultByIndex");
    if (EOS_SessionSearch_CopySearchResultByIndex == nullptr) { missing_count++; }
    EOS_SessionSearch_Find = (EOS_SessionSearch_Find_fn)loader->find_symbol("EOS_SessionSearch_Find");
    if (EOS_SessionSearch_Find == nullptr) { missing_count++; }
    EOS_SessionSearch_GetSearchResultCount = (EOS_SessionSearch_GetSearchResultCount_fn)loader->find_symbol("EOS_SessionSearch_GetSearchResultCount");
    if (EOS_SessionSearch_GetSearchResultCount == nullptr) { missing_count++; }
    EOS_SessionSearch_Release = (EOS_SessionSearch_Release_fn)loader->find_symbol("EOS_SessionSearch_Release");
    if (EOS_SessionSearch_Release == nullptr) { missing_count++; }
    EOS_Sessions_CreateSessionModification = (EOS_Sessions_CreateSessionModification_fn)loader->find_symbol("EOS_Sessions_CreateSessionModification");
    if (EOS_Sessions_CreateSessionModification == nullptr) { missing_count++; }
    EOS_Sessions_CreateSessionSearch = (EOS_Sessions_CreateSessionSearch_fn)loader->find_symbol("EOS_Sessions_CreateSessionSearch");
    if (EOS_Sessions_CreateSessionSearch == nullptr) { missing_count++; }
    EOS_Sessions_DestroySession = (EOS_Sessions_DestroySession_fn)loader->find_symbol("EOS_Sessions_DestroySession");
    if (EOS_Sessions_DestroySession == nullptr) { missing_count++; }
    EOS_Sessions_JoinSession = (EOS_Sessions_JoinSession_fn)loader->find_symbol("EOS_Sessions_JoinSession");
    if (EOS_Sessions_JoinSession == nullptr) { missing_count++; }
    EOS_Sessions_QueryInvites = (EOS_Sessions_QueryInvites_fn)loader->find_symbol("EOS_Sessions_QueryInvites");
    if (EOS_Sessions_QueryInvites == nullptr) { missing_count++; }
    EOS_Sessions_RejectInvite = (EOS_Sessions_RejectInvite_fn)loader->find_symbol("EOS_Sessions_RejectInvite");
    if (EOS_Sessions_RejectInvite == nullptr) { missing_count++; }
    EOS_Sessions_SendInvite = (EOS_Sessions_SendInvite_fn)loader->find_symbol("EOS_Sessions_SendInvite");
    if (EOS_Sessions_SendInvite == nullptr) { missing_count++; }
    EOS_Sessions_UpdateSession = (EOS_Sessions_UpdateSession_fn)loader->find_symbol("EOS_Sessions_UpdateSession");
    if (EOS_Sessions_UpdateSession == nullptr) { missing_count++; }
    EOS_Sessions_UpdateSessionModification = (EOS_Sessions_UpdateSessionModification_fn)loader->find_symbol("EOS_Sessions_UpdateSessionModification");
    if (EOS_Sessions_UpdateSessionModification == nullptr) { missing_count++; }
    EOS_Shutdown = (EOS_Shutdown_fn)loader->find_symbol("EOS_Shutdown");
    if (EOS_Shutdown == nullptr) { missing_count++; }
    EOS_Stats_IngestStat = (EOS_Stats_IngestStat_fn)loader->find_symbol("EOS_Stats_IngestStat");
    if (EOS_Stats_IngestStat == nullptr) { missing_count++; }
    EOS_Stats_QueryStats = (EOS_Stats_QueryStats_fn)loader->find_symbol("EOS_Stats_QueryStats");
    if (EOS_Stats_QueryStats == nullptr) { missing_count++; }
    EOS_TitleStorageFileTransferRequest_Release = (EOS_TitleStorageFileTransferRequest_Release_fn)loader->find_symbol("EOS_TitleStorageFileTransferRequest_Release");
    if (EOS_TitleStorageFileTransferRequest_Release == nullptr) { missing_count++; }
    EOS_TitleStorage_ReadFile = (EOS_TitleStorage_ReadFile_fn)loader->find_symbol("EOS_TitleStorage_ReadFile");
    if (EOS_TitleStorage_ReadFile == nullptr) { missing_count++; }
    EOS_UI_GetFriendsVisible = (EOS_UI_GetFriendsVisible_fn)loader->find_symbol("EOS_UI_GetFriendsVisible");
    if (EOS_UI_GetFriendsVisible == nullptr) { missing_count++; }
    EOS_UI_HideFriends = (EOS_UI_HideFriends_fn)loader->find_symbol("EOS_UI_HideFriends");
    if (EOS_UI_HideFriends == nullptr) { missing_count++; }
    EOS_UI_IsSocialOverlayPaused = (EOS_UI_IsSocialOverlayPaused_fn)loader->find_symbol("EOS_UI_IsSocialOverlayPaused");
    if (EOS_UI_IsSocialOverlayPaused == nullptr) { missing_count++; }
    EOS_UI_PauseSocialOverlay = (EOS_UI_PauseSocialOverlay_fn)loader->find_symbol("EOS_UI_PauseSocialOverlay");
    if (EOS_UI_PauseSocialOverlay == nullptr) { missing_count++; }
    EOS_UI_SetDisplayPreference = (EOS_UI_SetDisplayPreference_fn)loader->find_symbol("EOS_UI_SetDisplayPreference");
    if (EOS_UI_SetDisplayPreference == nullptr) { missing_count++; }
    EOS_UI_ShowFriends = (EOS_UI_ShowFriends_fn)loader->find_symbol("EOS_UI_ShowFriends");
    if (EOS_UI_ShowFriends == nullptr) { missing_count++; }
    EOS_UserInfo_CopyUserInfo = (EOS_UserInfo_CopyUserInfo_fn)loader->find_symbol("EOS_UserInfo_CopyUserInfo");
    if (EOS_UserInfo_CopyUserInfo == nullptr) { missing_count++; }
    EOS_UserInfo_QueryUserInfo = (EOS_UserInfo_QueryUserInfo_fn)loader->find_symbol("EOS_UserInfo_QueryUserInfo");
    if (EOS_UserInfo_QueryUserInfo == nullptr) { missing_count++; }
    EOS_UserInfo_Release = (EOS_UserInfo_Release_fn)loader->find_symbol("EOS_UserInfo_Release");
    if (EOS_UserInfo_Release == nullptr) { missing_count++; }
    return missing_count == 0;
}

}

#endif
