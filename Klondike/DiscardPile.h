#pragma once
#include "BasePile.h"

#include <optional>

struct StockPile;

struct DiscardPile final : private BasePile {
	friend struct StockPile;
	using BasePile::game;

	auto topCard() const -> std::optional<Card> {
		if (data().empty()) return {};
		return data().back();
	}

	bool turnToStock(StockPile& stock);
	bool sendTopToPile(BasePile& pile);

private:
	friend struct Game;
	using BasePile::BasePile;

	bool AddCards(PileIt begin, PileIt end) final override;
};
