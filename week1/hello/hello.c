#include <stdio.h>
#include <cs50.h>

int main(int argc, char ** argv)
{
    // Getting the name from user
    string name = get_string("What is your name?\n");

    // Printing the hello, name
    printf("hello, %s\n", name);
}
