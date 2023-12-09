#include <stdlib.h>
#include <stdio.h> // temporary solution for printing summary

#include "game.h"
#include "player.h"
#include "utils.h"


int size_;
struct player* population_;

// initialize population with pop_size players
void init_population(int pop_size) {
    size_ = pop_size;
    population_ = malloc(sizeof(struct player) * pop_size);
    for (int i = 0; i < pop_size; i++) {
        population_[i] = init_player();
    }
}

// initialize game state
void init_game(int pop_size) {
    init_population(pop_size);
}

void run_sim() {

}

void run_sim_i(int iterations) {
    for (;iterations > 0; iterations--)
        run_sim();
}

void summarize_game() {
    double offer_mean;
    double lbound_mean;
    double ubound_mean;
    double fitness_mean;
    for (int i = 0; i < size_; i++) {
        offer_mean += population_[i].offer;
        lbound_mean += population_[i].lbound;
        ubound_mean += population_[i].ubound;
        fitness_mean += population_[i].fitness;
    }
    offer_mean = offer_mean / size_;
    lbound_mean = lbound_mean / size_;
    ubound_mean = ubound_mean / size_;
    fitness_mean = fitness_mean / size_;
    printf("Offer mean: %f\nLower bound mean: %f\nUpper bound mean: %f\nFitness mean: %f\n",
     offer_mean, lbound_mean, ubound_mean, fitness_mean);
}

// exports current game state into a csv file
void export_game() {

}

// frees any dynamically allocated resources
void clear_game() {
    free(population_);
}

