#ifndef MAZE_H
#define MAZE_H

#include<stdio.h>
#include<stdlib.h>

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

extern Block goal;
extern Stair *stairs;
extern Pole *poles;
extern Walls *walls;

void init_game();
void load_stairs();
void load_poles();
void load_walls();
void load_flag();

#endif
