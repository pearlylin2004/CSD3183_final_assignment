# Pokemon Trainer Move Selection AI

A C++ based Pokemon battle simulator and AI research environment, testing simultaneous turn-based move selection using Gen 3 combat mechanics.

## AI Algorithms Implemented
This project evaluates advanced game-theoretic algorithms against a standard Greedy heuristic baseline:

- **Simultaneous Move Alpha-Beta (SMAB)**: Bypasses standard Minimax's structural inseparability in simultaneous games. Every decision node represents a zero-sum payoff matrix. The agent computes the maximum worst-case scenario (Maximin) while implementing strict dominance pruning to discard mathematically inferior branches.
- **Information Set Monte Carlo Tree Search (IS-MCTS)**: An advanced tree-search algorithm. Before the standard 4-stage MCTS loop (Selection, Expansion, Simulation, Backpropagation), the algorithm determinizes the state. It uses the Decoupled UCT formula to navigate the simultaneous action space and converges on robust strategies over numerous random rollouts.
- **Greedy Baseline**: A deterministic heuristic agent that picks the move with the highest immediate expected damage (factoring in STAB, type effectiveness, and accuracy). 

## Features
- **Headless `GameState` Engine**: An ultra-fast simulation environment designed specifically for running deep AI tree-searches efficiently.
- **Authentic Gen 3 Combat**: Includes Nintendo's stat formula, speed checks, priority items, and a fully updated type effectiveness matrix.
- **20 Pokemon Roster**: A diverse pool of 20 classic Pokemon (complete with PokeAPI front/back sprites) that automatically construct level-appropriate movesets and naturally cap at 4 moves.
- **5v5 Team Battles**: Trainers dynamically generate teams of 5 random Pokemon, significantly increasing the game-tree complexity for the AI to navigate.
- **Interactive GUI**: Utilizes **SFML** to visualize battles featuring an authentic 2x2 grid menu layout.

## Benchmarking Mode
When launching the application, you can bypass the GUI and run automated performance metrics. You can run individual matchups or sequence them back-to-back:
1. **Greedy vs Greedy**
2. **SMAB vs Greedy**
3. **MCTS vs Greedy**
4. **Run All Benchmarks**

The benchmark outputs total win percentages, draws, and the average computational time (ms) per match.

## Controls (GUI Mode)
- **Left / Right / Up / Down Arrows**: Navigate menus in the 2x2 grid
- **Z** or **Enter**: Select / Confirm / Advance text
- **X** or **Escape**: Go back / Cancel

## Dependencies & Installation
This project uses **SFML 3** for graphics and window management. The SFML libraries are vendored into the repository for portability, so you do not need `vcpkg`.

1. Open `pokemon/pokemon.slnx` or `pokemon/pokemon.vcxproj` in Visual Studio 2022.
2. Ensure the build configuration is set to `x64` (Debug or Release).
3. Build and Run the project!
