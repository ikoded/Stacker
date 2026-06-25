#include "../include/Player.h"

void Player::setUserScore(int score){
    Player::userScore = score;
}

void Player::addToUserScore(int score){
    Player::userScore += score;
}

int Player::getUserScore(){
    return Player::userScore;
}