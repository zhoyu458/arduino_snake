#ifndef NODE_H
#define NODE_H
#include "Constant.h"
#include <Arduino.h>
#include <LinkedList.h>
class Node {
  public:
    int row;
    int col;
    int red;
    int green;
    int blue;


  public:
    Node();
    Node(int r, int c);
    Node(int r, int c, int red, int green, int blue);

  public:
    bool compareTo(Node*);

    void reposition();

    void recolor();

    void refresh(LinkedList<Node*>* list);

};


#endif
