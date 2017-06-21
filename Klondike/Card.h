#pragma once

#include <cstdint>
#include <cassert>

// See: https://en.wikipedia.org/wiki/Standard_52-card_deck
enum class CardSymbol : uint8_t { Clubs, Diamonds, Hearts, Spades };
enum class CardRank : uint8_t { Ace, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King };

inline constexpr auto to_string(CardSymbol symbol) {
	static constexpr const char* names[] = { "Clubs", "Diamonds", "Hearts", "Spades" };
	return names[static_cast<int>(symbol)];
}

inline constexpr auto to_string(CardRank rank) {
	static constexpr const char* names[] = { "Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King" };
	return names[static_cast<int>(rank)];
}

inline constexpr auto next(CardRank rank) {
	if (rank == CardRank::King) return static_cast<CardRank>(-1); // invalid rank
	return static_cast<CardRank>(static_cast<int>(rank) + 1);
}

struct Card
{
	constexpr Card() = default;
	constexpr Card(const Card&) = default;
	constexpr Card& operator= (const Card&) = default;
	constexpr Card(Card&&) = default;
	constexpr Card& operator= (Card&&) = default;

	constexpr explicit Card(CardSymbol symbol, CardRank rank)
		: m_symbol(symbol), m_rank(rank) {}

	constexpr static auto FromId(int id) {
		assert(id >= 0 && id < 52);
		return Card{ static_cast<CardSymbol>(id / 13), static_cast<CardRank>(id % 13) };
	}

	constexpr auto operator==(const Card& o) const {
		return m_symbol == o.m_symbol && m_rank == o.m_rank;
	}

	constexpr auto symbol() const { return m_symbol; }
	constexpr auto rank() const { return m_rank; }

	constexpr auto isBlack() const { return m_symbol == CardSymbol::Clubs || m_symbol == CardSymbol::Spades; }
	constexpr auto isRed() const { return !isBlack(); }

	constexpr auto id() const { return static_cast<int>(m_symbol) * 13 + static_cast<int>(m_rank); }

private:
	CardSymbol m_symbol = CardSymbol::Clubs;
	CardRank m_rank = CardRank::Ace;
};
