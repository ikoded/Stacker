#include "../include/GameBoard.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>

/*

Useful global methods for this class

*/

// make sure file is closed after using
// used for scoreboard and past data
std::ofstream openFileAppend(){
    std::ofstream myFile;
    myFile.open("data/gameboard.txt", std::ios_base::app);
    return myFile;
}

template <typename T>
void printArray(const std::vector<T> &target){
    for(auto it = target.begin(); it != target.end(); ++it){
        std::cout << *it;
    }
}

/*

Setters and Getters

*/
void GameBoard::setDebug(bool value){
    GameBoard::DEBUG = value;
}

bool GameBoard::getDebug(){
    return GameBoard::DEBUG;
}

/*

Game Functionality

*/

void GameBoard::startGame(){
    for(int i = 0; i < 12; i++){
        for(int y = 0; y < 7; y++ ){
            std::cout;
        }
    }
    std::cout << "Welcome to Mega Stacker! (not copyright don't sue pease)" << std::endl;
    
}

// this function is for moving any row you are currently on
void GameBoard::moveRow(int row){
    // Initial variables
    std::vector<int> &rowTarget = this->gameboard[row];
    // game pieces start in the middle
    int IndexOne = 2;
    int IndexTwo = 3;
    int IndexThree = 4;
    // start going to right
    bool addorsub = true;

    int count = 0; // TEMP MAKE IT TRIGGER ON INPUT EVENT (MULTITHREADING)
    
    // set game pieces to start in middle
    rowTarget[IndexOne] = 1;
    rowTarget[IndexTwo] = 1;
    rowTarget[IndexThree] = 1;

    // print original starting point
    if(GameBoard::getDebug()) std::cout << "Row " << row << ": ";
    printArray(rowTarget);
    std::cout << std::flush;


    while(count !=15){
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        // clear only the moving pieces from the current row
        rowTarget[IndexOne] = 0;
        rowTarget[IndexTwo] = 0;
        rowTarget[IndexThree] = 0;
        indexMove(IndexOne,IndexTwo,IndexThree,addorsub,row);
        rowTarget[IndexOne] = 1;
        rowTarget[IndexTwo] = 1;
        rowTarget[IndexThree] = 1;

        std::cout << '\r';
        if(GameBoard::getDebug()) std::cout << "Row " << row << ": ";
        printArray(rowTarget);
        std::cout << std::flush;
        count++;
    }
    std::cout << std::endl;
}

// this function actively changes the first 4 parameters
// it has logic checking for boundaries of the row provided
// basically, it stops in adding if the third index on far right is at border
// same goes for subtracting but if the first index on far left is at index 0
void GameBoard::indexMove(int &indexone, int &indextwo, int &indexthree, bool &addorsub, int row){
    if(addorsub){
        int size = this->gameboard[row].size();
        if(indexthree==(size-1)){
            // this means that the index is at the right most point, need to reverse
            addorsub = false; // set to subtract
        }else{
            indexone++;
            indextwo++;
            indexthree++;
        }
    }else{
        if(indexone==0){
            // this means that index is at left most point, need to reverse
            addorsub = true; // set to add
        }else{
            indexone--;
            indextwo--;
            indexthree--;
        }
    }
}

void GameBoard::clearGameBoard(){
    GameBoard::gameboard.clear();
    GameBoard::gameboard = {
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
}
