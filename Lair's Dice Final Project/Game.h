// Zoe Domagalski. IT-312, 12/01/2023, 7-1 Final Project

// Liar's Dice
// It begins by displaying the game rules from a file and then prompts players to enter 
// the number of participants. Each player rolls a set of five dice, and the game proceeds in turns. During their turn, 
// a player guesses the total number of a specific dice number(e.g., "3 4") present across all players' dice. 
// The guess must be higher than the previous one. Players have the option to accuse the previous player of lying. 
// If an call is made, the game reveals all dice and determines the winner based on the last guess. 
// There is input validation, including checking for valid integers and makes sure guesses are within appropriate ranges. 
// The main game loop continues until a player is accused of lying, at which point the game is over and a winner is announced.


#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstdlib>

using namespace std;

class Game {
private:
    vector<Player> players;
    int currentPlayerIndex;
    int lastGuessNumber;
    int lastGuessCount;
    bool gameOver;

    int getValidIntegerInput(const string& prompt) {
        int value;
        while (true) {
            cout << prompt;
            cin >> value;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a number." << endl;
            }
            else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
        }
        return value;
    }

    bool getValidGuess(string prompt, int& count, int& number) {
        string input;
        cout << prompt;
        getline(cin, input);
        stringstream ss(input);

        ss >> count;
        if (ss.fail() || count < 1) {
            cout << "Invalid count. Please enter a valid number." << endl;
            return false;
        }

        ss >> number;
        if (ss.fail() || number < 1 || number > 6) {
            cout << "Invalid number. Please enter a number between 1 and 6." << endl;
            return false;
        }

        return true;
    }

    char getValidCharInput(const string& prompt) {
        char value;
        while (true) {
            cout << prompt;
            cin >> value;

            if (value == 'y' || value == 'n') {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
            else {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter 'y' or 'n'." << endl;
            }
        }
        return value;
    }

public:
    Game() : currentPlayerIndex(0), lastGuessNumber(0), lastGuessCount(0), gameOver(false) {
        srand(static_cast<unsigned int>(time(NULL)));
        initializeGame();
    }

    void initializeGame() {
        displayRules("liars_dice_rules.txt");
        int numPlayers = getValidIntegerInput("Enter the number of players (minimum 2): ");
        while (numPlayers < 2) {
            numPlayers = getValidIntegerInput("Please add more players. Enter the number of players (minimum 2): ");
        }

        players = vector<Player>(numPlayers);
        for (Player& player : players) {
            player.rollDice();
        }
    }

    void playGame() {
        while (!gameOver) {
            processTurn();
        }
    }

    void processTurn() {
        players[currentPlayerIndex].showDice();

        int guessedCount, guessedNumber;
        bool validInput = false;
        while (!validInput) {
            validInput = getValidGuess("Player " + to_string(currentPlayerIndex + 1) + ", enter your guess (count number): ", guessedCount, guessedNumber);
        }

        if (isGuessValid(guessedCount, guessedNumber)) {
            lastGuessNumber = guessedNumber;
            lastGuessCount = guessedCount;

            char choice = getValidCharInput("Do you want to call the previous player a liar? (y/n): ");
            if (choice == 'y') {
                gameOver = true;
                announceWinner();
            }
            else {
                currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
            }
        }
        else {
            cout << "Invalid guess. Try again." << endl;
        }
    }

    bool isGuessValid(int guessedCount, int guessedNumber) {
        return (guessedCount > lastGuessCount) || (guessedCount == lastGuessCount && guessedNumber > lastGuessNumber);
    }

    void showAllDice() {
        for (size_t i = 0; i < players.size(); ++i) {
            cout << "Player " << (i + 1) << ": ";
            players[i].showDice();
        }
    }

    void announceWinner() {
        showAllDice();

        int actualCount = 0;
        for (const Player& player : players) {
            actualCount += player.countDice(lastGuessNumber);
        }

        // Determine the player who made the accusation
        int callingPlayer = (currentPlayerIndex - 1 + players.size()) % players.size();

        if (actualCount >= lastGuessCount) {
            // The last guess was correct, so the player who made the accusation (callingPlayer) loses
            cout << "Player " << (currentPlayerIndex + 1) << " wins!" << endl;
        }
        else {
            // The last guess was incorrect, so the player who made the accusation (callingPlayer) wins
            cout << "Player " << (callingPlayer + 1) << " wins!" << endl;
        }
    }

    void displayRules(const string& filename) {
        ifstream file(filename);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                cout << line << endl;
            }
            file.close();
        }
        else {
            cout << "Unable to open the file containing the rules." << endl;
        }
    }
};

#endif