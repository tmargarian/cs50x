# Simulate a sports tournament

import csv
import sys
import random

# Number of simluations to run
N = 1000


def main():

    # Ensure correct usage
    if len(sys.argv) != 2:
        sys.exit("Usage: python tournament.py FILENAME")

    teams = []
    counts = {}
    # Read teams into memory from file
    with open(sys.argv[1], "r") as file:
        reader = csv.reader(file)
        next(reader)
        for row in reader:
            teams.append({"team": row[0], "rating": int(row[1])})
            counts[row[0]] = 0

    # TODO: Simulate N tournaments and keep track of win counts
    for i in range(N):
        counts[simulate_tournament(teams)] += 1

    # Print each team's chances of winning, according to simulation
    for team in sorted(counts, key=lambda team: counts[team], reverse=True):
        print(f"{team}: {counts[team] * 100 / N:.1f}% chance of winning")


def simulate_game(team1, team2):
    """Simulate a game. Return True if team1 wins, False otherwise."""
    rating1 = team1["rating"]
    rating2 = team2["rating"]
    probability = 1 / (1 + 10 ** ((rating2 - rating1) / 600))
    return random.random() < probability


def simulate_round(teams):
    """Simulate a round. Return a list of winning teams."""
    winners = []

    # Simulate games for all pairs of teams
    for i in range(0, len(teams), 2):
        if simulate_game(teams[i], teams[i + 1]):
            winners.append(teams[i])
        else:
            winners.append(teams[i + 1])

    return winners


def simulate_tournament(teams):
    """Simulate a tournament. Return name of winning team."""
    # If there are 0 teams to simulate 
    if len(teams) == 0:
        return False
    
    winning_teams = teams

    while True:
        if len(winning_teams) == 1:
            return winning_teams[0]["team"]
        else:
            winning_teams = simulate_round(winning_teams)


if __name__ == "__main__":
    main()
