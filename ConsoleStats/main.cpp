#include "Klondike.h"
#include "Game.h"

#include <iostream>
#include <string>

std::string to_string(const Card& card) {
	return std::string(to_string(card.symbol())) + ' ' + to_string(card.rank());
}

std::string to_string(const std::optional<Card>& opt) {
	if (opt.has_value()) return to_string(opt.value());
	return "nothing";
}

std::string to_string(const Pile& pile) {
	return std::accumulate(pile.begin(), pile.end(), std::string(), [](auto str, const auto& card) {
		if (str.empty()) return to_string(card);
		return str + ", " + to_string(card);
	});
}

void printGame(const Game& game) {
	std::cout << "Stock:   " << (game.isStockEmpty() ? "empty" : "full") << '\n';
	std::cout << "Discard: " << to_string(game.topCard(Game::discard)) << '\n';
	std::cout << "Tableau:\n";
	for (auto tableau : Game::tableau) {
		auto count = game.hiddenCardCount(tableau);
		auto cards = game.visibleCards(tableau);
		std::cout << " - //" << count << "/ " << to_string(cards) << '\n';
	}
	std::cout << "Foundation:\n";
	for (auto foundation : Game::foundation) {
		auto topCard = game.topCard(foundation);
		std::cout << " - " << to_string(topCard) << '\n';
	}
}

int main() {
	auto won = 0;
	auto total = 10000;
	for (int i = 0; i < total; i++) {
		auto game = Game::CreateRandomized();
		//printGame(game);
		Klondike::play(game);
		if (game.isWon()) won++;
		//std::cout << "\n-----\nAfter:\n-----\n";
		//printGame(game);
	}

	std::cout << "won " << won << " of " << total << " games\n";
	std::string x;
	std::cin >> x;
}
