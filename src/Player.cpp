#include "../include/Player.h"
#include <iostream>

void Player::setUserScore(int score){
    this->userScore = score;
}

void Player::addToUserScore(int score){
    this->userScore += score;
}

int Player::getUserScore(){
    return this->userScore;
}

void Player::addToLeaderboard(){
    std::ofstream myFile("data/player-data.csv", std::ios::app);
    if (!myFile) {
        std::cerr << "Could not open data/player-data.csv\n";
        return;
    }
    time_t timestamp = time(NULL);
    
    myFile << getPlayerName() << "," << getUserScore() << "," << ctime(&timestamp);

    myFile.close();
}

void Player::setPlayerName(std::string name){
    this->name = name;
}

std::string Player::getPlayerName(){
    return this->name;
}