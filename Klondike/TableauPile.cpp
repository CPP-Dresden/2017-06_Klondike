#include "TableauPile.h"

#include "FoundationPile.h"

bool TableauPile::sendTopToFoundation(FoundationPile &foundation)
{
	if (foundation.game() != game()) return false;
	if (data().empty()) return false;

	auto accepted = foundation.AddCards(data().end() - 1, data().end());
	if (!accepted) return false;

	data().pop_back();
	return true;
}

bool TableauPile::sendCardsToTableau(TableauPile &other, size_t count)
{
	if (other.game() != game()) return false;
	if (data().size() < count) return false;

	auto accepted = other.AddCards(data().end() - count, data().end());
	if (!accepted) return false;

	data().erase(data().end() - count, data().end());
	if (data().empty() && !m_hidden.empty()) {
		data().push_back(m_hidden.back());
		m_hidden.pop_back();
	}
	return true;
}

bool TableauPile::AddCards(PileIt begin, PileIt end)
{
	if (begin == end) return false;

	if (data().empty()) {
		if (begin->rank() != CardRank::King) return false;
	}
	else {
		if (data().back().rank() == next(begin->rank())) return false;
		if (data().back().isBlack() == begin->isBlack()) return false;
	}

	data().insert(data().end(), begin, end);
	return true;
}
