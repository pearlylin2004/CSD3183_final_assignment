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

## Dependencies
- Visual Studio C++ (Platform Toolset v143)
- SFML (vcpkg)
