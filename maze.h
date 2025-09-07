#ifndef MAZE_H
#define MAZE_H

#include<stdio.h>
#include<stdlib.h>

#define FLOORS 3
#define WIDTH 10
#define LENGTH 25

#define BAWANA_FOOD_POISONING_ROUNDS 3
#define BAWANA_DISORIENTED_ROUNDS 4
#define BAWANA_DISORIENTED_BONUS 50
#define BAWANA_TRIGGERED_BONUS 50
#define BAWANA_TRIGGERED_MULTIPLIER 2
#define BAWANA_HAPPY_BONUS 200
#define INITIAL_MOVEMENT_POINTS 100

#define MAX_STAIRS_FROM_SAME_CELL 2
#define MAX_POLES_FROM_SAME_CELL 1

typedef enum { NA, NORTH, SOUTH, EAST, WEST } Direction;
typedef enum { NA, FOOD_POISONING, DISORIENTED, TRIGGERED, HAPPY}BawanaState;
typedef enum { NORMAL, COST, BONUS, MULTIPLIER } ConsumeType;
typedef enum { STAIR, POLE, WALL, NORMAL} BlockType;

typedef struct{
	Player player;
	int round;
	int movement_dice;
	Direction direction_dice;
}Game;

typedef struct{
	char player_name;
	Block *current_block;
	Direction direction;
	int rem_points;
	Bawana bawana_effect;
}Player;

typedef struct{
	BawanaState state;
	int block_width_num;
	int block_length_num;
	int effect_rounds;
}Bawana;

typedef struct{
	int floor;
	int width_num;
	int length_num;
	ConsumeType type;
	int consume_value;
}Block;

typedef struct{
	int start_floor;
	int start_width_num;
	int start_length_num;	
	int end_floor;
	int end_width_num;
	int end_length_num;
}Stair;

typedef struct{
	int start_floor;
	int end_floor;
	int width_num;
	int length_num;
}Pole;

typedef struct{
	int floor;
	int start_width_num;
	int start_length_num;	
	int end_width_num;
	int end_length_num;
}Wall;

extern Block flag;
extern Stair *stairs;
extern Pole *poles;
extern Wall *walls;

extern Block maze[FLOORS][WIDTH][LENGTH];
extern Game game_state;

// MAIN FUNCTIONS

void init_game();
void init_maze();
Block* move_through_stair_or_pole(int floor, int width, int length);
void change_stair_direction();
int move_piece(Block *current_block);

// HELPER FUNCTIONS

int roll_dice();
Direction get_direction(int direction_dice);
int stairs_from_cell(int floor, int width_num, int length_num, Stair *out[]);
int poles_from_cell(int floor, int width_num, int length_num, Pole *out[]);
int is_blocked_by_wall(int floor, int width_num, int length_num);
int is_blocked_by_stair(int floor, int width_num, int length_num);
int is_in_the_playable_area(int floor, int width, int length);
int can_move_entirely(Block *current_block);
void mark_loops(BlockType type, int current_index,
				int visited_s[], int visited_p[],
				Stair *s[], Pole *p[], int non_looping_s[], int non_looping_p[]);
void load_stairs(const char *stairs_file);
void load_poles(const char *poles_file);
void load_walls(const char *walls_file);
void load_flag(const char *flag_file);

#endif
