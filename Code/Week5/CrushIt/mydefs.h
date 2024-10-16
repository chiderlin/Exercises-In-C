#pragma once

/* Put anything you like here, but
   certainly your 'state' structure 
   to hold your board e.g.
*/
#include <regex.h>
#define MAXROWS 20
#define WIDTH 5
#define HEIGHT 6
#define MAXCELL 30
#define BIGSTR 500
#define FNAME "b1.txt"
#define MATCH_COMBO 3

struct st {
   char board[MAXROWS][WIDTH];
   unsigned int actual_height;
   bool visited[MAXROWS][WIDTH];
};
typedef struct st state;

int getCurrentColumn(int idx_point);

char* getFileContent(const char* file_name);

int regexCheck(const char* txt);

void matchVertical(state* s);

void matchHirizontal(state* s);

void printBoard(state* s);

void printVisited(state* s);

void removeCombo(state* s);

bool checkRowDot(state* s, int row);

bool isEmptyArray(state* s);

void initState(state *s);
