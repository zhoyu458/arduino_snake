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
    this->list.add(n);
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

    int size = this->list.size();
    Node *tail = this->list.get(size - 1);

    this->add(new Node(tail->row, tail->col, fruit->red, fruit->green, fruit->blue));
}

void Snake::print(){
    for(int i = 0; i < this->list.size(); i++){

        Node* n = this->list.get(i);
        Serial.print("< ");
        Serial.print(n->row);
        Serial.print("  ");
        Serial.print(n->col);
        Serial.print(" >    ");

    }

    Serial.println();
}