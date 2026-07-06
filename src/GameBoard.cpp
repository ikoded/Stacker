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
    
    int IndexOne = 69 , IndexTwo = 69, IndexThree = 69;  //set to 69 to show index not set yet 
    if(row==0){
        IndexOne = 2;
        IndexTwo = 3;
        IndexThree = 4;
    }else{
        grabIndices(IndexOne,IndexTwo,IndexThree,GameBoard::pastRow); // checks in row where 1's are
        // changes indices automatically
    }
    
    // start going to right every row
    // could randomize this
    bool addorsub = true;

    // set indices if available
    if(IndexOne!=69) rowTarget[IndexOne] = 1;
    if(IndexTwo!=69) rowTarget[IndexTwo] = 1;
    if(IndexThree!=69) rowTarget[IndexThree] = 1;
    
    
    while(!getPassRow()){
        // order matters, this logic must happen first to show board correctly
        // clear only the moving pieces from the current row
        if(IndexOne!=69) rowTarget[IndexOne] = 0;
        if(IndexTwo!=69) rowTarget[IndexTwo] = 0;
        if(IndexThree!=69) rowTarget[IndexThree] = 0;

        indexMove(IndexOne,IndexTwo,IndexThree,addorsub,row);

        if(IndexOne!=69) rowTarget[IndexOne] = 1;
        if(IndexTwo!=69) rowTarget[IndexTwo] = 1;
        if(IndexThree!=69) rowTarget[IndexThree] = 1;

        std::cout << '\r'; // move to beginning of line
        printArray(rowTarget);
        std::cout << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << std::endl;
    setPassRow(false);
}

void GameBoard::grabIndices(int &indexone, int &indextwo, int &indexthree, std::vector<int> &rowList){
    int size = rowList.size();
    for(int i = 0; i < size; i++){
        int col = rowList[i];
        if(col==1){ // index found
            if(indexone==69){ // not assigned yet
                indexone = i;
                continue;
            }
            if(indextwo==69){ // not assigned yet
                indextwo = i;
                continue;
            }
            if(indexthree==69){ // not assigned yet
                indexthree = i;
                continue;
            }
        }
    }
}

// this function actively changes the first 4 parameters
// it has logic checking for boundaries of the row provided
// basically, it adds or subtracts (move right or left) the indices to move the game piece
// [1,2,3] when I talk about the boxes indices in a row of [0,1,2,3,4,5,6,7], those three (down to one index) can move anywhere horizontally in that row
void GameBoard::indexMove(int &indexone, int &indextwo, int &indexthree, bool &addorsub, int row){
    if(indexone==69){ // this means the left brick is off

        if(indextwo==69){ // this means the middle brick is off
            addorsub = helperMove(indexthree, addorsub, row); // since 1,2 is off [1,2,3] that leaves only three
        }else{
            addorsub = helperMove(indextwo, indexthree, addorsub, row); // since 1 is off [1,2,3] that leaves [2,3]
        }
    }

    if(indexthree==69){ // this means the right brick is off

        if(indextwo==69){ // this means the middle brick is off
            addorsub = helperMove(indexone, addorsub, row); // since 3,2 is off [1,2,3] that leaves only 1
        }else{
            addorsub = helperMove(indexone, indextwo, addorsub, row); // since 3 is off [1,2,3] that leaves [1,2]
        }
    }

    // none of them have fallen so call three index helper move
    addorsub = helperMove(indexone, indextwo, indexthree, addorsub, row); // move all 3

}

// Move logic with three indices
bool GameBoard::helperMove(int &indexone, int &indextwo,int &indexthree, bool addorsub, int row){
    int size = this->gameboard[row].size();
    if(addorsub){ // adding
        if(indexthree==(size-1)){ // hit right wall
            addorsub = false; // set to subtract
        }else{ // move right
            indexone++;
            indextwo++;
            indexthree++;
        }
    }else{ // subtracting
        if(indexone==0){ // hit left wall
            addorsub = true; // set to add
        }else{ // move left
            indexone--;
            indextwo--;
            indexthree--;
        }
    }
    return addorsub;
}

// Move logic with two indices
bool GameBoard::helperMove(int &indexone, int &indextwo, bool addorsub, int row){
    int size = this->gameboard[row].size();
    if(addorsub){ // adding
        if(indextwo==(size-1)){ // hit right wall
            addorsub=false;
        }else{ // move right
            indexone++;
            indextwo++;
        }
    }else{ // subtracting
        if(indexone==0){ // hit left wall
            addorsub = true;
        }else{ // move left
            indexone--;
            indextwo--;
        }
    }
    return addorsub;
}

// Move logic with one index
bool GameBoard::helperMove(int &indexone, bool addorsub, int row){
    int size = this-> gameboard[row].size();
    if(addorsub){
        if(indexone==(size-1)){ // hit right wall
            addorsub = false;
        }else{ // move right
            indexone++;
        }
    }else{
        if(indexone==0){
            addorsub = true;
        }else{
            indexone--;
        }
    }

    return addorsub;
}

// compare if the user brick fell where first bricks fell
bool GameBoard::compareUserBricks(std::vector<int> pastRow, int row){
    std::vector<int> currentRow = gameboard[row];
    // setup indices for current row
    int IndexOne = 69 , IndexTwo = 69, IndexThree = 69; //set to 69 to show index not set yet 
    grabIndices(IndexOne,IndexTwo,IndexThree,currentRow);
    // setup indices for past row
    int PastIndexOne = 69, PastIndexTwo = 69, PastIndexThree = 69;
    grabIndices(PastIndexOne,PastIndexTwo,PastIndexThree,pastRow);

    if(IndexOne==69){
        if(IndexTwo==69){ // both right side indices are not there
            if(IndexThree!=PastIndexThree){
                return false; // this means they did not align
            }else{
                return true; // this means they did align
            }
        }
        if(IndexTwo!=PastIndexTwo) return false; // did not line up
        if(IndexThree!=PastIndexThree) return false; // did not line up
    }

    if(IndexThree==69){
        if(IndexTwo==69){
            if(IndexOne!=PastIndexOne){
                return false; // this means the last index sis not align
            } else{
                return true;
            }
        }
        if(IndexTwo!=PastIndexTwo) return false; // did not line up
        if(IndexOne!=PastIndexOne) return false; // did not line up
    }

    return true;
}

// loop through moving rows and using multi thread to capture and print
void GameBoard::loopRows(){
    int rows = getRows();
    bool check;
    std::vector<int> &pastRow = GameBoard::pastRow;
    for(int i = 0; i < rows; i++){
        std::thread inputThread(&GameBoard::getUserInput, this);
        moveRow(i);
        inputThread.join();
        if(i!=0){
            check = compareUserBricks(pastRow,i);
            if(!check){
                std::cout << "Game Lost!" << std::endl;
                break;
            }
        }
        pastRow = gameboard[i];
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
