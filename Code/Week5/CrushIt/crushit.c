#include "crushit.h"
#include "mydefs.h"


/*
{1,2,3,4,5}
{B,B,B,D,B},
{C,D,A,A,C},
{D,A,A,B,D},
{A,A,B,C,A},
{A,B,C,D,A},
{B,C,A,A,A},
*/
bool initialise(state* s, const char* str)
{
  // default-> char* str is filename.
  // try to open it
  const char* txt = getFileContent(str);
  // printf("txt: %s\n", txt);
  if(!txt){
    txt = str;
  }
  // check 2D array is valid
  int isMatch = regexCheck(txt);
  if(!isMatch){
    return false;
  }

  int len = strlen(txt);
  printf("len: %i\n", len);
  
  if(len < MAXCELL || len%WIDTH != 0){
    printf("Input string too short (not a multiple of 5)\n");
    return false;
  }
  if(len/WIDTH > MAXROWS) {
    printf("Maxinum number of rows is 20.\n");
    return false;
  }
  s->actual_height = len/WIDTH;
  for(int i=0; i<len; i++){
    int current_row = i / WIDTH;
    // printf("current_row %i\n",current_row);
    int current_column = getCurrentColumn(i);
    s->board[current_row][current_column] = txt[i];
  }

  return true;
}


int getCurrentColumn(int index){
    int current_column = index % WIDTH;
    return current_column;
}

char* getFileContent(const char* file_name){
  FILE* fp = fopen(file_name, "rt");
  if(fp == NULL){
    fprintf(stderr, "cannot open file.\n");
    // if an err occurs or the file is empty, return empty string.
    // return file_name;
    return NULL;
  }

  char *fullContent = malloc(1);
  if(fullContent == NULL){
    perror("Unable to allocate memory.\n");
    fclose(fp);
    // return file_name;
    return NULL;

  }
  char text[BIGSTR];
  // read one line at a time
  while(fgets(text, sizeof(text), fp) != NULL){
    // remove newline character if present
    text[strcspn(text, "\n")] = '\0';
    size_t old_len = strlen(fullContent);
    size_t new_len = old_len + strlen(text) + 1;
    char *temp = realloc(fullContent, new_len);
    if(temp == NULL){
      perror("Unable to realloc memory.\n");
      fclose(fp);
    }
    fullContent = temp;
    strcat(fullContent, text);
  }
  // printf("Full file content\n%s",fullContent);

  fclose(fp);
  return fullContent;
}

int regexCheck(const char* txt){
  regex_t reegex;
  int result = regcomp(&reegex, "^[A-Z.]+$", REG_EXTENDED);
  if(result){
    printf("Could not compile regex.\n");
    return false;
  }
  result = regexec(&reegex, txt, 0, NULL, 0);
  // printf("result: %i",result);

  regfree(&reegex);

  // match format return ture, else false
  return result == 0;
}


bool tostring(const state* s, char* str)
{
  // from state*s -> char* str, put it back to str.
  if(str == NULL || s == NULL) {
    return false;
  }
  int index = 0;
  int height = s->actual_height;
  for(int row=0; row<height; row++){
    for(int col=0; col<WIDTH; col++){
      bool row_dots = checkRowDot((state*)s, row);
      if(!row_dots){
        str[index++] = s->board[row][col];
      }
    }
  }
  str[index] = '\0';
  strncpy(s->str, str, BIGSTR-1);
  // s->str[BIGSTR-1] = '\0';
  printf("str:%s\n", str); 
  return true;
}

/*
BBBDB 
CDAAC
DAABD
AABCA
ABCDA
BCAAA


...DB 0
CDAAC 1
DAABD 2
AABC. 3
ABCD. 4
BC... 5
*/
bool matches(state* s)
{
  printf("Before Matching:\n");
  printBoard(s);

  matchHirizontal(s);
  matchVertical(s);
  removeCombo(s);
  printf("After Matching:\n");
  printBoard(s);
  int len = strlen(s->str);
  int height = s->actual_height;
  if(len == 0 && height == HEIGHT){
    return false;
  }
  return true;
}

/*
...DB 0
CDAAC 1
D...D 2
AABC. 3
A.... 4
BC... 5

1 1 1 0
0 0 0 0
0 1 1 0 row
0 0 0 1 
0 1 1 1 <-
0 0 1 1 
*/
bool dropblocks(state* s)
{
  int height = s->actual_height;
  for(int col=0; col<WIDTH; col++){
    bool init_dot = false;
    int row = height - 1;
    int is_dot = s->visited[row][col] ? 1 : 0; //1 dot, 0:alpha
    int dot_row_pointer = -1;


    while(row >= 0){
      // printf("Current row: %d, dot_row_pointer: %d\n", row, dot_row_pointer);
      is_dot = s->visited[row][col] ? 1 : 0;
      if(is_dot && !init_dot){
        init_dot = true;
        dot_row_pointer = row;
      }

      if(!is_dot && init_dot){ // alpha
        int alpha = s->board[row][col];

        //swap 
        s->board[dot_row_pointer][col] = alpha;
        s->board[row][col] = '.';

        // change pointer
        dot_row_pointer = dot_row_pointer - 1;
        // printf("Swapped: row %d with dot_row_pointer %d\n", row, dot_row_pointer);
      }
      // ------reset visited------
      s->visited[row][col] = false;
      // ------reset visited------
      row--;
    }


  }
  printf("----after drop-------\n");
  printBoard(s);
  printf("----after drop-------\n");
  return true;
}

void matchVertical(state* s){
  /**
   *         // 向下找
        // 檢查過，並且一樣，把狀態變成true
        // 先確認狀態是不是true, 即使是true了，還是檢查跟目前的字母是不是一樣
        // 如果還是一樣，就把原先的字母也一起變成true
   */
  char (*board)[WIDTH] = s->board;
  int actual_height = s->actual_height;
  int row_idx = 0;
  if (actual_height > HEIGHT){
    row_idx = actual_height - HEIGHT;
  }
  for(int col=0; col<WIDTH; col++){
    int count = 1;
    int next = 0;
    for(int row=row_idx; row<actual_height; row++){
      // reset the counter if we have a new column
      count = 1;
      next = row+1;
      if(row<actual_height-2){
        char current_point = board[row][col];
        printf("v current_point: %c\n", current_point);
        char next_point = board[next][col];
        printf("v next_point: %c\n", next_point);

        //check vertical to the right
        while(next < actual_height && current_point != '.' && current_point==board[next][col]){
          count+=1;
          // printf("next: %i\n", next);
          // printf("count: %i\n", count);
          printf("v match: (%i, %i)\n",next ,col);
          next_point = board[next][col];
          current_point = next_point;
          next +=1;
        }
        int total_count = count;
        printf("v total count: %i\n",count);
        if(total_count >= 3){
          for(int i=0; i<total_count; i++){
            s->visited[row+i][col] = true;
          }
        }
      }
    }
  }
  printVisited(s);
}

void matchHirizontal(state* s){
  /**
   *         // 向右找
        // 檢查過，並且一樣，把狀態變成true
        // 先確認狀態是不是true, 即使是true了，還是檢查跟目前的字母是不是一樣
        // 如果還是一樣，就把原先的字母也一起變成true
   */
  char (*board)[WIDTH] = s->board;
  int row_idx = 0;
  int actual_height = s->actual_height;
  // printf("actual_height: %i\n",actual_height);
  if (actual_height > HEIGHT){
    row_idx = s->actual_height - HEIGHT;
  }
  // printf("row_idx: %i\n", row_idx);
  for(int row=row_idx; row<actual_height; row++){
    int count = 1;
    int next = 0;
    for(int col=0; col<WIDTH; col++){
      // reset the counter if we have a new column
      count = 1;
      next = col+1;
      if(col<WIDTH-2){
        char current_point = board[row][col];
        printf("h current_point: %c\n", current_point);
        char next_point = board[row][next];
        printf("h next_point: %c\n", next_point);

        //check horizontally to the right
        while(next < WIDTH && current_point != '.' && current_point==board[row][next]){
          count+=1;
          // printf("next: %i\n", next);
          // printf("count: %i\n", count);
          printf("h match: (%i, %i)\n",row ,next);
          next_point = board[row][next];
          current_point = next_point;
          next +=1;
        }
        int total_count = count;
        printf("h total count: %i\n",count);
        if(total_count >= 3){
          for(int i=0; i<total_count; i++){
            s->visited[row][col+i] = true;
          }
        }
      }

    }
  }
   // printVisited(s);
}

void removeCombo(state* s){
  int actual_height = s->actual_height;
  for (int row = 0; row < actual_height; row++) {
      for (int col = 0; col < WIDTH; col++) {
          // 1:true, 0:false
          int match = s->visited[row][col] ? 1 : 0;
          if(match) {
            s->board[row][col] = '.';
          }
      }
  }
}

void printBoard(state* s){
  int height = s->actual_height;
  char (*board)[WIDTH] = s->board;
  for(int i=0; i<height;i++){
    for(int j=0; j<WIDTH; j++){
      printf("%c ", board[i][j]);
    }
    printf("\n");
  }
}

void printVisited(state* s) {
    int actual_height = s->actual_height;
    for (int row = 0; row < actual_height; row++) {
        for (int col = 0; col < WIDTH; col++) {
            // 打印 1 表示 true, 0 表示 false
            printf("%d ", s->visited[row][col] ? 1 : 0);
        }
        printf("\n");  // 換行打印
    }
}

bool checkRowDot(state *s, int row){
  bool dot = true;
  char (*board)[WIDTH] = s->board;
  for(int col=0; col<WIDTH; col++){
    if(board[row][col] != '.'){
      dot = false;
    }
  }
  return dot;
}


void test(void)
{
    state s;
    char str[WIDTH*MAXROWS+1];
    assert(initialise(&s, "eleven.txt") == true);
    assert(matches(&s)==true);
    assert(tostring(&s, str) == true);
    assert(strcmp(str,    "") == 0);

    // assert(initialise(&s, "twelve.txt") == true);
    // assert(matches(&s)==true);
    // assert(tostring(&s, str) == true);
    // assert(strcmp(str,    "") == 0);
}
