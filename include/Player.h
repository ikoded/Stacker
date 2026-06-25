#pragma once

class Player {
    public:
        void setUserScore(int score);
        void addToUserScore(int score);
        int getUserScore();

    private:
        int userScore;
};