#include <QMessageBox>

#include "othello.h"
#include "player.h"
#include "minimax.h"
#include "boardwidget.h"


static QPoint pointArray[8][8] =
{
    {QPoint(0,0),QPoint(0,1),QPoint(0,2),QPoint(0,3),QPoint(0,4),QPoint(0,5),QPoint(0,6),QPoint(0,7)},
    {QPoint(1,0),QPoint(1,1),QPoint(1,2),QPoint(1,3),QPoint(1,4),QPoint(1,5),QPoint(1,6),QPoint(1,7)},
    {QPoint(2,0),QPoint(2,1),QPoint(2,2),QPoint(2,3),QPoint(2,4),QPoint(2,5),QPoint(2,6),QPoint(2,7)},
    {QPoint(3,0),QPoint(3,1),QPoint(3,2),QPoint(3,3),QPoint(3,4),QPoint(3,5),QPoint(3,6),QPoint(3,7)},
    {QPoint(4,0),QPoint(4,1),QPoint(4,2),QPoint(4,3),QPoint(4,4),QPoint(4,5),QPoint(4,6),QPoint(4,7)},
    {QPoint(5,0),QPoint(5,1),QPoint(5,2),QPoint(5,3),QPoint(5,4),QPoint(5,5),QPoint(5,6),QPoint(5,7)},
    {QPoint(6,0),QPoint(6,1),QPoint(6,2),QPoint(6,3),QPoint(6,4),QPoint(6,5),QPoint(6,6),QPoint(6,7)},
    {QPoint(7,0),QPoint(7,1),QPoint(7,2),QPoint(7,3),QPoint(7,4),QPoint(7,5),QPoint(7,6),QPoint(7,7)},
};

/**
 * The constructor of Othello.
 */
Othello::Othello(Player *player1, Player *player2,BoardWidget *board, bool turn) {
    p1 = player1;
    p2 = player2;
    boardwidget = board;
    isBlacksTurn = turn;
    max_depth = 0;
}

/**
 * The destructor of Othello.
 */
Othello::~Othello() {
    for (int i = 0; i < 8; i++) {
        delete[] tableState[i];
    }

    delete allowedMoves;
    delete[] tableState;
}

/**
 * Returns the next round player. If 1, then next player is black, else, next player is white
 */
int Othello::getNextTurn() {
    if (isBlacksTurn)
        return 1;
    else
        return 2;
}

/**
 * Sets the maximum depth of the minimax(alpha-beta) algorithm.
 */
void Othello::setMaxDepth(int depth) {
    max_depth = depth;
}

/**
 * Returns the maximum depth of the minimax(alpha-beta) algorithm.
 */
int Othello::getMaxDepth() {
    return max_depth;
}

/**
 * Sets the next round player. If argument is 1, then the next round player is black, else next round player is white
 */
void Othello::setNextTurn(int t) {
    if (t == 1)
        isBlacksTurn = true;
    else
        isBlacksTurn = false;
}

/**
 * Sets the table state
 */
void Othello::setTableState(QString &state) {
    QChar *data = state.data();
    int i = 0;
    int j = 0;

    while (!data->isNull()) {

        if (data->toAscii() == '0') {
            tableState[i][j] = 0;
            boardwidget->setIconAt(GREEN,i,j);
            boardwidget->setButtonName(QString::number(i)+QString::number(j)+"g",i,j);
        }
        else if (data->toAscii() == '1') {
            tableState[i][j] = 1;
            boardwidget->setIconAt(BLACK,i,j);
            boardwidget->setButtonName(QString::number(i)+QString::number(j)+"b",i,j);
        }
        else if (data->toAscii() == '2') {
            tableState[i][j] = 2;
            boardwidget->setIconAt(WHITE,i,j);
            boardwidget->setButtonName(QString::number(i)+QString::number(j)+"w",i,j);
        }

        if (j == 7) {
            i++;
            j = 0;
        } else
            j++;
        ++data;
    }
}

/**
 *  Sets the allowed moves
 */
void Othello::setAllowedMoves(QString &moves) {
    int i,j;
    QStringList list = moves.split("|");

    while (!allowedMoves->empty())
        allowedMoves->removeLast();

    QStringList::iterator iter;

    for (iter = list.begin(); iter != list.end(); iter++){
        i = (*iter).toInt();

        iter++;
        if (iter == list.end())
            break;

        j = (*iter).toInt();
        allowedMoves->append(&pointArray[i][j]);
        boardwidget->setIconAt(VALIDGREEN, i, j);
        boardwidget->setButtonName(QString::number(i) + QString::number(j) + "v", i, j);
    }
}

/**
 * Returns a String with the allowed moves
 */
QString* Othello::getAllowedMoves() {
    QString *strAMoves = new QString();
    QPoint *point;

    foreach (point, *allowedMoves) {
        strAMoves->append(QString::number(point->x()));
        strAMoves->append("|");
        strAMoves->append(QString::number(point->y()));
        strAMoves->append("|");
    }

    return strAMoves;
}

/**
 * Returns a String with the current state of the table
 */
QString* Othello::getTableState() {
    QString *strState = new QString();

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            strState->append(QString::number(tableState[i][j]) );
        }
    }

    return strState;
}

/**
  * initialiazes the tableState array and the allowedMoves array.
  * in the tableState array, empty/green = 0, black = 1, white = 2
  * in the allowedMoves array, not legitimate move = false, legitimate move = true
  */
void Othello::initializeTableStateAndAllowedMoves()
{

    tableState = new unsigned char*[8];
    for (int i = 0; i < 8; i++)
        tableState[i] = new unsigned char[8];


    allowedMoves = new QList<QPoint *> ();

    //set everything to empty and not legitimate
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            tableState[i][j] = 0;
        }
    }

    //initialize the tableState
    tableState[3][4] = 1;
    tableState[4][3] = 1;
    tableState[3][3] = 2;
    tableState[4][4] = 2;

    //initialize the allowedMoves
    allowedMoves->append(&pointArray[3][2]);
    allowedMoves->append(&pointArray[2][3]);
    allowedMoves->append(&pointArray[5][4]);
    allowedMoves->append(&pointArray[4][5]);

    boardwidget->setIconAt(VALIDGREEN, 3, 2);
    boardwidget->setIconAt(VALIDGREEN, 2, 3);
    boardwidget->setIconAt(VALIDGREEN, 5, 4);
    boardwidget->setIconAt(VALIDGREEN, 4, 5);

    boardwidget->setButtonName("32v",3,2);
    boardwidget->setButtonName("23v",2,3);
    boardwidget->setButtonName("54v",5,4);
    boardwidget->setButtonName("45v",4,5);

}

/**
 * Returns true if the move at (i,j) is allowed
 */
bool Othello::allowed(const int i, const int j) {
    return allowed(allowedMoves,i,j);
}

/**
  * returns true if the move is legitimate else returns false
  */
bool Othello::allowed( QList<QPoint *> *allowed, const int i, const int j)
{
    QPoint *ptr;
    foreach (ptr, *allowed) {
        if (ptr->x() == i && ptr->y() == j)
            return true;
    }
    return false;
}
/**
 * Updates the table with the move at (i,j)
 */
Player* Othello::updateBoardTableStateAllowedMoves(const int i, const int j) {
    return updateBoardTableStateAllowedMoves(tableState,allowedMoves,i,j);
}

/**
 * Updates the specified table with the move at (i,j)
 */
static int alreadyNotAllowed = 0;
Player* Othello::updateBoardTableStateAllowedMoves(unsigned char **state,QList<QPoint *> *allowed,const int k, const int l)
{
    int i,j;
    short totalScore;

    if (p1->getScore() == 0) {
        QMessageBox::information(boardwidget, "Othello FPG", "The winner is " + p2->getName(), QMessageBox::Ok);
        return NULL;
    } else if (p2->getScore() == 0) {
        QMessageBox::information(boardwidget, "Othello FPG", "The winner is " + p1->getName(), QMessageBox::Ok);
        return NULL;
    }

    if (p2->getTurn() == isBlacksTurn) {
        if (allowedMoves->size() == 1) {
            i = allowedMoves->first()->x();
            j = allowedMoves->first()->y();
        } else {
            int *res = alpha_beta(this, max_depth);
            i = res[0];
            j = res[1];
            delete res;
        }
    } else {
        i = k;
        j = l;
    }

    updateBoardTableState(state,i,j, true);
    isBlacksTurn = !isBlacksTurn;
    totalScore = p1->getScore() + p2->getScore();
    if ( !updateAllowedMoves(allowed,state) && totalScore != 64)
    {
        if (alreadyNotAllowed > 1)
        {
            QString winner = p1->getScore() > p2->getScore() ? p1->getName() : p2->getName();
            QMessageBox::information(boardwidget, "Othello FPG", "The winner is " + winner, QMessageBox::Ok);
            return NULL;
        }
        alreadyNotAllowed++;
        if (p1->getTurn() == isBlacksTurn)
            QMessageBox::warning(boardwidget, "Othello FPG", "It is not allowed to make a move :(", QMessageBox::Ok);
        else
            QMessageBox::warning(boardwidget, "Othello FPG", "Computer can't make a move. Your turn :D", QMessageBox::Ok);
        isBlacksTurn = !isBlacksTurn;
        return updateBoardTableStateAllowedMoves(state,allowed,i,j);
    }
    else if (totalScore == 64)
    {
        if (p1->getScore() == 32) {
            QMessageBox::information(boardwidget, "Othello FPG", "Tie", QMessageBox::Ok);
            return NULL;
        }
        QString winner = p1->getScore() > p2->getScore() ? p1->getName() : p2->getName();
        QMessageBox::information(boardwidget, "Othello FPG", "The winner is " + winner, QMessageBox::Ok);
        return NULL;
    }

    alreadyNotAllowed = 0;
    if (p2->getTurn() == isBlacksTurn) {
        return p2;
    } else {
        return p1;
    }
}

/**
  * updates the board and the tableState array, with the move at (i,j)
  */
void Othello::updateBoardTableState(unsigned char** state,const int i, const int j, bool changeIcons)
{
    unsigned char** series = getSeries(state,i, j);
    updateTableState(state,i, j, series, changeIcons);

    for (int i = 0; i < seriesNumber; i++) {
        delete[] series[i];
    }
    delete[] series;
}

/**
  * return a pointer to an array[seriesNumber][2], with every row being a series of pawns to be changed between the row and (i,j)
  */
unsigned char** Othello::getSeries(unsigned char **state,const int i, const int j)
{
    seriesNumber = 0;
    int upperLeft = checkUpperLeft(state,i, j);
    if (upperLeft != -1)
        seriesNumber++;
    int lowerLeft = checkLowerLeft(state,i, j);
    if (lowerLeft != -1)
        seriesNumber++;
    int left = checkLeft(state,i, j);
    if (left != -1)
        seriesNumber++;
    int upperRight = checkUpperRight(state,i, j);
    if (upperRight != -1)
        seriesNumber++;
    int lowerRight = checkLowerRight(state,i, j);
    if (lowerRight != -1)
        seriesNumber++;
    int right = checkRight(state,i, j);
    if (right != -1)
        seriesNumber++;
    int top = checkTop(state,i, j);
    if (top != -1)
        seriesNumber++;
    int bottom = checkBottom(state,i, j);
    if (bottom != -1)
        seriesNumber++;

    unsigned char** series = new unsigned char*[seriesNumber];
    for (int k = 0; k < seriesNumber; k++)
        series[k] = new unsigned char[2];

    int k = 0;
    //set the coordinates based on the results of the aforesaid methods
    if (upperLeft != -1)
    {
        series[k][0] = upperLeft;
        series[k][1] = j - (i - upperLeft);
        k++;
    }

    if (lowerLeft != -1)
    {
        series[k][0] = lowerLeft;
        series[k][1] = j - (lowerLeft - i);
        k++;
    }

    if (left != -1)
    {
        series[k][0] = i;
        series[k][1] = left;
        k++;
    }

    if (upperRight != -1)
    {
        series[k][0] = upperRight;
        series[k][1] = j + (i - upperRight);
        k++;
    }

    if (lowerRight != -1)
    {
        series[k][0] = lowerRight;
        series[k][1] = j + (lowerRight - i);
        k++;
    }

    if (right != -1)
    {
        series[k][0] = i;
        series[k][1] = right;
        k++;
    }

    if (top != -1)
    {
        series[k][0] = top;
        series[k][1] = j;
        k++;
    }

    if (bottom != -1)
    {
        series[k][0] = bottom;
        series[k][1] = j;
    }

    //return the series
    return series;
}

/**
  * uses the entries in the series array and changes the tableState and the board icons for these series
  */
void Othello::updateTableState(unsigned char** state,const int i, const int j, unsigned char** series, bool changeIcons)
{


    short tBlack = 0;
    short tWhite = 0;
    for (int k = 0; k < seriesNumber; k++)
    {
        int c;
        if (series[k][0] < i && series[k][1] < j) //upperLeft
        {
            c = j;
            for (int r = i; r > series[k][0]; r--)
            {
                if (isBlacksTurn) {
                    if (state[r][c] != 1)
                        tBlack++;
                    if (state[r][c] == 2)
                        tWhite--;
                    state[r][c] = 1;
                }
                else {
                    if (state[r][c] != 2)
                        tWhite++;
                    if (state[r][c] == 1)
                        tBlack--;
                    state[r][c] = 2;
                }
                if (changeIcons)
                    changeIcon(r,c);
                c--;
            }
        }
        else if (series[k][0] > i && series[k][1] < j) //lowerLeft
        {
            c = j;
            for (int r = i; r < series[k][0]; r++)
            {
                if (isBlacksTurn) {
                    if (state[r][c] != 1)
                        tBlack++;
                    if (state[r][c] == 2)
                        tWhite--;
                    state[r][c] = 1;
                }
                else {
                    if (state[r][c] != 2)
                        tWhite++;
                    if (state[r][c] == 1)
                        tBlack--;
                    state[r][c] = 2;
                }
                if (changeIcons)
                    changeIcon(r,c);
                c--;
            }
        }
        else if (series[k][0] == i && series[k][1] < j) //left
        {
            for (c = j; c > series[k][1]; c--)
            {
                if (isBlacksTurn) {
                    if (state[i][c] != 1)
                        tBlack++;
                    if (state[i][c] == 2)
                        tWhite--;
                    state[i][c] = 1;
                }
                else {
                    if (state[i][c] != 2)
                        tWhite++;
                    if (state[i][c] == 1)
                        tBlack--;
                    state[i][c] = 2;
                }
                if (changeIcons)
                    changeIcon(i,c);
            }
        }
        else if (series[k][0] == i && series[k][1] > j) //right
        {
            for (int c = j; c < series[k][1]; c++)
            {
                if (isBlacksTurn) {
                    if (state[i][c] != 1)
                        tBlack++;
                    if (state[i][c] == 2)
                        tWhite--;
                    state[i][c] = 1;
                }
                else {
                    if (state[i][c] != 2)
                        tWhite++;
                    if (state[i][c] == 1)
                        tBlack--;
                    state[i][c] = 2;
                }
                if (changeIcons)
                    changeIcon(i,c);
            }
        }
        else if (series[k][0] < i && series[k][1] > j) //upperRight
        {
            c = j;
            for (int r = i; r > series[k][0]; r--)
            {
                if (isBlacksTurn) {
                    if (state[r][c] != 1)
                        tBlack++;
                    if (state[r][c] == 2)
                        tWhite--;
                    state[r][c] = 1;
                }
                else {
                    if (state[r][c] != 2)
                        tWhite++;
                    if (state[r][c] == 1)
                        tBlack--;
                    state[r][c] = 2;
                }
                if (changeIcons)
                    changeIcon(r,c);
                c++;
            }
        }
        else if (series[k][0] > i && series[k][1] > j) //lowerRight
        {
            c = j;
            for (int r = i; r < series[k][0]; r++)
            {
                if (isBlacksTurn) {
                    if (state[r][c] != 1)
                        tBlack++;
                    if (state[r][c] == 2)
                        tWhite--;
                    state[r][c] = 1;
                } else {
                    if (state[r][c] != 2)
                        tWhite++;
                    if (state[r][c] == 1)
                        tBlack--;
                    state[r][c] = 2;
                }
                if (changeIcons)
                    changeIcon(r,c);
                c++;
            }
        }
        else if (series[k][0] < i && series[k][1] ==j) //top
        {
            for (int r = i; r > series[k][0]; r--)
            {
                if (isBlacksTurn) {
                    if (state[r][j] != 1)
                        tBlack++;
                    if (state[r][j] == 2)
                        tWhite--;
                    state[r][j] = 1;
                }
                else {
                    if (state[r][j] != 2)
                        tWhite++;
                    if (state[r][j] == 1)
                        tBlack--;
                    state[r][j] = 2;
                }
                if (changeIcons)
                    changeIcon(r,j);
            }
        }
        else //bottom
        {
            for (int r = i; r < series[k][0]; r++)
            {
                if (isBlacksTurn) {
                    if (state[r][j] != 1)
                        tBlack++;
                    if (state[r][j] == 2)
                        tWhite--;
                    state[r][j] = 1;
                }
                else {
                    if (state[r][j] != 2)
                        tWhite++;
                    if (state[r][j] == 1)
                        tBlack--;
                    state[r][j] = 2;
                }
                if (changeIcons)
                    changeIcon(r,j);
            }
        }
    }

    if (changeIcons) {
        if (p2->getTurn()) {
            p2->incrScore(tBlack);
            p1->incrScore(tWhite);
        } else {
            p2->incrScore(tWhite);
            p1->incrScore(tBlack);
        }
    }
}
/**
  * changes the icon at the (i, j) button
  */
void Othello::changeIcon(const int i, const int j)
{
    if (isBlacksTurn)
    {
        boardwidget->setIconAt(BLACK,i,j);
    }
    else
    {
        boardwidget->setIconAt(WHITE,i,j);
    }

}

/**
  * Updates the allowed moves. Returns true if there are available moves
  * Returns false if there are no available moves
  * allowed Moves are stored in the argument list(allowed)
  */
bool Othello::updateAllowedMoves(QList<QPoint *> *allowed, unsigned char** state)
{
    bool allow = false;
    while (!allowed->empty())
        allowed->removeLast();

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (state[i][j] != 0) {

            }
            else if ((checkUpperLeft(state,i,j) != -1) || (checkUpperRight(state,i,j) != -1) || (checkLowerLeft(state,i,j) != -1) ||
                     (checkLowerRight(state,i,j) != -1) || (checkLeft(state,i,j) != -1) || (checkRight(state,i,j) != -1) || (checkTop(state,i,j) != -1) || (checkBottom(state,i,j) != -1)) {
                allowed->append(&pointArray[i][j]);
                boardwidget->setIconAt(VALIDGREEN, i, j);
                boardwidget->setButtonName(QString::number(i) + QString::number(j) + "v", i, j);
                allow = true;
            }
            else {
                boardwidget->setIconAt(GREEN, i, j);
                boardwidget->setButtonName(QString::number(i) + QString::number(j) + "g", i, j);
            }
        }
    }
    return allow;
}

/**
  * Returns the upper left position(the row it is in) that consists a series.
  * If no series, then returns -1.
  */
int Othello::checkUpperLeft(unsigned char **state,const int i, const int j)
{
    if (i != 0 && j !=0) //if i || j == 0 then there's no upper left spot left
    {
        if (isBlacksTurn)
        {
            if (state[i-1][j-1] != 2)//no series, since there's no different pawn
                return -1;
            else
            {
                int r = i-1, c = j-1;
                while (r > 0 && c > 0)
                {
                    r--;
                    c--;
                    if (state[r][c] == 1)
                        return r;
                    else if (state[r][c] == 0) //if empty, return -1
                        return -1;
                }
            }
        }
        else
        {
            if (state[i-1][j-1] != 1)//no series, since there's no different pawn
                return -1;
            else
            {
                int r = i-1, c = j-1;
                while (r > 0 && c > 0)
                {
                    r--;
                    c--;
                    if (state[r][c] == 2)
                        return r;
                    else if (state[r][c] == 0) //if empty, return -1
                        return -1;
                }
            }
        }
    }

    return -1;
}

/**
  * Returns the upper right position(the row it is in) that consists a series.
  * If no series, then returns -1.
  */
int Othello::checkUpperRight(unsigned char **state,const int i, const int j)
{
    if (i != 0 && j !=7) //if i || j == 0 then there's no upper right spot left
    {
        if (isBlacksTurn)
        {
            if (state[i-1][j+1] != 2)//no series, since there's no different pawn
                return -1;
            else
            {
                int r = i-1, c = j+1;
                while (r > 0 && c < 7)
                {
                    r--;
                    c++;
                    if (state[r][c] == 1)
                        return r;
                    else if (state[r][c] == 0) //if empty, return -1
                        return -1;
                }
            }
        }
        else
        {
            if (state[i-1][j+1] != 1)//no series, since there's no different pawn
                return -1;
            else
            {
                int r = i-1, c = j+1;
                while (r > 0 && c < 7)
                {
                    r--;
                    c++;
                    if (state[r][c] == 2)
                        return r;
                    else if (state[r][c] == 0) //if empty, return -1
                        return -1;
                }
            }
        }
    }
    return -1;
}

/**
  * Returns the lower left position(the row it is in) that consists a series.
  * If no series, then returns -1.
  */
int Othello::checkLowerLeft(unsigned char **state,const int i, const int j)
{
    if (i != 7 && j !=0) //if i || j == 0 then there's no lower left spot left
    {
        if (isBlacksTurn)
        {
            if (state[i+1][j-1] != 2)//no series, since there's no different pawn
                return -1;
            else
            {
                int r = i+1, c = j-1;
                while (r < 7 && c > 0)
                {
                    r++;
                    c--;
                    if (state[r][c] == 1)
                        return r;
                    else if (state[r][c] == 0) //if empty, return -1
                        return -1;
                }
            }
        }
        else
        {
            if (state[i+1][j-1] != 1)//no series, since there's no different pawn
                return -1;
            else
            {
                int r = i+1, c = j-1;
                while (r < 7 && c > 0)
                {
                    r++;
                    c--;
                    if (state[r][c] == 2)
                        return r;
                    else if (state[r][c] == 0) //if empty, return -1
                        return -1;
                }
            }
        }
    }

    return -1;
}

/**
  * Returns the lower right position(the row it is in) that consists a series.
  * If no series, then returns -1.
  */
int Othello::checkLowerRight(unsigned char **state,const int i, const int j)
{

    if (i != 7 && j !=7) //if i || j == 0 then there's no upper right spot left
    {
        if (isBlacksTurn)
        {
            if (state[i+1][j+1] != 2)//no series, since there's no different pawn
                return -1;
            else
            {
                int r = i+1, c = j+1;
                while (r < 7 && c < 7)
                {
                    r++;
                    c++;
                    if (state[r][c] == 1)
                        return r;
                    else if (state[r][c] == 0) //if empty, return -1
                        return -1;
                }
            }
        }
        else
        {
            if (state[i+1][j+1] != 1)//no series, since there's no different pawn
                return -1;
            else
            {
                int r = i+1, c = j+1;
                while (r < 7 && c < 7)
                {
                    r++;
                    c++;
                    if (state[r][c] == 2)
                        return r;
                    else if (state[r][c] == 0) //if empty, return -1
                        return -1;
                }
            }
        }
    }
    return -1;
}

/**
  * Returns the left position(the column it is in) that consists a series.
  * If no series, then returns -1.
  */
int Othello::checkLeft(unsigned char **state,const int i, const int j)
{
    if (j !=0) //if i || j == 0 then there's no lower left spot left
    {
        if (isBlacksTurn)
        {
            if (state[i][j-1] != 2)//no series, since there's no different pawn
                return -1;
            else
            {
                int c = j-1;
                while (c > 0)
                {
                    c--;
                    if (state[i][c] == 1)
                        return c;
                    else if (state[i][c] == 0) //if empty, return -1
                        return -1;
                }
            }
        }
        else
        {
            if (state[i][j-1] != 1)//no series, since there's no different pawn
                return -1;
            else
            {
                int c = j-1;
                while (c > 0)
                {
                    c--;
                    if (state[i][c] == 2)
                        return c;
                    else if (state[i][c] == 0) //if empty, return -1
                        return -1;
                }
            }
        }
    }
    return -1;
}

/**
  * Returns the right position(the column it is in) that consists a series.
  * If no series, then returns -1.
  */
int Othello::checkRight(unsigned char **state,const int i, const int j)
{

    if (j != 7) //if i || j == 0 then there's no lower left spot left
    {
        if (isBlacksTurn)
        {
            if (state[i][j+1] != 2)//no series, since there's no different pawn
                return -1;
            else
            {
                int c = j+1;
                while (c > 0)
                {
                    c++;
                    if (state[i][c] == 1)
                        return c;
                    else if (state[i][c] == 0) //if empty, return -1
                        return -1;
                }
            }
        }
        else
        {
            if (state[i][j+1] != 1)//no series, since there's no different pawn
                return -1;
            else
            {
                int c = j+1;
                while (c > 0)
                {
                    c++;
                    if (state[i][c] == 2)
                        return c;
                    else if (state[i][c] == 0) //if empty, return -1
                        return -1;
                }
            }
        }
    }

    return -1;
}

/**
  * Returns the top position(the row it is in) that consists a series.
  * If no series, then returns -1.
  */
int Othello::checkTop(unsigned char **state,const int i, const int j)
{
    if (i !=0) //if i || j == 0 then there's no lower left spot left
    {
        if (isBlacksTurn)
        {
            if (state[i-1][j] != 2)//no series, since there's no different pawn
                return -1;
            else
            {
                int r = i-1;
                while (r > 0)
                {
                    r--;
                    if (state[r][j] == 1)
                        return r;
                    else if (state[r][j] == 0) //if empty, return -1
                        return -1;
                }
            }
        }
        else
        {
            if (state[i-1][j] != 1)//no series, since there's no different pawn
                return -1;
            else
            {
                int r = i-1;
                while (r > 0)
                {
                    r--;
                    if (state[r][j] == 2)
                        return r;
                    else if (state[r][j] == 0) //if empty, return -1
                        return -1;
                }
            }
        }
    }
    return -1;
}

/**
  * Returns the bottom position(the row it is in) that consists a series.
  * If no series, then returns -1.
  */
int Othello::checkBottom(unsigned char **state,const int i, const int j)
{
    if (i != 7) //if i || j == 0 then there's no lower left spot left
    {
        if (isBlacksTurn)
        {
            if (state[i+1][j] != 2)//no series, since there's no different pawn
                return -1;
            else
            {
                int r = i+1;
                while (r < 7)
                {
                    r++;
                    if (state[r][j] == 1)
                        return r;
                    else if (state[r][j] == 0) //if empty, return -1
                        return -1;
                }
            }
        }
        else
        {
            if (state[i+1][j] != 1)//no series, since there's no different pawn
                return -1;
            else
            {
                int r = i+1;
                while (r < 7)
                {
                    r++;
                    if (state[r][j] == 2) {
                        return r;
                    }
                    else if (state[r][j] == 0) {
                        return -1;
                    }
                }
            }
        }
    }
    return -1;
}

/**
  * Returns the allowed moves list
  */
QList<QPoint *> * Othello::getAllowed() {
    return this->allowedMoves;

}

/**
  * Returns the state of the table
  */
unsigned char** Othello::getState() {
    return this->tableState;
}

bool Othello::getP2Turn() {
    return p2->getTurn();
}

void Othello::setBoolNextTurn(bool black) {
    isBlacksTurn = black;
}

bool Othello::getBoolNextTurn()
{
    return isBlacksTurn;
}
