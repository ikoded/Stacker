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
    gameBoard.moveRow(0);
    this_thread::sleep_for(chrono::seconds(20));
    gameBoard.clearGameBoard();


    return 0;
}