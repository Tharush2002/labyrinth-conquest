#ifndef MAZE_H
#define MAZE_H

#include<stdio.h>
#include<stdlib.h>

#define FLOORS 3
#define WIDTH 10
#define LENGTH 25

typedef struct{
	Block block;
	int consume;
}Cell;

typedef struct{
	int floor;
	int width_num;
	int length_num;
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

void init_game();
void load_stairs(const char *stairs_file);
void load_poles(const char *poles_file);
void load_walls(const char *walls_file);
void load_flag(const char *flag_file);

#endif
