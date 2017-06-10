#pragma once
class Card
{
public:
	enum class Symbol { HEARTS, DIAMONDS, SPADES, CLUBS };
	enum class Status { UP, DOWN };
	enum class Color { RED, BLACK };

	Card(int value, Symbol symbol, Status status);
	Card(const Card&) = default;
	~Card();

	Color getColor() const;
	int getValue() const;

	Status getStatus() const;
	void setStatus(Status status);

private:
	int m_value; //!< Value of the card (1:A,2,3,..,10,11:J,12:Q,13:K) 
	Symbol m_symbol;
	Status m_status;
};

