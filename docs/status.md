Interface status, SDK 1.19.1, wrapper 0.2.0

States: implemented, partial, not-supported, requires-external,
requires-backend, unavailable-on-platform.

| interface | state | notes |
| platform init, create, shutdown, release | implemented | EOS_Initialize, EOS_Platform_Create with product, sandbox, deployment, client credentials, encryption key, cache directory, RTC options |
| platform tick | implemented | EOS_Platform_Tick driven by EOS autoload timer |
| logging bridge | implemented | EOS log callback to Godot, secret sanitizer |
| auth login, logout, link account | implemented | credential types passed through, continuance token vault with integer refs |
| auth verify tokens | partial | local id validation only, server token verify pending |
| connect login, logout, create user | implemented | device id and external credentials, continuance token vault |
| user info query and cache read | implemented | QueryUserInfo, CopyUserInfo |
| friends query, count, invite, accept, reject | implemented | full invite lifecycle |
| presence set, query, has | implemented | modification flow with status and details record |
| sessions create, search, join, destroy | implemented | attributes at create, join uses session handle registry from search |
| sessions invites, attributes read and write | implemented | send, query, reject, modification update flow, registry reads |
| lobbies create, join by id, join by details, leave, invite | implemented | details-handle join via search registry |
| lobbies search and details read | implemented | search, member list, owner, attributes, bucket, slots |
| p2p send and receive | implemented | reliability selectable, NAT type cached read |
| p2p relay control | implemented | global setting, no per-user state |
| stats query and ingest | implemented | leaderboard scores flow through stats ingest |
| achievements unlock, query, definitions | implemented | V2 definitions with localized text |
| leaderboards definitions, ranks, user scores | implemented | read only by EOS design, writes go through stats |
| player data storage list, read, write | implemented | chunked FileTransferRequest pipeline with handle release |
| title storage read | implemented | chunked FileTransferRequest pipeline with handle release |
| ecom query offers, entitlements, checkout | implemented | single-offer checkout entry |
| ui show and hide friends | implemented | async overlay calls |
| ui pause, paused state, visibility, notification location | implemented | sync overlay state calls |
| social overlay preferences | implemented | display preference location |
| metrics begin and end session | implemented | Epic account type, controller unknown, server ip optional |
| metrics custom events | not-supported | EOS SDK 1.19.1 exposes no custom event API, only player sessions |
| reports send behavior report | implemented | category passed as int, see EOS_EPlayerReportsCategory |
| sanctions query with sanction list | implemented | count plus action, reference id, timestamps |
| sanctions appeal | implemented | reason passed as int with sanction reference id |
| rtc join and leave room | implemented | default voice options, lobby voice rooms excluded by EOS design |
| rtc sending and receiving volume | implemented | async volume updates per room |
| rtc admin | not-supported | server-side room management, needs backend token and service role |
| anticheat client and server | partial | availability reported, session registration needs game-server setup |
| mods enumerate and info | implemented | installed and available types, cached info read |
| kws age gate query | implemented | country code and age of consent in result |
| custom invites set payload and send | implemented | payload must be set before send by EOS design |
| progression snapshot begin, add, submit, end | implemented | key value pairs per snapshot id |
| integrated platform | requires-external | options container is platform specific and must be built before platform create |
| application lifecycle status | implemented | SetApplicationStatus passthrough |

EOS imposes its own limits: Ecom and overlay need Dev Portal
configuration, sanctions and RTC admin need backend roles, AntiCheat
needs a registered game-server build.
