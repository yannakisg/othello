#ifndef PLAYER_H
#define PLAYER_H

#include <QString>

class ScoreWidget;
class QLabel;

class Player {

public:

    Player(bool isHuman, const QString &name);

    void setName(const QString &name);
    QString getName() const;

    bool getTurn();
    void setTurn(bool turn);
    void incrScore(short n);
    void setScoreWidget(ScoreWidget *widget);
    void setScore(short n);
    void clearAll();
    short getScore();

private:
    ScoreWidget *scorewidget;
    QString playerName;
    bool isBlack; /* isBlack == true ? FIRST : SECOND */
    short score;
};

#endif // PLAYER_H
