#include <stdlib.h>
#include <limits.h>

#include "minimax.h"
#include "othello.h"

static int straight_lines(unsigned char **tableState, bool p2);
static int diagonal_lines(unsigned char **tableState, bool p2);

static int weights[8][8] =  {
                    { 15000, -3000, 1000, 800 , 800 ,  1000, -3000, 15000 },
                    { -3000, -5000, -450, -500, -500, -450 , -5000, -3000 },
                    { 1000 , -450 , 30  , 10  , 10  , 30   , -450 , 1000  },
                    { 800  , -500 , 10  , 50  , 50  , 10   , -500 , 800   },
                    { 800  , -500 , 10  , 50  , 50  , 10   , -500 , 800   },
                    { 1000 , -450 , 30  , 10  , 10  , 30   , -450 , 1000  },
                    { -3000, -5000, -450, -500, -500, -450 , -5000, -3000 },
                    { 15000, -3000, 1000, 800 , 300 , 1000 , -3000, 15000 }
};
static int total_moves = 0;

/**
  * Returns a table pointer that holds the position of the next move of the AI player
  */
int* alpha_beta(Othello *othello, int max_depth) {
    int *res = new int[2];
    int v;

    total_moves++;
    if (max_depth == 0) {
        v = max_value(othello,othello->getState(), INT_MIN,INT_MAX,-1,max_depth,res);
    } else {
        v = max_value(othello, othello->getState(), INT_MIN, INT_MAX,0,max_depth,res);
    }

    total_moves++;
    return res;
}

/**
  * Returns the evaluation of the best move(the move with the greatest evaluation). The point of this move is returned in the argument array list (res).
  */
int max_value(Othello *othello, unsigned char **tableState, int a, int b,int depth, int max_depth, int *res) {

    int i,j,temp,v,k;
    QPoint *point;

    QList<QPoint *> *tempAllowed = new QList<QPoint *>();

    if (!othello->updateAllowedMoves(tempAllowed,tableState) || max_depth == depth) {
        temp = tempAllowed->size();
        delete tempAllowed;

        return evaluation(tableState, temp, othello->getP2Turn(), true);
    }

    unsigned char **tempTable = new unsigned char*[8];

    for(i = 0; i < 8; i++)
    {
        tempTable[i] = new unsigned char[8];
        memcpy(tempTable[i], tableState[i], sizeof(unsigned char)*8);

    }


    v = INT_MIN;
    foreach(point, *tempAllowed)
    {
        i = point->x();
        j = point->y();

        othello->updateBoardTableState(tempTable, i, j, false);
        othello->setBoolNextTurn(!othello->getBoolNextTurn());

        total_moves++;
        temp = min_value(othello,tempTable,a,b,depth+1,max_depth,res);
        total_moves--;
        othello->setBoolNextTurn(!othello->getBoolNextTurn());

        if (v < temp)
        {
            v = temp;
            if (depth == 0 || max_depth == 0){
                res[0] = i;
                res[1] = j;
            }
        }
        if (v >= b)
        {
            if (depth == 0|| max_depth == 0) {
                res[0] = i;
                res[1] = j;
            }
            for ( k = 0; k < 8; k++) {
                delete[] tempTable[k];
            }

            delete[] tempTable;
            delete tempAllowed;
            return v;
        }
        a = a > v ? a : v;

        for(i = 0; i < 8; i++)
        {
          memcpy(tempTable[i], tableState[i], sizeof(unsigned char)*8);
        }
    }


    for (k = 0; k < 8; k++)
    {
        delete[] tempTable[k];
    }
    delete[] tempTable;
    delete tempAllowed;
    return v;

}

/**
  * Returns the evaluation of the best move(the move with the lowest evaluation). The point of this move is returned in the argument array list (res).
  */
int min_value(Othello *othello, unsigned char **tableState, int a, int b, int depth, int max_depth, int *res)
{

    int i,j,temp,v,k;
    QPoint *point;

    QList<QPoint *> *tempAllowed = new QList<QPoint *>();

    if (!othello->updateAllowedMoves(tempAllowed,tableState) || max_depth == depth)
    {
        temp = tempAllowed->size();
        delete tempAllowed;
        return evaluation(tableState, temp, (othello->getP2Turn()), false);
    }

    unsigned char **tempTable = new unsigned char*[8];
    for(i = 0; i < 8; i++)
    {
        tempTable[i] = new unsigned char[8];
        memcpy(tempTable[i], tableState[i], sizeof(unsigned char)*8);
    }


    v = INT_MAX;

    foreach(point, *tempAllowed)
    {
        i = point->x();
        j = point->y();
        othello->updateBoardTableState(tempTable, i, j, false);
        othello->setBoolNextTurn(!othello->getBoolNextTurn());
        total_moves++;
        temp = max_value(othello,tempTable,a,b,depth+1,max_depth,res);
        total_moves--;
        othello->setBoolNextTurn(!othello->getBoolNextTurn());

        if (v > temp)
        {
            v = temp;
        }

        if (v <= a)
        {
            for ( k = 0; k < 8; k++) {
                delete[] tempTable[k];
            }

            delete[] tempTable;
            delete tempAllowed;

            return v;
        }
        b = b < v ? b : v;

        for(i = 0; i < 8; i++)
        {
             memcpy(tempTable[i], tableState[i], sizeof(unsigned char)*8);
        }
    }


    for (k = 0; k < 8; k++) {
        delete[] tempTable[k];

    }
    delete[] tempTable;
    delete tempAllowed;

    return v;
}

/**
  * Evaluation method.
  * The most important part is the weight of the move. The weight is computed with the static weights array
  * The next most important part is the the weights of different forms of lines.
  * Next is how many moves are available moves of the state(the more available moves, the better for us)
  * Finally, we count the whites and the blacks and compute they're difference
  */
int evaluation(unsigned char **tableState, int availableMoves, bool p2Black, bool isMax)
{
    int blackWeight = 0, whiteWeight = 0, whiteCounter = 0, blackCounter = 0, difference, weightDifference, availableMovesWeight;
    int line_accountability = 0;

    if (!isMax)
        availableMovesWeight = -availableMoves;
    else
        availableMovesWeight = availableMoves;

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (tableState[i][j] != 0)
            {
                if (tableState[i][j] == 1)//black
                {
                    blackCounter++;
                    blackWeight += weights[i][j];
                }
                else
                {
                    whiteCounter++;
                    whiteWeight += weights[i][j];
                }
            }
        }
    }

    difference = ((whiteCounter - blackCounter) * total_moves) >> 1;
    weightDifference = whiteWeight - blackWeight;
    line_accountability = straight_lines(tableState,p2Black);
    line_accountability += diagonal_lines(tableState,p2Black);


    if (p2Black) {
        difference = -difference;
        weightDifference = -weightDifference;
    }

    int score = difference + weightDifference*30 + availableMovesWeight*10 + line_accountability * 20;

    return score;
}

/**
  * Computes the weight of diagonal lines. Complete lines are more desirable than incomplete ones.
  */
static int diagonal_lines(unsigned char **tableState, bool p2) {
    unsigned char **p = tableState;
    int middleB = 0;
    int middleW = 0;
    int near_middleB = 0;
    int near_middleW = 0;
    int betw_middle_cornerB = 0;
    int betw_middle_cornerW = 0;
    int near_cornerB = 0;
    int near_cornerW = 0;
    int cornerB = 0;
    int cornerW = 0;
    int d = 0;
    int sb , sw;

    // middle left
    d = p[0][0] + p[1][1] + p[2][2] + p[3][3] + p[4][4] + p[5][5] + p[6][6] + p[7][7];
    if (d == 8)
        middleB += 20;
    else if (d == 16)
        middleW += 20;
    else if (p[0][0] == 0 || p[7][7] == 0)
    {
        if (d == 7)
         middleB += 12;
        else if (d == 14)
            middleW += 12;
    }

    //middle right
    d = p[0][7] + p[1][6] + p[2][5] + p[3][4] + p[4][3] + p[5][2] + p[6][1] + p[7][0];
    if (d == 8)
        middleB += 20;
    else if (d == 16)
        middleW += 20;
    else if (p[0][7] == 0 || p[7][0] == 0)
    {
        if (d == 7)
         middleB += 12;
        else if (d == 14)
            middleW += 12;
    }

    // near middle top left
    d = p[0][1] + p[1][2] + p[2][3] + p[3][4] + p[4][5] + p[5][6] + p[6][7];
    if (d == 7)
        near_middleB += 16;
    else if (d == 14)
        near_middleW += 16;
    else if (p[0][1] == 0 || p[6][7] == 0)
    {
        if (d == 6)
        near_middleB += 8;
        else if (d == 12)
            near_middleW += 8;
    }

    // near middle bottom left
    d = p[1][0] + p[2][1] + p[3][2] + p[4][3] + p[5][4] + p[6][5] + p[7][6];
    if (d == 7)
        near_middleB += 16;
    else if (d == 14)
        near_middleW += 16;
    else if (p[1][0] == 0 || p[7][6] == 0)
    {
        if (d == 6)
         near_middleB += 8;
        else if (d == 12)
            near_middleW += 8;
    }

    // near middle top right
    d = p[0][6] + p[1][5] + p[2][4] + p[3][3] + p[4][3] + p[5][2] + p[6][1];
    if (d == 7)
        near_middleB += 16;
    else if (d == 14)
        near_middleW += 16;
    else if (p[0][6] == 0 || p[6][1] == 0)
    {
        if (d == 6)
         near_middleB += 8;
        else if (d == 12)
            near_middleW += 8;
    }

    // near middle bottom right
    d = p[1][7] + p[2][6] + p[3][5] + p[4][4] + p[5][3] + p[6][2] + p[7][1];
    if (d == 7)
        near_middleB += 16;
    else if (d == 14)
        near_middleW += 16;
    else if (p[1][7] == 0 || p[7][1] == 0)
    {
        if (d == 6)
         near_middleB += 8;
        else if (d == 12)
            near_middleW += 8;
    }

    // between middle and corner left top
    d = p[0][2] + p[1][3] + p[2][4] + p[3][5] + p[4][6] + p[5][7];
    if (d == 6)
        betw_middle_cornerB += 12;
    else if (d == 12)
        betw_middle_cornerW += 12;
    else if (p[0][2] == 0 || p[5][7] == 0)
    {
        if (d == 5)
            betw_middle_cornerB += 4;
        else if (d == 10)
            betw_middle_cornerW += 4;
    }

    // between middle and corner left bottom
    d = p[2][0] + p[3][1] + p[4][2] + p[5][3] + p[6][4] + p[7][5];
    if (d == 6)
        betw_middle_cornerB += 12;
    else if (d == 12)
        betw_middle_cornerW += 12;
    else if (p[2][0] == 0 || p[7][5] == 0)
    {
        if (d == 5)
            betw_middle_cornerB += 4;
        else if (d == 10)
            betw_middle_cornerW += 4;
    }


    // between middle and corner right top
    d = p[5][0] + p[4][1] + p[3][2] + p[2][3] + p[1][4] + p[0][5];
    if (d == 6)
        betw_middle_cornerB += 12;
    else if (d == 12)
        betw_middle_cornerW += 12;
    else if (p[5][0] == 0 || p[0][5] == 0)
    {
        if (d == 5)
            betw_middle_cornerB += 4;
        else if (d == 10)
            betw_middle_cornerW += 4;
    }

    // between middle and corner right bottom
    d = p[2][7] + p[3][6] + p[4][5] + p[5][4] + p[6][3] + p[7][2];
    if (d == 6)
        betw_middle_cornerB += 12;
    else if (d == 12)
        betw_middle_cornerW += 12;
    else if (p[2][7] == 0 || p[7][2] == 0)
    {
        if (d == 5)
            betw_middle_cornerB += 4;
        else if (d == 10)
            betw_middle_cornerW += 4;
    }

    // near corner left top (a)
    d = p[0][3] + p[1][4] + p[2][5] + p[3][6] + p[4][7];
    if (d == 5)
        near_cornerB += 8;
    else if (d == 10)
       near_cornerW += 8;
    else if (p[0][3] == 0 || p[4][7] == 0)
    {
        if (d == 4)
            near_cornerB += 3;
        else if (d == 8)
            near_cornerW += 3;
    }

    // near corner left top (b)
    d = p[0][4] + p[1][5] + p[2][6] + p[3][7];
    if (d == 4)
        near_cornerB += 8;
    else if (d == 8)
       near_cornerW += 8;
    else if (p[0][4] == 0 || p[3][7] == 0)
    {
        if (d == 3)
            near_cornerB += 3;
        else if (d == 6)
            near_cornerW += 3;
    }

    // near corner left bottom (a)
    d = p[3][0] + p[4][1] + p[5][2] + p[6][3] + p[7][4];
    if (d == 5)
        near_cornerB += 8;
    else if (d == 10)
       near_cornerW += 8;
    else if (p[3][0] == 0 || p[7][4] == 0)
    {
        if (d == 4)
            near_cornerB += 3;
        else if (d == 8)
            near_cornerW += 3;
    }

    // near corner left bottom (b)
    d = p[4][0] + p[5][1] + p[6][2] + p[7][3];
    if (d == 4)
        near_cornerB += 8;
    else if (d == 8)
       near_cornerW += 8;
    else if (p[4][0] == 0 || p[7][3] == 0)
    {
        if (d == 3)
            near_cornerB += 3;
        else if (d == 6)
            near_cornerW += 3;
    }

    // near corner right top (a)
    d = p[0][4] + p[1][3] + p[2][2] + p[3][1] + p[4][0];
    if (d == 5)
        near_cornerB += 8;
    else if (d == 10)
       near_cornerW += 8;
    else if (p[0][4] == 0 || p[4][0] == 0)
    {
        if (d == 4)
            near_cornerB += 3;
        else if (d == 8)
            near_cornerW += 3;
    }

    // near corner right top (b)
    d = p[0][3] + p[1][2] + p[2][3] + p[3][4];
    if (d == 4)
        near_cornerB += 8;
    else if (d == 8)
       near_cornerW += 8;
    else if (p[0][3] == 0 || p[3][4] == 0)
    {
        if (d == 3)
            near_cornerB += 3;
        else if (d == 6)
            near_cornerW += 3;
    }

    // near corner right bottom (a)
    d = p[3][7] + p[4][6] + p[5][5] + p[6][4] + p[7][3];
    if (d == 5)
        near_cornerB += 8;
    else if (d == 10)
       near_cornerW += 8;
    else if (p[3][7] == 0 || p[7][3] == 0)
    {
        if (d == 4)
            near_cornerB += 3;
        else if (d == 8)
            near_cornerW += 3;
    }

    // near corner left bottom (b)
    d = p[4][7] + p[5][6] + p[6][5] + p[7][4];
    if (d == 4)
        near_cornerB += 8;
    else if (d == 8)
       near_cornerW += 8;
    else if (p[4][7] == 0 || p[7][4] == 0)
    {
        if (d == 3)
            near_cornerB += 3;
        else if (d == 6)
            near_cornerW += 3;
    }

    // corner left top (a)
    d = p[0][5] + p[1][6] + p[2][7];
    if (d == 3)
        cornerB += 4;
    else if (d == 6)
        cornerW += 4;
    if (p[0][5] == 0 || p[2][7] == 0) {
        if (d == 2)
            cornerB += 2;
        else if (d == 4)
            cornerW += 2;
    }
    // corner left top (b)
    d = p[0][6] + p[1][7];
    if (d == 2)
        cornerB += 3;
    else if (d == 4)
        cornerW += 3;
    if (p[0][6] == 0 || p[1][7] == 0) {
        if (d == 1)
            cornerB += 1;
        else if (d == 2)
            cornerW += 1;
    }

    //corner left top (c)
    if (p[0][7] == 1)
        cornerB += 1;
    else if (p[0][7] == 2)
        cornerW += 1;


    // corner left bottom (a)
    d = p[5][0] + p[6][1] + p[7][2];
    if (d == 3)
        cornerB += 4;
    else if (d == 6)
        cornerW += 4;
    if (p[5][0] == 0 || p[7][2] == 0) {
        if (d == 2)
            cornerB += 2;
        else if (d == 4)
            cornerW += 2;
    }
    // corner left bottom (b)
    d = p[6][0] + p[7][1];
    if (d == 2)
        cornerB += 3;
    else if (d == 4)
        cornerW += 3;
    if (p[6][0] == 0 || p[7][1] == 0) {
        if (d == 1)
            cornerB += 1;
        else if (d == 2)
            cornerW += 1;
    }

    //corner left bottom (c)
    if (p[7][0] == 1)
        cornerB += 1;
    else if (p[7][0] == 2)
        cornerW += 1;



    // corner right top (a)
    d = p[0][2] + p[1][1] + p[2][0];
    if (d == 3)
        cornerB += 4;
    else if (d == 6)
        cornerW += 4;
    if (p[0][2] == 0 || p[2][0] == 0) {
        if (d == 2)
            cornerB += 2;
        else if (d == 4)
            cornerW += 2;
    }
    // corner right top (b)
    d = p[0][1] + p[1][0];
    if (d == 2)
        cornerB += 3;
    else if (d == 4)
        cornerW += 3;
    if (p[0][1] == 0 || p[1][0] == 0) {
        if (d == 1)
            cornerB += 1;
        else if (d == 2)
            cornerW += 1;
    }

    //corner right top (c)
    if (p[0][0] == 1)
        cornerB += 1;
    else if (p[0][0] == 2)
        cornerW += 1;

    // corner right bottom (a)
    d = p[5][7] + p[6][6] + p[7][5];
    if (d == 3)
        cornerB += 4;
    else if (d == 6)
        cornerW += 4;
    if (p[5][7] == 0 || p[7][5] == 0) {
        if (d == 2)
            cornerB += 2;
        else if (d == 4)
            cornerW += 2;
    }
    // corner right bottom (b)
    d = p[7][6] + p[6][7];
    if (d == 2)
        cornerB += 3;
    else if (d == 4)
        cornerW += 3;
    if (p[6][7] == 0 || p[7][6] == 0) {
        if (d == 1)
            cornerB += 1;
        else if (d == 2)
            cornerW += 1;
    }

    //corner right bottom (c)
    if (p[7][7] == 1)
        cornerB += 1;
    else if (p[7][7] == 2)
        cornerW += 1;


    sb = cornerB + near_cornerB + near_middleB + betw_middle_cornerB + middleB;
    sw = cornerW + near_cornerW + near_middleW + betw_middle_cornerW + middleW;

    return ((p2) ? (sb - sw) : (sw - sb));
}

/**
  * Computes the weight of straight lines. Complete lines are more desirable than incomplete ones.
  * Also lines adjacent to the side of the board are more useful than lines across the centre.
  */
static int straight_lines(unsigned char **tableState, bool p2) {
    int verticalB = 0;
    int verticalW = 0;
    int horizontalB = 0;
    int horizontalW = 0;
    int e1,e2, i,j, factor,pos;


    for (i = 0; i < 8; i++) {
        pos = i << 3;
        memcpy(&e1,tableState+pos,4);
        memcpy(&e2,tableState+pos+4,4);

        if (i == 0 || i == 7)
            factor = 20;
        else if(i == 1 || i == 6)
            factor = 15;
        else
            factor = 10;

        if (e1 == e2 && e1 != 0) {

            if (e1 == 0x01010101) {
                verticalB += factor;
            }
            else if (e1 == 0x02020202){
                verticalW += factor;
            } else if (e1 == 0x00010101 || e1 == 0x01010100)
                verticalB += factor - 8;
            else if ( e1 == 0x00020202 || e1 == 0x02020200)
                verticalW += factor - 8;
        }

        e1 = 0;

        for (j = 0; j < 8; j++) {
            if (tableState[j][i] == 0)
                e2 = j;
            e1 += tableState[j][i];
        }

        if (e1 == 8) {
            horizontalB += factor;
        } else if (e1 == 16) {
            horizontalW += factor;
        } else if (e2 == 0 || e2 == 7) {
            if (e1 == 7)
                horizontalB += factor - 8;
            else if (e1 == 14)
                horizontalW += factor - 8;
        }

    }

    return ( (p2) ? ((verticalB + horizontalB) - (verticalW + horizontalW)) : ((verticalW + horizontalW) - (verticalB + horizontalB)));
}
