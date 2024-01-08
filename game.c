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

// frees all players in population_
void clear_players() {
    for (int i = 0; i < size_; i++) {
        free(population_[i]);
        population_[i] = NULL;
    }
}

// frees players and population_
void clear_game() {
    clear_players();
    free(population_);
    population_ = NULL;
}

FILE *csv_file;

// exports current game state into a csv file along with the iteration the generation is in
void write_csv(int iteration) {
    if (!csv_file) {
        csv_file = fopen("output.csv", "w");
        if (!csv_file) {
            printf("Failed to open to output.csv\n");
            return;
        }
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



// initialize population with pop_size players
void init_population(int pop_size) {
    size_ = pop_size;
    population_ = malloc(sizeof(struct player*) * pop_size);

    if (!population_) {
        printf("failed\nUnable to allocate sufficient memory\n");
        exit(0);
    }

    for (int i = 0; i < pop_size; i++) {
        // visualize progress
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
}

// initialize game, so far only need to initialize population
void init_game(int pop_size) {
    init_population(pop_size);
}

// helper to simulate one generation interactions
void sim_gen() {
    for (int p = 0; p < size_; p++) {
        struct player* player1 = population_[p];
        double offer = player1->offer;

        DEBUG_PRINT("\nPlayer %d's turn, offering %.2f:\n", p, offer);

        for (int i = 0; i < INTERACTIONS_PER_ITER; i++) {
            int p2_i = get_rand_range(0, (double) size_);

            // checking for p2_i == size_ should not be necessary as *almost* never happens
            // but does very often on windows somehow
            while (p2_i == p || p2_i == size_)
                p2_i = get_rand_range(0, (double) size_);

            struct player* player2 = population_[p2_i];
            int accept = player2->lbound <= offer && player2->ubound >= offer;

            if (accept) {
                player1->fitness += offer;
                if (SECONDARY_INTERACT)
                    player2->fitness += MAX_RESOURCE - offer;
            }

            DEBUG_PRINT("%s with player %d (%.2f, %.2f), fitness: %.2f\n", accept ? "Success" : "Failed ", p2_i, player2->lbound, player2->ubound, player1->fitness);
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

    // using qsort to find top percentile
    qsort(population_, size_, sizeof(struct player*), compare_fitness);

    // determine how many children it produces based on fitness proportion in top percentile
    int children_tot = ((double) 1 - MUTATION_RATE - (KEEP_SELF ? FITNESS_CUTOFF : 0)) * size_;
    DEBUG_PRINT("\nNumber of children expected from reproduction: %d\n", children_tot);


    double fit_tot = 0;
    for (int p = 0; p < top_n; p++) {
        survivors[p] = population_[p];
        fit_tot += population_[p]->fitness;
    }

    DEBUG_PRINT("Total fitness: %.2f\n", fit_tot);

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
        
        DEBUG_PRINT("\nTop %d has %d children in next gen with %.2f fitness\n", p + 1, children_n, survivors[p]->fitness);
        for (int i = 0; i < children_n; i++) {
            int parent2_i = get_rand_range(0, top_n);
            
            // once again checking if the index is out of bounds
            while (parent2_i == top_n || (!SELF_RPD && parent2_i == p)) 
                parent2_i = get_rand_range(0, top_n);

            struct player* parent2 = survivors[parent2_i];

            double avg_offer = avg(parent->offer, parent2->offer);
            double avg_lbound = avg(parent->lbound, parent2->lbound);
            double avg_ubound = avg(parent->ubound, parent2->ubound);

            struct player* child = malloc(sizeof(struct player));
            struct player c = {avg_offer, avg_lbound, avg_ubound, 0};
            *child = c;

            next_pop[np++] = child;

            DEBUG_PRINT("Top %d reproduced with top %d: offers %.2f, accepts (%.2f, %.2f)\n", p + 1, parent2_i + 1, avg_offer, avg_lbound, avg_ubound);
        }
    }
    DEBUG_PRINT("\nTotal expected players in next before mutation: %d\n", np);

    // in case of rounding error from reproduction, mutation can fill in gaps
    while (np < size_) {
        struct player* m = malloc(sizeof(struct player));
        *m = init_player();
        next_pop[np++] = m;
    }
    
    DEBUG_PRINT("Total next population size after mutation: %d\n", np);
}

// runs one iteration of population and handles both sim and next generations
void run_sim() {
    // first ensure next generation is possible
    // instead, could overwrite in existing population and just free a player at each step
    
    struct player** next_pop = malloc(sizeof(struct player*) * size_);
    if (!next_pop) {
        printf("Failed to allocate sufficient memory for next generation\n");
        return;
    }
    
    
    // printf("here2\n");
    sim_gen();

    // printf("here3\n"); 
    next_gen(next_pop);

    // printf("here4\n");    
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
    printf("Iteration %d ===============\n", iteration);
    printf("Offer mean: %.4f\nLower bound mean: %.4f\nUpper bound mean: %.4f\n",
     offer_mean, lbound_mean, ubound_mean);
    printf("===========================\n", iteration);
}

