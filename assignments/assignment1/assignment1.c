/**
* Traverses a properly structured list, and returns the ordered
* concatenation of all strings, including those in nested sublists.
*
* When applied to the two lists drawn above, the following strings
* would be returned:
*
* ConcatAll(gameThree) would return "YankeesDiamondbacks"
* ConcatAll(nestedNumbers) would return "onethreesix"
*/

#include "stdio.h"
#include "string.h"
#include "stddef.h"
#include "stdlib.h"
#include "./helpers.h"



char *ConcatAll(nodeType *list) {
  printf("\n");
}

//driver
int main(void) {
  printf("%d%d%d", sizeof(nodeType), sizeof(void*), sizeof(nodeType*));
  //generate gameThree list
  int i1 = 2, i2 = 2;
  nodeType* gameThree = createNode (
    List, createNode(String, "Yankees", NULL),
    createNode(
      List, createNode(Integer, &i1, NULL),
      createNode(
        List, createNode(String, "Diamondbacks", NULL),
        createNode(
          List, createNode(Integer, &i2, createNode(Nil, NULL, NULL)), NULL
        )
      )
    )
  );
  //generate nestedNumbers list
}

