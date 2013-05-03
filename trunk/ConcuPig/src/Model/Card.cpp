#include "Card.h"

#include "../Helpers/Convert.h"

using namespace std;

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

string Card::toString(){
	return "N;S = " + Convert::toString((int)this->number) + ";" + Convert::toString((int) this->symbol);
}

Card::~Card() { }

