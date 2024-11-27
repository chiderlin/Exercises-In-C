#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

#define KEY_SIZE 100
#define TABLE_SIZE 1000

struct node {
  char key[KEY_SIZE];
  int value;
  int freq;
  struct node *next; // handing collisions through chaining
};

struct hashTable {
  struct node *table[TABLE_SIZE]; // array of pointers to nodes (the actual hash table)
  int size; // the size of the table
};

typedef struct node node;
typedef struct hashTable dict;

dict* dict_init(void);

bool dict_addword(dict* t, const char* wd);

int dict_wordcount(const dict* t);

dict* dict_spell(const dict* t, const char* str);

void dict_free(dict** t);

int dict_mostcommon(const dict* p);

void test(void);
