#include"Utils.h"

int calcLedNumberFromNode(Node* node){
  if( isEven((node->row))) return (node->row) * 16 + (node->col);
  return (node->row) * 16 + 16 - 1 - (node->col);
}


bool isEven(int n)
{
    return (!(n & 1));
}