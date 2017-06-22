#include "Game.h"

Game::Game(StartPiles && piles)
	: discardPile(std::move(piles.discardPile), this)
	, stockPile(std::move(piles.stock), this)
	, tableauPiles({ 
		TableauPile(std::move(piles.tableauHiddenPiles[0]), std::move(piles.tableauVisiblePiles[0]), this),
		TableauPile(std::move(piles.tableauHiddenPiles[1]), std::move(piles.tableauVisiblePiles[1]), this),
		TableauPile(std::move(piles.tableauHiddenPiles[2]), std::move(piles.tableauVisiblePiles[2]), this),
		TableauPile(std::move(piles.tableauHiddenPiles[3]), std::move(piles.tableauVisiblePiles[3]), this),
		TableauPile(std::move(piles.tableauHiddenPiles[4]), std::move(piles.tableauVisiblePiles[4]), this),
		TableauPile(std::move(piles.tableauHiddenPiles[5]), std::move(piles.tableauVisiblePiles[5]), this),
		TableauPile(std::move(piles.tableauHiddenPiles[6]), std::move(piles.tableauVisiblePiles[6]), this),
})	
, foundationPiles({
	FoundationPile(std::move(piles.foundationPiles[0]), this),
	FoundationPile(std::move(piles.foundationPiles[1]), this),
	FoundationPile(std::move(piles.foundationPiles[2]), this),
	FoundationPile(std::move(piles.foundationPiles[3]), this),
})
{}
