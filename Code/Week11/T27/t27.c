#include "t27.h"

// 'Internal' function prototypes 
// ...
dict* create_node(dict* p);
void print_dict(dict* p);

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
  struct dict* curr = p;
  for(int i=0; wd[i] != '\0'; i++){
    // trans all to small case
    char c = tolower(wd[i]);
    printf("c:%c\n", c);  // Output the lowercase character
    int index = c - 'a'; // a-z: 0-25
    printf("index:%i\n", index);
    if(curr->dwn[index] == NULL){
      printf("NULL\n");
      curr->dwn[index] = create_node(curr);
      curr->dwn[index]->up = curr;
    }
    
    printf("----------------------\n");
    curr = curr->dwn[index]; // move to next node
    
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


dict* create_node(dict* p)
{
  dict* node = dict_init();
  return node;
}

void dict_free(dict** d)
{
  if(*d){
    free(*d);
    *d = NULL; // prevent wild pointers
  }
}

int dict_wordcount(const dict* p)
{
  if(!p) return 0;
  int wd_cnt = 0;
  if(p->terminal){
    wd_cnt += 1;
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
  if(p == NULL) return NULL;
  dict* curr = p;
  int len = strlen(str);
  for(int i=0; i<len; i++){
    char c = tolower(str[i]);
    int index = c - 'a';
    if(!curr->dwn[index]) return NULL;
    if(i+1 == len && curr->terminal) return curr;
    curr = curr->dwn[index]; //move to next word;
  }
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

//TODO
// CHALLENGE1
unsigned dict_cmp(dict* p1, dict* p2)
{
  // p1: c->a->r
  // p2: p->a->r->t
  if(!p1 || !p2) return 0; // unsure if one of them is null
  // what should i return. return p1+p2 step or not cmp (return 0).
  

}

//TODO
// CHALLENGE2
void dict_autocomplete(const dict* p, const char* wd, char* ret)
{
}
