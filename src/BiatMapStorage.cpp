#include "BitMapStorage.h"

BitMapStorage::BitMapStorage(unsigned int size)
{
    this->size = size;

    this->bitMapArray = new byte[this->size];

    this->maxNumber = size * BYTE_SIZE - 1;

    this->reset();

 
}
BitMapStorage::~BitMapStorage(){
     delete this->bitMapArray;
     this->bitMapArray = NULL;

}
bool BitMapStorage::getNumberStatus(unsigned int num)
{

    while (num > this->maxNumber)
    {
        Serial.println("Bit map number is greater than the max value");
        delay(1000);
    }

    unsigned int index = num / BYTE_SIZE;

    byte numberOnTheIndex = this->bitMapArray[index];

    byte bitIndex = num % BYTE_SIZE;

    byte result = (1 << (7 - bitIndex)) & numberOnTheIndex;

    return bool(result);
}

void BitMapStorage::setNumberStatus(unsigned int num, bool newStatus)
{
    unsigned int index = num / BYTE_SIZE;

    byte bitIndex = num % BYTE_SIZE;

    if (newStatus == true)
    {
        this->bitMapArray[index] |= (1 << (BYTE_SIZE - 1 - bitIndex));
    }
    else
    {
        this->bitMapArray[index] &= ~(1 << (BYTE_SIZE - 1 - bitIndex));
    }
}

void BitMapStorage::printBitMapbyNumber(unsigned int num)

{

    unsigned int index = num / BYTE_SIZE;
    byte numberOnTheIndex = this->bitMapArray[index];

    for (int i = BYTE_SIZE - 1; i >= 0; i--)
    {
        byte mask = (1 << i);

        if ((numberOnTheIndex & mask) != 0)
            Serial.print(1);
        else
            Serial.print(0);
    }

    Serial.println();
}

void BitMapStorage::reset(){
       for (unsigned int i = 0; i < this->size; i++)
    {
        *(this->bitMapArray+i) = 0;
    }
}

void BitMapStorage::print(){


    Serial.println("--------------------------------------------------------------");

    for(unsigned int i = 0; i < this->size; i++){
        Serial.print(this->bitMapArray[i]);
        Serial.print("  ");
    }
    Serial.println();
    Serial.println("--------------------------------------------------------------");

}

 unsigned int BitMapStorage::getSum(){
    unsigned int sum = 0;
    for(unsigned int i = 0; i < this->size; i++){
        sum += this->bitMapArray[i];
    }

    return sum;
 }