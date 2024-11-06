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

char* getFileContent(const char* filename);
char* toStringFormat(char* txt);
void printBoard(board* b, int height, int width);
void printBoards(state* s);
char* board2str(board* b, int height, int width);
bool lockColumn(board* b, int height, int column);
void generateMove(state* s);
void movePointer(state* s);
bool isUniqueBoard(state* s, board* b);
bool isSolution(board *b, int height, int width);
void printProcess(state* s, board* final_b);

state* stateInit(void);

// add element
void boardAdd(state* s, board* b);
