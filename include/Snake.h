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
  void clear();
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

  // the crosscheck is to prevent snake end up with dead end, in other word, snake will hit itself.
  // after having a fruit. However, fruit node will get passed
  // into the function, because DFS search start with fruit find snake head
  // The check is not checking hit wall situation
  bool leftCrossCheckOk(Node *n);

  bool rightCrossCheckOk(Node *n);

  bool upCrossCheckOk(Node *n);

  bool downCrossCheckOk(Node *n);
};

#endif