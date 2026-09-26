# 💩 TimeStranger-NX — Public Roadmap
A native Nintendo Switch homebrew save editor for *Digimon Story: Time Stranger* — no PC, no bridge, no SD card shuffling required. 

*(Yes, the poop emoji is canonical. Sukamon approves. Every save editor needs a mascot.)*

### 🎯 Roadmap Scope: What This IS / IS NOT

| This Roadmap IS... | This Roadmap IS NOT... |
| :--- | :--- |
| A technical blueprint for native, offline save editing via Direct NAND Injection. | A generic "cheat engine" or tool for online competitive exploitation. |
| Focused on filesystem quirks, data integrity, and forensic safety. | A commitment to bypassing Nintendo's anti-cheat or ban systems. |
| Driven by community feedback, open-source transparency, and hardware validation. | A strict timeline with guaranteed, fixed release dates. |

---

### 🏗️ Project Architecture & Phase Tree

```text
TimeStranger-NX Development Phases
├── Phase 1: Core Engine & Safety (v0.1.x - v0.3.x) ✅
│   ├── Direct NAND Injection Engine
│   ├── The fsFsCommit() Quirk Resolution
│   └── Zero-Lost-Saves Safety Pipeline (Auto-Backup + Rollback)
├── Phase 2: Value Hunter & Core Stats (v1.0.x) 🟢
│   ├── Read-Only Save Diffing Tool (CLI)
│   ├── In-App Value Hunter Scanner
│   └── Basic Stat Editing (HP, MP, Attack, etc.)
├── Phase 3: Advanced Content (v1.1.x) 🟡
│   ├── Item & Bag Editor
│   ├── Full Party / Digimon Team Editor
│   └── Save Slot Management & Cloning
└── Phase 4: UI Overhaul & Polish (v2.0.x) 🔴
    ├── SDL2 Graphical Renderer
    └── "Dry Run" Preview Mode & Advanced Automation
```

---

### 📂 Installation & SD Card Layout
*(Reference for upcoming features that interact with the SD card)*

```text
📦 SD (Root)
┣━ 📂 switch
┃  ┗━ 📂 TimeStranger-NX
┃     ┗━ 📜 TimeStranger-NX.nro
┗━ 📂 switch/TimeStranger-NX/backups (auto-created)
   ┣━ 📄 YYYYMMDD_HHMMSS_0001.bin
   ┗━ 📄 YYYYMMDD_HHMMSS_0000.bin
```

---

## ✅ Shipped — v0.1.0 (The Direct NAND Proof of Concept)
The foundational breakthrough. Proving that live, on-console editing is possible without JKSV/Checkpoint bridges.
- [x] **Title ID Targeting:** Accurate identification of `0x010062E01FE0C000`.
- [x] **Partition Discovery:** Located the active save container at `/savedata/0001.bin` through recursive filesystem scanning.
- [x] **Currency Editor:** Live modification of in-game Yen (Offset: `0x7973`, u32 Little-Endian).
- [x] **Native C++ UI:** Clean, console-optimized interface built with `libnx`.

## ✅ Shipped — v0.2.0 (The Safety Pipeline)
Bulletproofing the core architecture. If you can't edit safely, you shouldn't edit at all.
- [x] **The fsFsCommit() Fix:** Discovered and resolved a critical game-specific quirk where `fsFileWrite` silently rolls back on close unless `fsFsCommit(&saveFs)` is explicitly invoked.
- [x] **Auto-Backup System:** Automatically creates timestamped, read-only snapshots of both `0000.bin` and `0001.bin` to the SD card *before* any write operation.
- [x] **Emergency Rollback:** One-tap `[ZL]` menu option to instantly restore the latest known-good backup from the SD card.
- [x] **SD Space Validation:** Checks available storage before attempting backup creation to prevent mid-write corruption.

## ✅ Shipped — v0.3.0 (Community Tooling & Value Hunter CLI)
Empowering the community to find offsets independently, safely, and offline.
- [x] **Value Hunter CLI (`tools/value_hunter.py`):** A read-only, PC-based diffing tool that takes two save dumps and a known delta, outputting candidate u32/u16 little-endian offsets.
- [x] **Public Roadmap & Issue Tracking:** Transparent development via GitHub Projects and community-driven feature requests (e.g., Issue #1).
- [x] **Community Validation:** Active beta testing and feedback loops established with r/SwitchHacks and GBATemp communities.

---

## 🟢 Coming Next — v1.0.0 (The Public Release)
*Target Release: Late 2026*

### 🔍 Value Hunter Integration
- [ ] **In-App Scanner:** Port the Python diffing logic to native C++ for on-console, read-only scanning of the active save file.
- [ ] **Multi-Type Support:** Scan for u8, u16, and u32 values with delta filtering.
- [ ] **Results UI:** Clean, scrollable list of candidate offsets with `val1 -> val2` previews.

### ⚔️ Core Stat Editing
- [ ] **Digimon Stats:** Verified offsets and safe clamping for HP, MP, Attack, Defense, Speed, and Intelligence.
- [ ] **Trainer Stats:** Level, EXP, and Tamer name editing.
- [ ] **Checksum Awareness:** Basic checksum recalculation or flagging if the game employs them (pending reverse engineering).

### 🛡️ Safety & Reliability
- [ ] **Public v1.0.0 Release:** First stable, pre-compiled `.nro` binary available on the GitHub Releases page.
- [ ] **Strict Value Clamping:** Prevent users from entering values that exceed in-game maximums (e.g., negative HP, max int overflow).

---

## 🟡 Mid-Term — v1.1.0 (Advanced Content Expansion)
*Target Release: Q1 2027*

### 🎒 Content & Inventory
- [ ] **Item & Bag Editor:** Parse the item block and allow visual slot picking for key items, consumables, and equipment.
- [ ] **Digimon Team/Party Editor:** Edit the active party lineup, including levels, learned skills, and equipment slots.

### 💾 Save Management
- [ ] **Save Slot Management:** Support for editing multiple save slots if the game utilizes them.
- [ ] **Save Cloning/Export:** Ability to duplicate a save slot or export a raw `.bin` to the SD card for safekeeping.

---

## 🔴 Long-Term — v2.0.0 (UI Overhaul & Deep Automation)
*Target Release: 2027+*

### 🎨 UI & Graphics
- [ ] **Full Graphical UI:** SDL2 renderer replaces the text console (themed header, Sukamon logo, crisp TTF text, highlight-bar navigation).
- [ ] **Save File "Diff" Viewer:** Compare backup and current save side-by-side before writing.

### 🛡️ Advanced Safety
- [ ] **"Dry Run" Preview Mode:** See exactly what bytes will change before committing to NAND.
- [ ] **Rolling Backup Manager:** Keep the last 3-5 backups with timestamps and a visual picker on the SD card.
- [ ] **Game Version Validator:** Warn if save offsets don't match the current game patch version.

---

### 🎮 Target Compatibility Matrix

| Game | Status | Save Format | Target Version |
| :--- | :--- | :--- | :--- |
| **Digimon Story: Time Stranger** | 🟡 Active Dev | Native NAND (`/savedata/`) | v1.0.0+ |

---

### 🕳️ Known Gaps (Strictly Aligned with Roadmap)
*These limitations are known and explicitly scheduled for resolution to prevent feature drift.*
- [ ] **Text-Only UI:** The current interface is a developer-focused text console.
  - *Fix scheduled:* **v2.0.0** (SDL2 Graphical UI).
- [ ] **No Item/Bag Editing:** You cannot currently edit your inventory or key items.
  - *Fix scheduled:* **v1.1.0** (Item & Bag Editor).
- [ ] **Value Hunter is PC-Only (for now):** The diffing tool currently requires a PC and two JKSV dumps.
  - *Fix scheduled:* **v1.0.0** (Native In-App Scanner).

---

### 🛡️ Backup & Safety System Philosophy
Safety is the core pillar of this roadmap. Every feature that touches NAND is gated by our safety net:
- [x] **File Paths:** `sdmc:/switch/TimeStranger-NX/backups/YYYYMMDD_HHMMSS_0001.bin`
- [x] **Mandatory Commits:** All writes are followed by `fsFsCommit(&saveFs)` to guarantee persistent journal flushing, preventing the game's silent rollback quirk.
- [x] **Pre-Write Snapshots:** The app *refuses* to write to NAND without first successfully creating a timestamped backup.
- [x] **One-Button Rollback:** Press `[ZL]` on the main menu to instantly restore from the newest timestamped backup.

> 💡 **The Honest Note:** No save editor is 100% bulletproof. While TimeStranger-NX uses rigorous verification and mandatory backups, SD cards can fail, power can cut out, and edge cases in proprietary save structures can still exist. *Always* keep a secondary backup on your PC using a tool like JKSV or Checkpoint before doing major edits. Our safety net is your first line of defense, not your only one.

---

### 🤝 Credits & Acknowledgments
- **Switchbrew & devkitPro:** For maintaining the `libnx` toolchain that makes native Switch homebrew possible. ([Website](https://devkitpro.org))
- **The GBATemp & r/SwitchHacks Communities:** For early testing, hardware validation, and relentless feedback during the devlog phase. Special shoutout to early testers like `riskyjones` and `Duffmcmcmcwhalen`.
- **The PKHeX-NX & ACNH-Save-Editor Foundations:** The architectural patterns, safety pipelines, and community trust built in previous projects directly inform this tool's design.

---

### ⚖️ Responsible Use & Disclaimer
- **Offline Use Only:** Do not use edited saves in online multiplayer or official competitive environments.
- **Educational Purpose:** This software is provided "as-is" for research into console filesystems and data structures. Modifying save data always carries a risk.
- **No Liability:** I am not responsible for corrupted saves, lost progress, or console bans. Use at your own risk!
- **Not Affiliated:** This project is not affiliated with, endorsed by, or sponsored by Nintendo, Bandai Namco, or Media.Vision.

---
Built with devkitPro/libnx. Educational project — always keep backups! 💩
