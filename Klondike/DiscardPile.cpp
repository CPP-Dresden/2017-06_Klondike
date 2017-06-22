#include "DiscardPile.h"
#include "StockPile.h"

bool DiscardPile::turnToStock(StockPile & stock)
{
	if (stock.game() != game()) return false;
	if (data().empty()) return false;

	bool accepted = stock.turnCards(data().begin(), data().end());
	if (!accepted) return false;
	
	data().clear();
	return true;
}

bool DiscardPile::sendTopToPile(BasePile & pile)
{
	if (pile.game() != game()) return false;
	if (data().empty()) return false;

	bool accepted = pile.AddCards(data().end() - 1, data().end());
	if (!accepted) return false;

	data().pop_back();
	return true;
}

bool DiscardPile::AddCards(PileIt begin, PileIt end)
{
	if (std::distance(begin, end) != 1) return false;
	data().push_back(*begin);
	return true;
}
