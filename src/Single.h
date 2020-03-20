#pragma once
#include<vector>
#include <string>
class Single {
private:
	char mColor{}; //'m','p','s','z'
	uint8_t mValue{};
	bool mAkadora{}; //是否为红宝牌
public:
	constexpr Single() noexcept: Single(1, 'z' + 1, true) { }

	constexpr Single(const uint8_t value_, const char color_, const bool akadora) noexcept
		: mColor(color_), mValue(value_), mAkadora(akadora) {}

	//获取颜色
	[[nodiscard]] constexpr char color() const noexcept { return mColor; }
	//获取值
	[[nodiscard]] constexpr uint8_t value() const noexcept { return mValue; }
	//是否为红宝牌
	[[nodiscard]] constexpr bool isAkadora() const noexcept { return mAkadora; }

	//获取顺序上的下一张牌(默认为非红宝牌)
	[[nodiscard]] constexpr Single next() const noexcept {
		if (mColor == 'z' && mValue == 7)return Single(1, mColor, false);
		if (mValue == 9)return Single(1, mColor, false);
		return Single(mValue + 1, mColor, false);
	}

	//判断两张牌是否相等，不考虑是否为红宝牌
	[[nodiscard]] constexpr bool valueEqual(const Single& rhs) const noexcept {
		return mColor == rhs.mColor && mValue == rhs.mValue;
	}

	//判断两张牌是否相等，考虑是否为红宝牌
	constexpr bool operator ==(const Single& rhs) const {
		return (mColor == rhs.mColor && mValue == rhs.mValue && rhs.mAkadora == mAkadora);
	}

	constexpr bool operator !=(const Single& rhs) const noexcept { return !(*this == rhs); }
	//判断两张牌的大小，考虑是否为红宝牌，万<饼<索<字<Null,数字相同的情况下红宝牌更大
	constexpr bool operator <(const Single& rhs) const noexcept {
		return static_cast<int>(mColor) * 1000 + mValue * 10 + static_cast<int>(mAkadora) < static_cast<int>(rhs.mColor)
			* 1000 + rhs.mValue * 10 + static_cast<int>(rhs.mAkadora);
	}

	constexpr //判断两张牌的大小，考虑是否为红宝牌，万<饼<索<字<Null,数字相同的情况下红宝牌更大
	bool operator >(const Single& rhs) const noexcept {
		return static_cast<int>(mColor) * 1000 + mValue * 10 + static_cast<int>(mAkadora) > static_cast<int>(rhs.mColor)
			* 1000 + rhs.mValue * 10 + static_cast<int>(rhs.mAkadora);
	}

	[[nodiscard]] constexpr bool is19Z() const noexcept {
		if (mColor == 'z')return true;
		if (mValue == 1 || mValue == 9)return true;
		return false;
	}

	[[nodiscard]] std::string getDisplay() const;
	[[nodiscard]] std::string getString() const;
};

using Tiles = std::vector<Single>;

struct DiscardedTile {
	Single tile;
	bool exist;
	DiscardedTile() {}
	DiscardedTile(const Single& i_tile) {
		tile = i_tile;
		exist = true;
	}
};
using DiscardedTiles = std::vector<DiscardedTile>;
//Null牌在排序中永远是最大的
static constexpr Single Null{};

//单色状态压缩过的手牌，二进制压缩，为了减少遍历节省时间。每种牌最多支持7张
struct SparseSinglesOfColor {
	constexpr SparseSinglesOfColor() noexcept: SparseSinglesOfColor(0) {}

	template <unsigned int V, unsigned int C>
	constexpr auto& add() noexcept { return rep += C << Offsets[V], *this; }

	template <unsigned int V, unsigned int C>
	[[nodiscard]] constexpr auto add() const noexcept { return SparseSinglesOfColor{rep + (C << Offsets[V])}; }

	template <unsigned int V, unsigned int C>
	constexpr auto& remove() noexcept { return rep -= C << Offsets[V], *this; }

	template <unsigned int V, unsigned int C>
	[[nodiscard]] constexpr auto remove() const noexcept { return SparseSinglesOfColor{rep - (C << Offsets[V])}; }

	constexpr auto& add(const unsigned int v, const unsigned int c) noexcept { return rep += c << Offsets[v], *this; }

	[[nodiscard]] constexpr auto add(const unsigned int v, const unsigned int c) const noexcept {
		return SparseSinglesOfColor{rep + (c << Offsets[v])};
	}

	constexpr auto& remove(const unsigned int v, const unsigned int c) noexcept {
		return rep -= c << Offsets[v], *this;
	}

	[[nodiscard]] constexpr auto remove(const unsigned int v, const unsigned int c) const noexcept {
		return SparseSinglesOfColor{rep - (c << Offsets[v])};
	}

	constexpr auto& join(const SparseSinglesOfColor r) noexcept { return rep += r.rep, *this; }

	[[nodiscard]] constexpr auto join(const SparseSinglesOfColor r) const noexcept {
		return SparseSinglesOfColor{rep + r.rep };
	}

	template <unsigned int V>
	[[nodiscard]] constexpr auto get() const noexcept { return rep >> Offsets[V] & 0b111; }

	[[nodiscard]] constexpr auto get(const unsigned int v) const noexcept { return rep >> Offsets[v] & 0b111; }

	[[nodiscard]] constexpr auto raw() const noexcept { return rep; }

	[[nodiscard]] constexpr auto tileCount()const noexcept{
		return	(rep >> Offsets[1] & 0b111) +
			(rep >> Offsets[2] & 0b111) +
			(rep >> Offsets[3] & 0b111) +
			(rep >> Offsets[4] & 0b111) +
			(rep >> Offsets[5] & 0b111) +
			(rep >> Offsets[6] & 0b111) +
			(rep >> Offsets[7] & 0b111) +
			(rep >> Offsets[8] & 0b111) +
			(rep >> Offsets[9] & 0b111);
	}
private:
	uint32_t rep;
	explicit constexpr SparseSinglesOfColor(const uint32_t r) noexcept: rep(r) {}
	static constexpr unsigned int Offsets[] = {0u,0u, 3u, 6u, 9u, 16u, 19u, 22u, 25u, 28u};
};

//单色状态压缩过的手牌，5进制高压缩型，为了节省空间。每种牌最多支持4张
struct CompactSinglesOfColor {
	constexpr CompactSinglesOfColor() noexcept: CompactSinglesOfColor(0) {}
	
	template <unsigned int V, unsigned int C>
	constexpr auto& add() noexcept { return rep += C * Pow5[V], *this; }

	template <unsigned int V, unsigned int C>
	[[nodiscard]] constexpr auto add() const noexcept { return CompactSinglesOfColor{rep + C * Pow5[V]}; }

	template <unsigned int V, unsigned int C>
	constexpr auto& remove() noexcept { return rep -= C * Pow5[V], *this; }

	template <unsigned int V, unsigned int C>
	[[nodiscard]] constexpr auto remove() const noexcept { return CompactSinglesOfColor{rep - C * Pow5[V]}; }

	constexpr auto& add(const unsigned int v, const unsigned int c) noexcept { return rep += c * Pow5[v], *this; }

	[[nodiscard]] constexpr auto add(const unsigned int v, const unsigned int c) const noexcept {
		return CompactSinglesOfColor{rep + c * Pow5[v]};
	}

	constexpr auto& remove(const unsigned int v, const unsigned int c) noexcept { return rep -= c * Pow5[v], *this; }

	[[nodiscard]] constexpr auto remove(const unsigned int v, const unsigned int c) const noexcept {
		return CompactSinglesOfColor{rep - c * Pow5[v]};
	}
	
	constexpr auto& join(const CompactSinglesOfColor r) noexcept { return rep += r.rep, *this; }

	[[nodiscard]] constexpr auto join(const CompactSinglesOfColor r) const noexcept {
		return CompactSinglesOfColor{rep + r.rep };
	}
	
	// This function is slow relative to the sparse versions. 
	template <unsigned int V>
	[[nodiscard]] constexpr auto get() const noexcept { return rep / Pow5[V] % 5; }

	[[nodiscard]] constexpr auto get(const unsigned int v) const noexcept { return rep / Pow5[v] % 5; }

	[[nodiscard]] constexpr auto raw() const noexcept { return rep; }

	explicit constexpr CompactSinglesOfColor(const SparseSinglesOfColor r) noexcept
	: CompactSinglesOfColor(CompactSinglesOfColor{}
	       .add(9, r.get<9u>()).add(1, r.get<1u>()).add(2, r.get<2u>()).add(3, r.get<3u>())
	       .add(4, r.get<4u>()).add(5, r.get<5u>()).add(6, r.get<6u>()).add(7, r.get<7u>())
	       .add(8, r.get<8u>()))
	{}
	[[nodiscard]] explicit constexpr operator SparseSinglesOfColor() const noexcept {
		return SparseSinglesOfColor{}
		       .add(9, get<9u>()).add(1, get<1u>()).add(2, get<2u>()).add(3, get<3u>())
		       .add(4, get<4u>()).add(5, get<5u>()).add(6, get<6u>()).add(7, get<7u>())
		       .add(8, get<8u>());
	}
	[[nodiscard]] constexpr auto tileCount()const noexcept {
		return	(rep / Pow5[1] % 5) +
			(rep / Pow5[2] % 5) +
			(rep / Pow5[3] % 5) +
			(rep / Pow5[4] % 5) +
			(rep / Pow5[5] % 5) +
			(rep / Pow5[6] % 5) +
			(rep / Pow5[7] % 5) +
			(rep / Pow5[8] % 5) +
			(rep / Pow5[9] % 5);
	}
private:
	uint32_t rep;
	explicit constexpr CompactSinglesOfColor(const uint32_t r) noexcept: rep(r) {}
	static constexpr unsigned int Pow5[] = {0u,1u, 5u, 25u, 125u, 625u, 3125u, 15625u, 78125u, 390625u, 1953125u};
};

struct alignas(std::max_align_t) SparseSingles {
	SparseSinglesOfColor colors[4]{ {},{},{},{} };
	constexpr SparseSingles()noexcept{}
	explicit SparseSingles(const std::vector<Single>& handTile) noexcept {
		for (const auto& item : handTile) {
			if (item.color() == 'm')
				colors[0].add(item.value(), 1);
			if (item.color() == 'p')
				colors[1].add(item.value(), 1);
			if (item.color() == 's')
				colors[2].add(item.value(), 1);
			if (item.color() == 'z')
				colors[3].add(item.value(), 1);
		}
	}
	int getTileCount()const noexcept
	{
		auto count = 0;
		for (auto& item : colors)
			count += item.tileCount();
		return count;
	}
};

struct alignas(std::max_align_t) CompactSingles {
	CompactSinglesOfColor colors[4]{ {},{},{},{} };
	constexpr CompactSingles()noexcept{}
	explicit CompactSingles(const std::vector<Single> & handTile) noexcept {
		for (const auto& item:handTile) {
			if (item.color() == 'm')
				colors[0].add(item.value(), 1);
			if (item.color() == 'p')
				colors[1].add(item.value(), 1);
			if (item.color() == 's')
				colors[2].add(item.value(), 1);
			if (item.color() == 'z')
				colors[3].add(item.value(), 1);
		}
	}
	int getTileCount()const noexcept
	{
		auto count = 0;
		for (auto& item : colors)
			count += item.tileCount();
		return count;
	}
};

template <int EM = 36>
struct alignas(std::max_align_t) TileTypes {
	uint8_t tileType[EM];
	uint8_t count;
	
	constexpr void expandFrom(const CompactSingles pack) noexcept {
		count = 0u;
		for (auto i = 0u; i < 4u; ++i) {
			const auto col = pack.colors[i];
			for (auto j = 0u; j < 9u; ++j)
				if (const auto val = col.get(j); val) { tileType[count++] = j << 2 | i; }
		}
	}
	
	constexpr void expandFrom(const SparseSingles pack) noexcept {
		count = 0u;
		for (auto i = 0u; i < 4u; ++i) {
			const auto col = pack.colors[i];
			const auto raw = col.raw();
			if (const auto low = raw & 0xFFFFu; low) {
				if (const auto val = col.get<1>; val) { tileType[count++] = 0 << 2 | i; }
				if (const auto val = col.get<2>; val) { tileType[count++] = 1 << 2 | i; }
				if (const auto val = col.get<3>; val) { tileType[count++] = 2 << 2 | i; }
				if (const auto val = col.get<4>; val) { tileType[count++] = 3 << 2 | i; }
			}
			else {
				if (const auto val = col.get<5>; val) { tileType[count++] = 4 << 2 | i; }
				if (const auto val = col.get<6>; val) { tileType[count++] = 5 << 2 | i; }
				if (const auto val = col.get<7>; val) { tileType[count++] = 6 << 2 | i; }
				if (const auto val = col.get<8>; val) { tileType[count++] = 7 << 2 | i; }
				if (const auto val = col.get<9>; val) { tileType[count++] = 8 << 2 | i; }
			}
		}
	}
};
