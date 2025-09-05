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

typedef enum { NA=-1, NORTH, SOUTH, EAST, WEST } Direction;
typedef enum { NA=-1, FOOD_POISONING, DISORIENTED, TRIGGERED, HAPPY}BawanaState;
typedef enum { NORMAL, COST, BONUS, MULTIPLIER } ConsumeType;

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

void init_game();
void load_stairs(const char *stairs_file);
void load_poles(const char *poles_file);
void load_walls(const char *walls_file);
void load_flag(const char *flag_file);

#endif
