#include "../include/Player.h"
#include "../include/GameBoard.h"
#include <vector>
#include <chrono>
#include <thread>

using namespace std;

GameBoard gameBoard;
vector<thread> workerThreads;

void loopRows(){
    for(int i = 0; i < 12; i++){
       workerThreads.emplace_back(&GameBoard::getUserInput, &gameBoard); // ask for user input
        gameBoard.moveRow(i);
    }
}

int main(){
    
    gameBoard.setDebug(false);
    
    // notice since function of class you need to pass
    gameBoard.clearGameBoard();
    workerThreads.emplace_back(loopRows);

    for(auto& t: workerThreads){
        if(t.joinable()) t.join();
    }
    
    gameBoard.clearGameBoard();


    return 0;
}