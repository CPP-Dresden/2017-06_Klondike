#pragma once

template<class Container>
struct SkipFirstView {
	SkipFirstView(Container& c) : m_c(c) {}

	auto begin() {
		auto it = m_c.begin();
		if (it != m_c.end()) it++;
		return it;
	}
	auto begin() const {
		auto it = m_c.begin();
		if (it != m_c.end()) it++;
		return it;
	}

	auto end() { return m_c.end(); }
	auto end() const { return m_c.end(); }

private:
	Container& m_c;
};

template<class Container>
auto skipFirst(Container& c) {
	return SkipFirstView<Container>(c);
}
