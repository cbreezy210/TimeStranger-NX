# TimeStranger-NX 💩
> 💩 *Yes, the poop emoji is canonical. Sukamon approves. Every save editor needs a mascot - ours just happens to be a Digimon.*

**The first native Switch save editor for Digimon Story: Time Stranger.**

![Status](https://img.shields.io/badge/Status-Active%20Development-yellow?style=flat-square)
![Platform](https://img.shields.io/badge/Platform-Nintendo%20Switch-E60012?style=flat-square&logo=nintendo-switch&logoColor=white)
![Language](https://img.shields.io/badge/Language-C++-00599C?style=flat-square&logo=c%2B%2B&logoColor=white)
![License](https://img.shields.io/github/license/cbreezy210/TimeStranger-NX?style=flat-square&color=blue&cachebust=1)

---

## 🚀 What is this?
TimeStranger-NX is a native, homebrew save editor for *Digimon Story: Time Stranger* that runs directly on your Switch. 

Unlike traditional methods that require you to dump your save to the SD card via JKSV/Checkpoint, edit it on a PC, and restore it, TimeStranger-NX utilizes **Direct NAND Injection**. It mounts the game's save partition, modifies the values in real-time, and commits the changes directly to the system NAND. 

**No bridge required. No SD card shuffle. No PC needed.**

## ✨ Features
- ⚡ **Direct NAND Access**: Edits your save file live on the console's internal memory.
- 💰 **Currency Editor**: Seamlessly modify your in-game Yen.
- 🛡️ **Safe Writes**: Utilizes `fsFsCommit()` to ensure data integrity and prevent silent rollbacks.
- 🖥️ **Native C++ UI**: Clean, console-optimized interface built with `libnx`.

## 🚧 Roadmap (Coming Soon)
- [ ] Advanced item/monster editing
- [ ] Full save slot management
- [ ] Public v1.0.0 Release

## 📥 Installation

   > **⚠️ Status: Pre-release.** TimeStranger-NX is in active development — no public `.nro` builds are posted yet. Until v1.0.0 drops, build it yourself using the **Building from Source** section below. Prebuilt releases will appear on the [Releases page](https://github.com/cbreezy210/TimeStranger-NX/releases) at launch.

   **Standard install (once v1.0.0 is live):**
   1. Download the latest `TimeStranger-NX.nro` from the Releases page.
   2. Place the `.nro` file in your `/switch/TimeStranger-NX/` folder on your SD card.
   3. Launch the app via your Homebrew Menu (Sphaira/Atmosphere).

   **Right now (developers & testers):**
   1. Build from source (see below), then copy the resulting `.nro` to `/switch/TimeStranger-NX/` on your SD card.
   2. Launch via your Homebrew Menu (Sphaira/Atmosphere).

## 🕹️ Usage & Controls
1. **Important:** Fully close *Digimon Story: Time Stranger* (Home Menu -> X -> Close Software) before launching the editor.
2. Launch **TimeStranger-NX**.
3. Use the D-Pad/Buttons to adjust your values.
4. Press **[+]** to write the changes to the NAND and exit.
5. Boot the game to see your updated stats!

## 🔬 Technical Details & Reverse Engineering
*For developers and researchers interested in the Switch filesystem API:*

- **Title ID**: `0x010062E01FE0C000`
- **Save Structure**: The game utilizes a specific partition layout where the active save slot is typically located at `/savedata/0001.bin`.
- **The Commit Bug**: During development, we discovered that `fsFileWrite()` alone is insufficient for this specific save partition. The filesystem journal will silently roll back changes upon closing the file handle unless `fsFsCommit(&saveFs)` is explicitly called. This is a rare quirk compared to other Switch titles.
- **Offsets**:
  - **Yen**: `0x7973` (u32 Little-Endian)

💬 **From the Trenches**
> *"direct NAND save interaction is fucking sick"*  
> — u/Duffmcmcmcwhalen, r/SwitchHacks

> *"YO THIS IS INSANE. HUGE PROPS"*  
> — u/riskyjones, r/SwitchHacks

> *"Probably stats atm just so I can clear certain areas lol"* (Now tracked in [Issue #1: Value Hunter](https://github.com/cbreezy210/TimeStranger-NX/issues/1))  
> — u/riskyjones, r/SwitchHacks

## 🛠️ Building from Source
Requires [devkitPro](https://devkitpro.org/) and `libnx`.

```bash
git clone https://github.com/cbreezy210/TimeStranger-NX.git
cd TimeStranger-NX
make clean
make
```

## 🔗 Explore More Projects
- 🏠 **[cbreezy210 Main Profile](https://github.com/cbreezy210)** – See all my tools, AI projects, and community updates.
- 🔴⚪️ **[PKHeX-NX](https://github.com/cbreezy210/PKHeX-NX)** – Native Pokémon SV Save Editor.
- 🍃 **[ACNH-Save-Editor](https://github.com/cbreezy210/ACNH-Save-Editor)** – Native Animal Crossing companion tool.

## 🤝 Contributing
Contributions welcome! The most useful ways to help:
- **Report issues** with the 5-answer checklist from Troubleshooting (tool + version, install type, what didn't stick, error vs. silent revert, game fully closed?).
- **Save-format research:** PRs with verified offsets/checksums - include your diff method and dump tooling.
- **Docs & accessibility guides:** wiki improvements and walkthroughs (micaturtle's accessibility guide set the bar!).

Open an issue before large refactors so we can align on scope.

## 🙏 Credits
* **devkitPro and libnx communities** — Switch homebrew tooling: https://devkitpro.org
* **The GBATemp & r/HomebrewSwitch communities** — for early testing, hardware validation, and relentless feedback
* **The homebrew community for continuous support**
