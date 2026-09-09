#pragma once

#if YUGEN_EOS_HAS_SDK

#include "eos_base.h"
#include "eos_sdk.h"
#include <godot_cpp/classes/ref_counted.hpp>

namespace yugen {

class EOSLoader;

struct EOSApiTable {
    typedef void (EOS_CALL *EOS_Achievements_QueryPlayerAchievements_fn)(EOS_HAchievements Handle, const EOS_Achievements_QueryPlayerAchievementsOptions* Options, void* ClientData, const EOS_Achievements_OnQueryPlayerAchievementsCompleteCallback CompletionDelegate);
    typedef void (EOS_CALL *EOS_Achievements_UnlockAchievements_fn)(EOS_HAchievements Handle, const EOS_Achievements_UnlockAchievementsOptions* Options, void* ClientData, const EOS_Achievements_OnUnlockAchievementsCompleteCallback CompletionDelegate);
    typedef int32_t (EOS_CALL *EOS_Auth_GetLoggedInAccountsCount_fn)(EOS_HAuth Handle);
    typedef void (EOS_CALL *EOS_Auth_Login_fn)(EOS_HAuth Handle, const EOS_Auth_LoginOptions* Options, void* ClientData, const EOS_Auth_OnLoginCallback CompletionDelegate);
    typedef void (EOS_CALL *EOS_Auth_Logout_fn)(EOS_HAuth Handle, const EOS_Auth_LogoutOptions* Options, void* ClientData, const EOS_Auth_OnLogoutCallback CompletionDelegate);
    typedef int32_t (EOS_CALL *EOS_Connect_GetLoggedInUsersCount_fn)(EOS_HConnect Handle);
    typedef void (EOS_CALL *EOS_Connect_Login_fn)(EOS_HConnect Handle, const EOS_Connect_LoginOptions* Options, void* ClientData, const EOS_Connect_OnLoginCallback CompletionDelegate);
    typedef void (EOS_CALL *EOS_Connect_Logout_fn)(EOS_HConnect Handle, const EOS_Connect_LogoutOptions* Options, void* ClientData, const EOS_Connect_OnLogoutCallback CompletionDelegate);
    typedef EOS_Bool (EOS_CALL *EOS_EResult_IsOperationComplete_fn)(EOS_EResult Result);
    typedef void (EOS_CALL *EOS_Ecom_Checkout_fn)(EOS_HEcom Handle, const EOS_Ecom_CheckoutOptions* Options, void* ClientData, const EOS_Ecom_OnCheckoutCallback CompletionDelegate);
    typedef void (EOS_CALL *EOS_Ecom_QueryEntitlements_fn)(EOS_HEcom Handle, const EOS_Ecom_QueryEntitlementsOptions* Options, void* ClientData, const EOS_Ecom_OnQueryEntitlementsCallback CompletionDelegate);
    typedef void (EOS_CALL *EOS_Ecom_QueryOffers_fn)(EOS_HEcom Handle, const EOS_Ecom_QueryOffersOptions* Options, void* ClientData, const EOS_Ecom_OnQueryOffersCallback CompletionDelegate);
    typedef EOS_EpicAccountId (EOS_CALL *EOS_EpicAccountId_FromString_fn)(const char* AccountIdString);
    typedef EOS_Bool (EOS_CALL *EOS_EpicAccountId_IsValid_fn)(EOS_EpicAccountId AccountId);
    typedef EOS_EResult (EOS_CALL *EOS_EpicAccountId_ToString_fn)(EOS_EpicAccountId AccountId, char* OutBuffer, int32_t* InOutBufferLength);
    typedef int32_t (EOS_CALL *EOS_Friends_GetFriendsCount_fn)(EOS_HFriends Handle, const EOS_Friends_GetFriendsCountOptions* Options);
    typedef void (EOS_CALL *EOS_Friends_QueryFriends_fn)(EOS_HFriends Handle, const EOS_Friends_QueryFriendsOptions* Options, void* ClientData, const EOS_Friends_OnQueryFriendsCallback CompletionDelegate);
    typedef void (EOS_CALL *EOS_Friends_SendInvite_fn)(EOS_HFriends Handle, const EOS_Friends_SendInviteOptions* Options, void* ClientData, const EOS_Friends_OnSendInviteCallback CompletionDelegate);
    typedef const char* (EOS_CALL *EOS_GetVersion_fn)(void);
    typedef EOS_EResult (EOS_CALL *EOS_Initialize_fn)(const EOS_InitializeOptions* Options);
    typedef void (EOS_CALL *EOS_Leaderboards_QueryLeaderboardDefinitions_fn)(EOS_HLeaderboards Handle, const EOS_Leaderboards_QueryLeaderboardDefinitionsOptions* Options, void* ClientData, const EOS_Leaderboards_OnQueryLeaderboardDefinitionsCompleteCallback CompletionDelegate);
    typedef void (EOS_CALL *EOS_Leaderboards_QueryLeaderboardRanks_fn)(EOS_HLeaderboards Handle, const EOS_Leaderboards_QueryLeaderboardRanksOptions* Options, void* ClientData, const EOS_Leaderboards_OnQueryLeaderboardRanksCompleteCallback CompletionDelegate);
    typedef void (EOS_CALL *EOS_Leaderboards_QueryLeaderboardUserScores_fn)(EOS_HLeaderboards Handle, const EOS_Leaderboards_QueryLeaderboardUserScoresOptions* Options, void* ClientData, const EOS_Leaderboards_OnQueryLeaderboardUserScoresCompleteCallback CompletionDelegate);
    typedef void (EOS_CALL *EOS_LobbySearch_Release_fn)(EOS_HLobbySearch LobbySearchHandle);
    typedef void (EOS_CALL *EOS_Lobby_CreateLobby_fn)(EOS_HLobby Handle, const EOS_Lobby_CreateLobbyOptions* Options, void* ClientData, const EOS_Lobby_OnCreateLobbyCallback CompletionDelegate);
    typedef void (EOS_CALL *EOS_Lobby_JoinLobbyById_fn)(EOS_HLobby Handle, const EOS_Lobby_JoinLobbyByIdOptions* Options, void* ClientData, const EOS_Lobby_OnJoinLobbyByIdCallback CompletionDelegate);
    typedef void (EOS_CALL *EOS_Lobby_LeaveLobby_fn)(EOS_HLobby Handle, const EOS_Lobby_LeaveLobbyOptions* Options, void* ClientData, const EOS_Lobby_OnLeaveLobbyCallback CompletionDelegate);
    typedef void (EOS_CALL *EOS_Lobby_SendInvite_fn)(EOS_HLobby Handle, const EOS_Lobby_SendInviteOptions* Options, void* ClientData, const EOS_Lobby_OnSendInviteCallback CompletionDelegate);
    typedef EOS_EResult (EOS_CALL *EOS_Metrics_BeginPlayerSession_fn)(EOS_HMetrics Handle, const EOS_Metrics_BeginPlayerSessionOptions* Options);
    typedef EOS_EResult (EOS_CALL *EOS_P2P_GetNATType_fn)(EOS_HP2P Handle, const EOS_P2P_GetNATTypeOptions* Options, EOS_ENATType* OutNATType);
    typedef EOS_EResult (EOS_CALL *EOS_P2P_GetNextReceivedPacketSize_fn)(EOS_HP2P Handle, const EOS_P2P_GetNextReceivedPacketSizeOptions* Options, uint32_t* OutPacketSizeBytes);
    typedef EOS_EResult (EOS_CALL *EOS_P2P_ReceivePacket_fn)(EOS_HP2P Handle, const EOS_P2P_ReceivePacketOptions* Options, EOS_ProductUserId* OutPeerId, EOS_P2P_SocketId* OutSocketId, uint8_t* OutChannel, void* OutData, uint32_t* OutBytesWritten);
    typedef EOS_EResult (EOS_CALL *EOS_P2P_SendPacket_fn)(EOS_HP2P Handle, const EOS_P2P_SendPacketOptions* Options);
    typedef EOS_EResult (EOS_CALL *EOS_P2P_SetRelayControl_fn)(EOS_HP2P Handle, const EOS_P2P_SetRelayControlOptions* Options);
    typedef EOS_HAchievements (EOS_CALL *EOS_Platform_GetAchievementsInterface_fn)(EOS_HPlatform Handle);
    typedef EOS_HAuth (EOS_CALL *EOS_Platform_GetAuthInterface_fn)(EOS_HPlatform Handle);
    typedef EOS_HConnect (EOS_CALL *EOS_Platform_GetConnectInterface_fn)(EOS_HPlatform Handle);
    typedef EOS_HEcom (EOS_CALL *EOS_Platform_GetEcomInterface_fn)(EOS_HPlatform Handle);
    typedef EOS_HFriends (EOS_CALL *EOS_Platform_GetFriendsInterface_fn)(EOS_HPlatform Handle);
    typedef EOS_HLeaderboards (EOS_CALL *EOS_Platform_GetLeaderboardsInterface_fn)(EOS_HPlatform Handle);
    typedef EOS_HLobby (EOS_CALL *EOS_Platform_GetLobbyInterface_fn)(EOS_HPlatform Handle);
    typedef EOS_HMetrics (EOS_CALL *EOS_Platform_GetMetricsInterface_fn)(EOS_HPlatform Handle);
    typedef EOS_HP2P (EOS_CALL *EOS_Platform_GetP2PInterface_fn)(EOS_HPlatform Handle);
    typedef EOS_HPlayerDataStorage (EOS_CALL *EOS_Platform_GetPlayerDataStorageInterface_fn)(EOS_HPlatform Handle);
    typedef EOS_HPresence (EOS_CALL *EOS_Platform_GetPresenceInterface_fn)(EOS_HPlatform Handle);
    typedef EOS_HRTC (EOS_CALL *EOS_Platform_GetRTCInterface_fn)(EOS_HPlatform Handle);
    typedef EOS_HReports (EOS_CALL *EOS_Platform_GetReportsInterface_fn)(EOS_HPlatform Handle);
    typedef EOS_HSanctions (EOS_CALL *EOS_Platform_GetSanctionsInterface_fn)(EOS_HPlatform Handle);
    typedef EOS_HSessions (EOS_CALL *EOS_Platform_GetSessionsInterface_fn)(EOS_HPlatform Handle);
    typedef EOS_HStats (EOS_CALL *EOS_Platform_GetStatsInterface_fn)(EOS_HPlatform Handle);
    typedef EOS_HUI (EOS_CALL *EOS_Platform_GetUIInterface_fn)(EOS_HPlatform Handle);
    typedef EOS_HUserInfo (EOS_CALL *EOS_Platform_GetUserInfoInterface_fn)(EOS_HPlatform Handle);
    typedef EOS_EResult (EOS_CALL *EOS_Platform_SetApplicationStatus_fn)(EOS_HPlatform Handle, const EOS_EApplicationStatus NewStatus);
    typedef void (EOS_CALL *EOS_Platform_Tick_fn)(EOS_HPlatform Handle);
    typedef void (EOS_CALL *EOS_PlayerDataStorage_QueryFileList_fn)(EOS_HPlayerDataStorage Handle, const EOS_PlayerDataStorage_QueryFileListOptions* QueryFileListOptions, void* ClientData, const EOS_PlayerDataStorage_OnQueryFileListCompleteCallback CompletionCallback);
    typedef void (EOS_CALL *EOS_PresenceModification_Release_fn)(EOS_HPresenceModification PresenceModificationHandle);
    typedef EOS_EResult (EOS_CALL *EOS_PresenceModification_SetData_fn)(EOS_HPresenceModification Handle, const EOS_PresenceModification_SetDataOptions* Options);
    typedef EOS_EResult (EOS_CALL *EOS_PresenceModification_SetStatus_fn)(EOS_HPresenceModification Handle, const EOS_PresenceModification_SetStatusOptions* Options);
    typedef EOS_EResult (EOS_CALL *EOS_Presence_CreatePresenceModification_fn)(EOS_HPresence Handle, const EOS_Presence_CreatePresenceModificationOptions* Options, EOS_HPresenceModification* OutPresenceModificationHandle);
    typedef EOS_Bool (EOS_CALL *EOS_Presence_HasPresence_fn)(EOS_HPresence Handle, const EOS_Presence_HasPresenceOptions* Options);
    typedef void (EOS_CALL *EOS_Presence_QueryPresence_fn)(EOS_HPresence Handle, const EOS_Presence_QueryPresenceOptions* Options, void* ClientData, const EOS_Presence_OnQueryPresenceCompleteCallback CompletionDelegate);
    typedef void (EOS_CALL *EOS_Presence_SetPresence_fn)(EOS_HPresence Handle, const EOS_Presence_SetPresenceOptions* Options, void* ClientData, const EOS_Presence_SetPresenceCompleteCallback CompletionDelegate);
    typedef EOS_ProductUserId (EOS_CALL *EOS_ProductUserId_FromString_fn)(const char* ProductUserIdString);
    typedef EOS_Bool (EOS_CALL *EOS_ProductUserId_IsValid_fn)(EOS_ProductUserId AccountId);
    typedef EOS_EResult (EOS_CALL *EOS_ProductUserId_ToString_fn)(EOS_ProductUserId AccountId, char* OutBuffer, int32_t* InOutBufferLength);
    typedef void (EOS_CALL *EOS_RTC_JoinRoom_fn)(EOS_HRTC Handle, const EOS_RTC_JoinRoomOptions* Options, void* ClientData, const EOS_RTC_OnJoinRoomCallback CompletionDelegate);
    typedef void (EOS_CALL *EOS_RTC_LeaveRoom_fn)(EOS_HRTC Handle, const EOS_RTC_LeaveRoomOptions* Options, void* ClientData, const EOS_RTC_OnLeaveRoomCallback CompletionDelegate);
    typedef void (EOS_CALL *EOS_Reports_SendPlayerBehaviorReport_fn)(EOS_HReports Handle, const EOS_Reports_SendPlayerBehaviorReportOptions* Options, void* ClientData, const EOS_Reports_OnSendPlayerBehaviorReportCompleteCallback CompletionDelegate);
    typedef void (EOS_CALL *EOS_Sanctions_QueryActivePlayerSanctions_fn)(EOS_HSanctions Handle, const EOS_Sanctions_QueryActivePlayerSanctionsOptions* Options, void* ClientData, const EOS_Sanctions_OnQueryActivePlayerSanctionsCallback CompletionDelegate);
    typedef void (EOS_CALL *EOS_SessionDetails_Release_fn)(EOS_HSessionDetails SessionHandle);
    typedef void (EOS_CALL *EOS_SessionModification_Release_fn)(EOS_HSessionModification SessionModificationHandle);
    typedef EOS_EResult (EOS_CALL *EOS_SessionModification_SetPermissionLevel_fn)(EOS_HSessionModification Handle, const EOS_SessionModification_SetPermissionLevelOptions* Options);
    typedef EOS_EResult (EOS_CALL *EOS_SessionSearch_CopySearchResultByIndex_fn)(EOS_HSessionSearch Handle, const EOS_SessionSearch_CopySearchResultByIndexOptions* Options, EOS_HSessionDetails* OutSessionHandle);
    typedef void (EOS_CALL *EOS_SessionSearch_Find_fn)(EOS_HSessionSearch Handle, const EOS_SessionSearch_FindOptions* Options, void* ClientData, const EOS_SessionSearch_OnFindCallback CompletionDelegate);
    typedef uint32_t (EOS_CALL *EOS_SessionSearch_GetSearchResultCount_fn)(EOS_HSessionSearch Handle, const EOS_SessionSearch_GetSearchResultCountOptions* Options);
    typedef void (EOS_CALL *EOS_SessionSearch_Release_fn)(EOS_HSessionSearch SessionSearchHandle);
    typedef EOS_EResult (EOS_CALL *EOS_Sessions_CreateSessionModification_fn)(EOS_HSessions Handle, const EOS_Sessions_CreateSessionModificationOptions* Options, EOS_HSessionModification* OutSessionModificationHandle);
    typedef EOS_EResult (EOS_CALL *EOS_Sessions_CreateSessionSearch_fn)(EOS_HSessions Handle, const EOS_Sessions_CreateSessionSearchOptions* Options, EOS_HSessionSearch* OutSessionSearchHandle);
    typedef void (EOS_CALL *EOS_Sessions_DestroySession_fn)(EOS_HSessions Handle, const EOS_Sessions_DestroySessionOptions* Options, void* ClientData, const EOS_Sessions_OnDestroySessionCallback CompletionDelegate);
    typedef void (EOS_CALL *EOS_Sessions_JoinSession_fn)(EOS_HSessions Handle, const EOS_Sessions_JoinSessionOptions* Options, void* ClientData, const EOS_Sessions_OnJoinSessionCallback CompletionDelegate);
    typedef void (EOS_CALL *EOS_Sessions_UpdateSession_fn)(EOS_HSessions Handle, const EOS_Sessions_UpdateSessionOptions* Options, void* ClientData, const EOS_Sessions_OnUpdateSessionCallback CompletionDelegate);
    typedef EOS_EResult (EOS_CALL *EOS_Shutdown_fn)();
    typedef void (EOS_CALL *EOS_Stats_IngestStat_fn)(EOS_HStats Handle, const EOS_Stats_IngestStatOptions* Options, void* ClientData, const EOS_Stats_OnIngestStatCompleteCallback CompletionDelegate);
    typedef void (EOS_CALL *EOS_Stats_QueryStats_fn)(EOS_HStats Handle, const EOS_Stats_QueryStatsOptions* Options, void* ClientData, const EOS_Stats_OnQueryStatsCompleteCallback CompletionDelegate);
    typedef void (EOS_CALL *EOS_UI_HideFriends_fn)(EOS_HUI Handle, const EOS_UI_HideFriendsOptions* Options, void* ClientData, const EOS_UI_OnHideFriendsCallback CompletionDelegate);
    typedef void (EOS_CALL *EOS_UI_ShowFriends_fn)(EOS_HUI Handle, const EOS_UI_ShowFriendsOptions* Options, void* ClientData, const EOS_UI_OnShowFriendsCallback CompletionDelegate);
    typedef EOS_EResult (EOS_CALL *EOS_UserInfo_CopyUserInfo_fn)(EOS_HUserInfo Handle, const EOS_UserInfo_CopyUserInfoOptions* Options, EOS_UserInfo ** OutUserInfo);
    typedef void (EOS_CALL *EOS_UserInfo_QueryUserInfo_fn)(EOS_HUserInfo Handle, const EOS_UserInfo_QueryUserInfoOptions* Options, void* ClientData, const EOS_UserInfo_OnQueryUserInfoCallback CompletionDelegate);
    typedef void (EOS_CALL *EOS_UserInfo_Release_fn)(EOS_UserInfo* UserInfo);

    EOS_Achievements_QueryPlayerAchievements_fn EOS_Achievements_QueryPlayerAchievements = nullptr;
    EOS_Achievements_UnlockAchievements_fn EOS_Achievements_UnlockAchievements = nullptr;
    EOS_Auth_GetLoggedInAccountsCount_fn EOS_Auth_GetLoggedInAccountsCount = nullptr;
    EOS_Auth_Login_fn EOS_Auth_Login = nullptr;
    EOS_Auth_Logout_fn EOS_Auth_Logout = nullptr;
    EOS_Connect_GetLoggedInUsersCount_fn EOS_Connect_GetLoggedInUsersCount = nullptr;
    EOS_Connect_Login_fn EOS_Connect_Login = nullptr;
    EOS_Connect_Logout_fn EOS_Connect_Logout = nullptr;
    EOS_EResult_IsOperationComplete_fn EOS_EResult_IsOperationComplete = nullptr;
    EOS_Ecom_Checkout_fn EOS_Ecom_Checkout = nullptr;
    EOS_Ecom_QueryEntitlements_fn EOS_Ecom_QueryEntitlements = nullptr;
    EOS_Ecom_QueryOffers_fn EOS_Ecom_QueryOffers = nullptr;
    EOS_EpicAccountId_FromString_fn EOS_EpicAccountId_FromString = nullptr;
    EOS_EpicAccountId_IsValid_fn EOS_EpicAccountId_IsValid = nullptr;
    EOS_EpicAccountId_ToString_fn EOS_EpicAccountId_ToString = nullptr;
    EOS_Friends_GetFriendsCount_fn EOS_Friends_GetFriendsCount = nullptr;
    EOS_Friends_QueryFriends_fn EOS_Friends_QueryFriends = nullptr;
    EOS_Friends_SendInvite_fn EOS_Friends_SendInvite = nullptr;
    EOS_GetVersion_fn EOS_GetVersion = nullptr;
    EOS_Initialize_fn EOS_Initialize = nullptr;
    EOS_Leaderboards_QueryLeaderboardDefinitions_fn EOS_Leaderboards_QueryLeaderboardDefinitions = nullptr;
    EOS_Leaderboards_QueryLeaderboardRanks_fn EOS_Leaderboards_QueryLeaderboardRanks = nullptr;
    EOS_Leaderboards_QueryLeaderboardUserScores_fn EOS_Leaderboards_QueryLeaderboardUserScores = nullptr;
    EOS_LobbySearch_Release_fn EOS_LobbySearch_Release = nullptr;
    EOS_Lobby_CreateLobby_fn EOS_Lobby_CreateLobby = nullptr;
    EOS_Lobby_JoinLobbyById_fn EOS_Lobby_JoinLobbyById = nullptr;
    EOS_Lobby_LeaveLobby_fn EOS_Lobby_LeaveLobby = nullptr;
    EOS_Lobby_SendInvite_fn EOS_Lobby_SendInvite = nullptr;
    EOS_Metrics_BeginPlayerSession_fn EOS_Metrics_BeginPlayerSession = nullptr;
    EOS_P2P_GetNATType_fn EOS_P2P_GetNATType = nullptr;
    EOS_P2P_GetNextReceivedPacketSize_fn EOS_P2P_GetNextReceivedPacketSize = nullptr;
    EOS_P2P_ReceivePacket_fn EOS_P2P_ReceivePacket = nullptr;
    EOS_P2P_SendPacket_fn EOS_P2P_SendPacket = nullptr;
    EOS_P2P_SetRelayControl_fn EOS_P2P_SetRelayControl = nullptr;
    EOS_Platform_GetAchievementsInterface_fn EOS_Platform_GetAchievementsInterface = nullptr;
    EOS_Platform_GetAuthInterface_fn EOS_Platform_GetAuthInterface = nullptr;
    EOS_Platform_GetConnectInterface_fn EOS_Platform_GetConnectInterface = nullptr;
    EOS_Platform_GetEcomInterface_fn EOS_Platform_GetEcomInterface = nullptr;
    EOS_Platform_GetFriendsInterface_fn EOS_Platform_GetFriendsInterface = nullptr;
    EOS_Platform_GetLeaderboardsInterface_fn EOS_Platform_GetLeaderboardsInterface = nullptr;
    EOS_Platform_GetLobbyInterface_fn EOS_Platform_GetLobbyInterface = nullptr;
    EOS_Platform_GetMetricsInterface_fn EOS_Platform_GetMetricsInterface = nullptr;
    EOS_Platform_GetP2PInterface_fn EOS_Platform_GetP2PInterface = nullptr;
    EOS_Platform_GetPlayerDataStorageInterface_fn EOS_Platform_GetPlayerDataStorageInterface = nullptr;
    EOS_Platform_GetPresenceInterface_fn EOS_Platform_GetPresenceInterface = nullptr;
    EOS_Platform_GetRTCInterface_fn EOS_Platform_GetRTCInterface = nullptr;
    EOS_Platform_GetReportsInterface_fn EOS_Platform_GetReportsInterface = nullptr;
    EOS_Platform_GetSanctionsInterface_fn EOS_Platform_GetSanctionsInterface = nullptr;
    EOS_Platform_GetSessionsInterface_fn EOS_Platform_GetSessionsInterface = nullptr;
    EOS_Platform_GetStatsInterface_fn EOS_Platform_GetStatsInterface = nullptr;
    EOS_Platform_GetUIInterface_fn EOS_Platform_GetUIInterface = nullptr;
    EOS_Platform_GetUserInfoInterface_fn EOS_Platform_GetUserInfoInterface = nullptr;
    EOS_Platform_SetApplicationStatus_fn EOS_Platform_SetApplicationStatus = nullptr;
    EOS_Platform_Tick_fn EOS_Platform_Tick = nullptr;
    EOS_PlayerDataStorage_QueryFileList_fn EOS_PlayerDataStorage_QueryFileList = nullptr;
    EOS_PresenceModification_Release_fn EOS_PresenceModification_Release = nullptr;
    EOS_PresenceModification_SetData_fn EOS_PresenceModification_SetData = nullptr;
    EOS_PresenceModification_SetStatus_fn EOS_PresenceModification_SetStatus = nullptr;
    EOS_Presence_CreatePresenceModification_fn EOS_Presence_CreatePresenceModification = nullptr;
    EOS_Presence_HasPresence_fn EOS_Presence_HasPresence = nullptr;
    EOS_Presence_QueryPresence_fn EOS_Presence_QueryPresence = nullptr;
    EOS_Presence_SetPresence_fn EOS_Presence_SetPresence = nullptr;
    EOS_ProductUserId_FromString_fn EOS_ProductUserId_FromString = nullptr;
    EOS_ProductUserId_IsValid_fn EOS_ProductUserId_IsValid = nullptr;
    EOS_ProductUserId_ToString_fn EOS_ProductUserId_ToString = nullptr;
    EOS_RTC_JoinRoom_fn EOS_RTC_JoinRoom = nullptr;
    EOS_RTC_LeaveRoom_fn EOS_RTC_LeaveRoom = nullptr;
    EOS_Reports_SendPlayerBehaviorReport_fn EOS_Reports_SendPlayerBehaviorReport = nullptr;
    EOS_Sanctions_QueryActivePlayerSanctions_fn EOS_Sanctions_QueryActivePlayerSanctions = nullptr;
    EOS_SessionDetails_Release_fn EOS_SessionDetails_Release = nullptr;
    EOS_SessionModification_Release_fn EOS_SessionModification_Release = nullptr;
    EOS_SessionModification_SetPermissionLevel_fn EOS_SessionModification_SetPermissionLevel = nullptr;
    EOS_SessionSearch_CopySearchResultByIndex_fn EOS_SessionSearch_CopySearchResultByIndex = nullptr;
    EOS_SessionSearch_Find_fn EOS_SessionSearch_Find = nullptr;
    EOS_SessionSearch_GetSearchResultCount_fn EOS_SessionSearch_GetSearchResultCount = nullptr;
    EOS_SessionSearch_Release_fn EOS_SessionSearch_Release = nullptr;
    EOS_Sessions_CreateSessionModification_fn EOS_Sessions_CreateSessionModification = nullptr;
    EOS_Sessions_CreateSessionSearch_fn EOS_Sessions_CreateSessionSearch = nullptr;
    EOS_Sessions_DestroySession_fn EOS_Sessions_DestroySession = nullptr;
    EOS_Sessions_JoinSession_fn EOS_Sessions_JoinSession = nullptr;
    EOS_Sessions_UpdateSession_fn EOS_Sessions_UpdateSession = nullptr;
    EOS_Shutdown_fn EOS_Shutdown = nullptr;
    EOS_Stats_IngestStat_fn EOS_Stats_IngestStat = nullptr;
    EOS_Stats_QueryStats_fn EOS_Stats_QueryStats = nullptr;
    EOS_UI_HideFriends_fn EOS_UI_HideFriends = nullptr;
    EOS_UI_ShowFriends_fn EOS_UI_ShowFriends = nullptr;
    EOS_UserInfo_CopyUserInfo_fn EOS_UserInfo_CopyUserInfo = nullptr;
    EOS_UserInfo_QueryUserInfo_fn EOS_UserInfo_QueryUserInfo = nullptr;
    EOS_UserInfo_Release_fn EOS_UserInfo_Release = nullptr;

    int missing_count = 0;
    bool load(EOSLoader *loader);
};

}

#else

namespace yugen {

struct EOSApiTable {};

}

#endif
