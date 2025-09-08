# C-Hess
A simple chess game built in C using SDL2. This is still very much a work in progress.

## Compiling
To compile the project, make sure you have SDL2 installed on your system. Then, run the following command in the terminal:

```bash
make
```

This will generate an executable file named `chess.out` in the `./out` directory.

## Running
To run the game, execute the following command in the terminal:

```bash
./out/chess.out
```

## Controls
- Escape: Exit the game
- Space: Pause/Unpause the game

## Goals
- Implement full chess rules (check, checkmate, stalemate, castling, en passant, promotion)
- Implement 2-player mode on the same device
- Implement AI opponent -> Minimax algorithm with alpha-beta pruning, maybe even neural networks down the line
- Implement online multiplayer mode (if i get to it and don't get bored of this project)
