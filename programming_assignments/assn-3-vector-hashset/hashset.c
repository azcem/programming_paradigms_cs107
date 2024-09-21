#include "hashset.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define BUCKET_SIZE 4

void HashSetNew(hashset *h, int elemSize, int numBuckets,
		HashSetHashFunction hashfn, HashSetCompareFunction comparefn, HashSetFreeFunction freefn)
{
  assert(elemSize > 0 && numBuckets > 0);
  assert(hashfn != NULL && comparefn != NULL);
  //array of buckets
  vector* buckets = malloc(numBuckets * sizeof(vector));
  //init each bucket
  for (int i = 0; i < numBuckets; i++) {
    VectorNew(&buckets[i], elemSize, freefn, BUCKET_SIZE);
  }
  h->buckets = buckets;
  h->numBuckets = numBuckets;
  h->elemsCount = 0;
  h->HashSetHashFunction = hashfn;
  h->HashSetCompareFunction = comparefn;
  h->HashSetFreeFunction = freefn;
}

void HashSetDispose(hashset *h)
{
  for (int i = 0; i < h->numBuckets; i++) {
    VectorDispose(&h->buckets[i]);
  }
  free(h->buckets);
}

int HashSetCount(const hashset *h)
{ return h->elemsCount; }

void HashSetMap(hashset *h, HashSetMapFunction mapfn, void *auxData)
{
  assert(mapfn != NULL);
  for (int i = 0; i < h->numBuckets; i++) {
    VectorMap(&h->buckets[i], mapfn, auxData);
  }
}

void HashSetEnter(hashset *h, const void *elemAddr)
{
  assert(elemAddr != NULL);
  int bucketNum = h->HashSetHashFunction(elemAddr, h->numBuckets);
  assert(bucketNum >= 0 && bucketNum < h->numBuckets);

  h->elemsCount++;
  /* vector bucket = h->buckets[bucketNum]; */
  vector* bucket = h->buckets + bucketNum;
  for (int i = 0; i < VectorLength(bucket); i++) {
    void* elem = VectorNth(bucket, i);
    if (h->HashSetCompareFunction(elem, elemAddr) == 0) {
      VectorReplace(bucket, elemAddr, i);
      return;
    }
  }
  VectorAppend(bucket, elemAddr);
}

void *HashSetLookup(const hashset *h, const void *elemAddr)
{
  assert(elemAddr != NULL);
  int bucketNum = h->HashSetHashFunction(elemAddr, h->numBuckets);
  assert(bucketNum >=0 && bucketNum < h->numBuckets);

  vector* bucket = h->buckets + bucketNum;
  for (int i = 0; i < VectorLength(bucket); i++) {
    void* elem = VectorNth(bucket, i);
    if (h->HashSetCompareFunction(elem, elemAddr) == 0) return elem;
  }
  return NULL;
}
