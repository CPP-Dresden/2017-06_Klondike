#pragma once
#include "BasePile.h"

#include <optional>

struct FoundationPile final : public BasePile {
	auto topCard() const -> std::optional<Card> {
		if (data().empty()) return {};
		return data().back();
	}

	bool sendToFoundation(FoundationPile&);

private:
	friend struct Game;
	using BasePile::BasePile;

	friend struct TableauPile;
	bool AddCards(PileIt begin, PileIt end) final override;
};
