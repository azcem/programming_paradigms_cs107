typedef enum {
Integer, String, List, Nil
} nodeType;

nodeType* createNode(nodeType type, void* data, nodeType* nextElem);
void freeNodes(nodeType* node);
void printList(nodeType type, void* data, bool isRoot);
