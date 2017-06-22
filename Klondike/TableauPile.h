#pragma once
#include "BasePile.h"

struct FoundationPile;
struct TableauPile final : public BasePile {
	auto visibleCards() const { return data(); }
	auto hiddenCount() const { return m_hidden.size(); }

	bool sendTopToFoundation(FoundationPile&);
	bool sendCardsToTableau(TableauPile&, size_t count = 1);

private:
	Pile m_hidden;

	friend struct Game;
	TableauPile(Pile&& hidden, Pile&& visible, const Game* game)
		: BasePile(std::move(visible), game)
		, m_hidden(std::move(hidden))
	{}

	bool AddCards(PileIt begin, PileIt end) final override;
};
