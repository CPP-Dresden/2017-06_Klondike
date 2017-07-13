#include "Game.h"

#include "Meta/SkipFirst.h"

#include <random>

namespace {
	bool canAddCardToTableau(const Card& card, const Pile& toPile) {
		if (toPile.empty()) {
			return card.rank() == CardRank::King;
		}
		auto lastCard = toPile.back();
		if (card.isBlack() == lastCard.isBlack()) return false; // same color
		if (next(card.rank()) != lastCard.rank()) return false; // wrong rank
		return true;
	}

	bool canAddCardToFoundation(const Card& card, const Pile& toPile) {
		if (toPile.empty()) {
			return card.rank() == CardRank::Ace;
		}
		auto lastCard = toPile.back();
		if (card.symbol() != lastCard.symbol()) return false; // not same symbol
		if (next(lastCard.rank()) != card.rank()) return false; // wrong rank
		return true;
	}
}

bool Game::isValid(const State & state) {
	// each card exists once
	std::array<bool, Card::Total> cardSeen;
	cardSeen.fill(false);
	for (const auto* pile : {
		&state.discard, &state.stock,
		&state.tableaus[0].hidden, &state.tableaus[1].hidden, &state.tableaus[2].hidden, &state.tableaus[3].hidden, &state.tableaus[4].hidden, &state.tableaus[5].hidden, &state.tableaus[6].hidden,
		&state.tableaus[0].visible, &state.tableaus[1].visible, &state.tableaus[2].visible, &state.tableaus[3].visible, &state.tableaus[4].visible, &state.tableaus[5].visible, &state.tableaus[6].visible,
		&state.foundations[0], &state.foundations[1], &state.foundations[2], &state.foundations[3],
	}) {
		for (const auto& card : *pile) {
			auto id = card.id();
			if (id < 0 || id >= (int)cardSeen.size()) return false; // invalid index
			if (cardSeen[id]) return false; // duplicate card
			cardSeen[id] = true;
		}
	}
	if (std::any_of(cardSeen.begin(), cardSeen.end(), [](bool seen) { return !seen; })) return false; // missing card

																									  // tableau hidden empty or one visible
	for (const auto& tableau : state.tableaus) {
		if (!tableau.hidden.empty() && tableau.visible.empty()) return false;
	}

	// tableau visible cards conform
	for (const auto& tableau : state.tableaus) {
		if (tableau.visible.empty()) continue;
		auto last = tableau.visible.front();
		// we do not know if this is the last hidden card revealed !
		//if (tableau.hidden.empty() && last.rank != CardRank::King) return false;
		for (const Card& card : skipFirst(tableau.visible)) {
			if (card.isBlack() == last.isBlack()) return false; // alternate colors
			if (next(card.rank()) != last.rank()) return false; // increasing rank
			last = card;
		}
	}

	// foundation cards conform
	for (const auto& pile : state.foundations) {
		if (pile.empty()) continue;
		auto last = pile.front();
		if (last.rank() != CardRank::Ace) return false; // starts not with Ace
		for (const Card& card : skipFirst(pile)) {
			if (card.symbol() != last.symbol()) return false; // not same symbol
			if (next(last.rank()) != card.rank()) return false; // not increasing rank
			last = card;
		}
	}

	return true;
}

auto Game::CreateRandomized() -> Game
{
	return Game::FromState(CreateRandomizedState()).value();
}

auto Game::CreateRandomizedState() -> State
{
	auto state = CreateRandomizedStockState();
	auto& stock = state.stock;
	
	// fill tableaus
	int i = 0;
	for (auto& tableau : state.tableaus) {
		for (int j = 0; j < i; j++) {
			tableau.hidden.push_back(stock.back()); stock.pop_back();
		}
		tableau.visible.push_back(stock.back()); stock.pop_back(); // reveal one
		i++;
	}
	return state;
}

auto Game::CreateRandomizedStockState() -> State
{
	auto state = CreateOrderedStockState();
	auto& stock = state.stock;

	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(stock.begin(), stock.end(), g);
	return state;
}

auto Game::CreateOrderedStockState() -> State
{
	State state;
	auto& stock = state.stock;
	stock.resize(Card::Total);
	std::generate(stock.begin(), stock.end(), [id = 0]() mutable { return Card::FromId(id++); });
	return state;
}

bool Game::drawCard()
{
	if (m.stock.empty()) return false; // nothing to draw
	
	m.discard.push_back(m.stock.back());
	m.stock.pop_back();
	return true;
}

bool Game::turnDiscardPileToStock()
{
	if (!m.stock.empty()) return false; // stock still filled
	if (m.discard.empty()) return false; // no cards

	std::swap(m.discard, m.stock);
	std::reverse(m.stock.begin(), m.stock.end());
	return true;
}

bool Game::moveCard(Discard, Tableau t)
{
	auto& fromPile = m.discard;
	if (fromPile.empty()) return false; // nothing to take
	auto card = fromPile.back();
	auto& toPile = m.tableaus[static_cast<int>(t)].visible;
	if (!canAddCardToTableau(card, toPile)) return false; // invalid move

	toPile.push_back(card);
	fromPile.pop_back();
	return true;
}

bool Game::moveCard(Discard, Foundation f)
{
	auto& fromPile = m.discard;
	if (fromPile.empty()) return false; // nothing to take
	auto card = fromPile.back();
	auto& toPile = m.foundations[static_cast<int>(f)];
	if (!canAddCardToFoundation(card, toPile)) return false; // not allowed
	
	toPile.push_back(card);
	fromPile.pop_back();
	return true;
}

bool Game::moveCard(Tableau from, Foundation to)
{
	auto& fromTableau = m.tableaus[static_cast<int>(from)];
	auto& fromPile = fromTableau.visible;
	if (fromPile.empty()) return false; // nothing to take
	auto card = fromPile.back();
	auto& toPile = m.foundations[static_cast<int>(to)];
	if (!canAddCardToFoundation(card, toPile)) return false;

	toPile.push_back(card);
	fromPile.pop_back();
	if (fromPile.empty() && !fromTableau.hidden.empty()) { // reveal next hidden card
		fromPile.push_back(fromTableau.hidden.back());
		fromTableau.hidden.pop_back();
	}
	return true;
}

bool Game::moveCards(Tableau from, Tableau to, int n)
{
	if (n <= 0) return false; // move at least one card!
	auto& fromTableau = m.tableaus[static_cast<int>(from)];
	auto& fromPile = fromTableau.visible;
	if ((int)fromPile.size() < n) return false; // not enough to take
	auto card = *(fromPile.end() - n);
	auto& toPile = m.tableaus[static_cast<int>(to)].visible;
	if (!canAddCardToTableau(card, toPile)) return false;

	toPile.insert(toPile.end(), fromPile.end() - n, fromPile.end());
	fromPile.erase(fromPile.end() - n, fromPile.end());
	if (fromPile.empty() && !fromTableau.hidden.empty()) { // reveal next hidden card
		fromPile.push_back(fromTableau.hidden.back());
		fromTableau.hidden.pop_back();
	}
	return true;
}

bool Game::moveCard(Foundation from, Foundation to)
{
	auto& fromPile = m.foundations[static_cast<int>(from)];
	if (fromPile.empty()) return false; // nothing to take
	auto& toPile = m.foundations[static_cast<int>(to)];
	if (!toPile.empty()) return false; // cannot match color

	std::swap(fromPile, toPile);
	return true;
}
