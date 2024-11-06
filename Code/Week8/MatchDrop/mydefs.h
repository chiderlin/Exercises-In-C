// Change this file in any way you like
// Do not alter driver.c / Makefile / md.h
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>
#include <stdbool.h>

#define BRDSZ 6
#define MAXBRDS 500000
#define MAXSTR (BRDSZ*BRDSZ+BRDSZ+2)

/*
Here define struct state -
t might look something like:

struct board{
   2D array of tiles
   hawk
   parent
}

struct state{
    array of struct board[{},{},{}];
    etc.
}
*/

struct board
{
    int moves;
    char self[BRDSZ][BRDSZ];
    char hawk;
    int parent_idx;
};

struct state
{
    int pointer;
    struct board boards[MAXBRDS];
    int size;
    unsigned int board_height;
    unsigned int board_width;
    bool find_solution;
};

typedef struct board board;
typedef struct state state;

#include "md.h"

/*
    Read filename,
    if we can open it, return txt content.
    Otherwise, return NULL.
*/
char* getFileContent(const char* filename);


/*
    Format conversion from files.
    Replacing newline to -, skip last newline.
*/
char* toStringFormat(char* txt);


/*
    print struct board's board, if debub_mode is true,
    will print more information in struct board,
    otherwise, only print board 2D array.
*/
void printBoard(board* b, int height, int width, bool debug_mode);


/*
  print struct state infomation, also board 2D array.
*/
void printBoards(state* s);


/*
Format conversion from board to string,
to compare different board.
*/
char* board2str(board* b, int height, int width);


/* 
    Check each column, if the column is match. 
    Lock it.
*/
bool lockColumn(board* b, int height, int column);


/*
    1. Generate new child board
    2. Check if it's unique in board[], 
    3. if it is unique, push new child board into board[].
    4. check if it is solution, if it is,
    update struct state find_solution to TRUE.
 */
void generateMove(state* s);


/*
    Moving struct state board[] pointer.
*/
void movePointer(state* s);


/*
    check if 2D array isExist in state board[].
    return true if deplicate, else return false.
*/
bool isUniqueBoard(state* s, board* b);


/*
    Check if it is final soluction.
*/
bool isSolution(board *b, int height, int width);


/*
    If solve function verbose is true, invoke 
    this function, printing solution process.
*/
void printProcess(state* s, board* final_b);


/*
    Initilize new struct state.
*/
state* stateInit(void);


/*
    Add new child board into board[].
 */
void boardAdd(state* s, board* b);
