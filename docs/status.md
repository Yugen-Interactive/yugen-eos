Interface status, SDK 1.19.1, wrapper 0.1.0

States: implemented, partial, not-supported, requires-external,
requires-backend, unavailable-on-platform.

| interface | state | notes |
| platform init and shutdown | implemented | EOS_Initialize, EOS_Shutdown, dynamic library loading |
| platform tick | implemented | EOS_Platform_Tick driven by EOS autoload timer |
| logging bridge | implemented | EOS log callback to Godot, secret sanitizer |
| auth login and logout | implemented | credential types passed through, continuance reported |
| auth link and verify tokens | partial | verify helper local only, link pending |
| connect login and logout | implemented | device id and external credentials supported |
| connect create user | partial | continuance token handoff pending |
| user info query and cache read | implemented | QueryUserInfo, CopyUserInfo |
| friends query, count, invite | implemented | accept and reject pending |
| presence set, query, has | implemented | modification flow with status and details record |
| sessions create, search, join, destroy | implemented | join uses session handle registry from search |
| sessions invites and attributes | partial | attribute helpers pending |
| lobbies create, join by id, leave, invite | implemented | details-handle join pending, search pending |
| p2p send and receive | implemented | reliability selectable, NAT type cached read |
| p2p relay control | implemented | global setting, no per-user state |
| stats query and ingest | implemented | leaderboard scores flow through stats ingest |
| achievements unlock and query | implemented | definitions listing pending |
| leaderboards definitions, ranks, user scores | implemented | read only by EOS design, writes go through stats |
| player data storage file list | implemented | QueryFileList only |
| player data storage read and write | partial | FileTransferRequest chunk pipeline pending |
| title storage read | partial | FileTransferRequest chunk pipeline pending |
| ecom query offers, entitlements, checkout | implemented | single-offer checkout entry |
| ui show and hide friends | implemented | async overlay calls |
| social overlay pause and preferences | partial | getters pending |
| metrics begin session | implemented | Epic account type, server fields optional |
| reports send behavior report | implemented | category passed as int, see EOS_EPlayerReportsCategory |
| sanctions query active | implemented | appeal flow pending |
| rtc join and leave room | implemented | voice options default, audio device control pending |
| rtc admin | not-supported | server-side Nexus allocation, needs backend |
| anticheat client and server | partial | availability reported, session registration needs game-server setup |
| mods, kws, custom invites, integrated platform | partial | availability reported, per-game flows pending |
| progression snapshot | partial | availability reported, snapshot upload pending |
| application lifecycle status | implemented | SetApplicationStatus passthrough |

EOS imposes its own limits: Ecom and overlay need Dev Portal
configuration, sanctions and RTC admin need backend roles, AntiCheat
needs a registered game-server build.
