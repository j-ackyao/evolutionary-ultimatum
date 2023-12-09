#ifndef __player_h__
#define __player_h__


struct player {
	double offer;
	double lbound;
	double ubound;
	double fitness;
};

struct player init_player();

#endif