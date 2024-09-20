#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void VectorNew(vector *v, int elemSize, VectorFreeFunction freeFn, int initialAllocation)
{
  assert(elemSize > 0);
  assert(initialAllocation > 0);
  void* elemArray = malloc(initialAllocation * elemSize);
  v->elems = elemArray;
  v->elemSize = elemSize;
  v->size = initialAllocation;
  v->logicalLength = 0;
  v->initialAllocation = initialAllocation;
  v->VectorFreeFunction = freeFn;
}

void VectorDispose(vector *v)
{
  if (v->VectorFreeFunction) {
    for (int i = 0; i < v->logicalLength; i++) {
      v->VectorFreeFunction((char*)v->elems + i * v->elemSize);
    }
  }
  free(v->elems);
}

int VectorLength(const vector *v)
{ return v->logicalLength; }

void *VectorNth(const vector *v, int position)
{
  assert(position >= 0 && position <= v->logicalLength - 1);
  return (char*)v->elems + position * v->elemSize;
}

void VectorReplace(vector *v, const void *elemAddr, int position)
{
  assert(position >= 0 && position <= v->logicalLength-1);
  char* elem = (char*)v->elems + position * v->elemSize;
  if (v->VectorFreeFunction)
    v->VectorFreeFunction(elem);
  memcpy(elem, elemAddr, v->elemSize);
}

void VectorInsert(vector *v, const void *elemAddr, int position)
{
  assert(position >= 0 && position <= v->logicalLength);
  if (v->logicalLength + 1 > v->size) {
    void* newElems = realloc(v->elems, (v->size + v->initialAllocation) * v->elemSize);
    assert(newElems != NULL);
    v->elems = newElems;
    v->size += v->initialAllocation;
  }
  if (position != v->logicalLength) {
    char* chunkToMove = (char*)v->elems + position * v->elemSize;
    int chunkSize = (char*)v->elems + v->logicalLength * v->elemSize - chunkToMove;
    memmove(chunkToMove + v->elemSize, chunkToMove, chunkSize);
    memcpy(chunkToMove, elemAddr, v->elemSize);
  } else {
    memcpy((char*)v->elems + v->logicalLength * v->elemSize, elemAddr, v->elemSize);
  }
  /* char* shiftPosition = (char*)v->elems + position * v->elemSize; */
  /* char* destination = shiftPosition + v->elemSize; */
  /* int byteCount = (v->logicalLength - position) * v->elemSize; */
  /* memmove(destination, shiftPosition, byteCount); */
  /* memcpy(shiftPosition, elemAddr, v->elemSize); */
  v->logicalLength++;
}

void VectorAppend(vector *v, const void *elemAddr)
{
  VectorInsert(v, elemAddr, v->logicalLength);
  /* if (v->logicalLength == v->size) { */
  /*   v->elems = realloc(v->elems, v->size + v->initialAllocation); */
  /*   v->size += v->initialAllocation; */
  /* } */
  /* memcpy((char*)v->elems + v->logicalLength * v->elemSize, elemAddr, v->elemSize); */
  /* v->logicalLength++; */
}

void VectorDelete(vector *v, int position)
{
  assert(position >=0 && position <= v->logicalLength-1);
  char* elem = (char*)v->elems + position * v->elemSize;
  char* source = elem + v->elemSize;
  int byteCount = (v->logicalLength - position - 1) * v->elemSize;
  if (v->VectorFreeFunction)
    v->VectorFreeFunction(elem);
  if (byteCount != 0) {
    memmove(elem, source, byteCount);
  }
  v->logicalLength--;
}

void VectorSort(vector *v, VectorCompareFunction compare)
{
  assert(compare != NULL);
  qsort(v->elems, v->logicalLength, v->elemSize, compare);
}

void VectorMap(vector *v, VectorMapFunction mapFn, void *auxData)
{
  assert(mapFn != NULL);
  for (char* elem = v->elems; elem != (char*)v->elems + v->logicalLength * v->elemSize; elem += v->elemSize) {
    mapFn(elem, auxData);
  }
}

static const int kNotFound = -1;
int VectorSearch(const vector *v, const void *key, VectorCompareFunction searchFn, int startIndex, bool isSorted)
{
  assert(startIndex >= 0 && startIndex <= v->logicalLength);
  assert(key != NULL);
  assert(searchFn != NULL);
  // do binary seach
  if (isSorted) {
    char* elem = bsearch(key, v->elems, v->logicalLength, v->elemSize, searchFn);
    return elem == NULL? kNotFound : (int)(elem - (char*)v->elems) / v->elemSize;
  }
  // do linear search
  for (char* elem = (char*)v->elems + startIndex * v->elemSize; elem != (char*)v->elems + v->logicalLength * v->elemSize; elem += v->elemSize) {
    if (searchFn(elem, key) == 0) return (int)(elem - (char*)v->elems) / v->elemSize;
  }
  return kNotFound;
} 
