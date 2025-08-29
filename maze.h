#ifndef MAZE_H
#define MAZE_H

typedef struct{
	int floor;
	int block_width_num;
	int block_length_num;
}block;

typedef struct{
	int start_floor;
	int start_block_width_num;
	int start_block_length_num;	
	int end_floor;
	int block_cell_width_num;
	int block_cell_length_num;
}stair;

typedef struct{
	int start_floor;
	int end_floor;
	int block_width_num;
	int block_length_num;
}pole;

typedef struct{
	int floor;
	int start_block_width_num;
	int start_block_length_num;	
	int end_block_width_num;
	int end_block_length_num;
}wall;

typedef struct{
	int floor;
	int block_width_num;
	int block_length_num;
}goal;

#endif
