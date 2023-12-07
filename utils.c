#include <stdlib.h>
#include "utils.h"

// 0 to 1
double get_rand() {
	return (double) rand() / RAND_MAX;
}

// low to high
double get_rand_range(int low, int high) {
	return get_rand()*(high - low) + low;
}

double max(double a, double b) {
	return a > b ? a : b;
}

double min(double a, double b) {
	return a < b ? a : b;
}