#include "Card.h"

Card::Card( char number, char symbol) : number(number), symbol(symbol) { }

char Card::getNumber()
{
	return number;
}

char Card::getSymbol()
{
	return symbol;
}

Card::~Card() { }

