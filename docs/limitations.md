Limitations

EOS is a proprietary online service. Many features need Dev Portal
setup, product configuration, or backend roles before any client
code can succeed. The wrapper reports these as data, it cannot
remove them.

Client secrets and server credentials must never ship in a game.
Server-side flows such as RTC admin, sanction appeals review, and
AntiCheat server registration belong on infrastructure the
developer controls.

The Social Overlay is unavailable on headless platforms and where
Epic does not ship it. Voice needs OS microphone permission and a
joined RTC room. Ecom needs catalog configuration per sandbox.

File transfer read and write for storage interfaces is the largest
pending wrapper work. The query side is done, the chunked
FileTransferRequest pipeline is specified in docs/status.md.

Platform support follows Epic. If EOS drops a platform, the loader
reports it and the interfaces return NotImplemented. The wrapper
does not polyfill missing EOS binaries.
