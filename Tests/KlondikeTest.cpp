#include "Klondike.h"
#include "Game.h"
#include "Card.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <functional>

TEST(Klondike, OrderedState) {
	auto state = Game::CreateOrderedStockState();
	EXPECT_TRUE(Game::isValid(state));
}

TEST(Klondike, RandomizedState) {
	auto state = Game::CreateRandomizedState();
	EXPECT_TRUE(Game::isValid(state));
}

TEST(Klondike, drawCard) {
	Game::State state = Game::CreateOrderedStockState();
	std::swap(state.stock, state.discard);
	state.stock.push_back(state.discard.back()); state.discard.pop_back();
	auto game = Game::FromState(Game::State{ state }).value();

	EXPECT_FALSE(game.isStockEmpty());

	auto success = game.drawCard();
	EXPECT_TRUE(success);

	EXPECT_TRUE(game.isStockEmpty());
	using namespace testing;
	EXPECT_TRUE(game.topCard(Game::discard).has_value());
	EXPECT_THAT(game.topCard(Game::discard).value(), Eq(state.stock.back()));

	success = game.drawCard();
	EXPECT_FALSE(success);
}

TEST(Klondike, turnDiscardPileToStock) {
	Game::State state = Game::CreateOrderedStockState();
	std::swap(state.stock, state.discard); // move all cards to discard
	auto game = Game::FromState(Game::State{ state }).value();

	EXPECT_TRUE(game.isStockEmpty());

	auto success = game.turnDiscardPileToStock();
	EXPECT_TRUE(success);

	EXPECT_FALSE(game.isStockEmpty());
	EXPECT_FALSE(game.topCard(Game::discard).has_value());

	// check that first card drawn is first discarded card
	success = game.drawCard();
	EXPECT_TRUE(success);
	EXPECT_TRUE(game.topCard(Game::discard).has_value());
	using namespace testing;
	EXPECT_THAT(game.topCard(Game::discard).value(), Eq(state.discard.front()));
}

TEST(Klondike, moveDiscardToTableau) {
	Game::State state = Game::CreateOrderedStockState();
	std::swap(state.stock, state.discard); // move all cards to discard
	std::swap(*(state.discard.end() - 2), *(state.discard.end() - 15)); // get queen with other color
	auto game = Game::FromState(Game::State{ state }).value();

	auto success = game.moveCard(Game::discard, Game::tableau[0]);
	EXPECT_TRUE(success); // spades king

	success = game.moveCard(Game::discard, Game::tableau[1]);
	EXPECT_FALSE(success); // not a king

	success = game.moveCard(Game::discard, Game::tableau[0]);
	EXPECT_TRUE(success); // hearts queen

	success = game.moveCard(Game::discard, Game::tableau[0]);
	EXPECT_TRUE(success); // spades jack

	success = game.moveCard(Game::discard, Game::tableau[0]);
	EXPECT_FALSE(success); // spades ten (not matching)
}

TEST(Klondike, moveDiscardToFoundation) {
	Game::State state = Game::CreateOrderedStockState();
	std::reverse(state.stock.begin(), state.stock.end());
	std::swap(state.stock, state.discard); // move all cards to discard
	std::swap(*(state.discard.end() - 3), *(state.discard.end() - 16)); // get 3 with other color
	auto game = Game::FromState(Game::State{ state }).value();

	auto success = game.moveCard(Game::discard, Game::foundation[0]);
	EXPECT_TRUE(success); // spades ace

	success = game.moveCard(Game::discard, Game::foundation[1]);
	EXPECT_FALSE(success); // not an ace

	success = game.moveCard(Game::discard, Game::foundation[0]);
	EXPECT_TRUE(success); // spades 2

	success = game.moveCard(Game::discard, Game::foundation[0]);
	EXPECT_FALSE(success); // hearts 3 (not matching)
}

TEST(Klondike, moveTableauToFoundation) {
	Game::State state = Game::CreateOrderedStockState();
	std::reverse(state.stock.begin(), state.stock.end());
	state.tableaus[0].hidden.push_back(state.stock[13 + 10]); state.stock.erase(state.stock.begin() + 13 + 10); // hearts 3
	state.tableaus[0].visible.push_back(state.stock.back()); state.stock.pop_back(); // clubs ace
	state.tableaus[1].visible.push_back(state.stock.back()); state.stock.pop_back(); // clubs 2
	auto game = Game::FromState(Game::State{ state }).value();

	using namespace testing;
	EXPECT_THAT(game.hiddenCardCount(Game::tableau[0]), Eq(1));

	auto success = game.moveCard(Game::tableau[0], Game::foundation[0]);
	EXPECT_TRUE(success); // clubs ace

	// revealed the hidden card
	EXPECT_THAT(game.hiddenCardCount(Game::tableau[0]), Eq(0));
	EXPECT_FALSE(game.visibleCards(Game::tableau[0]).empty());

	success = game.moveCard(Game::tableau[0], Game::foundation[1]);
	EXPECT_FALSE(success); // not an ace

	success = game.moveCard(Game::tableau[1], Game::foundation[0]);
	EXPECT_TRUE(success); // clubs 2

	success = game.moveCard(Game::tableau[0], Game::foundation[0]);
	EXPECT_FALSE(success); // hearts 3 (not matching)
}

TEST(Klondike, moveTableauToTableau) {
	Game::State state = Game::CreateOrderedStockState();
	std::reverse(state.stock.begin(), state.stock.end());
	state.tableaus[0].visible.push_back(state.stock[13 + 11]); state.stock.erase(state.stock.begin() + 13 + 11); // hearts 2
	state.tableaus[0].visible.push_back(state.stock.back()); state.stock.pop_back(); // clubs ace
	state.tableaus[1].visible.push_back(state.stock.back()); state.stock.pop_back(); // clubs 2
	state.tableaus[2].visible.push_back(state.stock.back()); state.stock.pop_back(); // clubs 3
	auto game = Game::FromState(Game::State{ state }).value();

	auto success = game.moveCards(Game::tableau[0], Game::tableau[2], 2);
	EXPECT_TRUE(success); // clubs 3 - hearts 2 - clubs ace
	EXPECT_TRUE(game.visibleCards(Game::tableau[0]).empty());
	using namespace testing;
	EXPECT_THAT(game.visibleCards(Game::tableau[2]).size(), Eq(3));
}

TEST(Klondike, moveFoundationToFoundation) {
	Game::State state = Game::CreateOrderedStockState();
	std::reverse(state.stock.begin(), state.stock.end());
	state.foundations[0].push_back(state.stock.back()); state.stock.pop_back();
	auto game = Game::FromState(Game::State{ state }).value();

	auto moved = game.moveCard(Game::foundation[0], Game::foundation[1]);
	EXPECT_TRUE(moved);

	EXPECT_FALSE(game.topCard(Game::foundation[0]).has_value());
	EXPECT_TRUE(game.topCard(Game::foundation[1]).has_value());
}

TEST(Klondike, moveEmptyFoundationToFoundation) {
	Game::State state = Game::CreateOrderedStockState();
	auto game = Game::FromState(Game::State{ state }).value();

	auto moved = game.moveCard(Game::foundation[0], Game::foundation[1]);
	EXPECT_FALSE(moved);

	EXPECT_FALSE(game.topCard(Game::foundation[0]).has_value());
	EXPECT_FALSE(game.topCard(Game::foundation[1]).has_value());
}
