#pragma once
#include <vector> // used for gameboard and such
#include <string> // string library
#include <atomic> // used for race condition of threads setting `passRow`
#include <iostream> // used for outputting and grabbing inputs
#include <fstream> // used for files of player data like scoreboards (WIP)
#include <chrono> // used for ms/seconds sleeping a thread
#include <thread> // used for grabbing user input while displaying game
#include <conio.h> // used for get user input method 
#include <algorithm>

class GameBoard{
    public:
        /*
        
        Setters & Getters
        
        */

        // debug flag
        void setDebug(bool value); 
        bool getDebug();
        // if user passed the row level, this is controlled by two threads at once
        void setPassRow(bool value);
        bool getPassRow();
        // vector used for comparing if user messed up
        // private so it does not become manipulated on accident
        void setLastRow(std::vector<int> row);
        std::vector<int> getLastRow();
        // rows used in game
        void setRows(int rows);
        int getRows();

        /*
        
        Public Game Functionality
        
        */

        // start game menu
        void startGame();
        
        // initializer
        // calls initalize gameboard with default rows of 12
        GameBoard();

    private:
        /*
        
        Private Game Functionality
        
        */
        // initalize gameboard
        void initializeGameBoard();
        
        // move row left and right
        // indexMove is helper function for border logic
        void moveRow(int row);
        void grabIndices(int &indexone, int &indextwo, int &indexthree, std::vector<int> &rowList); // grabs indices in row
        void indexMove(int &indexone, int &indextwo, int &indexthree, bool &addorsub, int row);
        bool helperMove(int &indexone, int &indextwo,int &indexthree, bool addorsub, int row); // helper if 3 indices
        bool helperMove(int &indexone, int &indextwo, bool addorsub, int row); // helper if 2 indices
        bool helperMove(int &indexone, bool addorsub, int row); // helper if 1 index
        
        bool compareUserBricks(std::vector<int> &pastRow, int row);
        // loop over how ever many rows set
        void loopRows();
        // get user input for each row (ran in second thread)
        void getUserInput();
        // change rules of game, consists of:
        // - rows
        void changeRules();

        // debug menu
        void debugMethod();

        /*
        
        Private variables
        
        */
        bool DEBUG;
        std::atomic<bool> passRow{false};
        int rows = 12;
        std::vector<int> pastRow;
        // gameboard, potenitally make it private
        std::vector<std::vector<int>> gameboard;
};