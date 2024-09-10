typedef enum {
Integer, String, List, Nil
} nodeType;

nodeType* createNode(nodeType type, void* data, nodeType* nextElem);
