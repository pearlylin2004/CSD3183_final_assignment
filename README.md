# Pokemon Trainer Move Selection AI

A C++ based Pokemon battle simulator and AI research environment, testing simultaneous turn-based move selection using Gen 3 combat mechanics.

## AI Algorithms Implemented
This project evaluates advanced game-theoretic algorithms against a standard Greedy heuristic baseline:

- **Simultaneous Move Alpha-Beta (SMAB)**: Bypasses standard Minimax's structural inseparability in simultaneous games. Every decision node represents a zero-sum payoff matrix. The agent computes the maximum worst-case scenario (Maximin) while implementing strict dominance pruning to discard mathematically inferior branches.
- **Information Set Monte Carlo Tree Search (IS-MCTS)**: An advanced tree-search algorithm. Before the standard 4-stage MCTS loop (Selection, Expansion, Simulation, Backpropagation), the algorithm determinizes the state. It uses the Decoupled UCT formula to navigate the simultaneous action space and converges on robust strategies over numerous random rollouts.
- **Greedy Baseline**: A deterministic heuristic agent that picks the move with the highest immediate expected damage (factoring in STAB, type effectiveness, and accuracy). 

## Features
- Headless, ultra-fast `GameState` engine designed specifically for AI tree-search simulations.
- Interactive GUI Mode utilizing **SFML** for visualizing agent battles.
- Automated AI Benchmarking tool (simulates hundreds of battles in milliseconds).
- Authentic Gen 3 combat engine (Nintendo's stat formula, speed checks, priority items, type effectiveness).

## Benchmarking Mode
When launching the application, you can bypass the GUI and run automated performance metrics:
1. **Greedy vs Greedy**
2. **SMAB vs Greedy**
3. **MCTS vs Greedy**

The benchmark outputs total wins, draws, and the average computational time per match.

## Controls (GUI Mode)
- **Up / Down Arrows**: Navigate menus
- **Z** or **Enter**: Select / Confirm / Advance text
- **X** or **Escape**: Go back / Cancel

## Dependencies & Installation
This project uses **SFML 3** for graphics and window management. The SFML libraries are vendored into the repository for portability, so you do not need `vcpkg`.

1. Open `pokemon/pokemon.slnx` or `pokemon/pokemon.vcxproj` in Visual Studio 2026.
2. Ensure the build configuration is set to `x64` (Debug or Release).
3. Build and Run the project!
