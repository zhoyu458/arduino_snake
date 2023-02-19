#ifndef PATH_SEARCH_H
#define PATH_SEARCH_H

#include "Node.h"
#include "BitMapStorage.h"
#include <LinkedList.h>
#include <MemoryFree.h>

class PathSearch
{
public:
    Node *fruit;
    LinkedList<Node *> *snakeList;
    BitMapStorage *ledMatrixBitMap;
    String *path;
    String *result;
    bool *findPath;
    unsigned int *previousBitMapSum; // the varibale is used to check if dfs search fails by compare previousSum with current sum , if no chang, then dfs could not find an answer.

public:
    PathSearch();
    PathSearch(
        Node *fruit,
        LinkedList<Node *> *snakeList);

    ~PathSearch();
    String getPath();

    bool hitWall(int r, int c);

    bool hitSnakeBody(int r, int c);

    bool hitSnakeHead(int r, int c);

    void clear();

    void dfs(int r, int c);
};

#endif