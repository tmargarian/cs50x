from cs50 import *
import sys

# Valid card number lengths
valid_numbers = [13, 15, 16]
number = get_int("Number: ")

# Main function with all validity checks


def main():
    if len(str(number)) not in valid_numbers:
        print("INVALID")
        sys.exit(1)
    else:
        if (str(number)[0:2] == "34" or str(number)[0:2] == "37") and checksum(number):
            print("AMEX")
            sys.exit(0)
        elif (str(number)[0] == "5" and str(number)[1] in ["1", "2", "3", "4", "5"]) and checksum(number):
            print("MASTERCARD")
            sys.exit(0)
        elif str(number)[0] == "4" and checksum(number):
            print("VISA")
            sys.exit(0)
        else:
            print("INVALID")
            sys.exit(1)

# Implementing the Luhn's algorithm


def checksum(number):
    str_number = str(number)

    # Calculting the first sum of numbers
    first_sum = 0
    # Counting from the second digit at the end 
    for i in range(len(str_number) - 2, -1, -2):
        digit = str(int(str_number[i]) * 2)
        # Adding up individual digits 
        for j in range(len(digit)):
            first_sum += int(digit[j])

    # Adding the second sum
    second_sum = first_sum
    # Counting from the last digit fown
    for i in range(len(str_number) - 1, -1, -2):
        digit = str(int(str_number[i]))
        # Adding up individual digits 
        for j in range(len(digit)):
            second_sum += int(digit[j])

    if str(second_sum)[len(str(second_sum)) - 1] == '0':
        return True
    else:
        return False


main()
