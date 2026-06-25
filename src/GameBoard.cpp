#include "../include/GameBoard.h"
#include <iostream>
#include <fstream>

// make sure file is closed
std::ofstream openFileAppend(){
    std::ofstream myFile;
    myFile.open("data/gameboard.txt", std::ios_base::app);
    return myFile;
}

void GameBoard::displayMenu(){

}

void GameBoard::dispayScoreBoard(){

}

void GameBoard::startGame(){
    for(int i = 0; i < 12; i++){
        for(int y = 0; y < 7; y++ ){
            std::cout;
        }
    }
    std::cout << "Welcome to Mega Stacker! (not copyright don't sue pease)" << std::endl;
    
}

void GameBoard::createTopBottom(char characterChoice = '*'){
    std::ofstream myFile = openFileAppend();
    for(int i = 0; i < 7; i++ ){
        for(int y = 0; y < 5; y++){
            myFile << characterChoice;
        }
        myFile << " ";
    }
    myFile << std::endl;
    myFile.close();
}

void GameBoard::createSides(){
    std::ofstream myFile = openFileAppend();
    for(int i = 0; i < 7; i++ ){
        for(int y = 0; y < 5; y++){
            char characterNeeded = (y==0||y==4) ? '*' : ' ';
            myFile << characterNeeded;
        }
        myFile << " ";
    }
    myFile << std::endl;
    myFile.close();
}

void GameBoard::createRow(){
    createTopBottom();
    for(int i = 0; i < 2; i++){
        createSides();
    }
    createTopBottom();
}

void GameBoard::processGameBoard(){
    int rows = this->gameboard.size();
    int cols = this->gameboard[0].size();
    for(int i =0; i < rows; i++){
        for(int y = 0; y < cols; y++){
            
        }
    }
}

// this function is for moving any row you are currently on
void GameBoard::moveRow(int row){
    // Initial variables
    bool check = true;
    std::vector<int> rowTarget = this->gameboard[row];
    // game pieces start in the middle
    int IndexOne = 2;
    int IndexTwo = 3;
    int IndexThree = 4;
    // start going to right
    bool addorsub = true;
    // set game pieces to start in middle
    rowTarget[IndexOne] = 1;
    rowTarget[IndexTwo] = 1;
    rowTarget[IndexThree] = 1;

    while(check){
        // SLEEP RIGHT HERE IN BETWEEN EACH MOVE
        // set the middle game pieces to zero
        rowTarget[IndexOne] = 0;
        rowTarget[IndexTwo] = 0;
        rowTarget[IndexThree] = 0;
        // using logic identify index and which way it can move
        indexMove(IndexOne,IndexTwo,IndexThree,addorsub,row);
        // set next index spots to 1 (this moves it over left and right in loop, never going out of bounds)
        // since c++ is fast, it updates pretty quickly
        rowTarget[IndexOne] = 1;
        rowTarget[IndexTwo] = 1;
        rowTarget[IndexThree] = 1;
    }
}

// this function actively changes the first 4 parameters
// it has logic checking for boundaries of the row provided
// basically, it stops in adding if the third index on far right is at border
// same goes for subtracting but if the first index on far left is at index 0
void GameBoard::indexMove(int &indexone, int &indextwo, int &indexthree, bool &addorsub, int row){{
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

}

void GameBoard::clearGameBoard(){
    std::ofstream myFile("data/gameboard.txt");
    myFile << "";
    myFile.close();
}
