#pragma once
#include <vector>
#include <string>

class GameBoard{
    public:
        void setDebug(bool value);
        bool getDebug();
        void startGame();
        void initializeGameBoard();
        // move row left and right
        void moveRow(int row);
        void indexMove(int &indexone, int &indextwo, int &indexthree, bool &addorsub, int row);
        // clear gameboard file
        void clearGameBoard();

        GameBoard();

        std::vector<std::vector<int>> gameboard;

    private:
        bool DEBUG;
};