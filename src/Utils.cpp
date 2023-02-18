#include "Utils.h"

int calcLedNumberFromNode(Node *node)
{
  if (isEven((node->row)))
    return (node->row) * COLS + (node->col);
  return (node->row) * COLS + COLS - 1 - (node->col);
}

bool isEven(int n)
{
  return (!(n & 1));
}

int calcLedNumberFromNode(int r, int c){
    if (isEven((r)))
    return (r) * COLS + (c);
    return (r) * COLS + COLS - 1 - c;
}

int* calcCoordinateFromLedNum(int num)
{

  int r = num / COLS;

  int c = 0;

  if (isEven(num))
    c = num / 7;
  else
    c = (COLS - 1) - (num % 7);
  
  int A[2] = {r, c};

  return A;
}

String reverseString(String inStr){
    String res = "";
    for(int i = inStr.length() - 1; i >= 0; i--){
        res += inStr.charAt(i);
    }

    return res;
}