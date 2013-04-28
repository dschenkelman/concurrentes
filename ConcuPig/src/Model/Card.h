#ifndef CARD_H_
#define CARD_H_

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
};

#endif /* CARD_H_ */
