#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define ABC_LENGTH 26

//const int ABC_LENGTH = 26;

int main(int argc, string argv[])
{
    // Checking the passed argument 
    if (argc != 2) // not 1 argument passed
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    
    string key = argv[1]; // reading the key from command argument

    // converting the key into lowercase and checking for non letter chars
    for (int i = 0, n = strlen(key); i < n; i++)
    {
        key[i] = tolower(key[i]);
        if (!isalpha(key[i]))
        {
            printf("The key should contain only english letters!\n");
            return 1;
        }
    }
    
    // checking that key is 26 characters of length
    if (strlen(key) != ABC_LENGTH) 
    {
        printf("Key should have %i characters!\n", ABC_LENGTH);
        return 1;
    }
    
    int abc_counter[ABC_LENGTH] = {0}; 

    // counting differnet letters in the key
    for (int i = 0; i < ABC_LENGTH; i++)
    {
        abc_counter[key[i] - 97]++; 
    }

    // checking if any letter appered more or less than once
    for (int i = 0; i < ABC_LENGTH; i++)
    {
        if (abc_counter[i] != 1)
        {
            printf("Key should contain different characters!\n");
            return 1;
        }
    }

    // KEY CHECKS PASSED! 

    // obtaining the plaintext
    string plaintext = get_string("plaintext:  ");
    int p_text_len = strlen(plaintext);

    for (int i = 0; i < p_text_len; i++)
    {
        if (isalpha(plaintext[i]))
        {
            if (isupper(plaintext[i]))
            {
                plaintext[i] = toupper(key[plaintext[i] - 65]);
            }
            else
            {
                plaintext[i] = key[plaintext[i] - 97];
            }
        }
        else 
        {
            plaintext[i] = plaintext[i];
        }
    }
    printf("ciphertext: %s\n", plaintext);
    return 0;
}