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
#include "stdbool.h"
#include "string.h"
#include "stddef.h"
#include "stdlib.h"
#include "./helpers.h"



char *ConcatAll(nodeType *list) {
  printf("\n");
}

//driver
int main(void) {
  //generate gameThree list
  int i1 = 2, i2 = 2;
  nodeType* gameThree = createNode (
    List, createNode(String, "Yankees", NULL),
    createNode(
      List, createNode(Integer, &i1, NULL),
      createNode(
        List, createNode(String, "Diamondbacks", NULL),
        createNode(
          List, createNode(Integer, &i2, NULL),
          createNode(Nil, NULL, NULL))
        )
      )
    )
  ;
  printList(*gameThree, gameThree+1, true);
  freeNodes(gameThree);
  int i4 = 4, i5 = 5;
  //generate nestedNumbers list
  nodeType* nestedNumbers = createNode (
    List, createNode(String, "one", NULL),
    createNode (
      List, createNode(
        List, createNode(
          List, createNode(Integer, &i2, NULL),
          createNode(
            List, createNode (
              List, createNode(String, "three", NULL),
              createNode (
                List, createNode(Integer, &i4, NULL), createNode(Nil, NULL, NULL)
              )
            ), createNode(Nil, NULL, NULL)
          )
        ), createNode(Nil, NULL, NULL)
      ), createNode(
        List, createNode(Integer, &i5, NULL),
        createNode (
          List, createNode(String, "six", NULL), createNode(Nil, NULL, NULL)
        )
      )
    )
  );
  printList(*nestedNumbers, nestedNumbers+1, true);
  //dummyList
  nodeType* dummyList = createNode (
    List, createNode(String, "Yankees", NULL),
    createNode(List, createNode(Integer, &i1, NULL), createNode(Nil, NULL, NULL))
  );
}

