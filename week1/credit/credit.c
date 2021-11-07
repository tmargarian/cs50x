#include <cs50.h>
#include <stdio.h>
#include <math.h> // Math library for pow() function

int main(void)
{
    // initializing the card number
    long card_number;

    // checking that the length of the credit number is indeed 16
    card_number = get_long("Number: ");

    // getting the card number length in digits
    int n_digits = floor(log10(card_number)) + 1;

    // splitting the 16 digit card number into individual numbers
    int ind_numbers[n_digits]; // individual card numbers initizliation
    for (int i = 0; i < n_digits; i++)
    {
        ind_numbers[i] = (card_number / (long) pow(10, n_digits - 1 - i)) % 10;
    }
    // card number checks
    if ((n_digits != 15) & (n_digits != 16) & (n_digits != 13)) // card number has wrong length
    {
        printf("INVALID\n");
    }
    else // card number has 13, 15 or 16 digits
    {
        // Luhn's algorithm

        // calculating the first sum of doubled digits 1,3,...,15
        // and the second sum of even digits 2,4,..,16
        int first_sum = 0;
        int second_sum = 0;
        for (int i = 0; i < n_digits; i++)
        {
            if (i % 2 != 0) // if the digit is in odd position
            {
                if (ind_numbers[n_digits - i - 1] * 2 > 9) // double digit produt - adding two digits
                {
                    first_sum += 1 + ((ind_numbers[n_digits - i - 1] * 2) % 10);
                }
                else
                {
                    first_sum += ind_numbers[n_digits - i - 1] * 2;
                }
            }
            else // if the digit is in even position
            {
                second_sum += ind_numbers[n_digits - i - 1];
            }
            printf("\n");
        }

        // testing the algorithm
        if ((first_sum + second_sum) % 10 != 0) // test not passed
        {
            printf("INVALID\n");
        }
        else // test passed
        {
            if ((n_digits == 15) & (ind_numbers[0] == 3) & ((ind_numbers[1] == 4) | (ind_numbers[1] == 7)))
            {
                printf("AMEX\n");
            }
            else if ((n_digits == 16) & (ind_numbers[0] == 5) & (ind_numbers[1] > 0) & (ind_numbers[1] < 6))
            {
                printf("MASTERCARD\n");
            }
            else if (((n_digits == 16) | (n_digits == 13)) & (ind_numbers[0] == 4))
            {
                printf("VISA\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
    }
}