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


/**
 * Check file if file extention is .txt,
 * if yes, return true.
 * else return false.
 */
bool isTxtFile(const char* filename);

/**
 * Check CrushIt string only include "A-Z" and ".".
 * if match, return true.
 * else return false.
 */
int regexCheck(const char* txt);

/**
 * Get current column from CrushIt string, 
 * and transform CrushIt string into 2D array.
 */
int getCurrentColumn(int index);

/**
 * initialise 'state' bool visited 2D array default value is all 0.
 */
void initState(state *s);

/**
 * Before matches blocks, check if 2D array already empty,
 * if yes, return true.
 * else return false.
 */
bool isEmptyArray(state* s);

/**
 * Matching vertically for last 6 rows, 
 * begin from index 0 down to last row.
 * Last 2 rows in each column, just skip.
 * if continuely match count to 3 or more, turn s->visited[row][col] value to true.
 */
void matchVertical(state* s);

/**
 * Matching hirizontally for last 6 rows, 
 * begin from index 0 move right to last column.
 * Last 2 columns in each row, just skip.
 * if continuely match count to 3 or more, turn s->visited[row][col] value to true.
 */
void matchHirizontal(state* s);


/**
 * Remove all the combo in board 2D array, 
 * from alfabet to '.'
 */
void removeCombo(state* s);

/**
 * Print 2D array board.
 */
void printBoard(state* s);

/**
 * Print 2D array visited.
 */
void printVisited(state* s);

/**
 * if that row is all '.' return true,
 * else return false.
 *  */ 
bool checkRowDot(state* s, int row);
