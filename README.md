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


[![Build status](https://ci.appveyor.com/api/projects/status/s6j4yt4dhe4ct4dm?svg=true)](https://ci.appveyor.com/project/ske66/bitquest)

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

Output goes to `build/dist/` (`.dmg` on macOS, `.zip` on Windows).

## Controls

| Action | Keys |
| --- | --- |
| Move | A / D or Left / Right |
| Jump | W or Up |
| Attack | Space or Left click |
| Block | C or Right click |
| Interact | E |
| Sword / Bow | 2 / 1 |
| Pause | Escape |

