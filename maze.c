#include "maze.h"

Block flag;
Stair *stairs;
Pole *poles;
Wall *walls;

int stairs_count, poles_count, walls_count;

void init_game(){
	load_stairs("stairs.txt");
	load_poles("poles.txt");
	load_walls("walls.txt");
	load_flag("flag.txt");
}

void load_stairs(const char *stairs_file){
	FILE *fp = fopen(stairs_file, "r");
	if(fp == NULL){
		printf("Error opening stairs.txt\n");
		return;
	}

	int start_floor, start_width_num, start_length_num, end_floor, end_width_num, end_length_num;
	int n = 0;

	while(fscanf(fp, "[%d, %d, %d, %d, %d, %d]", &start_floor, &start_width_num, &start_length_num, &end_floor, &end_width_num, &end_length_num) != EOF){
		if(n==0){
			stairs = (Stair *)malloc(sizeof(Stair));
		}else{
			stairs = realloc(stairs, (n+1)*sizeof(Stair));
		}
		stairs[n].start_floor = start_floor;
		stairs[n].start_width_num = start_width_num;
		stairs[n].start_length_num = start_length_num;
		stairs[n].end_floor = end_floor;
		stairs[n].end_width_num = end_width_num;
		stairs[n].end_length_num = end_length_num;
	
		n++;		
	}
	stairs_count = ++n;
	fclose(fp);
}

void load_poles(const char *poles_file){
	FILE *fp = fopen(poles_file, "r");
	if(fp == NULL){
		printf("Error opening poles.txt\n");
		return;
	}
	
	int start_floor, end_floor, width_num, length_num;
	int n = 0;

	while(fscanf(fp, "[%d, %d, %d, %d]", &start_floor, &end_floor, &width_num, &length_num) != EOF){
		if(n==0){
			poles = (Pole *)malloc(sizeof(Pole));
		}else{
			poles = realloc(poles, (n+1)*sizeof(Pole));
		}
		poles[n].start_floor = start_floor;
		poles[n].end_floor = end_floor;
		poles[n].width_num = width_num;
		poles[n].length_num = length_num;
	
		n++;
	}	
	poles_count = ++n;
	fclose(fp);	
}

void load_walls(const char *walls_file){
	FILE *fp = fopen(walls_file, "r");
	if(fp == NULL){
		printf("Error opening walls.txt\n");
		return;
	}

	int floor, start_width_num, start_length_num, end_width_num, end_length_num;
	int n = 0;

	while(fscanf(fp, "[%d, %d, %d, %d, %d]", &floor, &start_width_num, &start_length_num, &end_width_num, &end_length_num) != EOF){
		if(n==0){
			walls = (Wall *)malloc(sizeof(Wall));
		}else{
			walls = realloc(stairs, (n+1)*sizeof(Wall));
		}
		walls[n].floor = floor;
		walls[n].start_width_num = start_width_num;
		walls[n].start_length_num = start_length_num;
		walls[n].end_width_num = end_width_num;
		walls[n].end_length_num = end_length_num;
	
		n++;		
	}
	walls_count = ++n;
	fclose(fp);
}

void load_flag(const char *flag_file){
	FILE *fp = fopen(flag_file, "r");
	if(fp == NULL){
		printf("Error opening flag.txt\n");
		return;
	}

	int floor, width_num, length_num;

	if(fscanf(fp, "[%d, %d, %d]", &floor, &width_num, &length_num) == 3){
		flag.floor = floor;
		flag.width_num = width_num;
		flag.length_num = length_num;
	}
	fclose(fp);
}
