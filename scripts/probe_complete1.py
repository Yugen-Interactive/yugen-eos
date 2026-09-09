import re
import pathlib

b = pathlib.Path(r"D:\yugen-eos\third_party\eos_sdk\SDK\Include")
T = {f.name: f.read_text(errors="replace") for f in b.glob("*.h")}
A = "\n".join(T.values())

def struct(name, length=1500):
    i = A.find("EOS_STRUCT(" + name)
    print("--- " + name + " ---")
    print(A[i:i + length] if i >= 0 else "NOT FOUND")

def decls(pattern):
    print("--- " + pattern + " ---")
    for m in sorted(set(re.findall(pattern, A))):
        print("   ", m)

struct("EOS_Platform_Options", 2600)
i = A.find("EOS_DECLARE_FUNC(EOS_HPlatform) EOS_Platform_Create")
print("--- Platform_Create decl ---")
print(A[i - 200:i + 400])
struct("EOS_PlayerDataStorage_WriteFileOptions", 1200)
struct("EOS_PlayerDataStorage_WriteFileDataCallbackInfo", 900)
struct("EOS_PlayerDataStorage_ReadFileOptions", 900)
struct("EOS_PlayerDataStorage_OnReadFileCompleteCallbackInfo", 900)
struct("EOS_PlayerDataStorage_OnWriteFileCompleteCallbackInfo", 900)
struct("EOS_PlayerDataStorage_OnFileTransferProgressCallbackInfo", 700)
decls(r"EOS_PlayerDataStorageFileTransferRequest_\w+")
struct("EOS_TitleStorage_ReadFileOptions", 900)
struct("EOS_TitleStorage_OnReadFileCompleteCallbackInfo", 700)
decls(r"EOS_TitleStorageFileTransferRequest_\w+")
i = A.find("EOS_ContinuanceToken")
print("--- ContinuanceToken ---")
print(A[i - 100:i + 700])
struct("EOS_Auth_LinkAccountOptions", 1100)
struct("EOS_Connect_CreateUserOptions", 600)
struct("EOS_Friends_AcceptInviteOptions", 600)
struct("EOS_Friends_RejectInviteOptions", 600)
