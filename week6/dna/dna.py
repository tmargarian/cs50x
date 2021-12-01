import sys
import csv


def main():
    # Checking the number of command line arguments
    if len(sys.argv) != 3:
        print("Usage: dna.py <database_file.csv> <sequence_file.txt>")
        sys.exit(1)
    
    # Reading the file that needs to be sequences
    dna_seq_file = open(sys.argv[2], "r")
    dna_seq = dna_seq_file.read()
    dna_seq_file.close()

    # Reading the database of existing sequences
    patterns = []
    patterns_search = {}

    # Reading the pattern types from database
    row_num = 1
    with open(sys.argv[1], "r") as file:
        reader = csv.reader(file)
        for row in reader:
            if row_num == 1:
                patterns = row[1:len(row)]
            row_num += 1

    # Counting the number of patterns                
    for i in range(len(patterns)):
        patterns_search[patterns[i]] = 0
        for j in range(round(len(dna_seq) / len(patterns[i])) + 1):
            if dna_seq.find(patterns[i] * j) != -1:
                patterns_search[patterns[i]] = j

    # Comparing the sequencing results to the database
    with open(sys.argv[1], "r") as file:
        reader = csv.reader(file)
        next(reader)  # skipping the header this time
        for row in reader:
            db_entry_list = [int(i) for i in row[1:len(row)]]
            db_entry = {patterns[i]: db_entry_list[i] for i in range(len(patterns))}
            if db_entry == patterns_search:
                print(row[0])
                sys.exit(0)

    # If we got here - there was no match
    print("No match")
    sys.exit(0)
          

main()