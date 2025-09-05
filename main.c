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

int main(){
	init_seed("./seed.txt");
	init_game();	
	return 0;
}
