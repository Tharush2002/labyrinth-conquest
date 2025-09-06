#include "maze.h"

Block flag;
Stair *stairs = NULL;
Pole *poles = NULL;
Wall *walls = NULL;

Block maze[FLOORS][WIDTH][LENGTH];

int stairs_count, poles_count, walls_count;

void init_game(){
	load_stairs("stairs.txt");
	load_poles("poles.txt");
	load_walls("walls.txt");
	load_flag("flag.txt");

	init_maze();
}

void init_maze(){
	for(int f=0; f<FLOORS; f++){
		for(int w=0; w<WIDTH; w++){
			for(int l=0; l<LENGTH; l++){
				Block b = { -1, -1, -1, -1, -1 };

				// If the block is blocked by wall or stair or out of playable area, mark it as invalid block
				if(is_blocked_by_wall(f, w, l) || is_blocked_by_stair(f, w, l) || !is_in_the_playable_area(f, w, l)){
					maze[f][w][l] = b;
					continue;
				}

				// Initialize all blocks as normal blocks
				b.floor = f;
				b.width_num = w;
				b.length_num = l;
				b.type = NORMAL;
				b.consume_value = 0;

				maze[f][w][l] = b;
			}
		}
	}
}

int is_blocked_by_wall(int floor, int width_num, int length_num){
	for(int i=0; i<walls_count; i++){
		if(walls[i].floor == floor){
			if((width_num >= walls[i].start_width_num && width_num <= walls[i].end_width_num) &&
			   (length_num >= walls[i].start_length_num && length_num <= walls[i].end_length_num)){
				return 1;
			}
		}
	}
	return 0; 
}

int is_blocked_by_stair(int floor, int width_num, int length_num){
	for(int i=0; i<stairs_count; i++){
		int upper = (stairs[i].start_floor > stairs[i].end_floor) ? stairs[i].start_floor : stairs[i].end_floor;
		int lower = (stairs[i].start_floor < stairs[i].end_floor) ? stairs[i].start_floor : stairs[i].end_floor;

		if((abs(stairs[i].start_floor-stairs[i].end_floor)>1) && (floor > lower && floor < upper)){
			return 1;
		}
	}
	return 0; 
}

int is_in_the_playable_area(int floor, int width, int length){
	switch(floor){
		case 0:
			if(((width>=6 && width<WIDTH) && (length>=8 && length<=16)) || ((width>=6 && width<WIDTH) && (length>=20 && length<LENGTH))){
				return 0;
			}
		case 1:
			if((width>=0 && width<=5) && (length>=8 && length<=16)){
				return 0;
			}
		case 2:
			if(!(length>=8 && length<=16)){
				return 0;
			}
	}
	return 1;
}

void load_stairs(const char *stairs_file){
	FILE *fp = fopen(stairs_file, "r");
	if(fp == NULL){
		printf("Error opening stairs.txt\n");
		return;
	}

	int start_floor, start_width_num, start_length_num, end_floor, end_width_num, end_length_num;
	int n = 0;
	char buffer[100];

	while(1){
		int matched = fscanf(fp, "[%d, %d, %d, %d, %d, %d]", &start_floor, &start_width_num, &start_length_num, &end_floor, &end_width_num, &end_length_num);

		if(matched == EOF){
			break;
		}else if(matched != 6){
			fgets(buffer, sizeof(buffer), fp);
            printf("Error : Skipping error line stairs.txt (got %d values but expect 6)\n", matched);
			continue;
		}else if(!is_in_the_playable_area(start_floor, start_width_num, start_length_num) || !is_in_the_playable_area(end_floor, end_width_num, end_length_num)){
			printf("Error : Skipping error line stairs.txt (stairs out of playable area)\n");
			continue;
		}

		Stair *temp = realloc(stairs, (n+1)*sizeof(Stair));
		if (temp == NULL) {
    		printf("Memory allocation failed for stairs array\n");
    		free(stairs);
    		fclose(fp);
   			return;
		}
		stairs = temp;

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
	char buffer[100];

	while(1){
		int matched = fscanf(fp, "[%d, %d, %d, %d]", &start_floor, &end_floor, &width_num, &length_num);
		
		if(matched == EOF){
			break;
		}else if(matched != 4){
			fgets(buffer, sizeof(buffer), fp);
			printf("Error : Skipping error line poles.txt (got %d values but expect 4)\n", matched);
			continue;
		}else if(!is_in_the_playable_area(start_floor, width_num, length_num)){
			printf("Error : Skipping error line poles.txt (poles out of playable area)\n");
			continue;
		}

		Pole *temp = realloc(poles, (n+1)*sizeof(Pole));
		if (temp == NULL) {
    		printf("Memory allocation failed for poles array\n");
    		free(stairs);
    		fclose(fp);
   			return;
		}
		poles = temp;

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
	char buffer[100];

	while(1){
		int matched = fscanf(fp, "[%d, %d, %d, %d, %d]", &floor, &start_width_num, &start_length_num, &end_width_num, &end_length_num);

		if(matched == EOF){
			break;
		}else if(matched != 5){
			fgets(buffer, sizeof(buffer), fp);
			printf("Error : Skipping error line walls.txt (got %d values but expect 5)\n", matched);
			continue;
		}

		Wall *temp = realloc(walls, (n+1)*sizeof(Wall));
		if (temp == NULL) {
			printf("Memory allocation failed for walls array\n");
			free(walls);
			fclose(fp);
   			return;
		}
		walls = temp;

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
	}else{
		printf("Error : No matched cell found for flag in flag.txt\n");
	}
	fclose(fp);
}

void change_stair_direction(){
	for(int i=0; i<stairs_count; i++){
		int temp_floor = stairs[i].start_floor;
		int temp_width_num = stairs[i].start_width_num;
		int temp_length_num = stairs[i].start_length_num;

		stairs[i].start_floor = stairs[i].end_floor;
		stairs[i].start_width_num = stairs[i].end_width_num;
		stairs[i].start_length_num = stairs[i].end_length_num;

		stairs[i].end_floor = temp_floor;
		stairs[i].end_width_num = temp_width_num;
		stairs[i].end_length_num = temp_length_num;
	}
}

Direction get_direction(int direction_dice){
	switch(direction_dice){
		case 2:
			return NORTH;
		case 3:
			return EAST;
		case 4:
			return SOUTH;
		case 5:
			return WEST;
		default:
			return NA; 
	}
}

int roll_dice(){
	return (rand() % 6) + 1;
}