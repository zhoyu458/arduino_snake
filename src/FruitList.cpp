#include "FruitList.h"

FruitList::FruitList()
{
    this->list = new LinkedList<Node *>();
}

FruitList::~FruitList()
{
    clear();
}

void FruitList::clear()
{
    for (int i = 0; i < this->list->size(); i++)
    {
        Node *n = this->list->get(i);
        delete n;
        n = NULL;
    }
    this->list->clear();
}

void FruitList::addToTail(Node *n)
{
    this->list->add(this->list->size(), n);
}

void FruitList::addToHead(Node *n)
{
    this->list->add(n);
}
