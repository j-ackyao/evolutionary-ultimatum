#include <stdlib.h>
#include <stdio.h> // temporary solution for printing summary
#include <math.h>

#include "game.h"
#include "player.h"
#include "utils.h"


int size_;
struct player** population_;


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
        if (pop_size / 10 != 0 && i % (pop_size / 10) == 0) {
            printf(".");
            fflush(stdout);
        }
        struct player* p = malloc(sizeof(struct player));
        if (!p) {
            printf("failed\nUnable to allocate sufficient memory\n");
            clear_game();
            exit(0);
        }
        *p = init_player();
        population_[i] = p;
    }

	printf(" Done\n");
}

// initialize game state
void init_game(int pop_size) {
    init_population(pop_size);
}

// helper to simulate one generation interactions
void sim_gen() {
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
                player1->fitness += offer;
                player2->fitness += MAX_RESOURCE - offer;
            }
        }
    }
}

// compare function for qsort in next_gen
int compare_fitness(const void* a, const void* b) {
    double fitness1 = (*(struct player**) a)->fitness;
    double fitness2 = (*(struct player**) b)->fitness;
    return (fitness1 < fitness2) - (fitness2 < fitness1);
}

// determines next population
void next_gen(struct player** next_pop) {
    int top_n = size_ * FITNESS_CUTOFF;
    struct player* survivors[top_n];

    // can probably replace this with a linear time function to find top percentile
    qsort(population_, size_, sizeof(struct player*), compare_fitness);

    // determine how many children it produces based on fitness proportion in top percentile
    int children_tot = ((double) 1 - FITNESS_CUTOFF - MUTATION_RATE) * size_;
    printf("children: %d\n", children_tot);
    double fit_tot = 0;

    for (int p = 0; p < top_n; p++) {
        survivors[p] = population_[p];
        fit_tot += population_[p]->fitness;
        // printf("top %d player has fitness %f with offer %f accepting %f to %f\n", p, population_[p]->fitness, population_[p]->offer, population_[p]->lbound, population_[p]->ubound);
    }

    // reproduce
    int np = 0;
    for (int p = 0; p < top_n; p++) {

        int children = ceil(children_tot * survivors[p]->fitness / fit_tot);

        // if (children_tot < children)
        //     children = children_tot;

        // ++children;
        np += children;
        // children_tot -= children;
        printf("top %d has %d in next gen from %f fitness\n", p, children, survivors[p]->fitness);
    }
    printf("total expected players in next before mutation: %d\n", np);
    
    
}

// runs one iteration of population and handles both sim and next generations
void run_sim() {
    // first ensure next generation is possible
    struct player** next_pop = malloc(sizeof(struct player*) * size_);
    if (!population_) {
        printf("Failed to allocate sufficient memory for next generation\n");
        return;
    }


    sim_gen();
    next_gen(next_pop);

    // clear_players();
    // population_ = next_pop;
    free(next_pop);
}

void run_sim_i(int iterations) {

    for (int i = 0; i < iterations; i++) {
        if (iterations / 10 != 0 && i % (iterations / 10) == 0) {
            printf(".");
            fflush(stdout);
        }
        run_sim();
    }


	printf(" Done\n");
        
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

