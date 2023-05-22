#include <string.h> // For strlen(), strcmp(), strcpy()
#include "markov_chain.h"
#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))

#define EMPTY -1
#define BOARD_SIZE 100
#define MAX_GENERATION_LENGTH 60
#define DICE_MAX 6
#define NUM_OF_TRANSITIONS 20

#define DECIMAL 10
#define CLI_ERR_NUM "USAGE: necessary input  : 1. seed 2. number of pathes\
 to genrate.\n"

#define RAND_WALK_STR "Random Walk %d:"
#define SNAKE_STR " [%d]-snake to %d ->"
#define LADDER_STR " [%d]-ladder to %d ->"
#define CELL_STR " [%d] ->"
#define END_STR " [%d]"

// #define ERR_CREATE "Error: create_board faild."
enum CliArg
{
    SEED_ARG = 1,
    PATH_NUM_ARG,
    CLI_ARG
};

/**
 * represents the transitions by ladders and snakes in the game
 * each tuple (x,y) represents a ladder from x to if x<y or a snake otherwise
 */
const int transitions[][2] = {{13, 4},
                              {85, 17},
                              {95, 67},
                              {97, 58},
                              {66, 89},
                              {87, 31},
                              {57, 83},
                              {91, 25},
                              {28, 50},
                              {35, 11},
                              {8, 30},
                              {41, 62},
                              {81, 43},
                              {69, 32},
                              {20, 39},
                              {33, 70},
                              {79, 99},
                              {23, 76},
                              {15, 47},
                              {61, 14}};

/**
 * struct represents a Cell in the game board
 */
typedef struct Cell
{
    int number;    // Cell number 1-100
    int ladder_to; // ladder_to represents the jump of the ladder in case
    // there is one from this square
    int snake_to; // snake_to represents the jump of the snake in case
    // there is one from this square
    //  both ladder_to and snake_to should be -1 if the Cell doesn't have them
} Cell;

/** Error handler **/
static int handle_error(char *error_msg, MarkovChain **database)
{
    printf("%s", error_msg);
    if (database != NULL)
    {
        free_database(database);
    }
    return EXIT_FAILURE;
}


static int create_board(Cell *cells[BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        cells[i] = malloc(sizeof(Cell));
        if (cells[i] == NULL)
        {
            for (int j = 0; j < i; j++)
            {
                free(cells[j]);
            }
            handle_error(ALLOCATION_ERROR_MASSAGE, NULL);
            return EXIT_FAILURE;
        }
        *(cells[i]) = (Cell){i + 1, EMPTY, EMPTY};
    }

    for (int i = 0; i < NUM_OF_TRANSITIONS; i++)
    {
        int from = transitions[i][0];
        int to = transitions[i][1];
        if (from < to)
        {
            cells[from - 1]->ladder_to = to;
        }
        else
        {
            cells[from - 1]->snake_to = to;
        }
    }
    return EXIT_SUCCESS;
}

/**
 * fills database
 * @param markov_chain
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
static int fill_database(MarkovChain *markov_chain)
{
    Cell *cells[BOARD_SIZE];
    if (create_board(cells) == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }
    MarkovNode *from_node = NULL, *to_node = NULL;
    size_t index_to;
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        add_to_database(markov_chain, cells[i]);
    }

    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        from_node = get_node_from_database(markov_chain, cells[i])->data;

        if (cells[i]->snake_to != EMPTY || cells[i]->ladder_to != EMPTY)
        {
            index_to = MAX(cells[i]->snake_to, cells[i]->ladder_to) - 1;
            to_node = get_node_from_database(markov_chain, cells[index_to])
                          ->data;
            add_node_to_frequencies_list(from_node, to_node, markov_chain);
        }
        else
        {
            for (int j = 1; j <= DICE_MAX; j++)
            {
                index_to = ((Cell *)(from_node->data))->number + j - 1;
                if (index_to >= BOARD_SIZE)
                {
                    break;
                }
                to_node = get_node_from_database(markov_chain,
                                                 cells[index_to])
                              ->data;
                add_node_to_frequencies_list(from_node, to_node,
                                             markov_chain);
            }
        }
    }
    // free temp arr
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        free(cells[i]);
    }
    return EXIT_SUCCESS;
}

/**
 * This function generates a specified number of paths in the Markov chain and
 * prints the generated paths using the generate_tweet function.
 * @param markov_chain A pointer to the Markov chain.
 * @param n The number of paths to generate.
 * @param max_len The maximum length of each path.
 */
static void generate_n_paths(MarkovChain *markov_chain, unsigned int n,
                             int max_len)
{
    for (unsigned int i = 1; i <= n; i++)
    {

        printf(RAND_WALK_STR, i);
        MarkovNode *first_node = markov_chain->database->first->data;
        generate_tweet(markov_chain, first_node, max_len);
    }
}

/**
 * This function checks if a given cell represents the ending cell
 *  based on its number.
 * @param data A pointer to the cell to check.
 * @return true if the cell represents the ending cell, false otherwise.
 */
static bool ending_cell(void *data)
{
    int cell_num = ((Cell *)data)->number;
    return cell_num == BOARD_SIZE;
}

/**
 * This function prints the details of a given cell, including its number,and
 * if it is a snake or a ladder, it also prints the destination number.
 * @param data A pointer to the cell to print.
 */
static void cell_print(void *data)
{
    Cell *cell = (Cell *)data;
    if (ending_cell(data))
    {
        printf(END_STR, cell->number);
        return;
    }
    else if (cell->snake_to != EMPTY)
    {
        printf(SNAKE_STR, cell->number, cell->snake_to);
    }
    else if (cell->ladder_to != EMPTY)
    {
        printf(LADDER_STR, cell->number, cell->ladder_to);
    }
    else
    {
        printf(CELL_STR, cell->number);
    }
}

/**
 * This function compares two cells based on their numbers and
 *  returns the result.
 * @param data1 A pointer to the first cell.
 * @param data2 A pointer to the second cell.
 * @return An integer less than, equal to, or greater than zero if the
 *  number of the first cell is found to be less than, equal to, or
 *  greater than the number of the second cell, respectively.
 */
static int cell_cmpr(void *data1, void *data2)
{
    int num1 = ((Cell *)data1)->number;
    int num2 = ((Cell *)data2)->number;
    return num1 - num2;
}

static void free_cell(void *data)
{
    free(data);
}

/**
 * This function allocates memory for a new cell, copies the contents of the
 * source cell to the newly allocated memory, and returns a pointer to it.
 *
 * @param data A pointer to the source cell.
 *
 * @return A pointer to the newly allocated cell if successful,
 *  or NULL if memory allocation fails.
 */
static void *cell_alloc_cpy(void *data)
{
    Cell *dest = malloc(sizeof(Cell));
    if (!dest)
    {
        return NULL;
    }
    Cell *src = (Cell *)data;
    dest->number = src->number;
    dest->ladder_to = src->ladder_to;
    dest->snake_to = src->snake_to;
    return (void *)dest;
}


/**
 * This function iterates through the `cells` array, which represents the game
 * board, and frees the memory allocated for each cell.
  * @param[in] cells The array of cells representing the game board.
 */
void free_board(Cell *cells[BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        free(cells[i]);
    }
}

/**
 * @param argc num of arguments
 * @param argv 1) Seed
 *             2) Number of pathes to generate
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int argc, char *argv[])
{
    if (argc != CLI_ARG)
    {
        printf(CLI_ERR_NUM);
        return EXIT_FAILURE;
    }
    unsigned int seed = strtol(argv[SEED_ARG], NULL, DECIMAL);
    srand(seed);
    Cell *cells[BOARD_SIZE];
    if (create_board(cells))
    {
        return EXIT_FAILURE;
    }

    LinkedList data_base = {NULL, NULL, 0};
    MarkovChain markov_chain = {&data_base, cell_print, cell_cmpr, free_cell,
                                cell_alloc_cpy, ending_cell};
    MarkovChain *ptr_markov_chain = &markov_chain;
    if (fill_database(ptr_markov_chain))
    {
        handle_error(ALLOCATION_ERROR_MASSAGE, &ptr_markov_chain);
        return EXIT_FAILURE;
    }
    const int path_num = strtol(argv[PATH_NUM_ARG], NULL, DECIMAL);
    generate_n_paths(ptr_markov_chain, path_num, MAX_GENERATION_LENGTH);
    free_database(&ptr_markov_chain);
    free_board(cells);
    return EXIT_SUCCESS;
}
