#ifndef SNAKE_H
#define SNAKE_H
#include"LinkedList.h"
#include"Node.h"



class Snake {
  public:
    char direction;
    LinkedList<Node*> list;


  public:

    Snake();
    void move(char dir);
    void add(Node*);
    void eatFruit(Node*);
    int status(Node*);
    void print();
    int peek(Node* n, char dir);
    bool HitSelfSideCheckOk( char dir);

   

};


#endif