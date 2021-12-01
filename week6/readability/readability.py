from cs50 import *
import sys


def main():
    # Obtaining the text from user input
    text = get_string("Text: ")

    # Running the functions to obtain numbers of letters, words and sentences
    total_letters = count_letters(text)
    total_words = count_words(text)
    total_sentences = count_sentences(text)

    # Calculating the text readability index
    index = 5.88 * (total_letters / total_words) - 29.6 * (total_sentences / total_words) - 15.8

    # Checking the readability index and outputting the grade
    if index < 1:
        print("Before Grade 1")
    elif index > 16:
        print("Grade 16+")
    else:
        print(f"Grade {round(index)}")


def count_letters(text):
    """
    The letter counting function
    """
    letters_n = 0
    for i in range(len(text)):
        if text[i].isalpha():
            letters_n += 1
    return letters_n


def count_words(text):
    """
    The word counting function
    """
    words_n = 1
    for i in range(len(text)):
        if text[i] == ' ':
            words_n += 1
    return words_n


def count_sentences(text):
    """
    The sentence counting function
    """
    sentences_n = 0
    for i in range(len(text)):
        if text[i] == '.' or text[i] == '!' or text[i] == '?':
            sentences_n += 1
    return sentences_n


main()