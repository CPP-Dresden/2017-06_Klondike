#include "FoundationPile.h"

bool FoundationPile::sendToFoundation(FoundationPile &other)
{
	if (other.game() != game()) return false;
	if (data().size() != 1) return false;

	auto accepted = other.AddCards(data().begin(), data().end());
	if (!accepted) return false;

	data().clear();
}

bool FoundationPile::AddCards(PileIt begin, PileIt end)
{
	if (std::distance(begin, end) != 1) return false;

	if (data().empty()) {
		if (begin->rank() != CardRank::Ace) return false;
	}
	else {
		if (next(data().back().rank()) != begin->rank()) return false;
		if (data().back().symbol() != begin->symbol()) return false;
	}

	data().push_back(*begin);
	return true;
}
