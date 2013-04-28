#include "Card.h"

Card::Card( char number, char symbol) : number(number), symbol(symbol) { }

Card::Card( char* buffer) : number(buffer[0]), symbol(buffer[1]) { }

char Card::getNumber()
{
	return number;
}

void Card::serialize(char* buffer){
	buffer[0] = this->number;
	buffer[1] = this->symbol;
}

char Card::getSymbol()
{
	return symbol;
}

Card::~Card() { }

