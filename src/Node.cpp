#include "Node.h"
Node::Node()
{
    this->row = 0;
    this->col = 0;
    this->red = 0;
    this->green = 1;
    this->blue = 0;
}

Node::Node(int r, int c)
{
    this->row = r;
    this->col = c;
    this->red = 0;
    this->green = 1;
    this->blue = 1;
}

Node::Node(int r, int c, int red, int green, int blue)
{
    this->row = r;
    this->col = c;
    this->red = red;
    this->green = green;
    this->blue = blue;
}

bool Node::compareTo(Node *n)
{
    return ((this->row == n->row) && (this->col == n->col));
}

void Node::reposition()
{
    this->row = random(ROWS);
    this->col = random(COLS);
}

void Node::recolor()
{
    this->red = random(12) + 1;
    this->green = random(6)+ 1;
    this->blue = random(6) + 1;
}

void Node::refresh(LinkedList<Node *> *list)
{

    int size = list->size();

    bool canRefresh = true;

    while (true)
    {
        canRefresh = true;
        this->reposition();

        // Serial.print(this->row);
        // Serial.print("  ");
        // Serial.println(this->col);


        for (int i = 0; i < size; i++)
        {
            Node *n = list->get(i);
            if (this->row == n->row && this->col == n->col)
            {
                canRefresh = false;
                break;
            }
        }

        if(this->row == list->get(0)->row || this->row == list->get(0)->col ) continue;

        if(!canRefresh) continue;;

        if (canRefresh)
        {
            this->recolor();
            return;
        }
    }
}
