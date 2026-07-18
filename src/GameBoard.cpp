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
    this->DEBUG = value;
}

bool GameBoard::getDebug(){
    return this->DEBUG;
}

void GameBoard::setPassRow(bool value){
    this->passRow.store(value);
}

bool GameBoard::getPassRow(){
    return this->passRow.load();
}

void GameBoard::setRows(int rows){
    this->rows = rows;
}

int GameBoard::getRows(){
    return this->rows;
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
    int rows = this->getRows();
    this->gameboard.clear(); // clear it
    this->gameboard.resize(rows); // resize with exact rows set
    for(int i = 0; i < rows; i++){
        this->gameboard[i] = {0,0,0,0,0,0,0}; // set each row in the vector of vector ints
    }
}

// this function is for moving any row you want to move
void GameBoard::moveRow(int row){
    setPassRow(false); // just make sure row does not pass right away
    // Initial variables
    std::vector<int> &rowTarget = this->gameboard[row];
    
    int IndexOne = 69 , IndexTwo = 69, IndexThree = 69;  //set to 69 to show index not set yet 
    if(row==0){ // first row starts in same place
        IndexOne = 2;
        IndexTwo = 3;
        IndexThree = 4;
    }else{
        grabIndices(IndexOne,IndexTwo,IndexThree,this->pastRow); // checks in row where 1's are
        // changes indices automatically
    }
    
    // start going to right every row
    // could randomize this every row with 0-1 random gen
    bool addorsub = true;

    // set indices if available
    if(IndexOne!=69) rowTarget[IndexOne] = 1;
    if(IndexTwo!=69) rowTarget[IndexTwo] = 1;
    if(IndexThree!=69) rowTarget[IndexThree] = 1;
    
    // the passRow variable is a class member of gameboard that is used by both threads to check if to keep going foward
    while(!getPassRow()){
        // order matters, this logic must happen first to show board correctly
        // clear only the existing pieces from the current row
        if(IndexOne!=69) rowTarget[IndexOne] = 0;
        if(IndexTwo!=69) rowTarget[IndexTwo] = 0;
        if(IndexThree!=69) rowTarget[IndexThree] = 0;

        indexMove(IndexOne,IndexTwo,IndexThree,addorsub,row); // logic that checks where to move and moves them

        // "moved" index one left or right depending on location, this updates the row
        if(IndexOne!=69) rowTarget[IndexOne] = 1;
        if(IndexTwo!=69) rowTarget[IndexTwo] = 1;
        if(IndexThree!=69) rowTarget[IndexThree] = 1;

        std::cout << '\r'; // move to beginning of line
        printArray(rowTarget); // display row with update of move, this is how it is animated
        std::cout << std::flush; // flush all changes
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // this will make it harder or easier, going to add different difficulties with this
    }
    setPassRow(false); // set passRow to false for next one
}

// this function grabs and updates each indice with the respective index if available
void GameBoard::grabIndices(int &indexone, int &indextwo, int &indexthree, std::vector<int> &rowList){
    int size = rowList.size(); // size of row in case row changes
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
    if(indexone<8&&indextwo<8&&indexthree<8){
        addorsub = helperMove(indexone, indextwo, indexthree, addorsub, row); // move all 3
    }

}

// Move logic with three indices
bool GameBoard::helperMove(int &indexone, int &indextwo,int &indexthree, bool addorsub, int row){
    int size = this->gameboard[row].size();
    if(addorsub){ // move right
        if(indexthree==(size-1)){ // hit right wall
            addorsub = false; // change directions
        }else{ 
            indexone++;
            indextwo++;
            indexthree++;
        }
    }else{ // move left
        if(indexone==0){ // hit left wall
            addorsub = true; // change directions
        }else{
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
            addorsub = true; // change directions
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
    if(addorsub){ // move right
        if(indexone==(size-1)){ // hit right wall
            addorsub = false;
        }else{ 
            indexone++;
        }
    }else{ // move left
        if(indexone==0){ // hit left wall
            addorsub = true; // change directions
        }else{
            indexone--;
        }
    }

    return addorsub;
}

// compare user bricks to the last row
bool GameBoard::compareUserBricks(std::vector<int> &pastRow, int row){
    bool check = true; // default to true

    std::vector<int> currentRow = gameboard[row]; // grab current row
    // setup indices for current row
    int IndexOne = 69 , IndexTwo = 69, IndexThree = 69; //set to 69 to allow for dynamic indices sizes
    grabIndices(IndexOne,IndexTwo,IndexThree,currentRow); // these indices are for current row
    // setup indices for past row
    int PastIndexOne = 69, PastIndexTwo = 69, PastIndexThree = 69; //set to 69 to allow for dynamic indices sizes
    grabIndices(PastIndexOne,PastIndexTwo,PastIndexThree,pastRow); // these indices are for past row

    // set diffrow to 69 to start
    int diffrowone = 69, diffrowtwo = 69, diffrowthree = 69;

    // if past rows indices not 69, then set the diffrow for index by subtracting
    // these will all be the same if 3 or 2 bricks, of course if 2 bricks one index will be 69 (fell off)
    // if all 1 that means it fell to right
    // if all -1 that means it fell to left
    if(PastIndexOne!=69) diffrowone = IndexOne - PastIndexOne;
    if(PastIndexTwo!=69) diffrowtwo = IndexTwo - PastIndexTwo;
    if(PastIndexThree!=69) diffrowthree = IndexThree - PastIndexThree;

    // check how many bricks in play
    int sizebricks = 0;
    // add if difference exists (dynamic amount of bricks)
    if(diffrowone!=69) sizebricks++;
    if(diffrowtwo!=69) sizebricks++;
    if(diffrowthree!=69) sizebricks++;

    switch(sizebricks){ // based on how many bricks left
        case 0: { // game over (should never get here)
            check = false;
            break;
        }
        case 1: { // 1 brick left
            if(diffrowone==0||diffrowtwo==0||diffrowthree==0){ // passed
            } else{
                // game over (this is usually what will end the game when getting to 1 brick)
                check = false;
            }
            break;
        }
        case 2: { // 2 bricks left
            if((diffrowone==0&&diffrowtwo==0)||(diffrowthree==0&&diffrowtwo==0)){ // passed
            } else if((diffrowone==1&&diffrowtwo==1)||(diffrowthree==1&&diffrowtwo==1)){ // lost a right brick
                if(diffrowone==1){ // this means the middle and left are the only bricks [(1,2),3]
                    this->gameboard[row].at(IndexTwo) = 0; // take off right most brick
                }else if (diffrowthree==1){ // this means right and middle are the only bricks [1,(2,3)]
                    this->gameboard[row].at(IndexThree) = 0; // take off right most brick
                } else { // all fell off board this means
                    check = false;
                }
            } else if ((diffrowone==-1&&diffrowtwo==-1)||(diffrowthree==-1&&diffrowtwo==-1)){ // lost a left brick
                if(diffrowone==-1){ // this means the middle and left are the only bricks [(1,2),3]
                    this->gameboard[row].at(IndexOne) = 0; // take off left most brick
                }else if (diffrowthree==-1){ // this means right and middle are the only bricks [1,(2,3)]
                    this->gameboard[row].at(IndexTwo) = 0; // take off left most brick
                } else{ // all fell off board this means
                    check = false;
                }
            } else { // all fell off board this means
                check = false;
            }
            break;
        }
        case 3: { // 3 bricks left
            if(diffrowone==0||diffrowtwo==0||diffrowthree==0){ // passed
            } else if(diffrowone==1||diffrowtwo==1||diffrowthree==1){ // lost a right brick
                this->gameboard[row].at(IndexThree) = 0; // take off right most brick
            } else if (diffrowone==-1||diffrowtwo==-1||diffrowthree==-1){ // lost a left brick
                this->gameboard[row].at(IndexOne) = 0; // take off left most brick
            } else{ // game over
                check = false;
            }
            break;
        }
        default: {
            // shouldn't ever get here, if it did error program
            break;
        }
    }

    if(IndexOne==69&&IndexTwo==69&&IndexThree==69){ // game over since all off board, realistically this was caught earlier in logic
        check = false;
    }
    return check;

}

// loop through moving rows and using multi thread to capture and print
void GameBoard::loopRows(){
    int rows = getRows(); // how many rows user will play, can change this in setting menu
    
    bool check;
    std::vector<int> &pastRow = this->pastRow;
    for(int i = 0; i < rows; i++){
        std::vector<int> &rowTarget = this->gameboard[i];
        std::thread inputThread(&getUserInput, this);
        moveRow(i); // run displaying rows ("move" bricks in row this means)
        inputThread.join(); // wait for user input thread to complete
        if(i!=0){
            check = compareUserBricks(pastRow, i);
            if(!check){
                std::cout << "Game Lost!" << std::endl;
                // TODO: Calculate Player Score
                break;
            }
            std::cout << '\r'; // move to beginning of line
            printArray(rowTarget); // update display of row in case something fell
            std::cout << std::flush; // flush all changes
        }
        // new line for new row
        std::cout << std::endl;
        // set pastRow where this current row ended since this row was manipulated if brick fell off
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
                // row has "passed" which is used for multi thread to tell display loop to stop
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
    for(std::vector<int> row : this->gameboard){
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
