#include "../include/GameBoard.h"

#define NAME_OF(x) #x // used for getting variable name listing rules out

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

// template to printArray
template <typename T>
void printArray(const std::vector<T> &target){
    for(auto it = target.begin(); it != target.end(); ++it){
        std::cout << *it;
    }
}

// clear cin for next use
void clearCIN(){
    std::cin.clear();
    std::cin.ignore(10000,'\n');
}

// clear stdout
void clearSTDOUT(){
    system("cls");
}

// sleep animation in between menus
void sleepAnimation(){
    for(int i = 0; i < 3; i++){
        std::cout << ".";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    clearSTDOUT(); // clear terminal
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

void GameBoard::setPassRow(bool value){
    GameBoard::passRow.store(value);
}

bool GameBoard::getPassRow(){
    return GameBoard::passRow.load();
}

void GameBoard::setRows(int rows){
    GameBoard::rows = rows;
}

int GameBoard::getRows(){
    return GameBoard::rows;
}

/*

Game Functionality

*/

// print game menu options for ease
void gameMenu(){
    std::cout << "Welcome to Mega Stacker! (not copyright don't sue pease)" << std::endl;
    std::cout << "1. Start Game\n2. Change difficulty\n3. Exit\nPlease enter a choice 1-3: ";
}

// start game with menu options
void GameBoard::startGame(){
    clearSTDOUT();
    char choice;
    bool check = true;
    while(check){
        gameMenu();
        std::cin >> choice;
        switch(choice){
            // Start game
            case '1':{
                std::cout << "Starting Game";
                sleepAnimation();
                loopRows(); // loop rows of game
                if(getDebug()){
                    debugMethod();
                }
                clearSTDOUT(); // clear terminal
                break;
            }
            // Change difficulty
            case '2':{
                std::cout << "Changing difficulty";
                sleepAnimation();
                changeRules(); // change rules menu
                clearSTDOUT(); // clear terminal
                break;
            }
            // Exiting
            case '3':{
                std::cout << "Exiting";
                sleepAnimation();
                check=false;
                break;
            }
            // Incorrect choice
            default:{
                std::cout << "Incorrect choice, please try again." << std::endl;
                clearSTDOUT(); // clear terminal
                break;
            }
        }

        clearCIN(); // clear cin and ignore new lines for next cin later
    }
}

// in future will be able to pass how big board is
void GameBoard::initializeGameBoard(){
    int rows = GameBoard::getRows();
    GameBoard::gameboard.clear();
    GameBoard::gameboard.resize(rows);
    for(int i = 0; i < rows; i++){
        GameBoard::gameboard[i] = {0,0,0,0,0,0,0}; 
    }
}

// this function is for moving any row you want to move
// WIP ADD FUNCTIONALITY FOR DYNAMIC AMOUNT OF INDICES
void GameBoard::moveRow(int row){
    setPassRow(false);
    // Initial variables
    std::vector<int> &rowTarget = this->gameboard[row];
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

    while(!getPassRow()){
        // order matters, this logic must happen first to show board correctly
        // clear only the moving pieces from the current row
        rowTarget[IndexOne] = 0;
        rowTarget[IndexTwo] = 0;
        rowTarget[IndexThree] = 0;

        indexMove(IndexOne,IndexTwo,IndexThree,addorsub,row);

        rowTarget[IndexOne] = 1;
        rowTarget[IndexTwo] = 1;
        rowTarget[IndexThree] = 1;

        std::cout << '\r';
        printArray(rowTarget);
        std::cout << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << std::endl;
    setPassRow(false);
}

// this function actively changes the first 4 parameters
// it has logic checking for boundaries of the row provided
// basically, it stops in adding if the third index on far right is at border
// same goes for subtracting but if the first index on far left is at index 0
// WIP ADD FUNCTIONALITY FOR DYNAMIC AMOUNT OF INDICES
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

// loop through moving rows and using multi thread to capture and print
void GameBoard::loopRows(){
    int rows = getRows();
    for(int i = 0; i < rows; i++){
        std::thread inputThread(&GameBoard::getUserInput, this);
        moveRow(i);
        inputThread.join();
    }
}

// grab users input if key presses x
// uses atomic value passRow and setter to handle race conditions of multi thread
void GameBoard::getUserInput(){
    while (true) {
        if (_kbhit()) { // a key was pressed
            char c = _getch(); // get the key immediately
            if (c == 'x') {
                setPassRow(true);
                break;
            }
        }
    }
}

// change rules of game so far rules consist of:
// - rows
void GameBoard::changeRules(){
    // variables to assign
    int rows;

    std::cout << "Current Rules:" << std::endl;
    std::cout << "- " << NAME_OF(rows) << ": " << getRows() << std::endl; 

    while(true){
        std::cout << "Rows: ";
        if(std::cin >> rows){
            break;
        }
        std::cout << "Please make sure rows is a number." << std::endl;
        clearCIN(); // clear cin and ignore new lines for next cin later
    }
    setRows(rows); // change to what user wants with setters
}

// used to see all of gameboard to make sure it saves history
void GameBoard::debugMethod(){
    int count = 0;
    // just prints gameboard with each row
    for(std::vector<int> row : GameBoard::gameboard){
        std::cout << "Row " << count << ": ";
        printArray(row);
        std::cout << std::endl;
        count++;
    }
    std::this_thread::sleep_for(std::chrono::seconds(30));
}

// initialize gameboard
GameBoard::GameBoard(){
    initializeGameBoard(); // sets board to size right away
}
