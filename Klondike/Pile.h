#pragma once

#include <vector>

#include "Card.h"

class Pile
{
	Pile();
	virtual ~Pile();

	void addCard(Card c);

	std::vector<Card> m_cards;
};

