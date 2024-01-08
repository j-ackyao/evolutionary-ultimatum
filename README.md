# evolutionary-ultimatum

### Setup
Recommended to compile the C program with `gcc *.c *.h -lm -o main` and running the program in a terminal.
- The `-lm` flag is used to include the math library.
- Adding the `-D DEBUG` flag will define the debug macro in `game.c` to print details that can be useful for closer inspection or debugging. Not recommended for large populations.

Hyperparameters can be found in `utils.h`, which can greatly alter the results of simulations.

### Running the simulation
After providing an initial population:
- Positive integer inputs are for running the population through the provided number of iterations.
- Negative integers are for special inputs, such as writing to a CSV file or exiting.

### Writing to file
When opting to write to CSV file, data of every player after the end of the iterations run will be written to the file.

The columns are `iteration, offer, lbound, ubound`.