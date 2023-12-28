#include <stdlib.h>
#include <stdio.h> // temporary solution for printing summary

#include "game.h"
#include "player.h"
#include "utils.h"


int size_;
struct player** population_;

// initialize population with pop_size players
void init_population(int pop_size) {
    size_ = pop_size;
    // could instead replace this with array of pointers to malloced players
    population_ = malloc(sizeof(struct player*) * pop_size);
    if (!population_) {
        printf("failed\nUnable to allocate sufficient memory\n");
        exit(0);
    }
    for (int i = 0; i < pop_size; i++) {
        struct player* p = malloc(sizeof(struct player));
        *p = init_player();
        population_[i] = p;
    }
}

// initialize game state
void init_game(int pop_size) {
    init_population(pop_size);
}

void clear_players() {
    for (int i = 0; i < size_; i++) {
        free(population_[i]);
        population_[i] = NULL;
    }
}

// frees any dynamically allocated resources
void clear_game() {
    clear_players();
    free(population_);
}

double sim_gen() {
    int accepts;
    for (int p = 0; p < size_; p++) {
        for (int i = 0; i < INTERACTIONS_PER_ITER; i++) {
            int p2_i = get_rand_range(0, (double) size_);
            while (p2_i == p)
                p2_i = get_rand_range(0, (double) size_);
            // opponent will almost never be size_
            // opponent is player two
            struct player* player1 = population_[p];
            struct player* player2 = population_[p2_i];
            double offer = player1->offer;
            if (player2->lbound <= offer && player2->ubound >= offer) {
                // accept offer
                accepts++;
                player1->fitness += offer;
                player2->fitness += MAX_RESOURCE - offer;
            }
        }
    }
    return MAX_RESOURCE * accepts;
}

void next_gen(double total_fit) {
    // todo
}

void run_sim_i(int iterations) {
    for (;iterations > 0; iterations--) {
        double total_fit = sim_gen();
        next_gen(total_fit);
    }
        
}

void summarize_game() {
    double offer_mean = 0;
    double lbound_mean = 0;
    double ubound_mean = 0;
    double fitness_mean = 0;
    for (int i = 0; i < size_; i++) {
        offer_mean += population_[i]->offer;
        lbound_mean += population_[i]->lbound;
        ubound_mean += population_[i]->ubound;
        fitness_mean += population_[i]->fitness;
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

