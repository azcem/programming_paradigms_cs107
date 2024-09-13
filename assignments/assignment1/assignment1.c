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



//Actual assignment
char *ConcatAll(nodeType *list) {
  if (!list || *list == Nil || *list == Integer) return strdup("");
  if (*list == String) {
    char* str = strdup((char*)(list+1));
    return str;
  }
  if (*list == List) {
    nodeType* firstBranch = *(nodeType**)((char*)list + sizeof(nodeType));
    char* front = ConcatAll(firstBranch);
    nodeType* secondBranch = *(nodeType**)((char*)list + sizeof(nodeType) + sizeof(nodeType*));
    char* back = ConcatAll(secondBranch);
    char* str = malloc(strlen(front) + strlen(back) + 1);
    strcpy(str, front);
    strcpy(str+strlen(front), back);
    free(back);
    free(front);
    return str;
  }
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
  char* str = ConcatAll(gameThree);
  printf("%s ", str);
  free(str);
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
  str = ConcatAll(nestedNumbers);
  printf("\n %s", str);
  free(str);
  freeNodes(nestedNumbers);
}

