#pragma once

#include "BasePile.h"

struct DiscardPile;
struct StockPile final : private BasePile {
	bool isEmpty() const { return data().empty(); }
	using BasePile::game;

	bool drawToDiscard(DiscardPile& discard);
	bool turnCards(PileIt begin, PileIt end);

private:
	friend struct Game;
	using BasePile::BasePile;

	bool AddCards(PileIt begin, PileIt end) final override;
};
