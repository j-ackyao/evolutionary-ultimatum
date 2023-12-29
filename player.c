#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#include "utils.h"


struct player init_player() {
	double a = get_rand_range(0, MAX_RESOURCE);
	double b = get_rand_range(0, MAX_RESOURCE);
	struct player p = {get_rand_range(0, MAX_RESOURCE), min(a, b), max(a, b), 0};
	return p;
}

