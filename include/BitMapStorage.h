#ifndef BITMAPSTORAGE_H
#define BITMAPSTORAGE_H

#include <Arduino.h>
#include "Constant.h"
#define BYTE_SIZE 8
class BitMapStorage
{

public:
    unsigned int totalBits;

    byte *bitMapArray;

    unsigned int maxNumber;

public:
    BitMapStorage(unsigned int totalBits);
    ~BitMapStorage();

    bool getNumberStatus(unsigned int num);

    void setNumberStatus(unsigned int num, bool newStatus);

    void printBitMapbyNumber(unsigned int num);

    void printBitMap();

    void reset();

    void print();

    unsigned int getSum();
};

#endif