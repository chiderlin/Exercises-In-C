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
#define MATCH_COMBO 3

struct st {
   char board[MAXROWS][WIDTH];
   unsigned int actual_height;
   bool visited[MAXROWS][WIDTH];
};
typedef struct st state;

/**
 * Default read filename, try to open txt file.
 * if we can open it, return txt content.
 * Otherwise, return NULL.
 * Only acccept txt format.
 */
char* getFileContent(const char* filename);

bool isTxtFile(const char* filename);

int regexCheck(const char* txt);

int getCurrentColumn(int idx_point);

void initState(state *s);

bool isEmptyArray(state* s);

void matchVertical(state* s);

void matchHirizontal(state* s);

void removeCombo(state* s);

void printBoard(state* s);

void printVisited(state* s);

bool checkRowDot(state* s, int row);
