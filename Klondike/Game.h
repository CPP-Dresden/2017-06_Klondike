#pragma once

#include "Card.h"

#include <array>
#include <vector>
#include <optional>
#include <algorithm>
#include <numeric>

using Pile = std::vector<Card>;
struct TableauPile {
	Pile hidden;
	Pile visible;
};

struct Game
{
	enum { TableauPiles = 7, FoundationPiles = Card::SymbolCount };

	struct State {
		Pile discard;
		Pile stock;
		std::array<TableauPile, TableauPiles> tableaus;
		std::array<Pile, FoundationPiles> foundations;
	};
	static bool isValid(const State& state);

	enum class Discard {};
	static constexpr const Discard discard = static_cast<Discard>(0);

	enum class Tableau { One, Two, Three, Four, Five, Six, Seven };
	static constexpr const std::array<Tableau, TableauPiles> tableau = { Tableau::One, Tableau::Two, Tableau::Three, Tableau::Four, Tableau::Five, Tableau::Six, Tableau::Seven };

	enum class Foundation { One, Two, Three, Four };
	static constexpr const std::array<Foundation, FoundationPiles> foundation = { Foundation::One, Foundation::Two, Foundation::Three, Foundation::Four };

	static auto CreateRandomized() -> Game;

	static auto CreateRandomizedState() -> State; // valid game state with cards on tableaus
	static auto CreateRandomizedStockState() -> State; // cards in stock are shuffled randomly
	static auto CreateOrderedStockState() -> State; // valid game state with all cards in stock

	static auto FromState(State&& state) -> std::optional<Game> {
		if (!isValid(state)) return {};
		Game game;
		game.m = std::move(state);
		return game;
	}

	auto isValid() const { return isValid(m); }

	auto isWon() const {
		return Card::Total == std::accumulate(
			m.foundations.begin(), m.foundations.end(), 
			0, 
			[](size_t sum, const auto& pile) { return sum + pile.size(); });
	}

	auto topDiscardCard() const -> std::optional<Card> {
		if (m.discard.empty()) return {};
		return m.discard.back();
	}
	
	auto isStockEmpty() const { return m.stock.empty(); }

	auto topFoundationCard(Foundation f) const -> std::optional<Card> {
		const auto& pile = m.foundations[static_cast<int>(f)];
		if (pile.empty()) return {};
		return pile.back();
	}
	
	auto visibleCards(Tableau t) const { return m.tableaus[static_cast<int>(t)].visible; }	
	auto hiddenCardCount(Tableau t) const { return m.tableaus[static_cast<int>(t)].hidden.size(); }

	// moves (return true if successfully executed, false if invalid)
	bool drawCard();
	bool turnDiscardPileToStock();

	bool moveCard(Discard, Tableau t);
	bool moveCard(Discard, Foundation f);
	bool moveCard(Tableau from, Foundation to);
	bool moveCards(Tableau from, Tableau to, int n = 1);
	bool moveCard(Foundation from, Foundation to);

private:
	State m;
};
