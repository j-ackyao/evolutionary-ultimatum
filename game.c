#include <stdlib.h>
#include <stdio.h> // temporary solution for printing summary
#include <math.h>

#include "game.h"
#include "player.h"
#include "utils.h"

#ifdef DEBUG
#define DEBUG_PRINT(S, ...) printf (S, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(S, ...) ((void) 0) // do nothing
#endif


int size_;
struct player** population_;


void clear_players() {
    for (int i = 0; i < size_; i++) {
        free(population_[i]);
        population_[i] = NULL;
    }
}

// frees any dynamically allocated resources of a game
void clear_game() {
    clear_players();
    free(population_);
    population_ = NULL;
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
    int children_tot = ((double) 1 - MUTATION_RATE - (KEEP_SELF ? FITNESS_CUTOFF : 0)) * size_;
    DEBUG_PRINT("Children: %d\n", children_tot);

    // keep track of survivors and total fitness
    double fit_tot = 0;
    for (int p = 0; p < top_n; p++) {
        survivors[p] = population_[p];
        fit_tot += population_[p]->fitness;
        // printf("top %d player has fitness %f with offer %f accepting %f to %f\n", p, population_[p]->fitness, population_[p]->offer, population_[p]->lbound, population_[p]->ubound);
    }

    DEBUG_PRINT("Total fitness: %f\n", fit_tot);

    // iterate through each survivor, prioritizing the highest fitness first
    // children created so far
    int children_sf = 0;
    // index for next pop
    int np = 0;
    for (int p = 0; p < top_n; p++) {
        // determine the number of children the parent can have based on fitness
        int children_n = round(children_tot * survivors[p]->fitness / fit_tot);
        if (children_n + children_sf > children_tot)
            children_n = children_tot - children_sf;
        children_sf += children_n;

        struct player* parent = survivors[p];

        // if next generation parent can stay, add to next pop
        if (KEEP_SELF) {
            struct player* next_gen_parent = malloc(sizeof(struct player));
            *next_gen_parent = *survivors[p];
            next_gen_parent->fitness = 0;
            next_pop[np++] = next_gen_parent;
        }
        
        for (int i = 0; i < children_n; i++) {
            int parent2_i = get_rand_range(0, top_n);
            // can reproduce with sel, too lazy to consider extremely small edge cases
            // while (second_parent == p) 
            //     second_parent = get_rand_range(0, top_n);

            struct player* parent2 = survivors[parent2_i];

            double avg_offer = avg(parent->offer, parent2->offer);
            double avg_lbound = avg(parent->lbound, parent2->lbound);
            double avg_ubound = avg(parent->ubound, parent2->ubound);

            struct player* child = malloc(sizeof(struct player));
            struct player c = {avg_offer, avg_lbound, avg_ubound, 0};
            *child = c;

            next_pop[np++] = child;

            DEBUG_PRINT("Parent %d reproduced with parent %d to make o %f l %f u %f\n", p, parent2_i, avg_offer, avg_lbound, avg_ubound);
        }

        DEBUG_PRINT("Top %d has %d in next gen from %f fitness\n", p, children_n, survivors[p]->fitness);
    }
    DEBUG_PRINT("Total expected players in next before mutation: %d\n", np);
    
    // in case of rounding error from reproduction, mutation can fill in gaps
    while (np < size_) {
        struct player* m = malloc(sizeof(struct player));
        *m = init_player();
        next_pop[np++] = m;
    }
    
    DEBUG_PRINT("Total next population size: %d\n", np);
}

// runs one iteration of population and handles both sim and next generations
void run_sim() {
    // first ensure next generation is possible
    // instead, could overwrite in existing population and just free a player at each step
    struct player** next_pop = malloc(sizeof(struct player*) * size_);
    if (!population_) {
        printf("Failed to allocate sufficient memory for next generation\n");
        return;
    }


    sim_gen();
    next_gen(next_pop);

    clear_game();
    population_ = next_pop;
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

void summarize_game(int iteration) {
    double offer_mean = 0;
    double lbound_mean = 0;
    double ubound_mean = 0;
    // double fitness_mean = 0;
    for (int i = 0; i < size_; i++) {
        offer_mean += population_[i]->offer;
        lbound_mean += population_[i]->lbound;
        ubound_mean += population_[i]->ubound;
        // fitness_mean += population_[i]->fitness;
    }
    offer_mean = offer_mean / size_;
    lbound_mean = lbound_mean / size_;
    ubound_mean = ubound_mean / size_;
    // fitness_mean = fitness_mean / size_;
    printf("Iteration %d ==============\n", iteration);
    printf("Offer mean: %f\nLower bound mean: %f\nUpper bound mean: %f\n",
     offer_mean, lbound_mean, ubound_mean);
    printf("===========================\n", iteration);
}


// csv writing stuff here
FILE *csv_file;



// exports current game state into a csv file
void write_csv(int iteration) {
    if (!csv_file) {
        csv_file = fopen("output.csv", "w");
        fprintf(csv_file, "iteration,offer,lbound,ubound\n");
    }
    for (int i = 0; i < size_; i++) {
        struct player* p = population_[i];
        fprintf(csv_file, "%d,%f,%f,%f\n", iteration, p->offer, p->lbound, p->ubound);
    }
}



// cleans up everything
void clean() {
    clear_game();
    
    if (csv_file)
        fclose(csv_file);
}
