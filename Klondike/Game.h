#pragma once

#include "DiscardPile.h"
#include "StockPile.h"
#include "TableauPile.h"
#include "FoundationPile.h"

#include <array>

struct Game
{
	struct StartPiles
	{
		Pile discardPile;
		Pile stock;
		std::array<Pile, 7> tableauHiddenPiles;
		std::array<Pile, 7> tableauVisiblePiles;
		std::array<Pile, 4> foundationPiles;
	};

	Game(StartPiles&& piles);

public:
	DiscardPile discardPile;
	StockPile stockPile;
	std::array<TableauPile, 7> tableauPiles;
	std::array<FoundationPile, 4> foundationPiles;
};
