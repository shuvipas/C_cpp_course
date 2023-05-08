#include <stdlib.h>
#include <string.h>

#include "linked_list.h"
#include "markov_chain.h"
#define DECIMAL 10
#define SENTENCE_MAX 1000
#define SEPARATE " \n\r"
#define MAX_TWEET_LEN 20

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
int fill_database(FILE *text_corpus, int words_to_read,
                  MarkovChain *markov_chain);

/**
 * Receive markov_chain, generate and print random sentence out of it. The
 * sentence most have at least 2 words in it.
 * @param markov_chain
 * @param n number of tweets to genarate.
 * @param  max_length maximum length of chain to generate
 */
void generate_n_tweets(MarkovChain *markov_chain, int n, int max_length);

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
    MarkovChain markov_chain = {&data_base};
    MarkovChain* ptr_markov_chain = &markov_chain;
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

int fill_database(FILE *text_corpus, int words_to_read, MarkovChain *markov_chain)
{
    bool continue_reading = true;
    int word_count = 0;
    char sentence[SENTENCE_MAX];
    while (continue_reading && fgets(sentence, SENTENCE_MAX, text_corpus) != NULL)
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
                                                          second->data);
                if (!added)
                {
                    free_database(&markov_chain);
                    return EXIT_FAILURE;
                }
            }
            first = second->data->last_word? NULL: second;
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


void generate_n_tweets(MarkovChain *markov_chain, int n, int max_length)
{
    for (int i = 1; i <= n; i++)
    {

        printf(TWEET_NUM_MSG, i);
        generate_tweet(markov_chain, NULL, max_length);
    }
}
