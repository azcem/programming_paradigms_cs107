#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"
#include "string.h"
#include "./helpers.h"

// createNode -> returns nodeType*
nodeType* createNode(nodeType type, void* data, nodeType* nextElem) {
  nodeType* node;
  if (type == Nil) {
    node = malloc(sizeof(nodeType));
    *node = type;
    return node;
  }
  if (type == Integer) {
    node = malloc(sizeof(nodeType) + sizeof(int));
    *node = type;
    *(int*)(node+1) = *(int*) data;
    return node;
  }
  if (type == String) {
    node = malloc(sizeof(nodeType) + (int) strlen((char*) data) + 1 );
    *node = type;
    strcpy((char*) (node+1), (char*) data);
    return node;
  }
  if (type == List) {
    node = malloc(sizeof(nodeType) + 2 * sizeof(nodeType*)); //hardcoded for now
    *node = type;
    *(nodeType**)(node+1) = data;
    *(nodeType**)(node+3) = nextElem;
    return node;
  }
}
// freeList -> takes nodeType*
void freeNodes(nodeType* node) {
  if (node == NULL) return;
  if (*node != List) {
    free(node);
    return;
  }
  //free nextElem
  freeNodes(*(nodeType**)(node+3));
  //free Elem
  freeNodes(*(nodeType**)(node+1));
  //free node
  free(node);
  return;
}

void printList(nodeType type, void* data, bool isRoot) {
  if (type == Nil) return;
  if (type == Integer) {
    printf("%d ", *(int*)data);
    return;
  }
  if (type == String) {
    printf("%s ", (char*) data);
    return;
  }
  if (type == List) {
    if (isRoot)
      printf("(");
    //print current root content
    nodeType* currentElem = *(nodeType**)(data);
    nodeType* nextElem = *((nodeType**)data+1);
    printList(*currentElem, currentElem+1, *currentElem == List && *nextElem != List);
    //print nextElem
    if (nextElem && *nextElem != Nil) {
      printList(*nextElem, nextElem+1, *currentElem == List && *nextElem != List);
    }
    if (isRoot)
      printf(")");
  }
}
