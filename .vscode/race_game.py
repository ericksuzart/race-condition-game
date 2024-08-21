import random
import threading
import time

# Shared resource
finish_line = 100
WhoWon = ""
player1_position = 0
player2_position = 0

def player_race(player_name, position):
    global player1_position, player2_position, WhoWon

    while True:
        # Simulate the player moving forward randomly
        move = 10
        if player_name == "Player 1":
            player1_position += move
            position = player1_position
        else:
            player2_position += move
            position = player2_position

        print(f"{player_name} moves to position {position}")

        # Check if the player has crossed the finish line
        if position >= finish_line:
            WhoWon = player_name
            break

        # Small delay to simulate race pace
        time.sleep(0.5)

def game():
    global WhoWon
    while True:
        if WhoWon != "":
            print(f"Winner is {WhoWon}")
            break
        time.sleep(1)

# Create threads for both players
player1_thread = threading.Thread(target=player_race, args=("Player 1", player1_position))
player2_thread = threading.Thread(target=player_race, args=("Player 2", player2_position))
game_thread = threading.Thread(target=game)

# Start the race
player1_thread.start()
player2_thread.start()
game_thread.start()

# Wait for both players to finish
player1_thread.join()
player2_thread.join()
game_thread.join()

print("Race finished!")
