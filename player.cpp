#include "player.h"
#include "scorewidget.h"

/**
  * Constructor of Player
  */
Player::Player(bool isHuman, const QString &name) {
    isBlack = isHuman;
    playerName = name;
    score = 2;
}

/**
  * Sets the score widget
  */
void Player::setScoreWidget(ScoreWidget *widget) {
    scorewidget = widget;
}

/**
  * Sets player's name
  */
void Player::setName(const QString &name) {
    playerName = name;
}

/**
  * Returns player's name
  */
QString Player::getName() const{
    return playerName;
}

/**
  * Sets  player's score
  */
void Player::setScore(short n) {
    scorewidget->setPoints(n);
    score = n;
}

/**
  * Increases player's score
  */
void Player::incrScore(short n) {
    scorewidget->addPoints(n);
    score += n;
}

/**
  * Initializes player's score
  */
void Player::clearAll() {
   scorewidget->clearAll();
}

/**
  * Returns player's score
  */
short Player::getScore() {
    return score;
}

/**
  * Sets player's turn
  * 1st TRUE  BLACK
  * 2nd FALSE WHITE
  */
void Player::setTurn(bool turn) {
    isBlack = turn;
}

/**
  * Returns player's turn
  */
bool Player::getTurn() {
    return isBlack;
}
