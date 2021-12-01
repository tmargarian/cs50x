from cs50 import *

# Testing for input from 1 to 8 inclusive
while True:
    number = get_int("Height: ")
    if (number >= 1) and (number <= 8):
        break
    else:
        continue

# Printing the pyramid
for i in range(number):
    print(" " * (number - 1 - i) + "#" * (i + 1) + "  " + "#" * (i + 1))