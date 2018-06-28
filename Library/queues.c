#ifndef QUEUES_H
#define QUEUES_H
#include "queues.h"
#endif

bool isEmpty(Element *queue){
  return (queue==NULL);
}

int push(Element **queue, Node *node){
  Element *element, *forward;
  
  if((element=(Element *) malloc(sizeof(Element)))==NULL){
    fprintf(stderr, "Error in push: not enough memory to add elements.\n");
    fprintf(stderr, "Error: program continuing with errors.\n");
    return 0;
  }
  
  element->node=node;
  element->next=NULL;
  element->f=node->ci;
  
  if(isEmpty(*queue)){
    *queue=element;
    return 1;
  }
  
  forward=*queue;
  if(forward->f > element->f){
    *queue=element;
    element->next=forward;
    return 0;
  }
  
  while(forward->next!=NULL && forward->next->f < element->f)
    forward=forward->next;

  element->next=forward->next;
  forward->next=element;
  return 0;
}

int pull(Element **queue){
  Element *aux=*queue;
  
  if(isEmpty(*queue))
    return 1;
  
  *queue=aux->next;
  free(aux);
  return 0;
}

int deleteQueue(Element **queue){
  while(!isEmpty(*queue))
    pull(queue);
  return 0;
}