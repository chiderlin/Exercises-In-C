#include "mydefs.h"

bool file2str(const char* fname, char* str)
{
   if(fname == NULL && str == NULL){
      return false;
   }
   //readfile
   char* txt = getFileContent(fname);
   // printf("%s\n",txt);

   //tostring
   char *format_str = toStringFormat(txt);
   strcpy(str, format_str);
   printf("format: %s\n", str);
   free(txt);
   return true;
}


state* str2state(const char* str)
{
   if(str == NULL){
      printf("str is null.\n");
      return NULL;
   }
   state* s = stateInit();
   if(s == NULL){
      fprintf(stderr, "Failed to initialize state.\n");
      return NULL;
   }
   board b;
   int str_len = strlen(str);
   printf("str_len: %d \n", str_len);
   s->pointer = 0;
   b.moves = 0;
   b.hawk = str[0];
   b.parent_idx = -1;
   int r = 0;
   int c = 0;
   for(int i=2; i<str_len; i++){
      b.self[r][c] = str[i];
      c+=1;
      if(str[i] == '-'){
         r+=1;
         s->board_width = c-1;
         c=0;
      }
      s->board_height = r+1;
   }


   // printf("b self:\n");
   // printBoard(&b, s->board_height, s->board_width);

   boardAdd(s, &b);
   // printf("parent b: \n");
   // printBoards(s);

   return s;
}

int solve(state* s, bool verbose)
{
   int height = s->board_height;
   int width = s->board_width;

   while(s->pointer < s->size) {
      int currentPointerIndex = s->pointer;
      int currentSize = s->size;
      printf("currentPointerIndex: %d \n", currentPointerIndex);
      printf("currentSize: %d \n", currentSize);
      printf("---------------\n");

      board* b = &s->boards[s->pointer];
      // printBoard(b, height, width);
      if(isSolution(b, height, width)){
         // if yes. print b->moves
         
         int moves = s->boards[s->pointer].moves;
         board* b = &s->boards[s->pointer];
         printf("solution total moves:%d \n", moves);
         // if verbose is true, track back to parents and print log
         if(verbose){
            printProcess(s, b);
         }
         return moves;
      }
      generateMove(s); // if generateMove, size++;
      movePointer(s); // pointer++;

   }
   // printBoards(s);
   return -1;
}


void printProcess(state* s, board* final_b)
{
   int height = s->board_height;
   int width = s->board_width;
   
   // get pointer, solution index
   int solutionIdx = s->pointer;
   printf("solutionIdx: %d\n", solutionIdx);
   int parent_idx = final_b->parent_idx;
   int moves = final_b->moves;
   // int solutionArr[moves+1];
   int* solutionArr = malloc((moves+1)*sizeof(int));
   if(solutionArr == NULL){
      fprintf(stderr, "Memory allocation failed.\n");
      exit(EXIT_FAILURE);
   }
   solutionArr[0] = solutionIdx;
   int index = 0;
   // if parent idx not null, tracing back to previous parent 
   while(parent_idx != -1){
      index ++;
      // get parent idx, store in arr, record parent index
      solutionArr[index] = parent_idx;
      // move to previous parent idx
      parent_idx = s->boards[parent_idx].parent_idx;
   }

   // print parent index array
   // for(int i=0; i<moves; i++){
   //    printf("%d ",solutionArr[i]);
   // }
   
   board* original_bd = &s->boards[0];
   printf("RESULT--------\n");
   printBoard(original_bd, height, width);
   printf("--------\n");
   for(int i=moves-1; i>=0; i--){
      int Boardindex = solutionArr[i];
      printf("index: %d \n", Boardindex);
      board* b = &s->boards[Boardindex];
      printBoard(b, height, width);
      printf("--------\n");
   }
   free(solutionArr);
}


//TODO write testcase
void test(void)
{

}

/* Lots of other functions, as required */

char* toStringFormat(char *txt)
{
   int len = strlen(txt);
   for(int i=0; i<len; i++){
      if(txt[i] == '\n' && i+1 != len){
         txt[i] = '-';
      }
   }
   if(len>0 && txt[len-1] == '\n'){
      txt[len-1] = '\0';
   }
   return txt;
}


char* getFileContent(const char* filename)
{
  FILE* fp = fopen(filename, "rt");
  if(fp == NULL){
    fprintf(stderr, "Warning: Cannot open file.\n");
    return NULL;
  }

  char *fullContent = malloc(1);
  if(fullContent == NULL){
    perror("Warning: Unable to allocate memory.\n");
    fclose(fp);
    return NULL;
  }
  fullContent[0] = '\0';
  char text[MAXBRDS];
  // read one line at a time
  while(fgets(text, sizeof(text), fp) != NULL){
    // remove newline character if present
   //  text[strcspn(text, "\n")] = '\0';
    size_t old_len = strlen(fullContent);
    size_t new_len = old_len + strlen(text) + 1;
    char *temp = realloc(fullContent, new_len);
    if(temp == NULL){
      perror("Unable to realloc memory.\n");
      free(fullContent);
      fclose(fp);
      return NULL;
    }
    fullContent = temp;
    strcat(fullContent, text);
  }
  // printf("Log: Full file content: %s\n",fullContent);

  fclose(fp);
  return fullContent;
}

/**
 H
 X H H I
 O I H I
 O O I X
 O X I X

 */

void printBoard(board* b, int height, int width)
{
   printf("printBoard-----\n");
   char hawk = b->hawk;
   printf("hawk: %c \n", hawk);

   int moves = b-> moves;
   printf("moves: %d \n", moves);

   int parent = b->parent_idx;

   printf("parent: %d \n", parent);
   printf("height: %d \n", height);
   printf("width: %d \n",width);
   for(int r=0; r<height; r++){
      for(int c=0; c<width; c++){
         printf("%c ", b->self[r][c]);
      }
      printf("\n");
   }
}

void printBoards(state* s){
   printf("size %d \n", s->size);
   printf("pointer: %d\n", s->pointer);
   for(int i=0; i<s->size; i++){
      printBoard(&s->boards[i], s->board_height, s->board_width); // send address, add &
   }
}

char* board2str(board* b, int height, int width)
{

   if(b == NULL){
      return NULL;
   }
   // declare str here, but return str, it will automatically free
   // instead of using fixed array declare, using malloc/calloc to create a new space for str!
   // char str[MAXSTR]; 

   // int maxLen = (height*width)+(height-1)+1;
   char* tmp_str = (char*)malloc(MAXSTR*sizeof(char));
   if(tmp_str == NULL){
      perror("board2str: Falled to allocate memory for board string.\n");
      exit(EXIT_FAILURE);
   }

   int index = 0;
   for(int r=0; r<height; r++){
      for(int c=0; c<width; c++){
         tmp_str[index++] = b->self[r][c];
      }
      if(r+1 != height){
         tmp_str[index++] = '-';
      }
   }
   tmp_str[index] = '\0';
   // printf("board2str Log: str: %s\n", tmp_str);

   return tmp_str;
}


bool lockColumn(board* b, int height, int column)
{
   if(b == NULL){
      fprintf(stderr, "Invalid board.\n");
      exit(EXIT_FAILURE);
   }

   // check each column first, if the column is all match. lock it.
   bool is_same = true;

   for(int r=0; r<height-1; r++){
      if(b->self[r][column] != b->self[r+1][column]){
         is_same = false;
      }
   }
   return is_same;
}


/**
A
ABC
ABC
ABC
CBA

C
ABC
ABC
ABC
ABA

B
AAC
ABC
ABC
CBA

A
ABA
ABC
ABC
CBC

 */
void generateMove(state* s)
{
   if(s == NULL){
      fprintf(stderr, "Invalid state or board.\n");
      exit(EXIT_FAILURE);
   }
   
   int height = s->board_height;
   int width = s->board_width;

   // printBoard(currentBoard, height, width);
   // get hawk
   // put hawk in each column, get new hawk
   // move all board
   // printBoards(s);
   // currentBoard.moves +=1;

   // only gen the column are not the same!
   for(int c=0; c<width; c++){
      board currentBoard = s->boards[s->pointer];
      bool is_lock = lockColumn(&currentBoard, height, c);
      // printf("is_lock:%s ", is_lock? "true":"false");
      if(!is_lock){
         // gen move
         char hawk = currentBoard.hawk;
         currentBoard.moves +=1;
         char new_hawk = currentBoard.self[height-1][c];
         for(int r=height-1; r>=0; r--){
            if(r == 0){
               currentBoard.self[r][c] = hawk;
            } else {
               currentBoard.self[r][c] = currentBoard.self[r-1][c];
            }
            currentBoard.hawk = new_hawk;
         }
         currentBoard.parent_idx = s->pointer;

         // check if 2D array isExist in state board[]
         bool is_unique_board = isUniqueBoard(s, &currentBoard);
         // printf("Is unique board: %s\n", is_unique_board ? "true" : "false");
         // gen new struct board (2D array, hawk, parent_idx, moves)
         // if not isExist in current board[], boardAdd
         if(is_unique_board){
            boardAdd(s, &currentBoard);
         }
         // printBoard(&currentBoard, height, width);
         //TODO: check solution again, once find, stop gen new move in board array
      }

   }
   // printBoards(s);
}


bool isUniqueBoard(state* s, board* b)
{
   // printf("in isUniqueBoard///\n");
   int size = s->size;
   int height = s->board_height;
   int width = s->board_width;
   printf("size:%d \n",size);
   char* current_board_str = board2str(b, height, width);
   if (current_board_str == NULL) {
      fprintf(stderr, "Error: Failed to generate string for current board.\n");
      return false;
   }
   //run through boards[] check if currentBoard is new one or already exists
   for(int i=0; i<size; i++){
      char* boards_str = board2str(&s->boards[i], height, width);
      if(strcmp(current_board_str, boards_str) == 0){
         free(boards_str);
         free(current_board_str);
         // printf("Board is not unique.\n");
         return false;
      }
      
      free(boards_str);
   }
   free(current_board_str);
   return true;
}



void movePointer(state* s)
{
   s->pointer +=1;
}


state* stateInit(void)
{
   state* s = (state*) calloc(1, sizeof(state));
   if(s == NULL){
      fprintf(stderr, "cannot calloc() space.\n");
      exit(EXIT_FAILURE);
   }
   s->size = 0;
   return s;
}

void boardAdd(state *s, board* b)
{
   if(s){
      if(s->size >= MAXBRDS){
         fprintf(stderr, "Collection overflow.\n");
         exit(EXIT_FAILURE);
      }
      s->boards[s->size] = *b;
      s->size = s->size +1;
   }
}


/*
C
height 3
0-2
ABC
ABC
>ABC
>ABA
*/
bool isSolution(board *b, int height, int width)
{
   // printf("in isSolution.....\n");
   for(int c=0; c<width; c++){
      for(int r=0; r<height-1; r++){
         if(b->self[r][c] != b->self[r+1][c]){
            return false;
         }
      }
   }
   return true;
}



