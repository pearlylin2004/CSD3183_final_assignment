# Pokemon C++ GUI Clone

A lightweight C++ and SFML-based clone of the classic Gen 3 Pokemon combat system (Fire Red / Emerald style). 

## Features
- True turn-based combat with speed checks and priority items
- Nintendo's accurate Stat calculation formula (using Level, Base Stats, and random IVs)
- Pokemon Learnsets that grant moves dynamically based on Level
- Turn-based event queue so animations sync perfectly with the text
- Smart enemy AI that uses Potions when at low health
- Randomized trainer teams with levels fluctuating between 8 and 10

## Controls
- **Up / Down Arrows**: Navigate menus
- **Z** or **Enter**: Select / Confirm / Advance text
- **X** or **Escape**: Go back / Cancel

## Dependencies & Installation
This project uses **SFML** for graphics and window management. If you are cloning this repository, you will need to install SFML to compile the code successfully. The easiest way to do this on Windows is using `vcpkg`:

1. Install `vcpkg` by following the instructions on the [official vcpkg GitHub](https://github.com/microsoft/vcpkg).
2. Integrate `vcpkg` with Visual Studio by running: `vcpkg integrate install`
3. Install SFML (64-bit) by running: `vcpkg install sfml:x64-windows`
4. Open the `.sln` file in Visual Studio, ensure the build configuration is set to `x64`, and Build
