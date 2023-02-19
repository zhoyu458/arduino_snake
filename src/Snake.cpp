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

    this->list->clear();
    delete this->list;
    this->list = NULL;

    delete this->previousTail;
    this->previousTail = NULL;
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

int Snake::status(Node *fruit)
{

    if(this->hasHitWall()) return HIT_WALL;

    if(this->hasHitSelf()) return HIT_SELF;

    Node* head = this->list->get(0);
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

    Node *head = this->list->get(0);

    if (this->direction == UP)
    { // exclude DOWN
        if (head->row < fruit->row and this->canGoUp())
            this->move(UP);
        else
        {
            if (canGoLeft())
                this->move(LEFT);
            else
                this->move(RIGHT);
        }
    }
    else if (this->direction == DOWN)
    { // exclude UP
        if (head->row > fruit->row and this->canGoDown())
            this->move(DOWN);
        else
        {
            if (canGoLeft())
                this->move(LEFT);
            else
                this->move(RIGHT);
        }
    }
    else if (this->direction == LEFT)
    { // exclude RIGHT
        if (head->col > fruit->col and this->canGoLeft())
            this->move(LEFT);
        else
        {
            if (canGoUp())
                this->move(UP);
            else
                this->move(DOWN);
        }
    }
    else if (this->direction == RIGHT)
    { // exclude LEFT

        if (head->col < fruit->col and this->canGoRight())
            this->move(RIGHT);
        else
        {
            if (canGoUp())
                this->move(UP);
            else
                this->move(DOWN);
        }
    }
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
    for (int i = 4; i < this->list->size(); i++)
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

    for (int i = 1; i < this->list->size() - 1; i++)
    {
        Node *n = this->list->get(i);

        if (head->row == n->row && head->col == n->col)
        {
            return true;
        }
    }

    return false;
}


