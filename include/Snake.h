#ifndef SNAKE_H
#define SNAKE_H
#include "LinkedList.h"
#include "Node.h"
#include <MemoryFree.h>
#include "BitMapStorage.h"
#include "Constant.h"
#include "Utils.h"
class Snake
{

public:
  static int headRedValue;
  static int headGreenValue;
  static int headBlueValue;

public:
  char direction;
  LinkedList<Node *> *list;
  Node *previousTail; // the variblbe keeps tracking the tail node before eating a fruit,for smoothing action of eating a fruit.

public:
  Snake();
  ~Snake();

  Node *getHead();
  Node *getTail();

  Snake *deepCopySnake();

  void move(char dir);

  // the reversemove function reverse the previous movement
  void reverseMove();
  void addToTail(Node *);
  void addToHead(Node *);

  void followSnakeTail();

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

  String pathParser(String inStr);
  String getHeadToTailPath(Node *fruit);
  String getHeadToFruitPath(Node *fruit);
  String planPath(Node *fruit, bool *interrupt);

  void virtualRun(Snake *s, String *path);
  void dfs(Node *startNode, Node *endNode, BitMapStorage *bs, String *path, String *result, bool *findPath, unsigned int *previousBitSum);
  String getHeadToTargetPath(Node *target);

  void checkPlanPath(Node *fruit, bool *interruptPath);

  String wandering(Node *fruit, bool *interruptPath);
  void wanderingDFS(Node *headCopy, BitMapStorage *bs, String *path, String *result, bool *findPath);
};

#endif