#include "../include/Player.h"
#include "../include/GameBoard.h"
#include <vector>
#include <chrono>
#include <thread>

using namespace std;

int main(){

    GameBoard gameBoard;
    gameBoard.clearGameBoard();
    gameBoard.setDebug(false);
    for(int i = 0; i < 12; i++){
        gameBoard.moveRow(i);
    }
    gameBoard.clearGameBoard();


    return 0;
}