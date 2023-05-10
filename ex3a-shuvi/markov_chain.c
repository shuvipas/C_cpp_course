#include "markov_chain.h"
#include <string.h>
#include <assert.h>
#define ENDING_SENTECE '.'
#define LAST_WORD_STR " %s\n"
#define WORD_STR " %s"

/**
 * Checks if there is a dot at the end of the word
 * @param word
 * @return 1 if has a '.' at the end of the word 0 else.
 */
int ending_word(char *word);

/**
 * Get random number between 0 and max_number [0, max_number).
 * @param max_number max number to return (not included).
 * @return Random num
 */
int get_random_number(int max_number);

/**
 * @brief Searches a linked list for a specific word or counts the number
 * of nodes.
 * @param[in] head The head node of the linked list.
 * @param[in] data_ptr The word to search for in the linked list. 
 * Pass NULL to disable the search.
 * @param[out] len A pointer to an integer variable to store the number of
 * nodes. Pass NULL to disable counting.
 * @return A pointer to the node containing the matching word if `data_ptr`
 * is not NULL and a match is found, NULL otherwise.
 */
Node *markov_chain_sweeper(const Node *head ,char *data_ptr, int* len);

Node *add_to_database(MarkovChain *markov_chain, char *data_ptr)
{
    Node *tmp = get_node_from_database(markov_chain, data_ptr);
    if (tmp == NULL)
    {
        char *word_data = malloc(strlen(data_ptr) + 1);
        if (!word_data)
        {
            free_database(&markov_chain);
            return NULL;
        }
        strcpy(word_data, data_ptr);
        MarkovNode *new_markov_node = malloc(sizeof(MarkovNode));
        if (!new_markov_node)
        {
            free(word_data);
            word_data =NULL;
            free_database(&markov_chain);
            return NULL;
        }
        int last = ending_word(word_data);
        *new_markov_node = (MarkovNode){word_data, NULL, 0, last};
        if (add(markov_chain->database, new_markov_node))
        {
            free(word_data);
            word_data =NULL;
            free(new_markov_node);
            new_markov_node =NULL;
            free_database(&markov_chain);
            return NULL; // add failed
        }
        return markov_chain->database->last;
    }
    return tmp;
}
Node *markov_chain_sweeper(const Node *head ,char *data_ptr, int* len)
{
    for(Node *tmp = head; tmp!= NULL; tmp = tmp->next)
    {
        if(data_ptr)
        {
            char *word = tmp->data->data;
            if (!strcmp(word, data_ptr))
            {
                return tmp;
            }
        }
        else if(len)
        {
            (*len)++;
        }
    }
    return NULL;
}
Node *get_node_from_database(MarkovChain *markov_chain, char *data_ptr)
{

    Node *head = markov_chain->database->first;
    return markov_chain_sweeper(head ,data_ptr, NULL);
}

bool add_node_to_frequencies_list(MarkovNode *first_node,
                                  MarkovNode *second_node)
{
    MarkovNodeFrequency *freq_node = first_node->frequencies_list;
    for (int i = 0; i < first_node->len_freq_list; i++)
    {
        if ((freq_node + i)->markov_node == second_node)
        {
            (freq_node + i)->frequency++; //= 1;
            return true;
        }
    }
    int len = first_node->len_freq_list;
    MarkovNodeFrequency *new_freq = realloc(freq_node,
                                            (len + 1) * sizeof((*new_freq)));
    assert(sizeof((*new_freq)) == sizeof(*new_freq));
    assert(sizeof((*new_freq)) == sizeof(MarkovNodeFrequency));
    if (new_freq == NULL)
    {
        return false;
    }
    first_node->frequencies_list = new_freq;
    new_freq[len] = (MarkovNodeFrequency){second_node, 1};
    first_node->len_freq_list++; //= 1;
    return true;
}

void free_database(MarkovChain **ptr_chain)
{
    Node *tmp_node;
    Node *next_node = (*ptr_chain)->database->first;
    for (int i = 0; i < (*ptr_chain)->database->size; i++) // loop on the nodes
    {
        tmp_node = next_node;
        free(tmp_node->data->frequencies_list); // free(MarkovNodeFrequency)
        tmp_node->data->frequencies_list = NULL;
        free(tmp_node->data->data); // free(data)
        tmp_node->data->data = NULL;
        free(tmp_node->data); // free(MarkovNode)
        tmp_node->data = NULL;
        next_node = tmp_node->next; // Saving the importent data from the node
        free(tmp_node); // free(node)
        tmp_node = NULL;
    }
}

MarkovNode *get_first_random_node(MarkovChain *markov_chain)
{
    int len = 0;
    Node *tmp_next = markov_chain->database->first;
    markov_chain_sweeper(tmp_next ,NULL, &len);
    bool got_it = false;
    while (!got_it)
    {
        int random_num = get_random_number(len);
        tmp_next = markov_chain->database->first;
        for (int i = 0; i < random_num; i++)
        {
            tmp_next = tmp_next->next;
        }
        if (!(tmp_next->data->last_word))
        {
            got_it = true;
        }
    }
    return tmp_next->data;
}

MarkovNode *get_next_random_node(MarkovNode *state_struct_ptr)
{
    assert(state_struct_ptr);
    int max_num = 0;
    const MarkovNodeFrequency *freq_node = state_struct_ptr->frequencies_list;
    assert(freq_node);

    for (int i = 0; i < state_struct_ptr->len_freq_list; i++)
    {
        max_num += (freq_node + i)->frequency;
    }
    int random_num = get_random_number(max_num);
    freq_node = state_struct_ptr->frequencies_list;
    while (random_num >= freq_node->frequency)
    {
        random_num -= freq_node->frequency;
        freq_node++;
    }
    return freq_node->markov_node;
}

void generate_tweet(MarkovChain *markov_chain, MarkovNode *first_node,
                    int max_length)
{
    MarkovNode *m_n = first_node;
    if (m_n == NULL)
    {
        m_n = get_first_random_node(markov_chain);
    }
    int count = 0;
    while (count < max_length)
    {
        if (m_n->last_word || count == max_length - 1)
        {
            printf(LAST_WORD_STR, m_n->data);
            return;
        }
        else
        {
            count++;
            printf(WORD_STR, m_n->data);
            m_n = get_next_random_node(m_n);
        }
    }
}

int get_random_number(int max_number)
{
    return rand() % max_number;
}

int ending_word(char *word)
{
    int n = strlen(word) - 1;
    return word[n] == ENDING_SENTECE;
}
