#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool is_cycle(int winner, int loser);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        } 
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]] += 1;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    int k = 0; // Pair index
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i; j < candidate_count; j++)
        {
            // If more voters vouch for i over j than otherwise
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[k].winner = i;
                pairs[k].loser = j;
                k++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[k].winner = j;
                pairs[k].loser = i;
                k++;
            }
        }
    }
    pair_count = k;
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int tmp_winner = 0;
    int tmp_loser = 0;

    // Selection sort of the pairs by margin of victory
    for (int i = 0; i < pair_count - 1; i++)
    {
        int max_margin = preferences[pairs[i].winner][pairs[i].loser] - 
                         preferences[pairs[i].loser][pairs[i].winner];
        int max_margin_index = i;
        for (int j = i + 1; j < pair_count; j++)
        {
            int new_margin = preferences[pairs[j].winner][pairs[j].loser] - 
                             preferences[pairs[j].loser][pairs[j].winner];
            if (new_margin > max_margin)
            {
                max_margin = new_margin;
                max_margin_index = j;
            }
        }
        
        // Swapping the max
        tmp_winner = pairs[max_margin_index].winner;
        tmp_loser  = pairs[max_margin_index].loser;

        pairs[max_margin_index].winner = pairs[i].winner;
        pairs[max_margin_index].loser  = pairs[i].loser;

        pairs[i].winner = tmp_winner;
        pairs[i].loser  = tmp_loser;
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        // If added pair doesn't create a loop - add pair
        if (!is_cycle(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}

// Test the locked[][] array for loops
bool is_cycle(int winner, int loser)
{   
    if (winner == loser)
    {
        // loser is the winner - we got a cycle
        return true;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[loser][i]) // the loser wins somewhere else
        {
            if (is_cycle(winner, i)) // replace the original loser to check for a loop again (recursion)
            {
                return true;
            }
        }
    }
    return false;
}

// Print the winner of the election
void print_winner(void)
{
    // Find candidate with all falses in locked[][] matrix
    int locked_col_falses[MAX] = {0};

    for (int j = 0; j < candidate_count; j++)
    {
        for (int k = 0; k < candidate_count; k++)
        {
            if (locked[k][j] == false)
            {
                locked_col_falses[j] += 1;
            }
        }
        // If the candidate got all falses in matrix - that's the winner
        if (locked_col_falses[j] == candidate_count)
        {
            printf("%s\n", candidates[j]);
        }
    }
    return;
}