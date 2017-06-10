#include "Game.h"


Game::Game(const GamePiles & piles) :
	m_piles(piles)
{
}

Game::~Game()
{
}

bool Game::turnDiscardPileToStock()
{
	if (!m_piles.m_stock.empty()) return false;
	if (m_piles.m_discardPile.empty()) return false;

	std::swap(m_piles.m_discardPile, m_piles.m_stock);
	
	//revert order for discard pile
	std::reverse(m_piles.m_stock.begin(), m_piles.m_stock.end());
	std::for_each(m_piles.m_stock.begin(), m_piles.m_stock.end(), [](Card& card) {card.setStatus(Card::Status::DOWN); });
	return true;
}

bool Game::drawFromStock()
{
	if (m_piles.m_stock.empty()) return false;
	
	m_piles.m_discardPile.push_back(m_piles.m_stock.back());
	m_piles.m_stock.pop_back();

	return true;
}

bool Game::moveFromFoundationToFoundation(Foundation from, Foundation to)
{
	if (m_piles.m_foundationPiles[(int)from].size() == 1 && m_piles.m_foundationPiles[(int)to].size() == 0) {
		std::swap(m_piles.m_foundationPiles[(int)to], m_piles.m_foundationPiles[(int)from]);
		return true;
	}
	return false;
}

GamePiles Game::getState() const
{
	return m_piles;
}
