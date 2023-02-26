#include "Node.h"
Node::Node()
{
    this->row = 0;
    this->col = 0;
    this->red = 200;
    this->green = 0;
    this->blue = 0;
}

Node::Node(int r, int c)
{
    this->row = r;
    this->col = c;
    this->red = 0;
    this->green = 15;
    this->blue = 0;
}

Node::Node(int r, int c, int red, int green, int blue)
{
    this->row = r;
    this->col = c;
    this->red = red;
    this->green = green;
    this->blue = blue;
}

bool Node::isSamePosition(Node *n)
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
    this->red = random(15) + 1;
    this->green = random(15) + 1;
    this->blue = random(15) + 1;

    // only allow two color involved
    // random again from 0 - 2, 0 set red to zero, 1 set green to zero, 2 set blue to zero
    bool showRed = random(0, 2);
    bool showGreen = random(0, 2);
    bool showBlue = random(0, 2);
    if (!showRed)
        this->red = 0;
    if (!showGreen)
        this->green = 0;
    if (!showBlue)
        this->blue = 0;

    if (!showRed and !showGreen and !showBlue)
    {
        int temp = random(0, 3);
        if (temp == 0)
            this->red = random(15) + 1;
        else if (temp == 1)
            this->green = random(15) + 1;
        else
            this->blue = random(15) + 1;
    }
}

void Node::refresh(LinkedList<Node *> *list)
{

    int size = list->size();

    bool canRefresh = true;

    while (true)
    {
        canRefresh = true;
        this->reposition();

        for (int i = 0; i < size; i++)
        {
            Node *n = list->get(i);
            if (this->row == n->row && this->col == n->col)
            {
                canRefresh = false;
                break;
            }
        }

        // if (this->row == list->get(0)->row || this->row == list->get(0)->col)
        //     continue;

        if (!canRefresh)
            continue;
        ;

        if (canRefresh)
        {
            this->recolor();
            return;
        }
    }
}

void Node::deepCopy(Node *n)
{
    this->deepCopyPosition(n);
    this->deepCopyColor(n);
}

void Node::deepCopyPosition(Node *n)
{
    this->row = n->row;
    this->col = n->col;
}

void Node::deepCopyColor(Node *n)
{

    this->red = n->red;
    this->green = n->green;
    this->blue = n->blue;
}

void Node::print()
{
    Serial.print("<");
    Serial.print(this->row);

    Serial.print("  ");
    Serial.print(this->col);

    Serial.println(">");
}