#include "Card.h"

Card::Card(int value, Symbol symbol, Status status)
	: m_value(value), m_symbol(symbol), m_status(status)
{

}

Card::~Card()
{
}

Card::Color Card::getColor() const
{
	switch (m_symbol)
	{
		case Symbol::HEARTS:
		case Symbol::DIAMONDS:
			return Color::RED;
		case Symbol::SPADES:
		case Symbol::CLUBS:
			return Color::BLACK;
	}
}

Card::Status Card::getStatus() const
{
	return m_status;
}

void Card::setStatus(Status status)
{
	m_status = status;
}

int Card::getValue() const
{
	return m_value;
}
