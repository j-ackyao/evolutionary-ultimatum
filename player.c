#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#include "utils.h"



struct player init_player() {
	double a = get_rand_range(0, 10);
	double b = get_rand_range(0, 10);
	struct player p = {get_rand_range(0, 10), min(a, b), max(a, b), 0};
	return p;
}

