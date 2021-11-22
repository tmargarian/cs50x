// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
// Hash output based on first three letters
const unsigned int N = 27 * (28 * 28) +  27 * 28 + 27; 

// Keep track of loaded words
int words_loaded = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Getting the linked list index through hash()
    int idx = hash(word);

    // Looping through the linked list
    for (node *tmp = table[idx]; tmp != NULL; tmp = tmp->next)
    {
        //printf("Word: %s; strcasecmp result: %i\n", tmp->word, strcasecmp(word, tmp->word));
        if (strcasecmp(word, tmp->word) == 0)
        {
            return true;
        }
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int n_first_letters = 3;

    // Initializing the array for first indices with zeros
    int *idx = malloc(n_first_letters * sizeof(int));
    for (int i = 0; i < n_first_letters; i++)
    {
        idx[i] = 0;
    }

    // Fillinf the idx {\0,a,b,c,...,z,\'}
    for (int i = 0; i < n_first_letters; i++)
    {
        // If the word terminated - leave the loop
        if (word[i] == '\0')
        {
            break;
        }
        else if (isalpha(word[i]))
        {
            idx[i] = tolower(word[i]) - 96;
        }
        else if (word[i] == '\'')
        {
            idx[i] = 27;
        }
    }    

    int res = 0;
    for (int i = 0; i < n_first_letters; i++)
    {
        res += (int) idx[i] * pow(28, n_first_letters - i - 1);
    }

    free(idx);
    return res;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Opening dictionary file
    FILE *dict_file = fopen(dictionary, "r");
    if (dict_file == NULL)
    {
        return false;
    }

    // Scanning dictionary file
    char dict_word[LENGTH + 1]; 
    while (fscanf(dict_file, "%s", dict_word) != EOF)
    {
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }
        strcpy(n->word, dict_word);

        // Getting the hash function output index
        int node_idx = hash(dict_word);

        // Storing the word into the hash table
        n->next = table[node_idx];
        table[node_idx] = n;

        // Update count of loaded words and free memory
        words_loaded++;
    }

    fclose(dict_file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return words_loaded;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        if (table[i] == NULL)
        {
            continue;
        }
        else 
        {
            node *cursor = table[i];
            node *tmp;
            
            while (cursor != NULL)
            {
                tmp = cursor;
                cursor = cursor->next;
                free(tmp);
            }
        }
    }
    return true;
}
