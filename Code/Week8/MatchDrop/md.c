#include "mydefs.h"

bool file2str(const char* fname, char* str)
{
   if(fname == NULL && str == NULL){
      return false;
   }
   //readfile
   char* txt = getFileContent(fname);

   //tostring
   char *format_str = toStringFormat(txt);
   strcpy(str, format_str);
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

   boardAdd(s, &b);

   return s;
}

int solve(state* s, bool verbose)
{
   int height = s->board_height;
   int width = s->board_width;

   while(s->pointer < s->size) {
      // int current_pointer_Idx = s->pointer;
      // int current_size = s->size;
      // printf("current_pointer_Idx: %d \n", current_pointer_Idx);
      // printf("current_size: %d \n", current_size);
      // printf("---------------\n");

      board* b = &s->boards[s->pointer];
      // printBoard(b, height, width, true);
      if(isSolution(b, height, width)){
         // if yes. print b->moves
         
         int moves = s->boards[s->pointer].moves;
         board* b = &s->boards[s->pointer];
         // printf("solution total moves:%d \n", moves);

         // if verbose is true, track back to parents and print log
         if(verbose){
            printProcess(s, b);
         }
         return moves;
      }
      if(!s->find_solution){
         generateMove(s); // if generateMove, size++;
      }
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
   int solution_idx = s->pointer;
   // printf("solution_idx: %d\n", solution_idx);
   int parent_idx = final_b->parent_idx;
   int moves = final_b->moves;

   int* solution_arr = malloc((moves+1)*sizeof(int));
   if(solution_arr == NULL){
      fprintf(stderr, "Memory allocation failed.\n");
      exit(EXIT_FAILURE);
   }
   solution_arr[0] = solution_idx;
   int index = 0;

   // if parent idx not null, tracing back to previous parent 
   while(parent_idx != -1){
      index ++;
      // get parent idx, store in arr, record parent index
      solution_arr[index] = parent_idx;
      // move to previous parent idx
      parent_idx = s->boards[parent_idx].parent_idx;
   }

   
   board* original_bd = &s->boards[0];
   // printf("RESULT--------\n");
   printBoard(original_bd, height, width, false);
   
   // printf("--------\n");
   for(int i=moves-1; i>=0; i--){
      int board_idx = solution_arr[i];
      board* b = &s->boards[board_idx];
      printBoard(b, height, width, false);
      // printf("--------\n");
   }
   free(solution_arr);
}

void test(void)
{
   char str[MAXSTR];
   state* s;

   // isSolution
   strcpy(str, "A-AAA-BBB-CCC-BBB");
   s = str2state(str);
   assert(isSolution(&s->boards[0], s->board_height, s->board_width)==0);
   free(s);

   strcpy(str, "A-ABC-ABC-ABC-CBA");
   s = str2state(str);
   assert(isSolution(&s->boards[0], s->board_height, s->board_width)==0);
   free(s);

   strcpy(str, "A-ABC-ABC-ABC-ABC");
   s = str2state(str);
   assert(isSolution(&s->boards[0], s->board_height, s->board_width)==1);
   free(s);

   //lockColumn
   strcpy(str, "A-ACC-ABB-ABC-ABC");
   s = str2state(str);
   assert(lockColumn(&s->boards[0], s->board_height, 0)==1);
   assert(lockColumn(&s->boards[0], s->board_height, 1)==0);
   free(s);


   // isUniqueBoard
   strcpy(str, "A-ACC-ABB-ABC-ABC");
   s = str2state(str);

   state* s2;
   strcpy(str, "A-ABC-ABC-ABC-ABC");
   s2 = str2state(str);
   assert(isUniqueBoard(s, &s2->boards[0])==1);
   assert(isUniqueBoard(s, &s->boards[0])==0);


   // board2str
   board b1 = {0, {{'X','O'}, {'O','X'}}, 'H', 0};
   char* result1 = board2str(&b1, 2, 2);
   assert(result1 != NULL);
   assert(strcmp(result1, "XO-OX") == 0);
   free(result1);

   // toStringFormat
   char test1[] = "Hello\nWorld\nTesting";
   assert(strcmp(toStringFormat(test1), "Hello-World-Testing") == 0);

   char test2[] = "Hello\nWorld\nTesting\n";
   assert(strcmp(toStringFormat(test2), "Hello-World-Testing") == 0);

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

  char *full_content = malloc(1);
  if(full_content == NULL){
    perror("Warning: Unable to allocate memory.\n");
    fclose(fp);
    return NULL;
  }
  full_content[0] = '\0';
  char text[MAXBRDS];
  // read one line at a time
  while(fgets(text, sizeof(text), fp) != NULL){
    size_t old_len = strlen(full_content);
    size_t new_len = old_len + strlen(text) + 1;
    char *temp = realloc(full_content, new_len);
    if(temp == NULL){
      perror("Unable to realloc memory.\n");
      free(full_content);
      fclose(fp);
      return NULL;
    }
    full_content = temp;
    strcat(full_content, text);
  }
  // printf("Log: Full file content: %s\n",fullContent);

  fclose(fp);
  return full_content;
}

void printBoard(board* b, int height, int width, bool debug_mode)
{
   if(debug_mode){
      printf("printBoard-----\n");
      char hawk = b->hawk;
      printf("hawk: %c \n", hawk);

      int moves = b-> moves;
      printf("moves: %d \n", moves);

      int parent = b->parent_idx;
      printf("parent: %d \n", parent);
      printf("height: %d \n", height);
      printf("width: %d \n",width);
   }

   for(int r=0; r<height; r++){
      for(int c=0; c<width; c++){
         printf("%c ", b->self[r][c]);
      }
      printf("\n");
   }
   printf("\n");
}

void printBoards(state* s){
   printf("size %d \n", s->size);
   printf("pointer: %d\n", s->pointer);
   for(int i=0; i<s->size; i++){
      printBoard(&s->boards[i], s->board_height, s->board_width, true);
   }
}

char* board2str(board* b, int height, int width)
{

   if(b == NULL){
      fprintf(stderr, "Invalid board.\n");
      return NULL;
   }

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

   return tmp_str;
}

bool lockColumn(board* b, int height, int column)
{
   if(b == NULL){
      fprintf(stderr, "Invalid board.\n");
      exit(EXIT_FAILURE);
   }

   // check each column, if the column is match. lock it.
   bool is_same = true;
   for(int r=0; r<height-1; r++){
      if(b->self[r][column] != b->self[r+1][column]){
         is_same = false;
      }
   }
   return is_same;
}

void generateMove(state* s)
{
   if(s == NULL){
      fprintf(stderr, "Invalid state or board.\n");
      exit(EXIT_FAILURE);
   }
   
   int height = s->board_height;
   int width = s->board_width;

   // only gen the column if not the same!
   for(int c=0; c<width; c++){
      board current_board = s->boards[s->pointer];
      bool is_lock = lockColumn(&current_board, height, c);
      if(!is_lock){
         // gen move
         char hawk = current_board.hawk;
         current_board.moves +=1;
         char new_hawk = current_board.self[height-1][c];
         for(int r=height-1; r>=0; r--){
            if(r == 0){
               current_board.self[r][c] = hawk;
            } else {
               current_board.self[r][c] = current_board.self[r-1][c];
            }
            current_board.hawk = new_hawk;
         }
         current_board.parent_idx = s->pointer;

         bool is_unique_board = isUniqueBoard(s, &current_board);
         // gen new struct board (2D array, hawk, parent_idx, moves)
         // if not isExist in current board[], boardAdd
         if(is_unique_board){
            boardAdd(s, &current_board);
         }

         // check solution again, update state. once find, stop gen new move in board array
         if(isSolution(&current_board, height, width) == 1){
            s->find_solution = true;
            return ;
         }
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
   // printf("size:%d \n",size);
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
   s->find_solution = false;
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

bool isSolution(board *b, int height, int width)
{
   for(int c=0; c<width; c++){
      for(int r=0; r<height-1; r++){
         if(b->self[r][c] != b->self[r+1][c]){
            return false;
         }
      }
   }
   return true;
}



