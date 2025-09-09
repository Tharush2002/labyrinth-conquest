#include "maze.h"

void init_seed(const char *seed_file){
	FILE *fp = fopen(seed_file, "r");
	if(fp == NULL){
		printf("Error opening seed.txt\n");
		return;
	}

	int seed;
	if(fscanf(fp, "%d", &seed) != 1){
		printf("Error reading seed from seed.txt\n");
	}else{
		srand(seed);
	}
	fclose(fp);
}

void init_players(){
	game_state.player[0] = (Player){ A, &maze[0][6][12], NORTH, INITIAL_MOVEMENT_POINTS, {BAWANA_NA, -1, -1, -1}, 0 };
	game_state.player[1] = (Player){ B, &maze[0][9][8], WEST, INITIAL_MOVEMENT_POINTS, {BAWANA_NA, -1, -1, -1}, 0 };
	game_state.player[2] = (Player){ C, &maze[0][9][16], EAST, INITIAL_MOVEMENT_POINTS, {BAWANA_NA, -1, -1, -1}, 0 };

	game_state.rounds = 0;
	game_state.movement_dice = -1;
}

int main(){
	init_seed("./seed.txt");
	init_game();
	
	init_players();

	do {
		printf("=====================\n");
		printf("\tRound - %d\n", game_state.rounds+1);
		printf("=====================\n");

		if(game_state.rounds > HOW_MANY_ROUNDS_TO_CHANGE_STAIR_DIR && 
			game_state.rounds % HOW_MANY_ROUNDS_TO_CHANGE_STAIR_DIR == 0){
			printf("Changing stairs direction...\n");
			change_stairs_direction();
		}

		for(int i=0; i<PLAYER_COUNT; i++){
			Player *current_player = &game_state.player[i];
			printf("Player %s's turn:\n", player_id_to_string(current_player->id));

			game_state.movement_dice = roll_dice();

			if(move_piece(current_player)==2){
				free_maze();
				return 0;
			};			

			current_player->player_rounds++;
		}
		game_state.rounds++;

	}while(1);

	free_maze();
	return 0;
}
