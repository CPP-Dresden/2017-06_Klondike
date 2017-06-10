#include "Klondike.h"
#include "Game.h"
#include "Card.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <functional>

TEST(Klondike, move) {
	GamePiles piles;

	piles.m_foundationPiles[0].push_back(Card(1, Card::Symbol::HEARTS, Card::Status::UP));

	Game game(piles);
	auto moved = game.moveFromFoundationToFoundation(Game::foundation()[0], Game::foundation()[1]);

	auto state = game.getState();

	EXPECT_EQ(true, moved);

	EXPECT_EQ(0, state.m_foundationPiles[0].size());
	EXPECT_EQ(1, state.m_foundationPiles[1].size());

}

TEST(Klondike, moveEmpty) {
	GamePiles piles;

	Game game(piles);
	auto moved = game.moveFromFoundationToFoundation(Game::foundation()[0], Game::foundation()[1]);

	auto state = game.getState();

	EXPECT_EQ(false, moved);

	EXPECT_EQ(0, state.m_foundationPiles[0].size());
	EXPECT_EQ(0, state.m_foundationPiles[1].size());

}

TEST(Klondike, drawFromStock) {
	GamePiles piles;

	piles.m_stock.push_back(Card(1, Card::Symbol::HEARTS, Card::Status::DOWN));

	Game game(piles);
	auto result = game.drawFromStock();

	auto state = game.getState();

	EXPECT_EQ(true, result);

	EXPECT_EQ(0, state.m_stock.size());
	EXPECT_EQ(1, state.m_discardPile.size());

	result = game.drawFromStock();

	state = game.getState();

	EXPECT_EQ(false, result);

	EXPECT_EQ(0, state.m_stock.size());
	EXPECT_EQ(1, state.m_discardPile.size());
}
TEST(Klondike, turnDiscardPile) {
	GamePiles piles;

	piles.m_discardPile.push_back(Card(1, Card::Symbol::HEARTS, Card::Status::UP));
	piles.m_discardPile.push_back(Card(2, Card::Symbol::HEARTS, Card::Status::UP));

	Game game(piles);
	auto result = game.turnDiscardPileToStock();

	auto state = game.getState();

	EXPECT_EQ(true, result);

	EXPECT_EQ(2, state.m_stock.size());
	EXPECT_EQ(0, state.m_discardPile.size());
		
	EXPECT_EQ(2, state.m_stock[0].getValue());
	EXPECT_EQ(1, state.m_stock[1].getValue());

	using namespace testing;
	//EXPECT_TRUE(std::all_of(state.m_stock.begin(), state.m_stock.end(), [](const auto& x) {return x.getStatus() == Card::Status::DOWN; }));
	EXPECT_THAT(state.m_stock, Each(ResultOf(std::function<Card::Status (const Card&)>([](const Card& x) {return x.getStatus(); }), Eq(Card::Status::DOWN))));	
}