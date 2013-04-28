#ifndef CARD_H_
#define CARD_H_

#include <string>

class Card {
private:
	char number;
	char symbol;
public:

	Card( char number, char symbol );
	Card( char* buffer);
	void serialize(char* buffer);
	virtual ~Card();

	char getNumber();
	char getSymbol();

	std::string toString();
};

#endif /* CARD_H_ */
