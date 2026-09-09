import re
import pathlib

b = pathlib.Path(r"D:\yugen-eos\third_party\eos_sdk\SDK\Include")
T = {f.name: f.read_text(errors="replace") for f in b.glob("*.h")}
A = "\n".join(T.values())

def struct(name, length=1300):
    i = A.find("EOS_STRUCT(" + name)
    print("--- " + name + " ---")
    print(A[i:i + length] if i >= 0 else "NOT FOUND")

def decls(pattern):
    print("--- " + pattern + " ---")
    for m in sorted(set(re.findall(pattern, A))):
        print("   ", m)

struct("EOS_Platform_ClientCredentials", 700)
i = A.find("EOS_DECLARE_FUNC(void) EOS_Platform_Release")
print("--- Platform_Release ---")
print(A[i - 200:i + 200] if i >= 0 else "NOT FOUND")
print("--- PF flags ---")
for m in sorted(set(re.findall(r"EOS_PF_\w+", A))):
    print("   ", m)
i = A.find("EOS_DECLARE_FUNC(EOS_PlayerDataStorageFileTransferRequest*) EOS_PlayerDataStorage_WriteFile")
print("--- PDS WriteFile decl ---")
print(A[i - 300:i + 400] if i >= 0 else "NOT FOUND")
i = A.find("EOS_DECLARE_FUNC(EOS_PlayerDataStorageFileTransferRequest*) EOS_PlayerDataStorage_ReadFile")
print("--- PDS ReadFile decl ---")
print(A[i - 300:i + 400] if i >= 0 else "NOT FOUND")
i = A.find("EOS_DECLARE_CALLBACK(EOS_PlayerDataStorage_OnWriteFileDataCallback")
print("--- WriteData cb typedef ---")
print(A[i - 100:i + 500] if i >= 0 else "NOT FOUND")
i = A.find("EOS_DECLARE_CALLBACK(EOS_PlayerDataStorage_OnReadFileDataCallback")
print("--- ReadData cb typedef ---")
print(A[i - 100:i + 500] if i >= 0 else "NOT FOUND")
struct("EOS_PlayerDataStorage_ReadFileDataCallbackInfo", 1100)
struct("EOS_PlayerDataStorage_WriteFileCallbackInfo", 800)
struct("EOS_PlayerDataStorage_ReadFileCallbackInfo", 800)
i = A.find("EOS_DECLARE_FUNC(EOS_TitleStorageFileTransferRequest*) EOS_TitleStorage_ReadFile")
print("--- TS ReadFile decl ---")
print(A[i - 300:i + 400] if i >= 0 else "NOT FOUND")
struct("EOS_TitleStorage_ReadFileDataCallbackInfo", 900)
struct("EOS_TitleStorage_ReadFileCallbackInfo", 700)
i = A.find("typedef struct EOS_ContinuanceTokenDetails")
print("--- ContinuanceToken typedef ---")
print(A[i - 200:i + 600] if i >= 0 else "NOT FOUND")
decls(r"EOS_\w*ContinuanceToken_Release")
struct("EOS_Auth_LinkAccountOptions", 2200)
decls(r"EOS_Auth_LinkAccountCallbackInfo|EOS_Auth_OnLinkAccountCallback")
i = A.find("EOS_DECLARE_FUNC(void) EOS_Connect_CreateUser")
print("--- Connect CreateUser decl ---")
print(A[i - 200:i + 300] if i >= 0 else "NOT FOUND")
decls(r"EOS_Connect_OnCreateUserCallback|EOS_Connect_CreateUserCallbackInfo")
decls(r"EOS_Friends_OnAcceptInviteCallback|EOS_Friends_AcceptInviteCallbackInfo|EOS_Friends_OnRejectInviteCallback|EOS_Friends_RejectInviteCallbackInfo")
