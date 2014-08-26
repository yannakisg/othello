#ifndef OTHELLO_H
#define OTHELLO_H

#include <QList>
#include <QPoint>

class Player;
class BoardWidget;
class QString;


class Othello {

public:
    Othello(Player *player1, Player *player2, BoardWidget *board, bool turn);
    ~Othello();
    bool allowed(QList<QPoint *> *,const int, const int);
    bool allowed(const int, const int);
    Player* updateBoardTableStateAllowedMoves(const int, const int);
    Player* updateBoardTableStateAllowedMoves(unsigned char**,QList<QPoint *> *,const int, const int);
    void initializeTableStateAndAllowedMoves();
    QString* getTableState();
    QString* getAllowedMoves();
    int getNextTurn();
    void setNextTurn(int t);
    void setTableState(QString& state);
    void setAllowedMoves(QString& moves);
    void setMaxDepth(int depth);
    int getMaxDepth();
    QList<QPoint *>* getAllowed();
    unsigned char **getState();
    bool updateAllowedMoves(QList<QPoint *> *, unsigned char **);
    bool getP2Turn();
    void setBoolNextTurn(bool);
    bool getBoolNextTurn();
    void updateBoardTableState(unsigned char**,const int, const int, bool);

private:

    void updateTableState(unsigned char**,const int, const int, unsigned char**, bool);

    void changeIcon(const int, const int);
    int checkUpperLeft(unsigned char**,const int, const int);
    int checkLowerLeft(unsigned char**,const int, const int);
    int checkLeft(unsigned char**,const int, const int);
    int checkUpperRight(unsigned char**,const int, const int);
    int checkLowerRight(unsigned char**,const int, const int);
    int checkRight(unsigned char**,const int, const int);
    int checkTop(unsigned char**,const int, const int);
    int checkBottom(unsigned char**,const int, const int);
    unsigned char** getSeries(unsigned char**,const int, const int);

    BoardWidget *boardwidget;
    bool isBlacksTurn;
    unsigned char** tableState;
    QList<QPoint *> *allowedMoves;
    int seriesNumber;
    int max_depth;
    Player *p1, *p2;
};

#endif // OTHELLO_H
