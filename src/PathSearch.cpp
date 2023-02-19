#include "PathSearch.h"
#include "Constant.h"
#include "Utils.h"

PathSearch::PathSearch(
    Node *fruit,
    LinkedList<Node *> *snakeList)
{
    this->fruit = fruit;
    this->snakeList = snakeList;

    // declare all pointers
    this->ledMatrixBitMap = new BitMapStorage(NUM_LEDS);
    this->path = new String();
    this->result = new String();
    this->findPath = new boolean();
    this->previousBitMapSum = new unsigned int();

    *this->path = "";
    *this->result = "";
    *this->findPath = false;
    *this->previousBitMapSum = 0;
}

PathSearch::~PathSearch()
{

    delete ledMatrixBitMap;
    delete path;
    delete result;
    delete findPath;
    delete previousBitMapSum;

    ledMatrixBitMap = NULL;
    path = NULL;
    result = NULL;
    findPath = NULL;
    previousBitMapSum = NULL;
}

String PathSearch::getPath()
{
    // -------------- prepare for DFS---------------------------/
    this->clear();

    // doing the same to the snake
    for (int i = 1; i < this->snakeList->size(); i++)
    {
        Node *n = this->snakeList->get(i);
        int snakeNum = calcLedNumberFromNode(n);
        this->ledMatrixBitMap->setNumberStatus(snakeNum, true);
    }

    dfs(fruit->row, fruit->col);

    return *(this->result);
}

void PathSearch::dfs(int r, int c)
{
    Serial.println(freeMemory());

    if (freeMemory() < MemoryLowerLimit)
    {
        // if it is going to run out of memory, then stop the dfs, and return
        *this->findPath = true;
        return;
    }

    if (*this->findPath == true)
        return;

    if (this->hitWall(r, c))
        return;

    // delay(100);
    // Serial.print("< ");
    // Serial.print(this->snakeList->get(0)->row);
    // Serial.print("  ");
    // Serial.print(this->snakeList->get(0)->col);

    // Serial.print(" >");
    // Serial.print("  < ");
    // Serial.print(r);
    // Serial.print("  ");
    // Serial.print(c);
    // Serial.println(" >");

    // this->ledMatrixBitMap->print();

    int fruitLedNum = calcLedNumberFromNode(r, c);

    if (this->ledMatrixBitMap->getNumberStatus(fruitLedNum) == true)
    {
        return;
    }

    if (hitSnakeHead(r, c))
    {
        for (unsigned int i = 0; i < path->length(); i++)
        {
            *(this->result) += String(path->charAt(i));
        }
        *this->findPath = true;
        return;
    }

    if (this->hitSnakeBody(r, c))
        return;

    this->ledMatrixBitMap->setNumberStatus(fruitLedNum, true);

    unsigned int curSum = this->ledMatrixBitMap->getSum();

    if (curSum == *this->previousBitMapSum)
    {
        *this->findPath = true;
        // if two sum are the same then no path is exist, will return "";
        return;
    }
    else
    {
        *this->previousBitMapSum = curSum;
    }

    if (r < this->snakeList->get(0)->row)
    { // check up first
        // check up
        *(this->path) += String(UP);
        dfs(r + 1, c);

        (*this->path) = this->path->substring(0, this->path->length() - 1);

        (*this->path) += String(LEFT);
        dfs(r, c - 1);
        (*this->path) = (this->path->substring(0, this->path->length() - 1));

        (*this->path) += String(RIGHT);
        dfs(r, c + 1);
        (*this->path) = (this->path->substring(0, this->path->length() - 1));

        (*this->path) += String(DOWN);
        dfs(r - 1, c);
        (*this->path) = (this->path->substring(0, this->path->length() - 1));
    }
    else if (r > this->snakeList->get(0)->row)
    {
        (*this->path) += String(DOWN);
        dfs(r - 1, c);
        (*this->path) = (this->path->substring(0, this->path->length() - 1));

        (*this->path) += String(LEFT);
        dfs(r, c - 1);
        (*this->path) = (this->path->substring(0, this->path->length() - 1));

        (*this->path) += String(RIGHT);
        dfs(r, c + 1);
        (*this->path) = (this->path->substring(0, this->path->length() - 1));

        (*this->path) += String(UP);
        dfs(r + 1, c);
        (*this->path) = (this->path->substring(0, this->path->length() - 1));
    }
    else if (c < this->snakeList->get(0)->col)
    {
        (*this->path) += String(RIGHT);
        dfs(r, c + 1);
        (*this->path) = (this->path->substring(0, this->path->length() - 1));

        (*this->path) += String(DOWN);
        dfs(r - 1, c);
        (*this->path) = (this->path->substring(0, this->path->length() - 1));

        (*this->path) += String(UP);
        dfs(r + 1, c);
        (*this->path) = (this->path->substring(0, this->path->length() - 1));

        (*this->path) += String(LEFT);
        dfs(r, c - 1);
        (*this->path) = (this->path->substring(0, this->path->length() - 1));
    }
    else
    {

        (*this->path) += String(LEFT);
        dfs(r, c - 1);
        (*this->path) = (this->path->substring(0, this->path->length() - 1));

        (*this->path) += String(DOWN);
        dfs(r - 1, c);
        (*this->path) = (this->path->substring(0, this->path->length() - 1));

        (*this->path) += String(UP);
        dfs(r + 1, c);
        (*this->path) = (this->path->substring(0, this->path->length() - 1));

        (*this->path) += String(RIGHT);
        dfs(r, c + 1);
        (*this->path) = (this->path->substring(0, this->path->length() - 1));
    }
}

bool PathSearch::hitWall(int r, int c)
{
    if (r < 0 || r >= ROWS)
    {
        return true;
    }
    if (c < 0 || c >= COLS)
    {
        return true;
    }
    return false;
}

bool PathSearch::hitSnakeBody(int r, int c)
{
    if (this->snakeList->size() < 4)
        return false;
    // head will never hit 2nd, 3rd and 4th part of its body
    for (int i = 4; i < this->snakeList->size(); i++)
    {
        Node *n = this->snakeList->get(i);
        if (n->row == r && n->col == c)
            return true;
    }

    return false;
}

bool PathSearch::hitSnakeHead(int r, int c)
{
    return (r == this->snakeList->get(0)->row && c == this->snakeList->get(0)->col);
}

void PathSearch::clear()
{

    this->ledMatrixBitMap->reset();

    *this->path = "";

    *this->findPath = false;

    *this->result = "";

    *this->previousBitMapSum = 0;
}
