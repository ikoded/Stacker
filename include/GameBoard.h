#pragma once
#include <vector>
#include <string>

class GameBoard{
    public:
        void setDebug(bool value);
        bool getDebug();
        void startGame();
        // move row left and right
        void moveRow(int row);
        void indexMove(int &indexone, int &indextwo, int &indexthree, bool &addorsub, int row);
        // clear gameboard file
        void clearGameBoard();

        std::vector<std::vector<int>> gameboard = {
            {0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0}
        };

    private:
        bool DEBUG;
};