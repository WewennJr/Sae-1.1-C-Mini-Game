# 🎮 Two-Player Strategy Game (C)

Small turn-based strategy game developed in C as part of a SAÉ project.

---

## 🧠 Concept

The game is played by **two players** on a **6×6 board**.

Each player controls **6 pieces of different sizes**.  
The objective is simple:

👉 Move **one piece to the opposite side of the board** to win.

Players take turns moving their pieces strategically.

---

## ⚙️ Compilation and launch

To compile and run the game:

```bash
gcc -Wall -c board.c -o board.o
gcc -Wall board.o -o game
./game
```

---

## 🎲 Game flow

### 1. Setup

At the beginning of the game, each player places their pieces alternately:

- 2 pieces of each size
- Placement by column (1 to 6)
- The game automatically chooses the row on the player’s side

🟢 Size 1 → movement of 1 tile  
🟡 Size 2 → movement of 2 tiles  
🔴 Size 3 → movement of 3 tiles  

---

### 2. Turn

On each turn, a player:

1. Selects a piece (row + column)
2. Chooses a possible direction:
   - ↑ North  
   - ↓ South  
   - → East  
   - ← West  
3. Moves the piece according to its size (maximum number of tiles)

Special commands:
- `A` → cancel the move
- `R` → undo the last step

---

### 3. Movement and rules

- A piece can only move if the move is valid
- The game automatically checks collisions and boundaries
- Invalid moves are rejected

---

### 4. Swap (special exchange)

If a piece ends its move on another piece:

- The player can choose to perform a **swap**
- Confirmation:
  - `O` → yes
  - `N` → no
- Then select the piece to swap with

---

### 5. End of the game

The game ends when a player reaches the opposite side of the board with a piece.

🎉 A victory message is displayed.

---

## 🛠️ Project structure

- `main.c` : main game loop
- `board.c` : board logic and game rules
- `board.h` : definitions and structures
- `game` : compiled executable

---

## 💡 Tips

- Anticipate opponent moves
- Manage large pieces carefully
- Use `A` and `R` if you make mistakes
- Control key positions on the board

---

## 🚀 Project goal

This project was made to practice:

- C programming
- Data structures and game logic
- Board management
- GCC compilation
