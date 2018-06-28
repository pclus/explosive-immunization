#ifndef STD_H
#define STD_H
  #include<stdlib.h>
  #include <stdio.h>
  #include "macros.h"
#endif

#ifndef NODE_H
#define NODE_H
  #include "node.h"
#endif

typedef struct element{
  Node *node;
  double f;
  struct element *next;
} Element;

// typedef enum { false, true } bool;

bool isEmpty(Element *queue);
int push(Element **queue, Node *node);
int pull(Element **queue);
int deleteQueue(Element **queue);

