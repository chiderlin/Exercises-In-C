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
#define MAXBRDS 200000
#define COLLTYPE "Fixed"
#define FIXEDSIZE 5000
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
    char* board_str;
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
    // int board_count;
    // char next_board[BRDSZ][BRDSZ];
    unsigned int board_height;
    unsigned int board_width;

};

typedef struct board board;
typedef struct state state;

#include "md.h"

char* getFileContent(const char* filename);
char* toStringFormat(char* txt);
void printBoard(board* b, int height, int width);
void printBoards(state* s);
char* board2str(board* b, int height, int width);
void generateMove(state* s);
void movePointer(state* s);
bool moveDown(state* s);
bool isUniqueBoard(state* s, board* b);
bool isSolution(board *b, int height, int width);
void printProcess(state* s, board* final_b);
void freeBoard(state* s);

state* stateInit(void);

// add element
void boardAdd(state* s, board* b);

// isexist
bool boardIsExist(state* s, board* b);

// size of coll
int stateSize(state *s);

// clear all space used
bool stateFree(state *s);