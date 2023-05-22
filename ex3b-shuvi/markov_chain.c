#include "linked_list.h"
#include "markov_chain.h"
#include <string.h>

#define LAST_STR "\n"

/**
 * Get random number between 0 and max_number [0, max_number).
 * @param max_number max number to return (not included).
 * @return Random num
 */
int get_random_number(int max_number)
{
    return rand() % max_number;
}


/**
 * If data_ptr in markov_chain, return it's node. Otherwise, create new
 * node, add to end of markov_chain's database and return it.
 * @param markov_chain the chain to look in its database
 * @param data_ptr the state to look for
 * @return node wrapping given data_ptr in given chain's database
 */
Node *add_to_database(MarkovChain *markov_chain, void *data_ptr)
{
    Node *tmp = get_node_from_database(markov_chain, data_ptr);
    if (tmp == NULL)
    {
        void *data = markov_chain->copy_func(data_ptr);
        if (data == NULL)
        {
            free_database(&markov_chain);
            return NULL;
        }
        MarkovNode *new_markov_node = malloc(sizeof(*new_markov_node));
        if (!new_markov_node)
        {
            free(data);
            data = NULL;
            free_database(&markov_chain);
            return NULL;
        }
        bool last = markov_chain->is_last(data_ptr);
        *new_markov_node = (MarkovNode){data, NULL, 0, last};
        if (add(markov_chain->database, new_markov_node))
        {
            free(data);
            data = NULL;
            free(new_markov_node);
            new_markov_node = NULL;
            free_database(&markov_chain);
            return NULL; // add failed
        }
        return markov_chain->database->last;
    }
    return tmp;
}

Node *get_node_from_database(MarkovChain *markov_chain, void *data_ptr)
{
    Node *tmp_next = markov_chain->database->first;
    while (tmp_next)
    {
        char *data1 = (char *)data_ptr;
        char *data2 = tmp_next->data->data;

        if (!(markov_chain->comp_func(data1, data2)))
        {
            return tmp_next;
        }
        tmp_next = tmp_next->next;
    }
    return NULL;
}

/**
 * Add the second markov_node to the counter list of the first markov_node.
 * If already in list, update it's counter value.
 * @param first_node
 * @param second_node
 * @param markov_chain
 * @return success/failure: true if the process was successful, false if in
 * case of allocation error.
 */
bool add_node_to_frequencies_list(MarkovNode *first_node,
                                  MarkovNode *second_node,
                                  MarkovChain *markov_chain)
{
    MarkovNodeFrequency *freq_node = first_node->frequencies_list;
    for (int i = 0; i < first_node->len_freq_list; i++)
    {
        if (!(markov_chain->comp_func((freq_node + i)->markov_node->data,
                                      second_node->data)))
        {
            (freq_node + i)->frequency++;
            return true;
        }
    }
    int len = first_node->len_freq_list;
    MarkovNodeFrequency *new_freq = realloc(freq_node,
                                            (len + 1) * sizeof((*new_freq)));
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
        (*ptr_chain)->free_data(tmp_node->data->data);// free(data)
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
    while (tmp_next)
    {
        len++;
        tmp_next = tmp_next->next;
    }
    bool got_it = false;
    while (!got_it)
    {
        int random_num = get_random_number(len);
        tmp_next = markov_chain->database->first;
        for (int i = 0; i < random_num; i++)
        {
            tmp_next = tmp_next->next;
        }
        if (!(tmp_next->data->last))
        {
            got_it = true;
        }
    }
    return tmp_next->data;
}

MarkovNode *get_next_random_node(MarkovNode *state_struct_ptr)
{

    int max_num = 0;
    const MarkovNodeFrequency *freq_node = state_struct_ptr->frequencies_list;
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
        markov_chain->print_func(m_n->data);

        if (m_n->last || count == max_length - 1)
        {
            printf(LAST_STR);
            return;
        }
        else
        {
            count++;
            m_n = get_next_random_node(m_n);
        }
    }
}

