#include "StockPile.h"

#include "DiscardPile.h"

bool StockPile::drawToDiscard(DiscardPile & discard)
{
	if (discard.game() != game()) return false;
	if (data().empty()) return false;

	auto accepted = discard.AddCards(data().end() - 1, data().end());
	if (!accepted) return false;

	data().pop_back();
	return true;
}

bool StockPile::turnCards(PileIt begin, PileIt end)
{
	if (!data().empty()) return false;

	data().insert(data().end(), begin, end);
	return true;
}

bool StockPile::AddCards(PileIt begin, PileIt end)
{
	return false;
}
