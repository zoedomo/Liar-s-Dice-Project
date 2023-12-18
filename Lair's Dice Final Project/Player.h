#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <cstdlib> 
#include <algorithm> 
#include <iostream> 

using namespace std;

class Player {
public:
    vector<int> dice;

    Player() {
        dice.resize(5);
    }

    void rollDice() {
        for (int& die : dice) {
            die = rand() % 6 + 1;
        }
    }

    void showDice() const {
        for (int die : dice) {
            cout << die << " ";
        }
        cout << endl;
    }

    int countDice(int number) const {
        return count(dice.begin(), dice.end(), number);
    }
};

#endif 