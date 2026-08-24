# BitQuest
Games Engineering Project 

A University and personal games dev project called BitQuest

Thomas Young
- Main Game Logic
- AI
- Engine


Mark Barton
- Level Design
- UI Development
- Engine
- Sound

https://ske66.github.io/BitQuest/


[![CI](https://github.com/MarkAtOmniux/BitQuest/actions/workflows/ci.yml/badge.svg)](https://github.com/MarkAtOmniux/BitQuest/actions/workflows/ci.yml)

## Download

Playable builds are on the [Releases](https://github.com/MarkAtOmniux/BitQuest/releases/latest) page. Each successful push to `master` updates that download.

- **Windows:** download `BitQuest-windows.zip`, unzip it, and run `BitQuest/BitQuest.exe`.
- **macOS:** download the `.dmg` (or the `.zip`), open `BitQuest.app`, and click the game window once so it has keyboard focus. If macOS says the app cannot be opened, right-click it and choose **Open**.

Do not use the zip files under a workflow run on the Actions tab unless you are debugging CI — those expire and are harder to find.

## How to play

You play as Bob, exploring a dungeon for treasure and fighting monsters on the way to **Gavin, The Pretty Terrible Wizard**.

From the main menu choose **New Game** or **Resume Game**. Click a menu button to select it.

Your goal is to survive the dungeon, collect gold, and defeat Gavin. Enemies include slimes, goblins, skeletons, orcs, and trolls — they pause and plant before attacking, so you can punish a swing or back off.

- Stand next to a **chest** and press **E** to open it for gold.
- Stand next to the **shop** and press **E** to buy arrows, ham, and upgrades.
- **Ham** fully restores health. You start with two; eat one with **H**.
- **1** switches to the bow (uses arrows). **2** switches to the sword.
- **Escape** pauses and saves. **F11** toggles fullscreen.

If you die, the game over screen returns you to the menu. Resume Game loads your last save.

### Controls

| Action | Keys |
| --- | --- |
| Move | A / D or Left / Right |
| Jump | W or Up |
| Attack | Space or Left click |
| Block | C or Right click |
| Interact | E |
| Eat ham | H |
| Bow / Sword | 1 / 2 |
| Pause / save | Escape |
| Fullscreen | F11 |

## Build and run

Requires **CMake 3.9+** and a C++14 compiler (Xcode Command Line Tools on macOS, Visual Studio 2017 or later on Windows). SFML 2.6.2 and Box2D are pulled in as git submodules.

```bash
git clone --recurse-submodules https://github.com/MarkAtOmniux/BitQuest.git
cd BitQuest
```

If you already cloned without submodules:

```bash
git submodule update --init --recursive
```

### macOS

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --target BITQUEST -j
open build/bin/BitQuest.app
```

Click the game window once so it has keyboard focus. Launch the `.app` bundle, not a leftover `BITQUEST` binary in `build/bin/`.

For a single Intel + Apple Silicon package, configure with `-DBITQUEST_UNIVERSAL=ON`.

### Windows

```bash
cmake -S . -B build
cmake --build build --config Release --target BITQUEST --parallel
```

Run `build\bin\Release\BitQuest.exe` (Visual Studio multi-config) or `build\bin\BitQuest.exe` (single-config generators). Keep that folder as the working directory so `res/` is found.

### Package a distributable

```bash
cmake --build build --target package_game -j
```

Output goes to `build/dist/` (`.dmg` on macOS, `.zip` on Windows). Pushing a `v*` tag also publishes a versioned GitHub Release.
