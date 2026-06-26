#pragma once
#include <vector>
#include <string>
#include <thread>
#include <atomic>

class GameBoard{
    public:
        void setDebug(bool value);
        bool getDebug();
        void setPassRow(bool value);
        bool getPassRow();
        void startGame();
        void initializeGameBoard();
        // move row left and right
        void moveRow(int row);
        void indexMove(int &indexone, int &indextwo, int &indexthree, bool &addorsub, int row);
        // user input
        void getUserInput();
        // clear gameboard file
        void clearGameBoard();

        GameBoard();

        std::vector<std::vector<int>> gameboard;

    private:
        bool DEBUG;
        std::atomic<bool> passRow{false};
};