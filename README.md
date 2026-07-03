# Pokemon Trainer Move Selection AI

A C++ based Pokemon battle simulator and AI research environment, testing simultaneous turn-based move selection using Gen 3 combat mechanics.

## AI Algorithms Implemented
This project evaluates advanced game-theoretic algorithms against a standard heuristic baseline in a 6v6 simultaneous-move environment:

- **Greedy Baseline**: A deterministic heuristic agent that picks the move with the highest immediate expected damage (factoring in STAB, type effectiveness, and accuracy). 

- **Simultaneous Move Alpha-Beta (SMAB)**: Bypasses standard Minimax's structural inseparability in simultaneous games. Every decision node represents a zero-sum payoff matrix where the agent computes the maximum worst-case scenario (Maximin).
  - *Expectiminimax*: Navigates the RNG of move accuracy by branching the game tree into deterministic hit/miss timelines, weighting the expected payoff by the probability of the move landing.
  - *Heuristic Tuning*: Evaluates raw HP differentials rather than percentages to accurately protect bulky Pokemon, and avoids artificial "Fear of Success" penalties by naturally calculating the threat of incoming enemy counters.

- **Information Set Monte Carlo Tree Search (IS-MCTS)**: An advanced tree-search algorithm utilizing the Decoupled UCT formula to navigate the simultaneous action space. Because it uses 50-turn rollouts.
  - *Guided Opponent Modeling*: Instead of assuming the opponent plays randomly, MCTS models the opponent using the `GreedyAgent` logic at the root, forcing it to correctly respect devastating enemy attacks.
  - *Heuristic-Guided Rollouts with Sigmoid Evaluation*: Replaces useless "random playouts" with highly-realistic 50-turn Greedy vs Greedy simulations. It converts the Delta HP generated during the rollout into a continuous 0.0 to 1.0 Win Probability using a Sigmoid function (aka S shape curve), mathematically teaching the agent how to claw back from massive HP deficits or execute long-term 50-turn defensive stall strategies. 

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
