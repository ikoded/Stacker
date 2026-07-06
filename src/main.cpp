#include "../include/Player.h"
#include "../include/GameBoard.h"

using namespace std;

GameBoard gameBoard;

int main(){
    gameBoard.setDebug(false); // set debugging messages

    gameBoard.startGame(); // start game menu
    
    return 0;
}