# 🏃‍♂️ Labyrinth Conquest 

A thrilling 3D maze game implementation in C where three players compete to capture the flag in a multi-floor maze filled with challenges, power-ups, and strategic obstacles!

## 🎮 Game Overview

**Labyrinth Conquest** is a **command-line interface (CLI)** turn-based strategy game featuring:

- 🏗️ **3-floor maze structure** with unique layouts
- 👥 **3 players** (A, B, C) competing simultaneously  
- 🎲 **Dual dice system** - movement dice and directional dice
- 🏴 **Flag capture objective** - first to reach wins!
- 🍽️ **Bawana area** with special effects
- ⚡ **Dynamic stairs** that change direction every 5 rounds
- 🕳️ **Poles** for quick descent between floors
- 🧱 **Walls** as strategic obstacles

## 🏢 Maze Structure

### Floor 0 (Ground Floor)
- **Area**: 856 sq ft (10×25 blocks)
- **Starting Area**: 4×9 blocks along south border
- **Special**: Contains Bawana area area

### Floor 1 (Second Floor) 
- **Area**: 784 sq ft
- **Layout**: Two 10×8 rectangular areas connected by a 4×9 bridge
- **Position**: Above the starting area

### Floor 2 (Third Floor)
- **Area**: 360 sq ft (10×9 blocks)
- **Position**: Above the starting area

## 🎲 Game Mechanics

### 🎯 Player Setup
- **Player A**: Starts at `[0,6,12]`, enters maze at `[0,5,12]`, faces NORTH
- **Player B**: Starts at `[0,9,8]`, enters maze at `[0,9,7]`, faces WEST  
- **Player C**: Starts at `[0,9,16]`, enters maze at `[0,9,17]`, faces EAST

### 🎲 Dice System
- **Movement Dice**: 1-6 (determines steps to move)
- **Direction Dice**: 4 directions + 2 empty faces
  - Face 1: Empty (keep current direction)
  - Face 2: North
  - Face 3: East
  - Face 4: South
  - Face 5: West
  - Face 6: Empty (keep current direction)

### 📏 Movement Rules
- Roll **6** on movement dice to enter maze from starting area
- Direction dice rolled every **4th turn** after entering maze
- Move only if entire path is clear (no partial movements)
- **100 movement points** to start, costs vary by cell type

### 🏗️ Special Elements

#### 🪜 Stairs
- Connect different floors (always lower to upper)
- Can be **unidirectional** or **bidirectional**
- Direction changes randomly every **5 rounds**
- Format: `[start_floor, start_w, start_l, end_floor, end_w, end_l]`

#### 🕳️ Poles  
- **One-way descent** from higher to lower floors
- Same position across all floors they traverse
- Format: `[start_floor, end_floor, width, length]`

#### 🧱 Walls
- Impassable straight-line obstacles
- Confined to single floor
- Format: `[floor, start_w, start_l, end_w, end_l]`

### 🍽️ Bawana area Effects

Located at `[0, 7-9, 21-24]` with entrance at `[0, 9, 19]`:

- 🤢 **Food Poisoning** (2 cells): Miss 3 turns, 0 movement points
- 😵 **Disoriented** (2 cells): Random movement for 4 turns, 50 movement points
- 😡 **Triggered** (2 cells): Move 2x speed, 50 movement points
- 😊 **Happy** (2 cells): No side effects, 200 movement points
- 🍎 **Normal** (4 cells): Random 10-100 movement points

### 💰 Cell Types & Movement Points

- **25%** Zero cost cells (0 points)
- **35%** Cost cells (1-4 points deducted)
- **25%** Bonus cells (+1 to +2 points)
- **10%** Big bonus cells (+3 to +5 points)  
- **5%** Multiplier cells (×2 or ×3 points)

### ⚔️ Combat System
- Land on opponent's cell = **capture them**
- Captured players return to starting area
- Movement points and effects reset for captured players

## 📋 Installation & Setup

### 🐧 Linux Users (Recommended)

#### Prerequisites
- GCC compiler
- Make utility

#### Installation Steps

1. **Install required tools** (if not already installed):
```bash
# Ubuntu/Debian
sudo apt update
sudo apt install gcc make

# CentOS/RHEL/Fedora
sudo yum install gcc make
# OR for newer versions
sudo dnf install gcc make

# Arch Linux
sudo pacman -S gcc make
```

2. **Compile and run**:
```bash
# Compile the project
make

# Run the game with logging
make run
```

### 🪟 Windows Users

Since `make` is primarily a Linux/Unix utility, Windows users have several options:

#### Option 1: Using MinGW-w64 (Recommended)

1. **Install MinGW-w64**:
   - Download from [MinGW-w64](https://www.mingw-w64.org/downloads/)
   - Or use [w64devkit](https://github.com/skeeto/w64devkit/releases) (portable)
   - Add the `bin` directory to your PATH

2. **Compile manually**:
```cmd
gcc -Wall -Wextra -std=c11 -c main.c -o main.o
gcc -Wall -Wextra -std=c11 -c maze.c -o maze.o
gcc main.o maze.o -o maze_runner.exe -lm
```

3. **Run the game**:
```cmd
maze_runner.exe
```

#### Option 2: Using MSYS2

1. **Install MSYS2** from [https://www.msys2.org/](https://www.msys2.org/)

2. **Install GCC and Make**:
```bash
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-make
```

3. **Use the Makefile**:
```bash
make
make run
```

#### Option 3: Using WSL (Windows Subsystem for Linux)

1. **Enable WSL** and install a Linux distribution
2. **Follow the Linux instructions** above

### 🍎 macOS Users

1. **Install Xcode Command Line Tools**:
```bash
xcode-select --install
```

2. **Compile and run**:
```bash
# Compile manually (no make needed)
gcc -Wall -Wextra -std=c11 main.c maze.c -o maze_runner -lm

# Run the game
./maze_runner
```

## 📁 Required Input Files

Create these files in the project directory:

#### `stairs.txt`
```
[0, 7, 5, 1, 7, 5]
[1, 8, 18, 2, 8, 15]
[0, 3, 3, 1, 3, 3]
[0, 6, 22, 1, 6, 22]
[0, 9, 19, 1, 9, 19]
# Add more stairs as needed
```

#### `poles.txt`
```
[2, 0, 9, 12]
[2, 1, 7, 10]
[1, 0, 8, 3]
[2, 0, 6, 20]
# Add more poles as needed
```

#### `walls.txt`
```
[0, 2, 10, 3, 10]
[0, 9, 5, 9, 6]
[1, 6, 0, 7, 0]
[1, 8, 20, 8, 21]
# Add more walls as needed
```

#### `flag.txt`
```
[2, 4, 11]
```

#### `seed.txt`
```
12345
```

## 🎮 How to Play

1. **Launch the game** from command line
2. **Game starts** with all players in starting area
3. **Roll dice** to move (need 6 to enter maze initially)
4. **Navigate** through the 3-floor maze avoiding obstacles
5. **Use stairs and poles** strategically to change floors
6. **Manage movement points** - visit Bawana when depleted
7. **Capture opponents** by landing on their cell
8. **Reach the flag** first to win! 🏆

## 📊 Game Output & Logging

The game runs entirely in the **command-line interface (CLI)** and provides:

- **Real-time gameplay output** showing all player actions
- **Detailed movement information** with positions and points
- **Special effects notifications** for Bawana and captures
- **Comprehensive logging** saved to `log_file.txt`

### 📝 Log File
When using `make run`, all game output is automatically saved to `log_file.txt`. This allows you to:
- **Review the entire game** after completion
- **Analyze player strategies** and movements
- **Debug any issues** or unexpected behavior
- **Share game results** with others

To view the log file:
```bash
# Linux/macOS
cat log_file.txt
# or
less log_file.txt

# Windows
type log_file.txt
# or open with any text editor
```

## 🎨 Customization Options

### 🏗️ Modify Game Elements

You can customize the game by editing the input files:

#### **Change Maze Layout**
- **Walls**: Edit `walls.txt` to create different obstacle patterns
- **Stairs**: Modify `stairs.txt` to change floor connections
- **Poles**: Update `poles.txt` for different descent routes
- **Flag**: Change `flag.txt` to place the winning objective elsewhere

#### **Change Game Randomization**
- **Seed**: Edit `seed.txt` with any integer to get different:
  - Cell type distributions (bonus/cost cells)
  - Bawana effect placements
  - Random direction changes when disoriented
  - Stair direction changes

### 📐 Format Guidelines
- **Coordinates**: `[floor, width, length]` where:
  - Floor: 0-2
  - Width: 0-9
  - Length: 0-24
- **All elements must be within playable areas**
- **Invalid entries will be skipped with error messages**

## 🧠 Developer Assumptions & Implementation Details

Since the original specification didn't cover all implementation details, several assumptions were made:

### 🎯 **Movement Mechanics**
- **Direction persistence**: Players maintain direction until direction dice is rolled
- **Blocked movement penalty**: 2 movement points deducted when blocked by walls
- **Partial movement**: Not allowed - must complete full movement or stay in place
- **Starting area logic**: Players need exactly 6 to enter, no movement cost while in starting area

### 🏗️ **Stairs & Poles Logic**
- **Maximum limits**: Max 2 stairs and 1 pole from same cell (prevents overcrowding)
- **Loop detection**: Comprehensive algorithm prevents infinite stair/pole loops
- **Direction changes**: All stairs change direction simultaneously every 5 rounds
- **Intermediate floors**: Stairs create blocked cells on intermediate floors they pass through
- **Closest destination**: When multiple stairs/poles available, choose closest to flag (Manhattan distance)
- **Tie-breaking**: Random selection when distances are equal

### 🍽️ **Bawana area**
- **Entrance logic**: Players always exit facing NORTH from entrance `[0,9,19]`
- **Effect distribution**: Exactly 2 cells each for food poisoning, disoriented, triggered, happy + 4 normal cells
- **Triggered effect**: Doubles movement distance (4 on dice = 8 cells moved)
- **Food poisoning**: After 3 rounds, player is randomly placed on another Bawana cell
- **Movement point reset**: Only happens when explicitly stated, not for captures/returns

### 💰 **Movement Points System**
- **Zero/negative points**: Automatic transport to Bawana
- **Multiplier cells**: Multiply the movement cost, not the total points
- **Bonus calculation**: Applied after cost calculation
- **Point persistence**: Retained when returning to starting area via poles/stairs

### 🎲 **Dice & Randomization**
- **Direction dice timing**: Every 4th turn after entering maze (not while in starting area)
- **Empty faces**: Player keeps current direction (faces 1 and 6)
- **Disoriented randomization**: Overrides direction dice, selects from all 4 directions
- **Seeded randomization**: Ensures reproducible games for testing

### ⚔️ **Combat & Interactions**
- **Capture conditions**: Only when landing exactly on opponent's cell
- **Jumping over**: Players can move through cells with opponents without capture
- **Reset conditions**: Captured players lose all Bawana effects and reset movement points
- **Starting area return**: Via poles/stairs retains current movement points

### 🏆 **Win Conditions**
- **Flag capture**: Immediate win upon reaching flag coordinates
- **No ties**: First player to reach flag wins instantly
- **Game termination**: All output completed before program exit

These assumptions ensure consistent gameplay while maintaining the strategic depth and complexity intended by the original design! 🎪

## 🔧 Build Commands

```bash
# Linux with Make
make           # Compile
make run       # Run with logging
make clean     # Clean build files

# Manual compilation (all platforms)
gcc -Wall -Wextra -std=c11 main.c maze.c -o maze_runner -lm

# Run manually
./maze_runner  # Linux/macOS
maze_runner    # Windows
```

## 🏗️ Code Structure

- **`main.c`**: Game initialization and main game loop
- **`maze.c`**: Core game mechanics and logic implementation  
- **`maze.h`**: Structures, enums, and function declarations
- **`Makefile`**: Build configuration (Linux only)

### 🏛️ Architecture
- **Modular design** with separated concerns
- **Comprehensive logging** system with file output
- **Memory management** with proper cleanup
- **Input validation** for all game files
- **Loop detection** for stairs/poles combinations

## 🎯 Features Implemented

- ✅ Complete 3-floor maze system with CLI interface
- ✅ Full dice mechanics with direction changes  
- ✅ Bawana area with all 5 effect types
- ✅ Dynamic stair direction changes every 5 rounds
- ✅ Player capture system with proper resets
- ✅ Movement point economy with various cell types
- ✅ Loop detection for stairs/poles combinations
- ✅ Comprehensive input validation and error handling
- ✅ Detailed game state logging to file
- ✅ Cross-platform compatibility

## 🎪 Fun Facts

- The game simulates a **2,000 sq ft building** across 3 floors!
- Each block represents **4 square feet** of real space
- Players can theoretically get stuck in **infinite loops** (but the code prevents this!)
- **Bawana area** serves both delicious and dangerous food 🍽️💀
- Stairs can **change direction mid-game**, adding strategic complexity
- The **log file** can grow quite large during long games - perfect for analysis! 📈

---

*Ready to conquer the maze? May the best player win! 🏆*

**Note** : This is a CLI-based game - no graphics, just pure strategic text-based gameplay! 🖥️
