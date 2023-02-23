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
  void addToHead(Node *);

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
  // the function is called before movement
  // Once the fruit has found a route, then the snake make a copy of itself, then the copy one will follow
  // the route to pick up the fruit and then run the checks below
  bool leftCrossWillHitSelf();

  bool rightCrossWillHitSelf();

  bool upCrossWillHitSelf();

  bool downCrossWillHitSelf();


// below functions are for snake detour route
  int getBodyCountOnUpperHalf();
  int getBodyCountOnLowerHalf();
  int getBodyCountOnLeftHalf();
  int getBodyCountOnRightHalf();

};

#endif