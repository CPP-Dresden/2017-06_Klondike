#pragma once
#include "Card.h"

#include <vector>

using Pile = std::vector<Card>;
using PileIt = Pile::const_iterator;

struct Game;
struct DiscardPile;
struct BasePile {
	BasePile(const BasePile&) = delete;
	BasePile& operator= (const BasePile&) = delete;

	BasePile(BasePile&&) = default;
	BasePile& operator= (BasePile&&) = default;

	const Game* game() const { return m_game; }

protected:
	BasePile(Pile&& data, const Game* game) : m_data(std::move(data)), m_game(game) {}

	friend struct DiscardPile;
	virtual bool AddCards(PileIt begin, PileIt end) = 0;
	const Pile& data() const { return m_data; }
	Pile& data() { return m_data; }

private:
	Pile m_data;
	const Game* m_game = nullptr;
};
