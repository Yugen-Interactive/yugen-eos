import pathlib
import re

REPO = pathlib.Path(r"D:\yugen-eos")
WIKI = pathlib.Path(r"C:\Users\hxgohxrr\AppData\Local\Temp\opencode\wiki")

PAGES = {
    "architecture.md": "Architecture.md",
    "installation.md": "Installation.md",
    "configuration.md": "Configuration.md",
    "building.md": "Building.md",
    "platforms.md": "Platforms.md",
    "api.md": "API.md",
    "troubleshooting.md": "Troubleshooting.md",
    "cicd.md": "CICD.md",
    "sdk_update.md": "SDK-Update.md",
    "limitations.md": "Limitations.md",
    "status.md": "Status.md",
}

LINKS = {
    "docs/architecture.md": "[[Architecture]]",
    "docs/installation.md": "[[Installation]]",
    "docs/configuration.md": "[[Configuration]]",
    "docs/building.md": "[[Building]]",
    "docs/platforms.md": "[[Platforms]]",
    "docs/api.md": "[[API]]",
    "docs/troubleshooting.md": "[[Troubleshooting]]",
    "docs/cicd.md": "[[CICD]]",
    "docs/sdk_update.md": "[[SDK-Update]]",
    "docs/limitations.md": "[[Limitations]]",
    "docs/status.md": "[[Status]]",
}

for src, dst in PAGES.items():
    text = (REPO / "docs" / src).read_text(errors="replace")
    for old, new in LINKS.items():
        text = text.replace(old, new)
    (WIKI / dst).write_text(text)

home = """Yugen EOS: Epic Online Services wrapper for Godot 4.

Wrapper 0.2.0, Godot 4.3+, EOS SDK 1.19.1. MIT licensed.

Start with [[Installation]] and [[Configuration]], then follow the
[[API]] guide. [[Status]] lists exactly what is implemented and
what is not.

Pages:

- [[Architecture]]
- [[Installation]]
- [[Configuration]]
- [[Building]]
- [[Platforms]]
- [[API]]
- [[Troubleshooting]]
- [[CICD]]
- [[SDK-Update]]
- [[Limitations]]
- [[Status]]
"""
(WIKI / "Home.md").write_text(home)

sidebar = """- [[Home]]
- [[Installation]]
- [[Configuration]]
- [[API]]
- [[Status]]
- [[Architecture]]
- [[Building]]
- [[Platforms]]
- [[Troubleshooting]]
- [[CICD]]
- [[SDK-Update]]
- [[Limitations]]
"""
(WIKI / "_Sidebar.md").write_text(sidebar)
print("pages written:", len(list(WIKI.glob("*.md"))))
