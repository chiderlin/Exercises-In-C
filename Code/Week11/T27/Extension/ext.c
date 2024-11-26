#include "ext.h"


unsigned int hash(const char *key);
dict* create_table(void);
void insert(dict* table, const char* key, int value);
int search(dict* table, const char* key);
void delete(dict* table, const char* key);
void print_table(dict* table);
char* string_to_lower(const char* str);

dict* dict_init(void)
{
  dict* t = (dict*)malloc(sizeof(dict));
  for(int i=0; i<TABLE_SIZE; i++){
    t->table[i] = NULL;
  }
  t->size = 0;
  return t;

}

bool dict_addword(dict* t, const char* wd)
{
  // insert
  if(!t || !wd) return false;
  char* lower_str = string_to_lower(wd);
  // printf("lower_str:%s\n",lower_str);
  unsigned int index = hash(lower_str);
  node* curr = t->table[index];
  while(curr != NULL){
    if(strcmp(curr->key, lower_str) == 0){
      curr->freq++;
      t->size++;
      return false;
    }
    curr = curr->next;
  }

  node* new_node = (node*)malloc(sizeof(node));
  if(!new_node){
    perror("Failed to allocate memory for new node");
    return false;
  }
  strcpy(new_node->key, lower_str);
  new_node->value = 1;
  new_node->freq = 1;
  new_node->next = t->table[index];
  t->table[index] = new_node; // insert at the head of linked list
  t->size++;

  free(lower_str);
  // print_table(t);
  return true;
}

int dict_wordcount(const dict* t)
{
  if(!t) return 0;
  return t->size;
}

dict* dict_spell(const dict* t, const char* str)
{
  // if find, return node, otherwise return null.
  if(!t || !str) return NULL;
  char* lower_str = string_to_lower(str);

  unsigned int index = hash(lower_str);
  node* curr = t->table[index];

  while(curr != NULL){
    if(strcmp(curr->key, lower_str)==0){
      return (dict*)curr;
    }
    curr = curr->next;
  }
  free(lower_str);
  // print_table(t);
  return NULL;
}

void dict_free(dict** t)
{
  //free, delete table
  if(!t || !(*t)) return;
  for(int i=0; i<TABLE_SIZE; i++){
    node* curr = (*t)->table[i];
    while(curr != NULL){
      node* temp = curr;
      curr = curr->next;
      free(temp); // free each node
    }
  }
  free(*t); // free the hash table itself
  *t = NULL;
}

int dict_mostcommon(const dict* t)
{
  if(!t) return -1;
  int max_freq = 0;
  // char most_common[KEY_SIZE] = "";
  for(int i=0; i<TABLE_SIZE; i++){
    node* curr = t->table[i];
    while(curr != NULL){
      if(curr->freq > max_freq){
        max_freq = curr->freq;
        // strcpy(dict_mostcommon, curr->key);
      }
      curr = curr->next;
    }
  }
  return max_freq;
}

void test(void)
{}

unsigned int hash(const char *key)
{
  unsigned int hash_value = 0;
  for(int i=0; key[i] != '\0'; i++){
    // multiply by 31(left shift by 5 and add the character value)
    hash_value = (hash_value << 5) + key[i];
  }
  return hash_value % TABLE_SIZE;
}

dict* create_table(void)
{
  dict* new_table = (dict*)malloc(sizeof(dict));
  for(int i=0; i<TABLE_SIZE; i++){
    new_table->table[i] = NULL;
  }
  return new_table;
}

void insert(dict* table, const char* key, int value)
{
  unsigned int index = hash(key);
  node* curr = table->table[index];

  // traverse the linked list to check if the key already exists.
  while(curr != NULL){
    // if the key exists
    if(strcmp(curr->key, key)==0){
      curr->value = value;
      curr->freq++;
      return;
    }
    curr = curr->next;
  }
  // the key not exists
  node* new_node = (node*)malloc(sizeof(node));
  strcpy(new_node->key, key);
  new_node->value = value;
  new_node->freq = 1;
  // insert new node at the beginning of the linked list
  new_node->next = curr;
  // set new_node becomes the first node in the linked list
  curr = new_node;
}

int search(dict* table, const char* key)
{
  unsigned int index = hash(key);
  node* curr = table->table[index];
  while(curr != NULL){
    if(strcmp(curr->key, key) == 0){
      // curr->freq++;
      return curr->value;
    }
    curr = curr->next;
  }
  return -1;
}

void delete(dict* table, const char* key)
{
  unsigned int index = hash(key);
  node* curr = table->table[index];
  node* prev = NULL;

  while(curr != NULL){
    if(strcmp(curr->key, key) == 0){
      if(prev == NULL){
        table->table[index] = curr->next;
      } else {
        prev->next = curr->next; //bypass the current node
      }
      free(curr);
      return;
    }
    prev = curr;
    curr = curr->next;
  }
}

void print_table(dict* table)
{
  for(int i=0; i<TABLE_SIZE; i++){
    node* curr = table->table[i];
    if(curr != NULL){
      printf("indexL %d:", i);
      while(curr != NULL){
        printf("(%s, %d, freq: %d) -> ", curr->key, curr->value, curr->freq);
        curr = curr->next;
      }
      printf("NULL\n");
    }
  }
}


char* string_to_lower(const char* str)
{
  if(str == NULL) return NULL;
  
  size_t len = strlen(str);
  char* lower_str = malloc(len+1);
  if(lower_str == NULL) return NULL;

  for(int i=0; str[i]!= '\0' ;i++){
    lower_str[i] = tolower((unsigned char)str[i]);
  }
  lower_str[len] = '\0';

  return lower_str;
}
