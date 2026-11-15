# Mario Kart WiiCompiled — Enhanced Fork

A native PC port of **Mario Kart Wii**, built through static recompilation.

**Original project credit:** WiiCompiled was created by **[Patchzzy (@patchzyy)](https://github.com/patchzyy)**. This repository is an independently maintained fork of [Patchzzy's original WiiCompiled project](https://github.com/patchzyy/Wiicompiled), maintained by [nvd123-YT](https://github.com/nvd123-YT). The original project and its author deserve credit for the foundation this fork builds on. Fork-specific changes are not official upstream releases.

WiiCompiled translates the game's PowerPC code into native code before launch. There is no Wii emulator, interpreter, or JIT running during gameplay, and no PowerPC CPU emulation in the runtime.

> [!IMPORTANT]
> **You must provide your own legally dumped copy of Mario Kart Wii.** This repository and its releases do not include Nintendo game code, game assets, or a game image. The setup tools run the translation and compilation locally against your disc image. Your game data is not uploaded.

---

## Features

### Graphics and display

- **Unlocked presentation with frame interpolation.** Mario Kart Wii normally targets 60 FPS. WiiCompiled can generate interpolated frames between the game's original frames, making motion look smoother on 120 Hz and 144 Hz displays.
  > [!WARNING]
  > Frame interpolation is experimental and can produce visual artifacts in certain situations.
- **Resizable aspect ratio.** Resize the game window to different shapes and sizes; the camera adjusts dynamically.
- **Native rendering through [Aurora](https://github.com/encounter/aurora).** The graphics layer uses Aurora, a source-level GameCube and Wii compatibility layer.
- **Higher internal resolution.** Render the game at multiples of the original console resolution.
- **Automatic focus pause.** Optionally pause gameplay when the window loses focus or is minimized, then resume when you return. Enable **Pause when unfocused or minimized** under F10 → Graphics; it is off by default.
- **MetalFX spatial upscaling.** The in-game settings bar exposes MetalFX spatial upscaling on supported macOS GPUs.

### Audio

- **Automatic music ducking.** When another application starts playing audio—such as Spotify or a YouTube video—WiiCompiled can automatically lower or mute the game's music until the other audio stops. This can be disabled.
- On **Windows**, music ducking uses system media controls.
- On **Linux**, it uses MPRIS-compatible media players.
- On **macOS 14.2 or later**, it detects active audio output from other applications while excluding WiiCompiled's own output. An application that keeps an audio stream open while silent may still cause the game's music to remain muted.

The F10 Audio menu also includes separate **Master, Music, Sound Effects, UI, and Voices** volume controls, a configurable keyboard shortcut for instant mute, and an optional audio-mixing worker-thread switch for troubleshooting.

### In-game settings

Press **F10** while the game window is focused to open the settings bar. It includes:

- Internal rendering resolution
- FPS counter
- MetalFX spatial upscaling on supported macOS GPUs
- Automatic pause while the game window is unfocused or minimized
- Assignment of controllers to all four GameCube controller ports
- Per-controller button mapping, including bumpers
- Dolphin-syntax input expressions and direct `GCPadNew.ini` import
- Controller vibration on/off
- Volume controls, instant mute, and the music-ducking toggle

Changes are written to `Config.toml` immediately and restored the next time the game starts.

### Controller support

- **Dolphin-compatible input expressions.** GameCube controls can use expressions written in Dolphin's input syntax, including its operators and functions. Import a Dolphin `GCPadNew.ini` directly from the F10 settings bar.
- **Four controller ports.** Assign controllers and configure each port independently.
- **Vibration control.** Disable force feedback across all controller ports with the in-game toggle.
- **Official Wii U / Switch GameCube adapter (WUP-028).** The adapter is supported. On Windows, install the WinUSB driver for it once using [Zadig](https://zadig.akeo.ie/), as with Dolphin.
- **Real Wii Remotes over Bluetooth.** Pair a Wii Remote with Windows through Settings → Bluetooth → Add device, then press **1+2** or **SYNC**. Leave the PIN field empty when prompted.
- **Shake to trick on motion-enabled gamepads.** In F10 → Controller settings, enable **Shake to trick** for a player using a controller with an exposed motion sensor. Flick the controller to trigger directional tricks off ramps or wheelies on bikes; the D-pad remains available. Adjust sensitivity and use the on-screen meter to test the motion threshold. Some Steam Input layouts hide motion sensors; disable Steam Input for the game if the sensor is unavailable.

Known Wii Remote limitations:

- The IR pointer is not implemented yet. Navigate menus with the D-pad and A; the game treats the remote as pointing away from the screen.
- Battery level is not reported to the game, and the Wii Remote speaker is not implemented.
- Only the Wii Remote's own accelerometer is calibrated. The Nunchuk currently uses SDL's fixed zero point.
- Classic Controller L/R triggers are reported as digital inputs (full pull on click); SDL does not expose their analog travel through this path.
- Disable Wii Remote support in the settings bar when using a Mayflash DolphinBar that already presents the remote as a regular gamepad.

### More built-in features

- **In-game custom texture pack controls.** Open F10 → Graphics → Custom texture packs to enable Dolphin-style replacements or texture dumping; both options are saved in `Config.toml` and require a restart. Place matching `.dds` or `.png` files in the `texture_replacements` folder inside WiiCompiled's user-data directory. Aurora indexes this folder at startup.
- **Texture dumping for pack creation.** Turn on **Dump unmatched textures** in the same F10 menu. Unmatched textures are written to `Cache/texture_dumps` with replacement-compatible filenames, giving you a starting point for creating your own pack. Dumping is only available while texture replacements are enabled.
- **Portable user-data mode.** Put an empty file named `portable.txt` in a writable folder containing the game executable or one of its parent folders (up to four levels above it). WiiCompiled then stores `Config.toml`, NAND data, cache, and logs in a `UserData` folder beside that marker instead of using the normal per-user application-data location. This is useful when keeping an installation and its user data together on an external drive.
- **Friend-code clipboard paste for Retro Rewind.** On the Register Friend page, enter a code with the keyboard or paste it with **Ctrl+V** (Windows/Linux) or **Command+V** (macOS). The input accepts up to 12 digits and ignores spaces, hyphens, and other non-digit characters.

---

## Download and installation

Download the setup package for your operating system from the [GitHub Releases page](https://github.com/nvd123-YT/MarioKartWiiCompiled-Enhanced/releases).

| Platform | Release package |
| --- | --- |
| Windows 10/11, 64-bit | `WiiCompiled-Setup.exe` |
| macOS, universal Apple Silicon and Intel | `WiiCompiled-Setup.dmg` |
| macOS, self-extracting alternative | `WiiCompiled-Setup-macos.run` |
| Linux, x86-64 | `WiiCompiled-Setup-x86_64.AppImage` |
| Linux, ARM64 | `WiiCompiled-Setup-aarch64.AppImage` |

The setup package contains the tools needed to prepare a native build; it is **not** a precompiled copy of Mario Kart Wii. During setup, choose your own clean disc image. The image is checked and processed on your computer, then the game is translated and compiled locally. Nothing is uploaded.

On macOS, open the DMG and use the included setup app. If Xcode Command Line Tools are missing, macOS may ask you to install them; complete Apple's installer and reopen setup.

WiiCompiled also has integration with [Wheel Wizard](https://github.com/TeamWheelWizard/WheelWizard), which can manage setup, updates, and launching. Wheel Wizard is a separate project with its own releases and documentation.

---

## Requirements

### Windows

- Windows 10 or Windows 11, 64-bit
- GPU: NVIDIA GTX 1650, AMD RX 6400, Intel Arc A310, or better
- CPU: Intel Core i5-8400, AMD Ryzen 5 2600 (or a comparable 4-core / 6-thread CPU at approximately 3.5 GHz), or better
- Approximately 20 GB of free space during installation; the finished game installation is approximately 5 GB

### macOS

- macOS 12 (Monterey) or later
- Apple Silicon (`arm64`) or Intel (`x86_64-v3`); pre-Haswell Intel processors are not supported
- A Metal-capable GPU
- Apple's Xcode Command Line Tools. Setup can open Apple's installer when they are missing.

The macOS music-ducking detection described above requires macOS 14.2 or later.

> [!NOTE]
> The listed Windows CPU and GPU minimums are driven largely by graphics-driver support and D3D12/Vulkan feature requirements, not by the original game's hardware demands.

### Required game image

Only clean, unmodified copies of these revisions are supported:

- **PAL:** `RMCP01`
- **NTSC-U:** `RMCE01`
- **NTSC-J:** `RMCJ01`

Accepted image formats are ISO, GCM, GCZ, CISO, WBFS, WIA, and RVZ.

Other revisions—including NTSC-K (`RMCK01`)—and modified executables are rejected. Translating a different revision against the wrong manifest can produce subtle and difficult-to-diagnose errors.

> [!NOTE]
> You are responsible for dumping your own game disc. This project does not provide game files or links to them.

---

## Retro Rewind

[Retro Rewind](https://wiki.tockdom.com/wiki/Retro_Rewind), the Mario Kart Wii mod distribution by ZPL and its team, can be built as its **own static profile**.

Rather than applying `Code.pul` as runtime patches, the Kamek/Pulsar code is translated together with the base game into a separate native executable. The project also includes build-guide support for the optional Retro-WFC payload used for Retro Rewind online multiplayer.

[Wheel Wizard](https://github.com/TeamWheelWizard/WheelWizard) can drive this workflow as well. Retro Rewind, Wheel Wizard, and WiiCompiled are independently developed projects; each has its own maintainers, contribution rules, and support channels. Check the relevant project's own README and contribution guidelines.

---

## Building from source

You still need to own a supported game copy when building from source.

### Prerequisites

- .NET 8 SDK
- CMake
- Ninja
- LLVM / Clang

The shipped Windows build uses LLVM-MinGW targeting `x86-64-v3`.

Build the translator:

```powershell
dotnet build translator/Translator.sln -c Release
```

The default translator test suite does not require game binaries or a host C++ compiler, so translator development can be done without game data.

For the full workflow—providing your own `main.dol` and `StaticR.rel`, translating the game, generating the manifest and build graph, and compiling the native runtime—see the [translator documentation](translator/README.md).

For a step-by-step guide to building WiiCompiled and Retro Rewind from source on Apple Silicon macOS, see the [macOS build guide](docs/building-macos.md).

The translator handles DOL and REL files generically. A translation alone does not make every game playable: a compatible runtime must also implement the systems that game uses, including graphics, audio, and input. See `projects/examples/generic-dol.yml` for the generic DOL example.

---

## FAQ

### Is WiiCompiled an emulator?

No. The game's code is statically translated and compiled into native code before you play. The runtime does not emulate a Wii CPU or GPU.

### Does this project include Mario Kart Wii?

No. The repository and releases do not distribute the game or its Nintendo assets. Supply your own supported disc image.

### Why does setup take so long?

The translated game executable is not included in the installer. Setup has to run the static recompiler across the game and then compile the generated C++ code on your computer. This is a one-time process for a given installation/build.

### Which game versions work?

Only clean PAL `RMCP01`, NTSC-U `RMCE01`, and NTSC-J `RMCJ01` revisions are accepted. NTSC-K `RMCK01` and modified executables are rejected.

### Can the translator recompile other GameCube or Wii games?

The translator itself works with DOL and REL files generically. A playable port also needs a suitable runtime for that game's hardware and software interfaces—graphics, audio, input, and anything else the game uses.

### The game crashed or stopped with an error. What should I send?

Errors are designed to be reported rather than silently swallowed. Include the error details and the run log from:

```text
%LOCALAPPDATA%\WiiCompiled\Logs
```

### Will original game bugs be fixed?

The base game aims to behave like the original hardware. Report problems where WiiCompiled behaves differently from the original game, rather than bugs that are already present in the original.

Retro Rewind changes some base-game behavior, so differences from the unmodified game may be intentional. If Retro Rewind behaves differently in WiiCompiled than it does on Wii or Dolphin, report that as a WiiCompiled compatibility issue.

### How accurate are the physics?

The project reports **100% physics parity** with the original game. The main evidence is matching in-game ghost replays: ghosts record player inputs rather than tracked positions, so matching replays across Wii, Dolphin, and WiiCompiled demonstrate that the simulation is behaving consistently.

### Is the project finished?

Not completely. The game is intended to be playable and the project's testing reports matching physics, but compatibility, rendering, networking, and performance remain active areas of development. Please open an issue when you encounter a problem so it can be investigated.

---

## AI usage

AI coding tools were used during development. Translated output is validated against real-hardware behavior, and the project's physics-parity testing is described above.

---

## Credits

- **[Patchzzy (@patchzyy)](https://github.com/patchzyy)** — creator of the original [WiiCompiled project](https://github.com/patchzyy/Wiicompiled), which this fork is based on.

Third-party components, licenses, and redistribution notices are documented in [THIRD-PARTY-NOTICES.md](THIRD-PARTY-NOTICES.md).

---

## License and disclaimer

WiiCompiled is free software distributed under the [GNU General Public License, version 3](LICENSE). It is provided in the hope that it will be useful, but **without any warranty**, including implied warranties of merchantability or fitness for a particular purpose. See the license for the full terms.

Any Mario Kart Wii distribution that incorporates WiiCompiled must be licensed under GPL-3.0 or a more permissive license.

This project is not affiliated with, endorsed by, or associated with Nintendo. Mario Kart Wii is a trademark of Nintendo. No Nintendo game code or game assets are distributed with this project.
