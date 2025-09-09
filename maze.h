#ifndef MAZE_H
#define MAZE_H

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#define FLOORS 3
#define WIDTH 10
#define LENGTH 25

#define BAWANA_FOOD_POISONING_ROUNDS 3
#define BAWANA_FOOD_POISONING_BONUS 0
#define BAWANA_DISORIENTED_ROUNDS 4
#define BAWANA_TRIGGERED_ROUNDS 3
#define BAWANA_DISORIENTED_BONUS 50
#define BAWANA_TRIGGERED_BONUS 50
#define BAWANA_TRIGGERED_MULTIPLIER 2
#define BAWANA_HAPPY_BONUS 200

#define BAWANA_ENTRANCE_FLOOR 0
#define BAWANA_ENTRANCE_WIDTH 9
#define BAWANA_ENTRANCE_LENGTH 19

#define INITIAL_MOVEMENT_POINTS 100

#define MAX_STAIRS_FROM_SAME_CELL 2
#define MAX_POLES_FROM_SAME_CELL 1

#define PLAYER_COUNT 3

#define HOW_MANY_ROUNDS_TO_CHANGE_STAIR_DIR 5

typedef enum { DIR_NA, NORTH, EAST, SOUTH, WEST } Direction;
typedef enum { BAWANA_NA, FOOD_POISONING, DISORIENTED, TRIGGERED, HAPPY, NORMAL}BawanaState;
typedef enum { CONSUMER_NA, ZERO, COST, BONUS, MULTIPLIER } ConsumeType;
typedef enum { STAIR, POLE, WALL, BK_NORMAL} BlockType;
typedef enum { UNI_UP, UNI_DOWN, BI } StairDirection;
typedef enum { A, B, C } PlayerID;

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
	StairDirection direction;
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

typedef struct{
	BawanaState state;
	int block_width_num;
	int block_length_num;
	int effect_rounds;
}Bawana;

typedef struct{
	PlayerID id;
	Block *current_block;
	Direction direction;
	int rem_points;
	Bawana bawana_effect;
	int player_rounds;
}Player;

typedef struct{
	Player player[PLAYER_COUNT];
	int rounds;
	int movement_dice;
	Direction direction_dice;
}Game;

extern Block flag;
extern Stair *stairs;
extern Pole *poles;
extern Wall *walls;

extern Block maze[FLOORS][WIDTH][LENGTH];
extern Game game_state;

// MAIN FUNCTIONS

void init_game();
void init_maze();
void assign_consumables();
void init_bawana();
Block* go_to_starting_pos(Player *player);
void go_to_bawana(Player *player);
int move_piece(Player *current_player);
Block* get_dest_block(Player *current_player, int *tot_cost, int*rem_mp);

// HELPER FUNCTIONS

int roll_dice();
Direction get_direction(int direction_dice);
void change_stair_direction();
void shuffle_array(void *arr, size_t n, size_t elem_size);
Direction get_direction(int direction_dice);
int is_blocked_by_wall(int floor, int width_num, int length_num);
int is_blocked_by_stair(int floor, int width_num, int length_num);

int is_in_the_playable_area(int floor, int width, int length);
int is_in_bawana_area(int floor, int width, int length);
int is_in_starting_area(int floor, int width, int length);

void set_destination_block(Block *block, Player *player);
void calc_mp_cost_and_rem(int *cost, int* rem_mp, Block *block);
Block* closest_sp_destination(Stair *s[], Pole *p[],
                              int non_looping_s[], int non_looping_p[]);

Block* move_from_stair_or_pole(int floor, int width, int length);
int stairs_from_cell(int floor, int width_num, int length_num, Stair *out[]);
int poles_from_cell(int floor, int width_num, int length_num, Pole *out[]);
void mark_loops(BlockType type, int current_index,
				int visited_s[], int visited_p[],
				Stair *s[], Pole *p[], int non_looping_s[], int non_looping_p[]);
int is_stair_loop(Stair *a, Stair *b);
int is_stair_pole_loop(Stair *stair, Pole *pole);

void load_stairs(const char *stairs_file);
void load_poles(const char *poles_file);
void load_walls(const char *walls_file);
int load_flag(const char *flag_file);
void free_maze();
char* direction_to_string(Direction dir);
char* bawana_state_to_string(BawanaState state);
char* player_id_to_string(PlayerID id);

// LOGGING FUNCTIONS

void log_cannot_move_from_starting_area(Player *player);
void log_can_move_from_starting_area(Player *player);
void log_in_maze_with_dir_dice(Player *player);
void log_in_maze_without_dir_dice(Player *player);
void log_is_blocked_by_wall(Player *player);
void log_at_dest(Player *player);
void log_deliver_to_bawana(Player *player);
void log_when_food_poisoning_starts(Player *player);
void log_when_food_poisoning_exists(Player *player);
void log_when_food_poisoning_ends(Player *player);
void log_when_disoriented_starts(Player *player);
void log_when_disoriented_exists(Player *player);
void log_when_disoriented_ends(Player *player);
void log_when_triggered_starts(Player *player);
void log_when_triggered_exists(Player *player);
void log_when_happy(Player *player);
void log_when_normal(Player *player);
void log_land_on_stair(Player *player, Block *prev_block);
void log_land_on_pole(Player *player, Block *prev_block);
void log_on_win(Player *player);


#endif
