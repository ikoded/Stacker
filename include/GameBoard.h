#pragma once
#include <vector>
#include <string>

class GameBoard{
    public:
        void displayMenu();
        void dispayScoreBoard();
        void startGame();
        // creation of brick in file
        void createTopBottom(char characterChoice); // top bottom of boxes
        void createSides(); // sides of boxes
        // creation of row of bricks in file
        void createRow();
        // proccess gameboard
        void processGameBoard();
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
};