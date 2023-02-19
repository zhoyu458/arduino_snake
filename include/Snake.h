#ifndef SNAKE_H
#define SNAKE_H
#include "LinkedList.h"
#include "Node.h"

class Snake
{
public:
  char direction;
  LinkedList<Node *> *list;
  Node *previousTail; // the variblbe keeps tracking the tail node before eating a fruit,for smoothing action of eating a fruit.

public:
  Snake();
  ~Snake();
  void move(char dir);
  void addToTail(Node *);
  void eatFruit(Node *);
  int status(Node *);
  void print();
  int peek(Node *n, char dir);
  bool HitSelfSideCheckOk(char dir);
  void guideMoveWithNoPathFound(Node *fruit);
  bool willHitWall(int r, int c);
  bool willHitSelf(int r, int c);

  bool hasHitWall();
  bool hasHitSelf();

  bool canGoUp();
  bool canGoDown();
  bool canGoLeft();
  bool canGoRight();
};

#endif