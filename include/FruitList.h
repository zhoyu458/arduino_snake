#ifndef FRUITLIST_H
#define FRUITLIST_H

#include <LinkedList.h>
#include "Node.h"
class FruitList
{
public:
    LinkedList<Node *> *list;
public:
    FruitList();
    ~FruitList();
    void clear();
    void addToTail(Node* n);
    void addToHead(Node* n);

};
#endif
