#include "stdlib.h"
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
    memcpy(node+1, data, sizeof(nodeType*));
    memcpy((char*)(node) + sizeof(nodeType) + sizeof(nodeType*), nextElem, sizeof(nodeType*));
    return node;
  }
}
// freeList -> takes nodeType*
