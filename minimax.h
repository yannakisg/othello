#ifndef MINIMAX_H
#define MINIMAX_H

class Othello;


int evaluation(unsigned char **tableState, int availableMoves, bool p2Black, bool isMax);

int* alpha_beta(Othello *othello, int max_depth);

int max_value(Othello *othello, unsigned char **tableState, int a, int b,int depth,int max_depth, int *res);

int min_value(Othello *othello, unsigned char **tableState, int a, int b,int depth,int max_depth, int *res);

#endif // MINIMAX_H
