#include <stdlib.h>
#include <stdio.h>

#include "utils.h"



int read_int() {
	int buf;
	int result = scanf("%d", &buf);
	if (result != 1) {
		printf("Invalid input or input stream error\n");
		return 0x80000000;
	}
	return buf;
}

// 0 to 1
double get_rand() {
	return rand() / (double) RAND_MAX;
}

// low to high
double get_rand_range(double low, double high) {
	return get_rand()*(high - low) + low;
}

double max(double a, double b) {
	return a > b ? a : b;
}

double min(double a, double b) {
	return a < b ? a : b;
}

double avg(double a, double b) {
	return (a + b) / 2;
}