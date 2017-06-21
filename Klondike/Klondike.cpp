#include "Klondike.h"

void Klondike::play(Game& game)
{
	auto progress = false;
	auto tableauChanged = true;
	auto discardChanged = true;
	auto foundationChanged = false;
	while (tableauChanged || discardChanged || foundationChanged) {
		auto success = false;

		if (!game.topDiscardCard().has_value()) {
			success = game.drawCard();
			if (success) continue;
			return; // probably done
		}

		if (foundationChanged) {
			progress = true;
			discardChanged = true;
			tableauChanged = true;
			foundationChanged = false;
		}

		if (tableauChanged) {
			progress = true;

			// try to move to foundation
			for (auto tableau : Game::tableau) {
				for (auto foundation : Game::foundation) {
					success = game.moveCard(tableau, foundation);
					if (success) break;
				}
				if (success) break;
			}
			if (success) {
				foundationChanged = true;
				continue;
			}

			// try to move cards left
			for (int from = Game::TableauPiles - 1; from > 1; from--) {
				auto fromVisible = game.visibleCards(Game::tableau[from]);
				for (int to = 0; to < from; to++) {
					success = game.moveCards(Game::tableau[from], Game::tableau[to], fromVisible.size());
					if (success) break;
				}
				if (success) break;
			}
			if (success) continue;
			tableauChanged = false;
		}
		if (discardChanged) {

			// try to move to foundation
			for (auto foundation : Game::foundation) {
				success = game.moveCard(Game::discard, foundation);
				if (success) break;
			}
			if (success) {
				foundationChanged = true;
				continue;
			}

			// try to move to tableau
			for (auto tableau : Game::tableau) {
				success = game.moveCard(Game::discard, tableau);
				if (success) break;
			}
			if (success) {
				tableauChanged = true;
				continue;
			}
			discardChanged = false;
		}

		if (game.isStockEmpty()) {
			if (!progress) return;
			success = game.turnDiscardPileToStock();
			if (success) {
				discardChanged = true;
				progress = false;
				continue;
			}
		}

		success = game.drawCard();
		if (success) {
			discardChanged = true;
			continue;
		}
	}
}
