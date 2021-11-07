#include <cs50.h>
#include <stdio.h>

int get_start_size(void);

int main(void)
{
    // Prompt for start size
    int start_size = get_start_size();

    // Prompt for end size
    int end_size;
    do
    {
        end_size = get_int("End size: ");
    }
    while (end_size < start_size);

    // Calculate number of years until we reach threshold
    int years = 0;
    while (start_size < end_size)
    {
        start_size = start_size + start_size / 3 - start_size / 4;
        years++;
    }

    // Print number of years
    printf("Years: %i\n", years);
}


// Function to get the start size which should be at least 9
int get_start_size(void)
{
    // initializing the start_size_ to avoid scope issues
    int start_size_;

    // do-while loop will repropmt user until he enters a number which is at least 9
    do
    {
        start_size_ = get_int("Start size: ");
    }
    while (start_size_ < 9);
    return start_size_;
}