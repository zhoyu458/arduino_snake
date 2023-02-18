#include "Snake.h"
#include "Constant.h"

Snake::Snake()
{
    this->direction = UP;
    this->list = LinkedList<Node *>();
}

void Snake::move(char dir)
{

    this->direction = dir;

    int size = this->list.size();
    for (int i = size - 2; i >= 0; i--)
    {
        Node *current = this->list.get(i);
        Node *next = this->list.get(i + 1);

        next->row = current->row;
        next->col = current->col;
    }
    Node *head = this->list.get(0);

    if (dir == UP)
        head->row++;
    else if (dir == DOWN)
        head->row--;
    else if (dir == LEFT)
        head->col--;
    else if (dir == RIGHT)
        head->col++;
}

void Snake::add(Node *n)
{
    this->list.add(this->list.size(), n);
}

int Snake::status(Node *fruit)
{
    Node *head = this->list.get(0);
    if (head->row < 0 || head->row > ROWS)
    {
        return HIT_WALL;
    }
    if (head->col < 0 || head->col > COLS)
    {

        return HIT_WALL;
    }
    // check if the snake hit itself
    int size = this->list.size();
    for (int i = 1; i < size - 1; i++)
    {
        Node *n = this->list.get(i);
        if (head->row == n->row && head->col == n->col)
        {
            return HIT_SELF;
        }
    }

    if (head->row == fruit->row && head->col == fruit->col)
        return HIT_FRUIT;
    return HIT_NOTHING;
}

void Snake::eatFruit(Node *fruit)
{

    if (this->list.size() < SNAKE_MAX_SIZE)
    {
        int size = this->list.size();
        Node *tail = this->list.get(size - 1);
        this->add(new Node(tail->row, tail->col, fruit->red, fruit->green, fruit->blue));
    }
    else
    {
        Node *head = this->list.get(0);
        head->red = fruit->red;
        head->green = fruit->green;
        head->blue = fruit->blue;
    }
}

void Snake::print()
{
    for (int i = 0; i < this->list.size(); i++)
    {

        Node *n = this->list.get(i);
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
    int size = this->list.size();
    for (int i = 1; i < size - 1; i++)
    {
        Node *n = this->list.get(i);
        if (r == n->row && c == n->col)
        {
            return HIT_SELF;
        }
    }

    return HIT_NOTHING;
}

bool Snake::HitSelfSideCheckOk(char dir)
{

    Node *head = this->list.get(0);
    if (dir == UP)
    {
        // if column is the same, and node row is less then snake body reutrn true
        for (int i = 1; i < this->list.size(); i++)
        {
            Node *n = this->list.get(i);
            if (n->col == head->col && head->row < n->row)
                return false;
        }
    }
    else if (dir == DOWN)
    {
        for (int i = 1; i < this->list.size(); i++)
        {
            Node *n = this->list.get(i);
            if (n->col == head->col && head->row > n->row)
                return false;
        }
    }
     else if (dir == LEFT)
    {
        for (int i = 1; i < this->list.size(); i++)
        {
            Node *n = this->list.get(i);
            if (n->row == head->row && head->col > n->col)
                return false;
        }
    }
     else if (dir == RIGHT)
    {
        for (int i = 1; i < this->list.size(); i++)
        {
            Node *n = this->list.get(i);
            if (n->row == head->row && head->row < n->row)
                return false;
        }
    }

    return true;
}



