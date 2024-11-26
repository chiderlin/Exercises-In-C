#include "t27.h"
#define MAXSTR 50
// 'Internal' function prototypes 
// ...
dict* create_child_node(dict* p);
void print_dict(dict* p);
void print_word_from_node(dict* node);
char* get_dict_word(dict* node);
int find_first_diff(char* str1, char* str2);
int sum_steps(char* str1, char* str2, int index);
dict* get_tail_wd_node(const dict* p, char*wd);
void find_max_freq(const dict* node, const dict** result_node, int* result_freq, bool recursive);

dict* dict_init(void)
{
  struct dict* node = malloc(sizeof(dict));
  for(int i=0; i<ALPHA; i++){
    node->dwn[i] = NULL;
  }
  node->up = NULL;
  node->terminal = false;
  node->freq = 0;
  return node;
}

/**
 * 0:a, 1:b, 2:c ... 25:z, 26:'
 */
bool dict_addword(dict* p, const char* wd)
{
  dict* curr = p;
  bool add = false;
  for(int i=0; wd[i] != '\0'; i++){
    // trans all to small case
    char c = tolower(wd[i]);
    printf("c:%c\n", c);  // Output the lowercase character
    int index = c - 'a'; // a-z: 0-25
    if(wd[i] == '\''){
      index = 26;
    }
    printf("index:%i\n", index);
    if(index<0 || index >= ALPHA){
      printf("Invalid character %c\n", c);
      return false;
    }
    if(curr->dwn[index] == NULL){
      add = true;
      printf("NULL\n");
      curr->dwn[index] = create_child_node(curr);
    }
    
    printf("----------------------\n");
    curr = curr->dwn[index]; // move to next node
  }

  // already added, just increase freq here. and return false
  if(!add && curr->terminal){
    curr->freq++;
    return false;
  }
  curr->terminal = true; // last node, terminal true.
  curr->freq++;
  print_dict(curr);
  return true;
}

void print_dict(struct dict* d) {
    if (d == NULL) return;
    printf("Terminal: %s\n", d->terminal? "true":"false");
    printf("Frequency: %d\n", d->freq);
    // Print the 'up' pointer (parent)
    if (d->up != NULL) {
        printf("Parent exists:\n");
        print_dict(d->up);  // Recursively print the parent
    } else {
        printf("No parent (this is the root).\n");
    }
        
    for (int i = 0; i < ALPHA; i++) {
        // Print down pointers (children)
        if (d->dwn[i] != NULL) {
            printf("Child[%d] exists.\n", i);
        }
    }
    
}


dict* create_child_node(dict* p)
{
  dict* node = dict_init();
  if(node != NULL){
    node->up = p;
  }
  return node;
}

void dict_free(dict** d)
{
  if(*d){
    // free recursively
    for(int i=0; i<ALPHA; i++){
      if((*d)->dwn[i] != NULL){
        dict_free((*d)->dwn[i]);
      }
    }
    free(*d);
    *d = NULL; // prevent wild pointers
  }
}

int dict_wordcount(const dict* p)
{
  if(!p) return 0;
  int wd_cnt = 0;
  if(p->terminal){
    // wd_cnt += 1;
    wd_cnt += p->freq;
  }
  for(int i=0; i<ALPHA; i++){
    wd_cnt += dict_wordcount(p->dwn[i]);
  }
  return wd_cnt;
}

int dict_nodecount(const dict* p)
{
  // only access nodes that are not NULL
  if(p==NULL){
    return 0; 
  }
  int cnt = 1;
  for(int i=0; i<ALPHA; i++){
    cnt += dict_nodecount(p->dwn[i]);
  }

  return cnt;
}

dict* dict_spell(const dict* p, const char* str)
{
  if(p == NULL || str == NULL) return NULL;
  
  dict* curr = p;
  int len = strlen(str);
  for(int i=0; i<len; i++){
    char c = tolower(str[i]);
    int index = c - 'a';
    if(str[i] == '\''){
      index = 26;
    }
    if(!curr->dwn[index]) return NULL;

    // final character is terminal
    if(i+1 == len && curr->dwn[index]->terminal) {
      curr = curr->dwn[index];
      print_dict(curr);
      print_word_from_node(curr);
      return curr;
    }
    curr = curr->dwn[index]; //move to next word;
  }

  return NULL;
}

int dict_mostcommon(const dict* p)
{
  if(!p) return 0;
  int max_freq = 0;
  if(p->terminal){
    max_freq = p->freq;
  }

  for(int i=0; i<ALPHA; i++){
    int child_freq = dict_mostcommon(p->dwn[i]);
    if(child_freq > max_freq){
      max_freq = child_freq;
    }
  }
  return max_freq;
}

// CHALLENGE1
unsigned dict_cmp(dict* p1, dict* p2)
{
  // print_word_from_node(p1);
  // print_word_from_node(p2);
  char* str1 = get_dict_word(p1);
  printf("Word1: %s\n", str1);

  char* str2 = get_dict_word(p2);
  printf("Word2: %s\n", str2);
  

  int index = find_first_diff(str1, str2);
  if(index == -1){
    return 0; //step 0
  }

  int step = sum_steps(str1, str2, index);
  printf("step: %i\n", step);

  free(str1);
  free(str2);
  return step;
}

int sum_steps(char* str1, char* str2, int index)
{  
  int step = 0;
  int len1 = strlen(str1); //cat 3
  int len2 = strlen(str2); //car 3
  // printf("index: %i\n", index); //idx=2

  int str1_total_index = len1-1; // 2
  int str2_total_index = len2-1; // 2
  if(str1_total_index-index >= 0){
    step += str1_total_index - index + 1; // 0+1
  }
  if(str2_total_index-index >= 0){
    step += str2_total_index - index + 1; // 0+1
  }

  // car & cart: 1
  // cat & cart: 3
  // car & cat: 2
  // car & part: 7
  // idx1 len1-1 = 2
  // idx2 len2-1 = 3
  return step;
}

int find_first_diff(char* str1, char* str2)
{
  int index = 0;
  while(str1[index] != '\0' && str2[index] != '\0'){
    if(str1[index] != str2[index]){
      return index;
    }
    index++;
  }

  // one str comes to the end, return the index.
  if(str1[index] != str2[index]){
    return index;
  }

  return -1; // same word

}


// p1: dict* r (r->a->c)
// p2: dict* t (t->r->a->p)
char* get_dict_word(dict* node)
{
  char* str = malloc(1);
  if(!str){
    perror("Failed to allocate memory");
    exit(EXIT_FAILURE);
  }

  int length = 0;
  while(node != NULL && node->up != NULL){
    int index = 0;
    dict* parent = node->up;
    while(index < ALPHA && parent->dwn[index] != node){
      index++;
    }
    if(index < ALPHA){
      char* temp = realloc(str, length+2); //+2 for the new char and '\0'
      if(!temp){
        free(str);
        perror("Failed to reallocate memory");
        exit(EXIT_FAILURE);
      }
      str = temp;
      str[length++] = 'a' + index;
    }

    node = parent;
  }
  str[length] = '\0';

  // reverse the string since it's constructed backwards
  for(int i=0; i<length/2; i++){
    char temp = str[i];
    str[i] = str[length-i-1];
    str[length-i-1] = temp;
  }
  // printf("Word: %s\n", str);
  return str;
}


//OK: 原本dict_spell()沒有return curr, 導致印出來一直是原本的dict* p
void print_word_from_node(dict* node)
{
  if(node == NULL) return;
  while(node != NULL && node->up != NULL){
    int index = 0;
    dict* parent = node->up;
    while(index < ALPHA && parent->dwn[index] != node){
      index++;
    }
    // printf("index value: %i\n", index);
    if(index < ALPHA){
      char current_char = 'a' + index;
      if(index == 26){
        current_char = '\'';
      }
      printf("current_char: %c\n", current_char);
    }

    // move backward to previous node
    node = parent;
  }
}

dict* get_tail_wd_node(const dict* p, char*wd)
{
  dict* curr = p;
  for(int i=0; wd[i] != '\0'; i++){
    char c = tolower(wd[i]);
    int index = c - 'a';
    if(index<0 || index > ALPHA || curr->dwn[index]== NULL){
      printf("Prefix not found.\n");
      return NULL;
    }
    curr = curr->dwn[index]; // move to child
  }
  return curr;
}

// CHALLENGE2
// wd:car
// ret:t

void dict_autocomplete(const dict* p, const char* wd, char* ret)
{
  if(p==NULL || wd==NULL || ret==NULL){
    printf("Invaild input.\n");
    return;
  }

  dict* curr = get_tail_wd_node(p, wd);
  // dict* curr = p;
  // for(int i=0; wd[i] != '\0'; i++){
  //   printf(" wd[i]:%c\n", wd[i]);
  //   char c = tolower(wd[i]);
  //   int index = c - 'a';
  //   printf("index:%i\n",index);
  //   if(index<0 || index > ALPHA || curr->dwn[index]== NULL){
  //     printf("Prefix not found.\n");
  //     return ;
  //   }
  //   curr = curr->dwn[index]; // move to child //r
  // }

  // find most frequent word starting from curr
  const dict* max_freq_node = NULL;
  int max_freq = -1;

  find_max_freq(curr, &max_freq_node, &max_freq, false);
  if(max_freq_node == NULL){
    strcpy(ret, "");
    return;
  }
  print_word_from_node(max_freq_node);
  printf("max_freq:%i\n",max_freq);
  
  char temp[100];
  int length = 0;
  while(max_freq_node != NULL && max_freq_node->up != NULL){
    const dict* parent = max_freq_node->up;
    for(int i=0; i<ALPHA; i++){
      if(parent->dwn[i] == max_freq_node){
        temp[length++] = 'a' + i;
        break;
      }
    }
    max_freq_node = parent;
  }
  temp[length] = '\0';
  int prefix_len = strlen(wd);
  for(int i=prefix_len; i< length; i++){
    ret[i-prefix_len] = temp[length-i-1];
  }
  ret[length-prefix_len] = '\0';
  printf("ret:%s\n",ret);

}

void find_max_freq(const dict* node, const dict** result_node, int* result_freq, bool recursive)
{
  if(node == NULL) return;

  if(recursive){
    if(node->freq > *result_freq){
      *result_freq = node->freq;
      *result_node = node;
    } 
  }

  for(int i=0; i<ALPHA; i++){
    if(node->dwn[i] != NULL){ //r
      find_max_freq(node->dwn[i], result_node, result_freq, true);
    }
  }
}


void test(void)
{
  dict* d = dict_init();
  assert(!d==NULL);

  // test get_tail_wd_node()
  assert(get_tail_wd_node(d, "node")==NULL);
  assert(dict_addword(d, "node"));
  assert(get_tail_wd_node(d, "node"));

  // get_dict_word()
  assert(dict_addword(d, "book"));
  assert(dict_addword(d, "bob"));
  dict *p1 = dict_spell(d, "book");
  dict *p2 = dict_spell(d, "bob");
  char* str1 = get_dict_word(p1);
  char* str2 = get_dict_word(p2);
  assert(strcmp(str1, "book")==0);
  assert(strcmp(str2, "bob")==0);

  // test find_first_diff()
  assert(find_first_diff(str1, str2)==2);
  assert(find_first_diff("dead", "deal")==3);
  
  // test sum_steps()
  assert(sum_steps(str1, str2, 2)==3);
  assert(sum_steps("dead", "deal", 3)==2);

  // test find_max_freq()
  assert(!dict_addword(d, "book")); // add twice
  const dict* result_node = NULL;
  int result_freq = -1;
  find_max_freq(d, &result_node, &result_freq, false);
  assert(result_node!= NULL);
  assert(result_freq==2);
  int b_index = 'b' - 'a';
  int o_index = 'o' - 'a';
  int k_index = 'k' - 'a';
  assert(result_node==d->dwn[b_index]->dwn[o_index]->dwn[o_index]->dwn[k_index]);

  free(str1);
  free(str2);
  dict_free(&d);

  // test create_child_node()
  dict* n = dict_init();
  dict* node = create_child_node(n->dwn[0]);
  assert(node!=NULL);
  assert(node->up==n->dwn[0]);

  free(node);
  free(n);
}
//Vdict* create_child_node(dict* p);

//Vchar* get_dict_word(dict* node);
//Vint find_first_diff(char* str1, char* str2);
//Vint sum_steps(char* str1, char* str2, int index);
//Vdict* get_tail_wd_node(const dict* p, char*wd);
//Vvoid find_max_freq(const dict* node, const dict** result_node, int* result_freq, bool recursive);
