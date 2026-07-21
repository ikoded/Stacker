#pragma once
#include <string>
#include <ctime>
#include <fstream>

class Player {
    public:
        void setUserScore(int score);
        void addToUserScore(int score);
        int getUserScore();
        void addToLeaderboard();

        void setPlayerName(std::string name);
        std::string getPlayerName();

    private:
        int userScore = 0;
        std::string name;
};