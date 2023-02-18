#include "PathSearch.h"
#include "Constant.h"
#include "Utils.h"

PathSearch::PathSearch(
    Node *fruit,                    // pass in when call get path
    LinkedList<Node *> *snakeList,  // pass in when call get path
    BitMapStorage *ledMatrixBitMap, // pass in at init
    String path,
    String result)
{
    this->fruit = fruit;
    this->snakeList = snakeList;
    this->ledMatrixBitMap = ledMatrixBitMap;
    this->path = path;
    this->result = result;
    this->previousBitMapSum = 0;
    this->findPath = false;

}

String PathSearch::getPath()
{

    // -------------- prepare for DFS---------------------------/
    this->clear();

    // doing the same to the snake
    for (int i = 2; i < this->snakeList->size(); i++)
    {
        Node *n = this->snakeList->get(i);
        int snakeNum = calcLedNumberFromNode(n);
        this->ledMatrixBitMap->setNumberStatus(snakeNum, true);
    }

    dfs(fruit->row, fruit->col);

    return this->result;
}

void PathSearch::dfs(int r, int c)
{




 


    if (this->findPath)
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
        for (unsigned int i = 0; i < path.length(); i++)
        {
            result += String(path.charAt(i));
        }
        findPath = true;
        return;
    }

    if (this->hitSnakeBody(r, c))
        return;

    this->ledMatrixBitMap->setNumberStatus(fruitLedNum, true);


    unsigned int curSum  = this->ledMatrixBitMap->getSum();

    if(curSum == previousBitMapSum) {
        findPath = true; 
        // if two sum are the same then no path is exist, will return "";
        return;
    }else{
        previousBitMapSum = curSum;
    }



    if (r < this->snakeList->get(0)->row)
    { // check up first
        // check up
        (this->path) += String(UP);
        dfs(r + 1, c);
        this->path = (this->path.substring(0, this->path.length() - 1));

        (this->path) += String(LEFT);
        dfs(r, c - 1);
        this->path = (this->path.substring(0, this->path.length() - 1));

        (this->path) += String(RIGHT);
        dfs(r, c + 1);
        this->path = (this->path.substring(0, this->path.length() - 1));

        (this->path) += String(DOWN);
        dfs(r - 1, c);
        this->path = (this->path.substring(0, this->path.length() - 1));
    }
    else if (r > this->snakeList->get(0)->row)
    {
        (this->path) += String(DOWN);
        dfs(r - 1, c);
        this->path = (this->path.substring(0, this->path.length() - 1));

        (this->path) += String(LEFT);
        dfs(r, c - 1);
        this->path = (this->path.substring(0, this->path.length() - 1));

        (this->path) += String(RIGHT);
        dfs(r, c + 1);
        this->path = (this->path.substring(0, this->path.length() - 1));

        (this->path) += String(UP);
        dfs(r + 1, c);
        this->path = (this->path.substring(0, this->path.length() - 1));
    }
    else if (c < this->snakeList->get(0)->col)
    {
        (this->path) += String(RIGHT);
        dfs(r, c + 1);
        this->path = (this->path.substring(0, this->path.length() - 1));

        (this->path) += String(DOWN);
        dfs(r - 1, c);
        this->path = (this->path.substring(0, this->path.length() - 1));

        (this->path) += String(UP);
        dfs(r + 1, c);
        this->path = (this->path.substring(0, this->path.length() - 1));

        (this->path) += String(LEFT);
        dfs(r, c - 1);
        this->path = (this->path.substring(0, this->path.length() - 1));
    }
    else
    {

        (this->path) += String(LEFT);
        dfs(r, c - 1);
        this->path = (this->path.substring(0, this->path.length() - 1));

        (this->path) += String(DOWN);
        dfs(r - 1, c);
        this->path = (this->path.substring(0, this->path.length() - 1));

        (this->path) += String(UP);
        dfs(r + 1, c);
        this->path = (this->path.substring(0, this->path.length() - 1));

        (this->path) += String(RIGHT);
        dfs(r, c + 1);
        this->path = (this->path.substring(0, this->path.length() - 1));
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

    // index start from one to skip the sanke head
    for (int i = 1; i < this->snakeList->size(); i++)
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

    this->path = "";

    this->findPath = false;

    this->result = "";

    this->previousBitMapSum = 0;
}
