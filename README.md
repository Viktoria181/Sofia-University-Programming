#Chess Game in C++
This project is a simple implementation of a chess-like game in C++. It features a board with rooks (pieces labeled A and B) and a player king (piece labeled K), along with an enemy king (piece labeled E). The objective of the game is to maneuver the player king to capture the enemy king while avoiding potential threats from the enemy rooks.

Features
Board size selection: Choose between different board sizes (minimum 6x6 and maximum 30x30).
Randomized piece placement: The rooks and player king are randomly placed on the board at the start of each game.
Movement validation: The game validates player moves to ensure they comply with chess rules and constraints.
Enemy AI: The enemy king is controlled by a simple AI that makes random moves while attempting to avoid direct threats from the player.
Instructions
Starting the Game

Run the program and follow the on-screen prompts.
Press [S] to start the game.
Alternatively, press [D] to choose custom board dimensions.
Playing the Game

After starting the game, you'll be prompted to select a piece to move (A, B, or K) and the destination row and column.
Enter the piece label (e.g., A, B, or K), the row, and the column when prompted.
The game will display the updated board after each move.
The game ends when the player captures the enemy king or when the enemy captures the player king.
Exiting the Game

To exit the game at any time, press any key other than [S] or [D].
Notes
Ensure that your terminal or command prompt window is large enough to display the entire game board.
This implementation is a simplified version of chess and does not include all chess rules and features.
