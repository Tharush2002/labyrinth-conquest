#include "maze.h"

Block flag;
Stair *stairs = NULL;
Pole *poles = NULL;
Wall *walls = NULL;

Block maze[FLOORS][WIDTH][LENGTH];
Game game_state;

int stairs_count, poles_count, walls_count;

// MAIN FUNCTIONS

void init_game(){
	// load_walls("walls.txt");
	// load_stairs("stairs.txt");
	// load_poles("poles.txt");
	// load_flag("flag.txt");

	init_maze();
	assign_consumables();
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

void assign_consumables(){
	int count = 0;

	for(int f=0; f<FLOORS; f++){
		for(int w=0; w<WIDTH; w++){
			for(int l=0; l<LENGTH; l++){
				if(maze[f][w][l].floor != -1 && 
					maze[f][w][l].width_num != -1 && 
					maze[f][w][l].length_num != -1 &&
					maze[f][w][l].type != -1 &&
					maze[f][w][l].consume_value != -1){
					count++;
				}
			}
		}
	}

	printf("Total valid blocks: %d\n", count);
}

void change_stair_direction(){
	StairDirection rand_dir = (Direction)rand()%3;

	switch(rand_dir){
		case UNI_DOWN:
			for(int i=0; i<stairs_count; i++){
				if(stairs[0].start_floor < stairs[0].end_floor){
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
				stairs[i].direction = UNI_DOWN;
			}
		case UNI_UP:
			for(int i=0; i<stairs_count; i++){
				if(stairs[0].start_floor > stairs[0].end_floor){
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
				stairs[i].direction = UNI_UP;
			}			
			break;
		case BI:
			for(int i=0; i<stairs_count; i++){
				stairs[i].direction = BI;
			}
	}
}

int move_piece(Block *current_block){
	if(can_move_entirely(current_block)){
		// switch(game_state.direction_dice){
		// 	case NORTH:
		// 		current_block->width_num -= game_state.movement_dice;
		// 		break;
		// 	case SOUTH:
		// 		current_block->width_num += game_state.movement_dice;
		// 		break;
		// 	case EAST:
		// 		current_block->length_num += game_state.movement_dice;
		// 		break;
		// 	case WEST:
		// 		current_block->length_num -= game_state.movement_dice;
		// 		break;
		// 	default:
		// 		printf("Invalid direction!\n");
		// 		return 0;
		// }
		// return 1;
		printf("Move successful!\n");
		return 1;
	}else{
		printf("Move blocked!\n");
		return 0;
	}
}



// HELPER FUNCTIONS

int roll_dice(){
	return (rand() % 6) + 1;
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
			return DIR_NA; 
	}
}

int stairs_from_cell(int floor, int width, int length, Stair *out[]){
	int count = 0;
	for(int i=0; i<stairs_count; i++){
		if((stairs[i].start_floor == floor) && (stairs[i].start_width_num == width) && (stairs[i].start_length_num == length)){
			out[count++] = &stairs[i];
		}
	}
	return count;
}

int poles_from_cell(int floor, int width, int length, Pole *out[]){
	int count = 0;
	for(int i=0; i<poles_count; i++){
		if((poles[i].start_floor == floor) && (poles[i].width_num == width) && (poles[i].length_num == length)){
			out[count++] = &poles[i];
		}
	}
	return count;
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
		int f_difference = abs(stairs[i].start_floor - stairs[i].end_floor);

		if(f_difference < 2 && !(floor > lower && floor < upper)) continue;

		if(width_num == stairs[i].start_width_num && 
			length_num == stairs[i].start_length_num && 
			length_num == stairs[i].end_length_num && 
			width_num == stairs[i].end_width_num){
			return 1;
		}else{
			for(int i = lower+1 ; i<upper ; i++){
				//
			}
		}
	}
	return 0; 
}

int is_in_the_playable_area(int floor, int width, int length){
	// Check if the coordinates are within the maze boundaries
	if(floor < 0 || floor >= FLOORS || width < 0 || width >= WIDTH || length < 0 || length >= LENGTH){
		return 0;
	}

	// Define the playable area for each floor
	switch(floor){
		case 0:
			if((width==6 && length>=20 && length<LENGTH) || (length==20 && width>=6 && width<WIDTH)){
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

int is_in_bawana_area(int floor, int width, int length){
	if((floor == 0) && (width > 6 && width < WIDTH) && (length > 20 && length < LENGTH)){
		return 1;
	}else{
		return 0;
	}
}

int is_in_starting_area(int floor, int width, int length){
	if((floor == 0) && (width >= 6 && width < WIDTH) && (length >= 8 && length <= 16)){
		return 1;
	}else{
		return 0;
	}
}

int can_move_entirely(Block *current_block){
	int f = current_block->floor;
	int w = current_block->width_num;
	int l = current_block->length_num;
	Block *block;

	for(int i=1 ; i<=game_state.movement_dice; i++){
		switch(game_state.direction_dice){
			case NORTH:
				w -= i;
				break;
			case SOUTH:
				w += i;
				break;
			case EAST:
				l += i;
				break;
			case WEST:
				l -= i;
				break;
			default:
				printf("Invalid direction!\n");
				return 0;
		}
		if(maze[f][w][l].floor == -1 && 
		   maze[f][w][l].width_num == -1 && 
		   maze[f][w][l].length_num == -1 &&
		   maze[f][w][l].type == -1 &&
		   maze[f][w][l].consume_value == -1){
			return 0;
			printf("Move blocked by wall!\n");
		}else if(!is_in_the_playable_area(f, w, l)){
			return 0;
			printf("Move out of bounds of the maze playable area!\n");
		}

		block = move_from_stair_or_pole(f, w, l);
		
		f = block->floor;
		w = block->width_num;
		l = block->length_num;		

		if(is_in_bawana_area(f, w, l)){
			printf("Move into Bawana area!\n");
			break;
		}else if(is_in_starting_area(f, w, l)){
			printf("Move into starting area!\n");
			break;
		}
	}

	set_destination_block(block);
	return 1;
}

void set_destination_block(Block *block){
	game_state.player.current_block = block;
}

Block* move_from_stair_or_pole(int floor, int width, int length){
	Pole *p[MAX_POLES_FROM_SAME_CELL] = {NULL};
	Stair *s[MAX_STAIRS_FROM_SAME_CELL] = {NULL};

	int s_count = stairs_from_cell(floor, width, length, s);
	int p_count = poles_from_cell(floor, width, length, p);

	if(p_count == 0 && s_count == 0){
		return &maze[floor][width][length];
	}else if(p_count == 1 && s_count == 0){
		return &maze[p[0]->end_floor][width][length];
	}else if(p_count == 0 && s_count == 1){
		return &maze[s[0]->end_floor][s[0]->end_width_num][s[0]->end_length_num];
	}else{
		int non_looping_s[MAX_STAIRS_FROM_SAME_CELL], non_looping_p[MAX_POLES_FROM_SAME_CELL];
		int visited_s[MAX_STAIRS_FROM_SAME_CELL] = {0};
		int visited_p[MAX_POLES_FROM_SAME_CELL] = {0};

		for (int i = 0; i < MAX_STAIRS_FROM_SAME_CELL; i++)	non_looping_s[i] = (s[i] != NULL) ? i : -1;
		for (int j = 0; j < MAX_POLES_FROM_SAME_CELL; j++)	non_looping_p[j] = (p[j] != NULL) ? j : -1;

		for (int i = 0; i < MAX_STAIRS_FROM_SAME_CELL; i++) {
			if (s[i] != NULL && !visited_s[i])
				mark_loops(STAIR, i, visited_s, visited_p, s, p, non_looping_s, non_looping_p);
		}

		for (int j = 0; j < MAX_POLES_FROM_SAME_CELL; j++) {
			if (p[j] != NULL && !visited_p[j])
				mark_loops(POLE, j, visited_s, visited_p, s, p, non_looping_s, non_looping_p);
		}

		return closest_sp_destination(s_count, p_count, non_looping_s, non_looping_p, floor, width, length);
	}
}

void mark_loops(BlockType type, int current_index,
                int visited_s[], int visited_p[],
                Stair *s[], Pole *p[], int non_looping_s[], int non_looping_p[]) {

    switch(type) {
        case STAIR:
			if (visited_s[current_index]) return; 
			visited_s[current_index] = 1;         

			for (int i = 0; i < MAX_STAIRS_FROM_SAME_CELL; i++) {
				if (s[i] != NULL) {
					if (is_stair_loop(s[current_index], s[i])) {
						non_looping_s[i] = -1;
						non_looping_s[current_index] = -1;
					}
					mark_loops(0, i, visited_s, visited_p, s, p, non_looping_s, non_looping_p);
				}
			}

			for (int j = 0; j < MAX_POLES_FROM_SAME_CELL; j++) {
				if (p[j] != NULL) {
					if (is_stair_pole_loop(s[current_index], p[j])) {
						non_looping_s[current_index] = -1;
						non_looping_p[j] = -1;
					}
					mark_loops(1, j, visited_s, visited_p, s, p, non_looping_s, non_looping_p);
				}
			}
			break;

		case POLE:
			if (visited_p[current_index]) return;  
			visited_p[current_index] = 1;          

			for (int i = 0; i < MAX_STAIRS_FROM_SAME_CELL; i++) {
				if (s[i] != NULL) {
					if (is_stair_pole_loop(s[i], p[current_index])) {
						non_looping_s[i] = -1;
						non_looping_p[current_index] = -1;
					}
					mark_loops(0, i, visited_s, visited_p, s, p, non_looping_s, non_looping_p);
				}
			}

			for (int j = 0; j < MAX_POLES_FROM_SAME_CELL; j++) {
				if (p[j] != NULL) {
					if (p[j]->end_floor == p[current_index]->start_floor &&
						p[current_index]->end_floor == p[j]->start_floor) {
						non_looping_p[j] = -1;
						non_looping_p[current_index] = -1;
					}
					mark_loops(1, j, visited_s, visited_p, s, p, non_looping_s, non_looping_p);
				}
			}
    }
}

int is_stair_loop(Stair *a, Stair *b) {
    if (a->end_floor == b->start_floor && a->start_floor == b->end_floor) {
        return 1;
    }

    if (a->direction == BI) {
        if ((a->start_floor == b->start_floor && a->end_floor == b->end_floor) ||
            (a->start_floor == b->end_floor && a->end_floor == b->start_floor)) {
            return 1;
        }
    }

    if (b->direction == BI) {
        if ((b->start_floor == a->start_floor && b->end_floor == a->end_floor) ||
            (b->start_floor == a->end_floor && b->end_floor == a->start_floor)) {
            return 1;
        }
    }

    return 0;
}

int is_stair_pole_loop(Stair *stair, Pole *pole) {
    if (pole->end_floor == stair->start_floor && stair->end_floor == pole->start_floor) {
        return 1;
    }

    if (stair->direction == BI) {
        if (pole->start_floor == stair->start_floor && pole->end_floor == stair->end_floor) {
            return 1;
        }
        if (pole->start_floor == stair->end_floor && pole->end_floor == stair->start_floor) {
            return 1;
        }
    }

    return 0;
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
		}else if(is_blocked_by_wall(start_floor, start_width_num, start_length_num) || is_blocked_by_wall(end_floor, end_width_num, end_length_num)){
			printf("Error : Skipping error line stairs.txt (stairs blocked by wall)\n");
			continue;
		}else{
			// Check if there are more than 2 stairs in the same cell
			int count = 0;
			for(int i=0; i<stairs_count; i++){
				if((stairs[i].start_floor == start_floor) && (stairs[i].start_width_num == start_width_num) && (stairs[i].start_length_num == start_length_num)){
					count++;
				}
				if((stairs[i].end_floor == end_floor) && (stairs[i].end_width_num == end_width_num) && (stairs[i].end_length_num == end_length_num)){
					count++;
				}
			}
			if(count >= MAX_STAIRS_FROM_SAME_CELL){
				printf("Error : Skipping error line stairs.txt (more than %d stairs in the same cell)\n", MAX_STAIRS_FROM_SAME_CELL);
				continue;
			}
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
		stairs[n].direction = BI;
	
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
		}else if(is_blocked_by_wall(start_floor, width_num, length_num) || 
			is_blocked_by_wall(end_floor, width_num, length_num) || 
			is_blocked_by_stair(start_floor, width_num, length_num) || 
			is_blocked_by_stair(end_floor, width_num, length_num)){

			printf("Error : Skipping error line poles.txt (poles blocked by wall)\n");
			continue;
		}else{
			// Check if there are more than designated poles in the same cell
			int count = 0;
			for(int i=0; i<poles_count; i++){
				if((poles[i].start_floor == start_floor) && (poles[i].width_num == width_num) && (poles[i].length_num == length_num)){
					count++;
				}
			}
			if(count >= MAX_POLES_FROM_SAME_CELL){
				printf("Error : Skipping error line poles.txt (more than 1 pole in the same cell)\n");
				continue;
			}
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
		}else if(!is_in_the_playable_area(floor, start_width_num, start_length_num) || !is_in_the_playable_area(floor, end_width_num, end_length_num)){
			printf("Error : Skipping error line walls.txt (walls out of playable area)\n");
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

int load_flag(const char *flag_file){
	FILE *fp = fopen(flag_file, "r");
	if(fp == NULL){
		printf("Error opening flag.txt\n");
		return;
	}

	int floor, width_num, length_num;

	if(fscanf(fp, "[%d, %d, %d]", &floor, &width_num, &length_num) == 3){
		if(is_in_the_playable_area(floor, width_num, length_num) 
			&& !is_blocked_by_stair(floor, width_num, length_num) 
			&& !is_blocked_by_wall(floor, width_num, length_num)){
			flag.floor = floor;
			flag.width_num = width_num;
			flag.length_num = length_num;
		}else{
			printf("Error : Flag out of playable area in flag.txt\n");
			return 0;
		}
	}else{
		printf("Error : No matched cell found for flag in flag.txt\n");
		return 0;
	}
	fclose(fp);
	return 1;
}