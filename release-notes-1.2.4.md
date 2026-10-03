# NitLink 1.2.4

Discord Rich Presence is now console-neutral and optional, capture-format preferences remain tied to each capture device, and tagged releases can be built and published automatically from GitHub Actions.

- **Console-neutral Discord presence.** Activity now describes NitLink as a capture-card viewer and uses “Playing via NitLink” instead of naming PlayStation 5, so it fits Nintendo Switch, Xbox, PlayStation, retro consoles, cameras, and other HDMI sources.
- **Discord can be disabled.** Turn off **F1 → About → Integrations → Discord Rich Presence** to disconnect immediately and prevent NitLink from opening Discord's local IPC pipe on later launches. The preference is saved in `nitlink.json` as `discord_rpc_enabled`.
- **Per-device capture preferences.** Resolution, native frame rate, and pixel-format selections are restored independently for every capture-device name. The same path applies to generic Media Foundation devices and does not require an Elgato card.
- **Automated Windows releases.** Pushing a version tag such as `v1.2.4` builds and tests NitLink on GitHub Actions, validates the distributable, and publishes the zip to that repository's GitHub Releases page.

Requirements: Windows 10 (1809+) or Windows 11, a DirectX 11 capable GPU, the Microsoft Edge WebView2 runtime, the VC++ 2015-2022 redistributable, and a Media Foundation compatible capture device.
