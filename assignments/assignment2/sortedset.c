#include "sortedset.h"
#include "string.h"
#include "stdlib.h"

#define NodeSize(clientElem) ((clientElem) + 2 * sizeof(int))

void setNew(sortedset* set, int elemSize, int (*cmpfn)(const void*, const void*)) {
  int* elems = malloc(kInitialCapacity * NodeSize(elemSize) + sizeof(int));
  set->root = elems;
  *set->root = -1;
  set->elemSize = elemSize;
  set->logicalSize = 0;
  set->allocatedSize = kInitialCapacity;
  set->cmpfn = cmpfn;
}

static int *FindNode(sortedset *set, const void *elem) {
  void *curr;
  int comp, *root = set->root;

  while (*root != -1) {
    curr = (char*) (set->root + 1) + *root * NodeSize(set->elemSize);
    comp = set->cmpfn(elem, curr);
    if (comp == 0) break;
    root = (int*) ((char*) curr + set->elemSize); //shifts to index of next smaller node
    if (comp > 0) root++; //shifts to index of next bigger node
  }
  return root;
}

void* setSearch(sortedset* set, const void* elemPtr) {
  int *node = FindNode(set, elemPtr);
  if (*node == -1) return NULL;
  return (char*) (set->root + 1) + *node * NodeSize(set->elemSize);
}

bool setAdd(sortedset* set, const void *elemPtr) {
  int *child;
  void *dest;

  child = FindNode(set, elemPtr);
  if (*child != -1) return false;

  if (set->logicalSize == set->allocatedSize) {
    set->root = realloc(set->root, sizeof(int) + set->allocatedSize * 2 * NodeSize(set->elemSize));
    set->allocatedSize *= 2;
    assert(set->root != NULL);
  }
  *child = set->logicalSize++;

  dest = (char*) (set->root + 1) + *(child) * NodeSize(set->elemSize);
  memcpy(dest, elemPtr, set->elemSize);
  child = (int*) ((char*) dest + set->elemSize);
  child[0] = -1;
  child[1] = -1;
  return true;
}

int main() {
  return 0;
}
