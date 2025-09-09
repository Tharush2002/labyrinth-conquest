#include "maze.h"

Block flag;
Stair *stairs = NULL;
Pole *poles = NULL;
Wall *walls = NULL;

Block maze[FLOORS][WIDTH][LENGTH];
Bawana bawana[BAWANA_SQUARES];

Game game_state;

int stairs_count, poles_count, walls_count;

// MAIN FUNCTIONS

void init_game(){
	// load_walls("walls.txt");
	// load_stairs("stairs.txt");
	// load_poles("poles.txt");
	// load_flag("flag.txt");

	init_maze();
}

void init_maze(){
	for(int f=0; f<FLOORS; f++){
		for(int w=0; w<WIDTH; w++){
			for(int l=0; l<LENGTH; l++){
				Block b = { -1, -1, -1, CONSUMER_NA, -1 };

				// If the block is blocked by wall or stair / out of playable area / in the bawana area / in the starting area mark it as invalid block
				if(is_blocked_by_wall(f, w, l) || 
					is_blocked_by_stair(f, w, l) || 
					!is_in_the_playable_area(f, w, l) || 
					is_in_bawana_area(f, w, l) ||
					is_in_starting_area(f, w, l)
					){
					maze[f][w][l] = b;
					continue;
				}

				// Initialize all blocks as normal blocks
				b = (Block){f, w, l, ZERO, 0};
				maze[f][w][l] = b;
			}
		}
	}
	assign_consumables();
	init_bawana();
}

void assign_consumables(){
	int consumable_blocks = 0;

	for(int f=0; f<FLOORS; f++){
		for(int w=0; w<WIDTH; w++){
			for(int l=0; l<LENGTH; l++){
				if(maze[f][w][l].floor != -1 && 
					maze[f][w][l].width_num != -1 && 
					maze[f][w][l].length_num != -1 &&
					maze[f][w][l].type != CONSUMER_NA &&
					maze[f][w][l].consume_value != -1){
					consumable_blocks++;
				}
			}
		}
	}

	Block *temp[consumable_blocks];
	int index = 0;

	for(int f=0; f<FLOORS; f++){
		for(int w=0; w<WIDTH; w++){
			for(int l=0; l<LENGTH; l++){
				if(maze[f][w][l].floor != -1 && 
					maze[f][w][l].width_num != -1 && 
					maze[f][w][l].length_num != -1 &&
					maze[f][w][l].type != CONSUMER_NA &&
					maze[f][w][l].consume_value != -1){

					temp[index++] = &maze[f][w][l]; 
				}
			}
		}
	}

	shuffle_array(temp, consumable_blocks, sizeof(Block*));

	int zero_consumable_blocks = consumable_blocks * 25 / 100;
	int one_to_four_consumable_blocks = consumable_blocks * 35 / 100;
	int one_to_two_bonus_blocks = consumable_blocks * 25 / 100;
	int three_to_five_bonus_blocks = consumable_blocks * 10 / 100;
	int multiplier_blocks = consumable_blocks * 5 / 100;

	index = 0;

	for(int i=0 ; i<zero_consumable_blocks ; i++, index++){
		temp[index]->type = ZERO;
		temp[index]->consume_value = 0;
	}

	for(int i=0 ; i<one_to_four_consumable_blocks ; i++, index++){
		temp[index]->type = COST;
		temp[index]->consume_value = (rand() % 4) + 1;
	}

	for(int i=0 ; i<one_to_two_bonus_blocks ; i++, index++){
		temp[index]->type = BONUS;
		temp[index]->consume_value = (rand() % 2) + 1;
	}

	for(int i=0 ; i<three_to_five_bonus_blocks ; i++, index++){
		temp[index]->type = BONUS;
		temp[index]->consume_value = (rand() % 3) + 3;
	}

	for(int i=0 ; i<multiplier_blocks ; i++, index++){
		temp[index]->type = MULTIPLIER;
		temp[index]->consume_value = (rand() % 2) + 2;
	}
}

void init_bawana(){
	int index = 0;
	Bawana *temp[BAWANA_SQUARES];
	for(int w=7 ; w<WIDTH ;w++){
		for(int l=21 ; l<LENGTH ; l++){
			bawana[index] = (Bawana){BAWANA_NA, w, l, -1};
			temp[index] = &bawana[index];
            index++;
		}		
	}

	shuffle_array(temp, BAWANA_SQUARES, sizeof(Bawana*));

	index = 0;
	for(int i=0 ; i<2 ; i++, index++){
		temp[i]->state = FOOD_POISONING;
		temp[i]->effect_rounds = BAWANA_FOOD_POISONING_ROUNDS;
	}
	for(int i=0 ; i<2 ; i++, index++){
		temp[i]->state = DISORIENTED;
		temp[i]->effect_rounds = BAWANA_DISORIENTED_ROUNDS;
	}
	for(int i=0 ; i<2 ; i++, index++){
		temp[i]->state = TRIGGERED;
		temp[i]->effect_rounds = BAWANA_TRIGGERED_ROUNDS;
	}
	for(int i=0 ; i<2 ; i++, index++){
		temp[i]->state = HAPPY;
		temp[i]->effect_rounds = -1;
	}
	for(int i=0 ; i<4 ; i++, index++){
		temp[i]->state = NORMAL;
		temp[i]->effect_rounds = -1;
	}
}

Block* go_to_starting_pos(Player *player){
	switch(player->id){
		case A:
			return &maze[0][5][12];
		case B:
			return &maze[0][9][7];
		case C:	
			return &maze[0][9][17];
		default:
			return NULL;
	}
}

void go_to_bawana(Player *player){
	int i = (rand() % BAWANA_SQUARES);
	player->bawana_effect = bawana[i];

	switch(player->bawana_effect.state){
		case FOOD_POISONING:
			player->rem_points = BAWANA_FOOD_POISONING_BONUS;
			log_when_food_poisoning_starts(player);
			break;
		case DISORIENTED:
			player->rem_points = BAWANA_DISORIENTED_BONUS;
			log_when_disoriented_starts(player);
			break;
		case TRIGGERED:
			player->rem_points = BAWANA_TRIGGERED_BONUS;
			log_when_triggered_starts(player);
			break;
		case HAPPY:
			player->rem_points = BAWANA_HAPPY_BONUS;
			log_when_happy(player);
			break;
		case NORMAL:
			player->rem_points = rand() % 89 + 11;
			log_when_normal(player);
			break;
		default:
			break;
	}
}

int move_piece(Player *current_player){
	int cost = 0, rem_mp = current_player->rem_points, moved_cells = 0;
	
	int should_directon_dice_roll = current_player->player_rounds%4==0 && 
				!is_in_starting_area(current_player->current_block->floor, current_player->current_block->width_num, current_player->current_block->length_num);

	if(should_directon_dice_roll) current_player->direction = roll_dir_dice(current_player->direction);
	
	Block *dest_block = get_dest_block(current_player, &cost, &rem_mp, &moved_cells);
	current_player->rem_points = rem_mp;

	if(dest_block == NULL){
		return 0;
	}else{
		if(flag.floor == dest_block->floor && flag.width_num == dest_block->width_num && flag.length_num == dest_block->length_num){
			log_player_won(current_player);
			return 2;
		}

		Block *start_pos = go_to_starting_pos(current_player);
		if(dest_block->floor == start_pos->floor && dest_block->width_num == start_pos->width_num && dest_block->length_num == start_pos->length_num){
			should_directon_dice_roll = should_directon_dice_roll == 0 ? 1 : 0;
		}

		should_directon_dice_roll ? log_in_maze_with_dir_dice(current_player) : log_in_maze_without_dir_dice(current_player);
		log_at_dest(current_player, moved_cells, cost);
		return 1;
	}
}

Block* get_dest_block(Player *current_player, int *tot_cost, int*rem_mp, int *moved_cells){
	int f = current_player->current_block->floor;
	int w = current_player->current_block->width_num;
	int l = current_player->current_block->length_num;
	
	if(is_in_starting_area(f, w, l)){
		if(game_state.movement_dice != 6){
			log_cannot_move_from_starting_area(current_player);
			return NULL;
		}else{
			log_can_move_from_starting_area(current_player);
			switch(current_player->id){
				case A:
				return &maze[0][5][12];
				case B:
				return &maze[0][9][7];
				case C:	
				return &maze[0][9][17];
			}
		}
	}

	Block *block = go_to_starting_pos(current_player);
	if(f == block->floor && w == block->width_num && l == block->length_num){
		current_player->direction = NORTH;
	}

	int cost = 0, rem = current_player->rem_points, i;

	block = NULL;
	Bawana *be = &current_player->bawana_effect;

	for(i=1 ; i<=game_state.movement_dice; i++){
		moved_cells++;
		// Check for Bawana effects
		switch(be->state){
			case FOOD_POISONING:
				// FOOD POISONING: cannot move this turn
				if(be->effect_rounds > 1){
					be->effect_rounds--;
					log_when_food_poisoning_exists(current_player);
					return NULL;
				}else if(be->effect_rounds == 1){
					go_to_bawana(current_player);
					log_when_food_poisoning_ends(current_player);
					return &maze[BAWANA_ENTRANCE_FLOOR][BAWANA_ENTRANCE_WIDTH][BAWANA_ENTRANCE_LENGTH];
				}
				break;
			case DISORIENTED:
				current_player->direction = (Direction)(rand()%4 + 1);
				// Further checking
				if(f==BAWANA_ENTRANCE_FLOOR && w==BAWANA_ENTRANCE_WIDTH && l==BAWANA_ENTRANCE_LENGTH && be->effect_rounds == BAWANA_DISORIENTED_ROUNDS){
					break;
				}else if(be->effect_rounds >= 1){
					current_player->direction = roll_direction_dice(current_player->direction);
					be->effect_rounds--;
					log_when_disoriented_exists(current_player);
				}else if(be->effect_rounds == 0){
					current_player->direction = NORTH;
					be = &(Bawana){BAWANA_NA, -1, -1, -1};
				}
				break;
			case TRIGGERED:
				// TRIGGERED: moves twice as fast (double the movement dice)
				if(be->effect_rounds > 1){
					game_state.movement_dice *= 2;
					be->effect_rounds--;
					log_when_triggered_exists(current_player);
				}else if(be->effect_rounds == 1){
					log_when_triggered_ends(current_player);
					be = &(Bawana){BAWANA_NA, -1, -1, -1};
				}
				break;
			default:
				break;
		}

		switch(current_player->direction){
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
				return NULL;
		}
		if(maze[f][w][l].floor == -1 && 
		   maze[f][w][l].width_num == -1 && 
		   maze[f][w][l].length_num == -1 &&
		   maze[f][w][l].type == CONSUMER_NA &&
		   maze[f][w][l].consume_value == -1){
			log_is_blocked_by_wall(current_player);
			return NULL;
		}else if(!is_in_the_playable_area(f, w, l)){
			log_is_blocked_by_wall(current_player);
			return NULL;
		}

		calc_mp_cost_and_rem(&cost, &rem, &maze[f][w][l]);

		if(flag.floor == f && flag.width_num == w && flag.length_num == l){
			block = &maze[f][w][l];
			log_player_won(current_player);
			break;
		}

		// Check if the player is moving onto a stair or pole
		block = move_from_stair_or_pole(current_player);

		if(!is_in_bawana_area(f, w, l) && !is_in_starting_area(f, w, l) && is_in_the_playable_area(f, w, l)){
			check_for_captures(current_player);
		}

		if(f==block->floor && w==block->width_num && l==block->length_num){
			continue;
		}else{
			f = block->floor;
			w = block->width_num;
			l = block->length_num;
		}	

		if(flag.floor == f && flag.width_num == w && flag.length_num == l){
			block = &maze[f][w][l];
			break;
		}else if(is_in_bawana_area(f, w, l)){
			go_to_bawana(current_player);
			log_deliver_to_bawana(current_player);
			block = &maze[BAWANA_ENTRANCE_FLOOR][BAWANA_ENTRANCE_WIDTH][BAWANA_ENTRANCE_LENGTH];
			break;
		}else if(is_in_starting_area(f, w, l)){
			block = go_to_starting_pos(current_player);
			break;
		}else{
			calc_mp_cost_and_rem(&cost, &rem, &maze[f][w][l]);			
			block = &maze[f][w][l];
		}
	}

	*tot_cost = cost;
	*rem_mp = rem;

	if(rem_mp <= 0){
		go_to_bawana(current_player);
		log_deliver_to_bawana_mp_depleted(current_player);
		block = &maze[BAWANA_ENTRANCE_FLOOR][BAWANA_ENTRANCE_WIDTH][BAWANA_ENTRANCE_LENGTH];
	}
	return block;
}

void set_destination_block(Block *block, Player *player){
	player->current_block = block;
}

void check_for_captures(Player *current_player){
	for(int i=0; i<PLAYER_COUNT; i++){
		if(game_state.player[i].id != current_player->id){
			if(game_state.player[i].current_block->floor == current_player->current_block->floor &&
			   game_state.player[i].current_block->width_num == current_player->current_block->width_num &&
			   game_state.player[i].current_block->length_num == current_player->current_block->length_num &&
			   is_in_the_playable_area(game_state.player[i].current_block->floor,
									 game_state.player[i].current_block->width_num,
									 game_state.player[i].current_block->length_num)){

				log_player_captured(current_player, &game_state.player[i]);
				go_to_starting_pos(&game_state.player[i]);
				game_state.player[i].rem_points = INITIAL_MOVEMENT_POINTS;
				game_state.player[i].bawana_effect = (Bawana){BAWANA_NA, -1, -1, -1};
			}
		}
	}
}


// HELPER FUNCTIONS

int roll_dice(){
	return (rand() % 6) + 1;
}

Direction roll_dir_dice(Direction prev_dir){
	int dir = roll_dice();
	switch(dir){
		case 1:
			return prev_dir;
		case 2:
			return NORTH;
		case 3:
			return EAST;
		case 4:
			return SOUTH;
		case 5:
			return WEST;
		case 6:
			return prev_dir;
		default:
			return DIR_NA; 
	}
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
			break;
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

void shuffle_array(void *arr, size_t n, size_t elem_size) {
    unsigned char *array = arr;
    unsigned char *temp = malloc(elem_size);
    if (!temp) return;

    for (size_t i = n - 1; i > 0; i--) {
        size_t j = rand() % (i + 1);

        memcpy(temp, array + i * elem_size, elem_size);
        memcpy(array + i * elem_size, array + j * elem_size, elem_size);
        memcpy(array + j * elem_size, temp, elem_size);
    }
    free(temp);
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

int is_blocked_by_stair(int floor, int width_num, int length_num) {
    for (int i = 0; i < stairs_count; i++) {
        int upper = (stairs[i].start_floor > stairs[i].end_floor) ? stairs[i].start_floor : stairs[i].end_floor;
        int lower = (stairs[i].start_floor < stairs[i].end_floor) ? stairs[i].start_floor : stairs[i].end_floor;

        if (floor < lower || floor > upper || upper == lower) continue; 

        double floor_diff = (double)abs(stairs[i].start_floor - stairs[i].end_floor);
        double current_floor_pos = (double)abs(floor - stairs[i].start_floor);
        double pos_factor = current_floor_pos / floor_diff;

        double calc_width = stairs[i].start_width_num + ((double)(stairs[i].end_width_num - stairs[i].start_width_num) * pos_factor);
        double calc_length = stairs[i].start_length_num + ((double)(stairs[i].end_length_num - stairs[i].start_length_num) * pos_factor);

        int rounded_calc_width = round(calc_width);
        int rounded_calc_length = round(calc_length);

        if (width_num == rounded_calc_width && length_num == rounded_calc_length) {
            return 1;
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
			break;
		case 1:
			if((width>=0 && width<=5) && (length>=8 && length<=16)){
				return 0;
			}
			break;
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


void calc_mp_cost_and_rem(int *cost, int* rem_mp, Block *block){
	if(block->type == COST){
		*cost += block->consume_value;
		*rem_mp -= block->consume_value;
	}else if(block->type == BONUS){
		*rem_mp += block->consume_value;
	}else if(block->type == MULTIPLIER){
		*rem_mp *= block->consume_value;
	}else{
		return;
	}
}

Block* closest_sp_destination(Stair *s[], Pole *p[],
                              int non_looping_s[], int non_looping_p[], Player *current_player) {
	typedef struct {
    	BlockType type;
    	int index;
	} Candidate;

    int min_distance = 10000;

	Candidate candidates[MAX_STAIRS_FROM_SAME_CELL + MAX_POLES_FROM_SAME_CELL];
    int candidate_count = 0;

    for (int i = 0; i < MAX_STAIRS_FROM_SAME_CELL; i++) {
        if (non_looping_s[i] != -1 && s[i] != NULL) {
            int dist = abs(s[i]->end_floor - flag.floor) +
                       abs(s[i]->end_width_num - flag.width_num) +
                       abs(s[i]->end_length_num - flag.length_num);
            if (dist < min_distance) {
                min_distance = dist;
                candidate_count = 0;
                candidates[candidate_count++] = (Candidate){STAIR, i};
            }else if(dist == min_distance){
				candidates[candidate_count++] = (Candidate){STAIR, i};
			}
        }
    }

    for(int j = 0; j < MAX_POLES_FROM_SAME_CELL; j++){
        if(non_looping_p[j] != -1 && p[j] != NULL){
            int dist = abs(p[j]->end_floor - flag.floor) +
                       abs(p[j]->width_num - flag.width_num) +
                       abs(p[j]->length_num - flag.length_num);

            if(dist < min_distance){
                min_distance = dist;
                candidate_count = 0;
                candidates[candidate_count++] = (Candidate){POLE, j};
            }else if(dist == min_distance){
				candidates[candidate_count++] = (Candidate){POLE, j};
			}
        }
    }

	if (candidate_count == 0) return NULL;

	int chosen = rand() % candidate_count;

    if(candidates[chosen].type == STAIR){
		if(current_player->current_block->floor != s[candidates[chosen].index]->start_floor){
			log_land_on_stair(current_player, current_player->current_block, &maze[s[candidates[chosen].index]->start_floor]
					 [s[candidates[chosen].index]->start_width_num]
					 [s[candidates[chosen].index]->start_length_num]);
		}else{
			log_land_on_stair(current_player, current_player->current_block, &maze[s[candidates[chosen].index]->end_floor]
					 [s[candidates[chosen].index]->end_width_num]
					 [s[candidates[chosen].index]->end_length_num]);
		}

        return &maze[s[candidates[chosen].index]->end_floor]
                     [s[candidates[chosen].index]->end_width_num]
                     [s[candidates[chosen].index]->end_length_num];
    }else if(candidates[chosen].type == POLE){
		log_land_on_pole(current_player, current_player->current_block, &maze[p[candidates[chosen].index]->end_floor]
					 [p[candidates[chosen].index]->width_num]
					 [p[candidates[chosen].index]->length_num]);
        return &maze[p[candidates[chosen].index]->end_floor]
                     [p[candidates[chosen].index]->width_num]
                     [p[candidates[chosen].index]->length_num];
    }else{
		return NULL;
	}
}

Block* move_from_stair_or_pole(Player *current_player){
	int floor = current_player->current_block->floor;
	int width = current_player->current_block->width_num;
	int length = current_player->current_block->length_num;

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
	}else if(p_count > MAX_POLES_FROM_SAME_CELL || s_count > MAX_STAIRS_FROM_SAME_CELL || p_count < 0 || s_count < 0){
		printf("Error: More than maximum poles or stairs from the same cell in move_from_stair_or_pole\n");
		return NULL;
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
		
		// Check that the player should go to bawana
		int no_stairs = 1, no_poles = 1;
		for(int i=0; i<MAX_STAIRS_FROM_SAME_CELL; i++){
			if(non_looping_s[i] != -1){
				no_stairs = 0;
				break;
			}
		}
		for(int j=0; j<MAX_POLES_FROM_SAME_CELL; j++){
			if(non_looping_p[j] != -1){
				no_poles = 0;
				break;
			}
		}
		if(no_stairs && no_poles){
			go_to_bawana(current_player);
			log_deliver_to_bawana(current_player);
			&maze[BAWANA_ENTRANCE_FLOOR][BAWANA_ENTRANCE_WIDTH][BAWANA_ENTRANCE_LENGTH];
		}else{
			// go to closest sp destination
			Block* next_block = closest_sp_destination(s, p, non_looping_s, non_looping_p, current_player);

			if (next_block == NULL) {
				return go_to_starting_pos(current_player);
			}

			return next_block;
		}				
	}
}

int stairs_from_cell(int floor, int width, int length, Stair *out[]){
	int count = 0;
	for(int i=0; i<stairs_count; i++){
		if((stairs[i].start_floor == floor) && (stairs[i].start_width_num == width) && (stairs[i].start_length_num == length)){
			out[count++] = &stairs[i];
		}

		if((stairs[i].direction == BI) && (stairs[i].end_floor == floor) && (stairs[i].end_width_num == width) && (stairs[i].end_length_num == length)){
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
			break;
		default:
			printf("Type other than stair or pole is in mark_loops\n");
			return;
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
		}else if(start_floor == end_floor){
			printf("Error : Skipping error line stairs.txt (stairs on the same floor)\n");
			continue;
		}else if(start_floor > end_floor){
			printf("Error : Skipping error line stairs.txt (start floor greater than end floor)\n");
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
				printf("Error : Skipping error line poles.txt (more than %d pole in the same cell)\n", MAX_POLES_FROM_SAME_CELL);
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
		return 0;
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

void free_maze(){
	if(stairs) free(stairs);
	if(poles) free(poles);
	if(walls) free(walls);
	if(bawana) free(bawana);
}

char* direction_to_string(Direction dir){
	switch(dir){
		case NORTH:
			return "NORTH";
		case SOUTH:
			return "SOUTH";
		case EAST:
			return "EAST";
		case WEST:
			return "WEST";
		default:
			return "DIR_NA";
	}
}

char* bawana_state_to_string(BawanaState state){
	switch(state){
		case FOOD_POISONING:
			return "FOOD_POISONING";
		case DISORIENTED:
			return "DISORIENTED";
		case TRIGGERED:
			return "TRIGGERED";
		case HAPPY:
			return "HAPPY";
		case NORMAL:
			return "NORMAL";
		default:
			return "BAWANA_NA";
	}
}

char* player_id_to_string(PlayerID id){
	switch(id){
		case A:
			return "A";
		case B:
			return "B";
		case C:
			return "C";
		default:
			return "ID_NA";
	}
}

// LOGGING FUNCTIONS

void log_cannot_move_from_starting_area(Player *player){
	printf("\t%s is at the starting area and rolls %d on the movement dice cannot enter the maze.\n", player_id_to_string(player->id), game_state.movement_dice);
}

void log_can_move_from_starting_area(Player *player){
	printf("\t%s is at the starting area and rolls 6 on the movement dice and is placed on [%d, %d, %d] of the maze.\n", player_id_to_string(player->id), player->current_block->floor, player->current_block->width_num, player->current_block->length_num);
}

void log_in_maze_with_dir_dice(Player *player){ 
	printf("\t%s rolls and %d on the movement dice and %s on the direction dice, changes direction to %s and moves %s by %d cells and is now at [%d, %d, %d].\n", player_id_to_string(player->id), game_state.movement_dice, direction_to_string(player->direction), direction_to_string(player->direction), direction_to_string(player->direction), game_state.movement_dice, player->current_block->floor, player->current_block->width_num, player->current_block->length_num);
}

void log_in_maze_without_dir_dice(Player *player){ 
	printf("\t%s rolls and %d on the movement dice and moves %s by %d cells and is now at [%d, %d, %d].\n", player_id_to_string(player->id), game_state.movement_dice, direction_to_string(player->direction), game_state.movement_dice, player->current_block->floor, player->current_block->width_num, player->current_block->length_num);
}

void log_is_blocked_by_wall(Player *player){
	printf("\t%s rolls and %d on the movement dice and cannot move in the %s. Player remains at [%d, %d, %d].\n", player_id_to_string(player->id), game_state.movement_dice, direction_to_string(player->direction), player->current_block->floor, player->current_block->width_num, player->current_block->length_num);
}

void log_at_dest(Player *player, int *cells, int *cost){
	printf("\t%s moved %d that cost %d movement points and is left with %d movement points and is moving in the %s.\n", player_id_to_string(player->id), *cells, *cost, player->rem_points, direction_to_string(player->direction));
}

void log_deliver_to_bawana_mp_depleted(Player *player){
	printf("\t%s movement points are depleted and requires replenishment. Transporting to Bawana.\n", player_id_to_string(player->id));
	printf("\t%s is place on a %s and effects take place.\n", player_id_to_string(player->id), bawana_state_to_string(player->bawana_effect.state));
}

void log_deliver_to_bawana(Player *player){
	printf("\t%s is delivering to Bawana due to landing on a stair or pole cell or a loop. %s is placed on a %s and effects take place.\n", player_id_to_string(player->id), player_id_to_string(player->id), bawana_state_to_string(player->bawana_effect.state));
}

void log_when_food_poisoning_starts(Player *player){
	printf("\t%s eats from Bawana and have a bad case of food poisoning. Will need three rounds to recover.\n", player_id_to_string(player->id));
}

void log_when_food_poisoning_exists(Player *player){
	printf("\t%s is still food poisoned and misses the turn.\n", player_id_to_string(player->id));
}

void log_when_food_poisoning_ends(Player *player){
	printf("\t%s is now fit to proceed from the food poisoning episode and now placed on a %s and the effects take place.\n", player_id_to_string(player->id), bawana_state_to_string(player->bawana_effect.state));
}

void log_when_disoriented_starts(Player *player){ 
	printf("\t%s eats from Bawana and is disoriented and is placed at the entrance of Bawana with 50 movement points.\n", player_id_to_string(player->id));
}

void log_when_disoriented_exists(Player *player){
	printf("\t%s rolls and %d on the movement dice and is disoriented and move in the %s and moves %d cells and is placed at the [%d, %d, %d].\n", player_id_to_string(player->id), game_state.movement_dice, direction_to_string(player->direction), game_state.movement_dice, player->current_block->floor, player->current_block->width_num, player->current_block->length_num);
}

void log_when_disoriented_ends(Player *player){
	printf("\t%s has recovered from disorientation.\n", player_id_to_string(player->id));
}

void log_when_triggered_starts(Player *player){
	printf("\t%s eats from Bawana and is triggered due to bad quality of food. %s is placed at the entrance of Bawana with 50 movement points.\n", player_id_to_string(player->id), player_id_to_string(player->id));
}

void log_when_triggered_exists(Player *player){
	printf("\t%s is triggered and rolls and %d on the movement dice and move in the %s and moves %d cells and is placed at the [%d, %d, %d].\n", player_id_to_string(player->id), game_state.movement_dice, direction_to_string(player->direction), 2*game_state.movement_dice, player->current_block->floor, player->current_block->width_num, player->current_block->length_num);
}

void log_when_happy(Player *player){
	printf("\t%s eats from Bawana and is happy. %s is placed at the entrance of Bawana with 200 movement points.\n", player_id_to_string(player->id), player_id_to_string(player->id));
}

void log_when_normal(Player *player){
	printf("\t%s eats from Bawana and earns %d movement points and is placed at the entrance of Bawana.\n", player_id_to_string(player->id), player->rem_points);
}

void log_land_on_stair(Player *player, Block *prev_block, Block *next_block){
	printf("\t%s lands on [%d, %d, %d] which is a stair cell. %s takes the stairs and now placed at [%d, %d, %d] in floor %d.\n", player_id_to_string(player->id), prev_block->floor, prev_block->width_num, prev_block->length_num, player_id_to_string(player->id), player->current_block->floor, player->current_block->width_num, player->current_block->length_num, player->current_block->floor);
}

void log_land_on_pole(Player *player, Block *prev_block, Block *next_block){
	printf("\t%s lands on [%d, %d, %d] which is a pole cell. %s slides down and now placed at [%d, %d, %d] in floor %d.\n", player_id_to_string(player->id), prev_block->floor, prev_block->width_num, prev_block->length_num, player_id_to_string(player->id), next_block->floor, next_block->width_num, next_block->length_num, next_block->floor);
}

void log_player_won(Player *player){
	printf("\t%s has reached the flag at [%d, %d, %d] and wins the game!\n", player_id_to_string(player->id), flag.floor, flag.width_num, flag.length_num);
}

void log_player_captured(Player *player, Player *captured_player){
	printf("\t%s has captured %s at [%d, %d, %d]. %s is sent to the starting area.\n", player_id_to_string(player->id), player_id_to_string(captured_player->id), captured_player->current_block->floor, captured_player->current_block->width_num, captured_player->current_block->length_num, player_id_to_string(captured_player->id));
}