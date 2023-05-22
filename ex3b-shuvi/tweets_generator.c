#include <stdlib.h>
#include <string.h>

#include "linked_list.h"
#include "markov_chain.h"

#define DECIMAL 10
#define SENTENCE_MAX 1000
#define SEPARATE " \n\r"
#define MAX_TWEET_LEN 20
#define ENDING_SENTECE '.'
#define WORD_STR " %s"

#define CLI_ERR_NUM "USAGE: necessary input  : 1. seed 2. number of tweets to\
 genrate 3. path to text corpus 4. number of words to read from the file.\n"
#define PATH_ERR_MSG "Error: The given file is invalid.\n"
#define TWEET_NUM_MSG "Tweet %d:"


enum CliArg
{
    SEED_ARG = 1,
    TWEET_NUM_ARG,
    PATH_ARG,
    WORDS_TO_READ_ARG,
    CLI_ARG

};

/**
 * This function reads words from a file, adds them to a Markov chain database,
 * and creates frequency lists for each word. If the `words_to_read` parameter
 * is greater than zero, the function stops reading from the file after that
 * many words have been read.
 * @param[in] text_corpus The file pointer to read from.
 * @param[in] words_to_read The number of words to read from the file.
 * If zero, reads until end of file is reached.
 * @param[out] markov_chain The Markov chain database to fill.
 * @return `EXIT_SUCCESS` if the database was filled successfully,
 * `EXIT_FAILURE` otherwise.
 */
static int fill_database(FILE *text_corpus, int words_to_read,
                         MarkovChain *markov_chain)
{
    bool continue_reading = true;
    int word_count = 0;
    char sentence[SENTENCE_MAX];
    while (continue_reading && fgets(sentence, SENTENCE_MAX,
                                     text_corpus) != NULL)
    {
        char *word = strtok(sentence, SEPARATE);
        Node *first = NULL;
        Node *second = NULL;
        while (word != NULL && continue_reading)
        {
            second = add_to_database(markov_chain, word);
            if (second == NULL)
            {
                free_database(&markov_chain);
                return EXIT_FAILURE;
            }
            if (first) // Will not go in to  at the first word of file
            {
                bool added = add_node_to_frequencies_list(first->data,
                                                          second->data,
                                                          markov_chain);
                if (!added)
                {
                    free_database(&markov_chain);
                    return EXIT_FAILURE;
                }
            }
            first = second->data->last ? NULL : second;
            word = strtok(NULL, SEPARATE);
            if (words_to_read)
            {
                word_count++;
                continue_reading = word_count < words_to_read;
            }
        }
    }
    return EXIT_SUCCESS;
}

/**
 * Receive markov_chain, generate and print random sentence out of it. The
 * sentence most have at least 2 words in it.
 * @param markov_chain
 * @param n number of tweets to genarate.
 * @param  max_length maximum length of chain to generate
 */
static void generate_n_tweets(MarkovChain *markov_chain, int n,
                              int max_length)
{
    for (int i = 1; i <= n; i++)
    {
        printf(TWEET_NUM_MSG, i);
        generate_tweet(markov_chain, NULL, max_length);
    }
}

/**
 * Checks if there is a dot at the end of the word
 * @param word
 * @return 1 if has a '.' at the end of the word 0 else.
 */
static bool ending_word(void *data)
{
    char *word = (char *)data;
    int n = strlen(word) - 1;
    return word[n] == ENDING_SENTECE;
}

/**
 * This function prints a word to the standard output using printf .
 * @param data A pointer to the word to be printed.
 */
static void word_print(void *data)
{
    char *word = (char *)data;
    printf(WORD_STR, word);
}

/**
 * This function compares two words using the strcmp function.
 * @param data1 A pointer to the first word.
 * @param data2 A pointer to the second word.
 * @return An integer less than, equal to, or greater than zero if the
 *  first word is found to be less than, equal to, or greater than the
 *  second word, respectively.
 */
static int word_cmpr(void *data1, void *data2)
{
    char *word1 = (char *)data1;
    char *word2 = (char *)data2;
    return strcmp(word1, word2);
}

static void free_word(void *data)
{
    free(data);
}
/**
 * This function allocates memory for a new word and copies the contents of 
 * the source word to the newly allocated memory.
 * @param data A pointer to the source word.
 * @return A pointer to the newly allocated word if successful, or NULL if
 *  memory allocation fails.
 */
static void *word_alloc_cpy(void *data)
{
    char *c_data = (char *)data;
    char *c_dst = malloc(strlen(c_data) + 1);
    if (!c_dst)
    {
        return NULL;
    }
    strcpy(c_dst, c_data);
    return (void *)c_dst;
}



int main(int argc, char *argv[])
{
    if (argc != CLI_ARG && argc != CLI_ARG - 1)
    {
        printf(CLI_ERR_NUM);
        return EXIT_FAILURE;
    }
    FILE *text_corpus = fopen(argv[PATH_ARG], "r");
    if (text_corpus == NULL)
    {
        printf(PATH_ERR_MSG);
        return EXIT_FAILURE;
    }
    unsigned int seed = strtol(argv[SEED_ARG], NULL, DECIMAL);
    srand(seed);
    LinkedList data_base = {NULL, NULL, 0};
    MarkovChain markov_chain = {&data_base, word_print, word_cmpr, free_word,
                                word_alloc_cpy, ending_word};
    MarkovChain *ptr_markov_chain = &markov_chain;
    int words_to_read = 0;
    if (argc == CLI_ARG)
    {
        words_to_read = strtol(argv[WORDS_TO_READ_ARG], NULL, DECIMAL);
    }
    int try = fill_database(text_corpus, words_to_read, &markov_chain);
    fclose(text_corpus);
    if (try)
    {
        printf(ALLOCATION_ERROR_MASSAGE);
        return EXIT_FAILURE;
    }
    int words_to_tweet = strtol(argv[TWEET_NUM_ARG], NULL, DECIMAL);
    generate_n_tweets(ptr_markov_chain, words_to_tweet, MAX_TWEET_LEN);
    free_database(&ptr_markov_chain);
    return EXIT_SUCCESS;
}


