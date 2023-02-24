#include "Snake.h"
#include "Constant.h"

Snake::Snake()
{
    this->list = new LinkedList<Node *>();
    this->direction = UP;
    this->previousTail = new Node();
}

Snake::~Snake()
{
    for (int i = 0; i < this->list->size(); i++)
    {
        Node *n = this->list->get(i);
        delete n;
        n = NULL;
    }
    this->list->clear();
    delete this->list;
    this->list = NULL;

    delete this->previousTail;
    this->previousTail = NULL;
}

void Snake::clear()
{
    for (int i = 0; i < this->list->size(); i++)
    {
        Node *n = this->list->get(i);
        delete n;
        n = NULL;
    }
    this->list->clear();
}

void Snake::move(char dir)
{
    // before move, keep a track of the tail node for eating fruit
    Node *tail = this->list->get(this->list->size() - 1);
    Node *head = this->list->get(0);

    previousTail->row = tail->row;
    previousTail->col = tail->col;
    previousTail->red = head->red;
    previousTail->green = head->green;
    previousTail->blue = head->blue;

    this->direction = dir;

    int size = this->list->size();
    for (int i = size - 2; i >= 0; i--)
    {
        Node *current = this->list->get(i);
        Node *next = this->list->get(i + 1);

        next->row = current->row;
        next->col = current->col;
    }

    if (dir == UP)
        head->row++;
    else if (dir == DOWN)
        head->row--;
    else if (dir == LEFT)
        head->col--;
    else if (dir == RIGHT)
        head->col++;
}

void Snake::addToTail(Node *n)
{
    this->list->add(this->list->size(), n);
}

void Snake::addToHead(Node *n)
{
    this->list->add(n);
}

int Snake::status(Node *fruit)
{

    if (this->hasHitWall())
        return HIT_WALL;

    if (this->hasHitSelf())
        return HIT_SELF;

    Node *head = this->list->get(0);
    if (head->row == fruit->row && head->col == fruit->col)
        return HIT_FRUIT;
    return HIT_NOTHING;
}

void Snake::eatFruit(Node *fruit)
{

    if (this->list->size() < SNAKE_MAX_SIZE)
    {
        // eat a fruit while the snake is not over its max length();
        //(1) snake head just update its color as the the fruit
        //(2) append the new Node to the tail use the "previousTail"

        Node *head = this->list->get(0);
        // udpate head color
        head->deepCopyColor(fruit);

        // add to new node to the tail
        Node *newNode = new Node();

        newNode->deepCopy(this->previousTail);
        this->addToTail(newNode);
    }
    else
    {
        Node *head = this->list->get(0);
        head->red = fruit->red;
        head->green = fruit->green;
        head->blue = fruit->blue;
    }
}

void Snake::print()
{
    for (int i = 0; i < this->list->size(); i++)
    {

        Node *n = this->list->get(i);
        Serial.print("< ");
        Serial.print(n->row);
        Serial.print("  ");
        Serial.print(n->col);
        Serial.print(" >    ");
    }

    Serial.println();
}

int Snake::peek(Node *n, char dir)
{
    int r = n->row;
    int c = n->col;

    if (dir == UP)
        r++;
    else if (dir == DOWN)
        r--;
    else if (dir == LEFT)
        c--;
    else if (dir == RIGHT)
        c++;

    if (r < 0 || r > ROWS)
    {
        return HIT_WALL;
    }
    if (c < 0 || c > COLS)
    {

        return HIT_WALL;
    }
    // check if the snake hit itself
    int size = this->list->size();
    for (int i = 1; i < size - 1; i++)
    {
        Node *n = this->list->get(i);
        if (r == n->row && c == n->col)
        {
            return HIT_SELF;
        }
    }

    return HIT_NOTHING;
}

bool Snake::HitSelfSideCheckOk(char dir)
{

    Node *head = this->list->get(0);
    if (dir == UP)
    {
        // if column is the same, and node row is less then snake body reutrn true
        for (int i = 1; i < this->list->size(); i++)
        {
            Node *n = this->list->get(i);
            if (n->col == head->col && head->row < n->row)
                return false;
        }
    }
    else if (dir == DOWN)
    {
        for (int i = 1; i < this->list->size(); i++)
        {
            Node *n = this->list->get(i);
            if (n->col == head->col && head->row > n->row)
                return false;
        }
    }
    else if (dir == LEFT)
    {
        for (int i = 1; i < this->list->size(); i++)
        {
            Node *n = this->list->get(i);
            if (n->row == head->row && head->col > n->col)
                return false;
        }
    }
    else if (dir == RIGHT)
    {
        for (int i = 1; i < this->list->size(); i++)
        {
            Node *n = this->list->get(i);
            if (n->row == head->row && head->row < n->row)
                return false;
        }
    }

    return true;
}

void Snake::guideMoveWithNoPathFound(Node *fruit)
{
    if (this->direction == UP)
    {
        if (this->canGoUp() and !this->upCrossWillHitSelf())
        {
            this->move(UP);
        }
        else if (this->canGoLeft() and !this->leftCrossWillHitSelf())
        {
            this->move(LEFT);
        }
        else
        {
            this->move(RIGHT);
        }
    }
    else if (this->direction == DOWN)
    {
        if (this->canGoDown() and !this->downCrossWillHitSelf())
        {
            this->move(DOWN);
        }
        else if (this->canGoLeft() and !this->leftCrossWillHitSelf())
        {
            this->move(LEFT);
        }
        else
        {
            this->move(RIGHT);
        }
    }
    else if (this->direction == LEFT)
    {
        if (this->canGoLeft() and !this->leftCrossWillHitSelf())
        {
            this->move(LEFT);
        }
        else if (this->canGoUp() and !this->upCrossWillHitSelf())
        {
            this->move(UP);
        }
        else
        {
            this->move(DOWN);
        }
    }
    else if (this->direction == RIGHT)
    {
        if (this->canGoRight() and !this->rightCrossWillHitSelf())
        {
            this->move(RIGHT);
        }
        else if (this->canGoUp() and !this->upCrossWillHitSelf())
        {
            this->move(UP);
        }
        else
        {
            this->move(DOWN);
        }
    }

    // if (this->direction == UP)
    // {
    //     if (this->canGoUp())
    //     {
    //         this->move(UP);
    //     }
    //     else if (this->canGoLeft())
    //     {
    //         this->move(LEFT);
    //     }
    //     else
    //     {
    //         this->move(RIGHT);
    //     }
    // }
    // else if (this->direction == DOWN)
    // {
    //     if (this->canGoDown())
    //     {
    //         this->move(DOWN);
    //     }
    //     else if (this->canGoLeft())
    //     {
    //         this->move(LEFT);
    //     }
    //     else
    //     {
    //         this->move(RIGHT);
    //     }
    // }
    // else if (this->direction == LEFT)
    // {
    //     if (this->canGoLeft())
    //     {
    //         this->move(LEFT);
    //     }
    //     else if (this->canGoUp())
    //     {
    //         this->move(UP);
    //     }
    //     else
    //     {
    //         this->move(DOWN);
    //     }
    // }
    // else if (this->direction == RIGHT)
    // {
    //     if (this->canGoRight())
    //     {
    //         this->move(RIGHT);
    //     }
    //     else if (this->canGoUp())
    //     {
    //         this->move(UP);
    //     }
    //     else
    //     {
    //         this->move(DOWN);
    //     }
    // }
}

// follow tail is applied when snake cannot found its tail or No path found approach to the fruit
void Snake::followSnakeTail()
{
    Snake *snakeCopy = this->deepCopySnake();
    // let the copy follow the its tail

    delete snakeCopy;
    snakeCopy = NULL;
}

bool Snake::willHitWall(int r, int c)
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
bool Snake::willHitSelf(int r, int c)
{

    if (this->list->size() < 4)
        return false;

    // head will never hit 2nd, 3rd and 4th part of its body
    for (int i = 1; i < this->list->size(); i++)
    {
        Node *n = this->list->get(i);
        if (n->row == r && n->col == c)
            return true;
    }

    return false;
}

bool Snake::canGoUp()
{
    int r = this->list->get(0)->row;
    int c = this->list->get(0)->col;
    return (!this->willHitWall(r + 1, c) && !this->willHitSelf(r + 1, c));
}
bool Snake::canGoDown()
{
    int r = this->list->get(0)->row;
    int c = this->list->get(0)->col;
    return (!this->willHitWall(r - 1, c) && !this->willHitSelf(r - 1, c));
}
bool Snake::canGoLeft()
{
    int r = this->list->get(0)->row;
    int c = this->list->get(0)->col;
    return (!this->willHitWall(r, c - 1) && !this->willHitSelf(r, c - 1));
}
bool Snake::canGoRight()

{
    int r = this->list->get(0)->row;
    int c = this->list->get(0)->col;
    return (!this->willHitWall(r, c + 1) && !this->willHitSelf(r, c + 1));
}

bool Snake::hasHitWall()
{
    Node *head = this->list->get(0);

    if (head->row < 0 || head->row > ROWS)
        return true;
    if (head->col < 0 || head->col > COLS)
        return true;

    return false;
}

bool Snake::hasHitSelf()
{

    Node *head = this->list->get(0);

    for (int i = 1; i < this->list->size(); i++)
    {
        Node *n = this->list->get(i);

        if (head->row == n->row && head->col == n->col)
        {
            return true;
        }
    }

    return false;
}

bool Snake::leftCrossWillHitSelf()
{
    Node *head = this->list->get(0);

    // left check is col--
    int r = head->row;

    int c = head->col;
    c--; // do not check the head itself

    while (c >= 0)
    {
        bool hitSelf = this->willHitSelf(r, c);
        if (hitSelf)
            return true;
        c--;
    }
    return false;
}

bool Snake::rightCrossWillHitSelf()
{

    Node *head = this->list->get(0);

    // right check is col++
    int r = head->row;
    int c = head->col;
    c++; // do not check the head itself
    while (c < COLS)
    {
        bool hitSelf = this->willHitSelf(r, c);
        if (hitSelf)
            return true;
        c++;
    }
    return false;
}

bool Snake::upCrossWillHitSelf()
{

    Node *head = this->list->get(0);

    // up check is row++
    int r = head->row;
    int c = head->col;
    r++; // do not check the head itself

    while (r < ROWS)
    {
        bool hitSelf = this->willHitSelf(r, c);
        if (hitSelf)
            return true;
        r++;
    }
    return false;
}

bool Snake::downCrossWillHitSelf()

{
    Node *head = this->list->get(0);
    // right check is row--
    int r = head->row;
    int c = head->col;
    r--; // do not check the head itself
    while (r >= 0)
    {
        bool hitSelf = this->willHitSelf(r, c);
        if (hitSelf)
            return true;
        r--;
    }
    return false;
}

int Snake::getBodyCountOnUpperHalf()
{
    int count = 0;
    int upperBoundary = ROWS - 1;
    int lowerBoundary = ROWS / 2;

    Serial.print("upperBoundary:");
    Serial.println(upperBoundary);
    Serial.print("lowerBoundary:");
    Serial.println(lowerBoundary);

    for (int i = 0; i < this->list->size(); i++)
    {
        Node *n = this->list->get(i);
        int r = n->row;
        int c = n->col;

        if (r >= lowerBoundary && r <= upperBoundary)
        {
            Serial.print("<");
            Serial.print(r);
            Serial.print("  ");
            Serial.print(c);
            Serial.println("> is included");
            count++;
        }
    }

    return count;
}
int Snake::getBodyCountOnLowerHalf()
{

    int count = 0;
    int upperBoundary = ROWS / 2;
    int lowerBoundary = 0;

    for (int i = 0; i < this->list->size(); i++)
    {
        Node *n = this->list->get(i);
        int r = n->row;
        if (r >= lowerBoundary && r < upperBoundary)
        {
            count++;
        }
    }

    return count;
}
int Snake::getBodyCountOnLeftHalf()
{

    int count = 0;
    int leftBoundary = 0;
    int rightBoundary = COLS / 2;

    for (int i = 0; i < this->list->size(); i++)
    {
        Node *n = this->list->get(i);
        int c = n->col;
        if (c >= leftBoundary && c <= rightBoundary)
        {
            count++;
        }
    }

    return count;
}
int Snake::getBodyCountOnRightHalf()

{

    int count = 0;
    int leftBoundary = COLS / 2;
    int rightBoundary = COLS - 1;

    for (int i = 0; i < this->list->size(); i++)
    {
        Node *n = this->list->get(i);
        int c = n->col;
        if (c > leftBoundary && c <= rightBoundary)
        {
            count++;
        }
    }

    return count;
}

Node *Snake::getHead()
{
    return this->list->get(0);
}
Node *Snake::getTail()
{
    return this->list->get(this->list->size() - 1);
}

String Snake::pathParser(String inStr)
{
    String result = "";
    for (int i = inStr.length() - 1; i >= 0; i--)
    {
        if (inStr.charAt(i) == UP)
        {
            result += DOWN;
        }
        else if (inStr.charAt(i) == DOWN)
        {
            result += UP;
        }
        else if (inStr.charAt(i) == LEFT)
        {
            result += RIGHT;
        }
        else if (inStr.charAt(i) == RIGHT)
        {
            result += LEFT;
        }
    }

    return result;
}
String Snake::getHeadToTailPath(Node *fruit)

{
    // snake size is less then 5, it never hit it slef
    if (this->list->size() < 4)
    {
        return "ok";
    }
    String result = "";
    String path = "";
    BitMapStorage bs = BitMapStorage(NUM_LEDS);
    Node headCopy = Node();
    Node tailCopy = Node();
    unsigned int previousBitSum = 0;

    headCopy.deepCopy(this->getHead());
    tailCopy.deepCopy(this->getTail());
    bool findPath = false;

    // tail to head, tail is the start and head is the end

    // set up the bit map, exclude snake head and tail
    for (int i = 1; i < this->list->size() - 1; i++)
    {
        Node *n = this->list->get(i);
        int ledNum = calcLedNumberFromNode(n);
        bs.setNumberStatus(ledNum, true);
    }

    int ledNum = calcLedNumberFromNode(fruit);
    bs.setNumberStatus(ledNum, true);

    dfs(&tailCopy, &headCopy, &bs, &path, &result, &findPath, &previousBitSum);
    bs.reset();

    result = this->pathParser(result);

    return result;
}
String Snake::getHeadToFruitPath(Node *fruit)
{
    // snake size is less then 5, it never hit it slef
    String result = "";
    String path = "";
    BitMapStorage bs = BitMapStorage(NUM_LEDS);
    Node headCopy = Node();
    Node fruitCopy = Node();
    unsigned int previousBitSum = 0;

    headCopy.deepCopy(this->getHead());
    fruitCopy.deepCopy(fruit);
    bool findPath = false;

    // tail to head, tail is the start and head is the end

    // set up the bit map, exclude snake head and tail
    for (int i = 1; i < this->list->size(); i++)
    {
        Node *n = this->list->get(i);
        int ledNum = calcLedNumberFromNode(n);
        bs.setNumberStatus(ledNum, true);
    }

    dfs(&fruitCopy, &headCopy, &bs, &path, &result, &findPath, &previousBitSum);

    result = this->pathParser(result);
    return result;
}
void Snake::dfs(Node *tailCopy, Node *headCopy, BitMapStorage *bs, String *path, String *result, bool *findPath, unsigned int *previousBitSum)

{

    if (freeMemory() < MemoryLowerLimit)
    {
        (*findPath) = true; // if run out of memory, then set findPath to ture to stop the search
        return;
    }
    // already found the path
    if (*findPath == true)
    {
        return;
    }

    if (this->willHitWall(tailCopy->row, tailCopy->col))
    {
        return;
    }

    // tail finds the head
    if (tailCopy->isSamePosition(headCopy))
    {

        *result = path->substring(0, path->length());
        *findPath = true;
    }

    int ledNum = calcLedNumberFromNode(tailCopy);
    // the point on the matrix has been searched
    if (bs->getNumberStatus(ledNum) == true)
    {
        return;
    }

    bs->setNumberStatus(ledNum, true);
    // consider row
    if (tailCopy->row < headCopy->row)
    {
        *path += UP;
        tailCopy->row++;
        this->dfs(tailCopy, headCopy, bs, path, result, findPath, previousBitSum);
        tailCopy->row--;
        *path = path->substring(0, path->length() - 1);

        *path += LEFT;
        tailCopy->col--;
        this->dfs(tailCopy, headCopy, bs, path, result, findPath, previousBitSum);
        tailCopy->col++;
        *path = path->substring(0, path->length() - 1);

        *path += RIGHT;
        tailCopy->col++;
        this->dfs(tailCopy, headCopy, bs, path, result, findPath, previousBitSum);
        tailCopy->col--;
        *path = path->substring(0, path->length() - 1);

        *path += DOWN;
        tailCopy->row--;
        this->dfs(tailCopy, headCopy, bs, path, result, findPath, previousBitSum);
        tailCopy->row++;
        *path = path->substring(0, path->length() - 1);
    }

    if (tailCopy->row > headCopy->row)
    {
        *path += DOWN;
        tailCopy->row--;
        this->dfs(tailCopy, headCopy, bs, path, result, findPath, previousBitSum);
        tailCopy->row++;
        *path = path->substring(0, path->length() - 1);

        *path += LEFT;
        tailCopy->col--;
        this->dfs(tailCopy, headCopy, bs, path, result, findPath, previousBitSum);
        tailCopy->col++;
        *path = path->substring(0, path->length() - 1);

        *path += RIGHT;
        tailCopy->col++;
        this->dfs(tailCopy, headCopy, bs, path, result, findPath, previousBitSum);
        tailCopy->col--;
        *path = path->substring(0, path->length() - 1);

        *path += UP;
        tailCopy->row++;
        this->dfs(tailCopy, headCopy, bs, path, result, findPath, previousBitSum);
        tailCopy->row--;
        *path = path->substring(0, path->length() - 1);
    }

    if (tailCopy->col < headCopy->col)
    {
        *path += RIGHT;
        tailCopy->col++;
        this->dfs(tailCopy, headCopy, bs, path, result, findPath, previousBitSum);
        tailCopy->col--;
        *path = path->substring(0, path->length() - 1);
        *path += DOWN;
        tailCopy->row--;
        this->dfs(tailCopy, headCopy, bs, path, result, findPath, previousBitSum);
        tailCopy->row++;
        *path = path->substring(0, path->length() - 1);

        *path += UP;
        tailCopy->row++;
        this->dfs(tailCopy, headCopy, bs, path, result, findPath, previousBitSum);
        tailCopy->row--;
        *path = path->substring(0, path->length() - 1);

        *path += LEFT;
        tailCopy->col--;
        this->dfs(tailCopy, headCopy, bs, path, result, findPath, previousBitSum);
        tailCopy->col++;
        *path = path->substring(0, path->length() - 1);
    }

    if (tailCopy->col > headCopy->col)
    {
        *path += LEFT;
        tailCopy->col--;
        this->dfs(tailCopy, headCopy, bs, path, result, findPath, previousBitSum);
        tailCopy->col++;
        *path = path->substring(0, path->length() - 1);

        *path += DOWN;
        tailCopy->row--;
        this->dfs(tailCopy, headCopy, bs, path, result, findPath, previousBitSum);
        tailCopy->row++;
        *path = path->substring(0, path->length() - 1);

        *path += UP;
        tailCopy->row++;
        this->dfs(tailCopy, headCopy, bs, path, result, findPath, previousBitSum);
        tailCopy->row--;
        *path = path->substring(0, path->length() - 1);

        *path += RIGHT;
        tailCopy->col++;
        this->dfs(tailCopy, headCopy, bs, path, result, findPath, previousBitSum);
        tailCopy->col--;
        *path = path->substring(0, path->length() - 1);
    }
}

Snake *Snake::deepCopySnake()
{

    Snake *snakeCopy = new Snake();
    for (int i = 0; i < this->list->size(); i++)
    {
        Node *snakeSection = this->list->get(i);
        Node *copy = new Node();
        copy->deepCopyPosition(snakeSection);
        snakeCopy->addToTail(copy);
    }

    return snakeCopy;
}