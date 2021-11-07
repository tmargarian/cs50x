#include <cs50.h>
#include <stdio.h>

// function prototype for repeating characters
void repeat_char(char c, int count);

int main(void)
{
    // initializing height variable
    int height;

    // promting the user to enter the height until it's in 1...8 range
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 | height > 8);

    // the loop for the pyramids
    for (int i = 0; i < height; i++)
    {
        repeat_char(' ', height - i - 1);
        repeat_char('#', i + 1);
        printf("  ");
        repeat_char('#', i + 1);
        printf("\n");
    }

}

// function for repeating characters
void repeat_char(char c, int count)
{
    for (int i = 0; i < count; i++)
    {
        printf("%c", c);
    }
}