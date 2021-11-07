#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

// prototypes for the counting functions
int count_letters(string text_);
int count_words(string text_);
int count_sentences(string text_);

int main(void)
{
    // Obtaining user text and counting letters, words and sentences
    string text = get_string("Text: ");
    int total_letters = count_letters(text);
    int total_words = count_words(text);
    int total_sentences = count_sentences(text);

    // calculating index
    double index = 5.88 * (1.0 * total_letters / total_words) - 29.6 * (1.0 * total_sentences / total_words) - 15.8;

    // printing the readability grade
    if (index < 1.0)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 16.0)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", (int) round(index));
    }
}

// HELPER FUNCTIONS FOR COUNTING 

// Counting letters
int count_letters(string text_)
{
    int letters_n = 0; // letter counter
    for (int i = 0; text_[i] != '\0'; i++)
    {  
        // If tolower() returns a lowercase letter -> i++
        if (tolower(text_[i]) > 96 && tolower(text_[i]) < 123)
        {
            letters_n += 1;
        }
    }
    return letters_n;
}

// Counting words
int count_words(string text_)
{
    int words_n = 1; // words counter (we have 1 more words than spaces)
    for (int i = 0; text_[i] != '\0'; i++)
    {
        if (text_[i] == ' ')
        {
            words_n += 1;
        }
    }
    return words_n;
}

// Counting sentences
int count_sentences(string text_)
{
    int sentences_n = 0; // sentence counter
    for (int i = 0; text_[i] != '\0'; i++)
    {
        if (text_[i] == '.' || text_[i] == '!' || text_[i] == '?')
        {
            sentences_n += 1;
        }
    }
    return sentences_n;
}