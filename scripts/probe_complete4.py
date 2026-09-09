import re
import pathlib

b = pathlib.Path(r"D:\yugen-eos\third_party\eos_sdk\SDK\Include")
T = {f.name: f.read_text(errors="replace") for f in b.glob("*.h")}
A = "\n".join(T.values())

i = A.find("OnWriteFileDataCallback")
print("--- WriteData typedef ctx ---")
print(A[i - 800:i + 400])
i = A.find("OnReadFileDataCallback,")
print("--- ReadData typedef ctx ---")
print(A[i - 800:i + 400])
i = A.find("ContinuanceToken will be")
print("--- token lifetime ---")
print(A[i - 500:i + 900] if i >= 0 else "NOT FOUND alt")
for m in re.finditer(r"ContinuanceToken[^.]{0,120}(releas|free|valid|consum)[^.]{0,200}", A):
    print("   ...", m.group(0)[:220].replace("\n", " "))
    break
