#pragma once

#include "Card.h"

#include <array>
#include <vector>

using Pile = std::vector<Card>;

struct GamePiles {
	Pile m_discardPile;
	Pile m_stock;
	std::array<Pile, 7> m_tableauPiles;
	std::array<Pile, 4> m_foundationPiles;
};

class Game
{
public:
	enum class Tableau { One, Two, Three, Four, Five, Six, Seven };
	enum class Foundation { One, Two, Three, Four };

	Game(const GamePiles& piles);
	~Game();

	//void restart();

	bool turnDiscardPileToStock();
	bool drawFromStock();
	//bool moveFromDiscardToTableau(Tableau);
	//bool moveFromDiscardToFoundation(Foundation);
	//bool moveFromTableauToTableau(int n, Tableau from, Tableau to);
	//bool moveFromTableauToFoundation(Tableau from, Foundation to);
	//bool moveFromFoundationToTableau(Foundation from, Tableau to); // not allowed
	bool moveFromFoundationToFoundation(Foundation from, Foundation to);

	//bool isWon() const;

	GamePiles getState() const;

	static std::array<Tableau, 7> tableau() { return { Tableau::One, Tableau::Two, Tableau::Three, Tableau::Four, Tableau::Five, Tableau::Six, Tableau::Seven }; }
	static std::array<Foundation, 4> foundation() { return { Foundation::One, Foundation::Two, Foundation::Three, Foundation::Four }; }

private:
	GamePiles m_piles;
};

