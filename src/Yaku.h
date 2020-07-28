#pragma once
#include <string_view>

//三位16进制数，第一位为番数(役满记d,两倍役满记f,食下役按照应有的番数算)，第二位:0=非食下役/食下役的门清版，1=门清限定，2=食下役的非门清版,第三位为编号
enum class Yaku {
	Riichi = 0x110,
	Ippatsu = 0x111,
	Menzenchintsumo = 0x112,
	Tanyao = 0x100,
	YakuhaiJikaze = 0x101,
	YakuhaiBakaze = 0x102,
	YakuhaiHaku = 0x103,
	YakuhaiHatsu = 0x104,
	YakuhaiChun = 0x105,
	Pinhu = 0x113,
	Iipeikou = 0x114,
	Chankan = 0x106,
	Rinshankaihou = 0x107,
	Haiteiraoyue = 0x108,
	Houteiraoyui = 0x109,

	doubleRiichi = 0x210,
	Sanshokudoukou = 0x200,
	Sankantsu = 0x201,
	Toitoihou = 0x202,
	Sanankou = 0x203,
	Shousangen = 0x204,
	Honroutou = 0x205,
	Chiitoitsu = 0x211,

	Honchantaiyaochuu = 0x206,
	HonchantaiyaochuuF = 0x126,
	Ikkitsuukan = 0x207,
	IkkitsuukanF = 0x127,
	Sanshokudoujun = 0x208,
	SanshokudoujunF = 0x128,

	Ryanpeikou = 0x310,
	Junchantaiyaochuu = 0x300,
	JunchantaiyaochuuF = 0x220,
	Honitsu = 0x301,
	HonitsuF = 0x221,

	Nagashimangan = 0x500,
	Chinitsu = 0x600,
	ChinitsuF = 0x520,


	Tenhou = 0xd00,
	Chihou = 0xd01,
	Daisangen = 0xd02,
	Suuankou = 0xd10,
	Tsuuiisou = 0xd03,
	Ryuuiisou = 0xd04,
	Chinroutou = 0xd05,
	Kokushimusou = 0xd11,
	Shousuushi = 0xd06,
	Suukantsu = 0xd07,
	Chuurenpoutou = 0xd12,

	Suuankoutanki = 0xf10,
	Kokushijuusanmenmachi = 0xf11,
	Chuurenkyuumenmachi = 0xf12,
	Daisuushi = 0xf00,
};

constexpr unsigned int yakuToIndex(const Yaku yaku) noexcept {
	switch (yaku) {
	case Yaku::Riichi: return 0;
	case Yaku::Ippatsu: return 1;
	case Yaku::Menzenchintsumo: return 2;
	case Yaku::Tanyao: return 3;
	case Yaku::YakuhaiJikaze: return 4;
	case Yaku::YakuhaiBakaze: return 5;
	case Yaku::YakuhaiHaku: return 6;
	case Yaku::YakuhaiHatsu: return 7;
	case Yaku::YakuhaiChun: return 8;
	case Yaku::Pinhu: return 9;
	case Yaku::Iipeikou: return 10;
	case Yaku::Chankan: return 11;
	case Yaku::Rinshankaihou: return 12;
	case Yaku::Haiteiraoyue: return 13;
	case Yaku::Houteiraoyui: return 14;
	case Yaku::doubleRiichi: return 15;
	case Yaku::Sanshokudoukou: return 16;
	case Yaku::Sankantsu: return 17;
	case Yaku::Toitoihou: return 18;
	case Yaku::Sanankou: return 19;
	case Yaku::Shousangen: return 20;
	case Yaku::Honroutou: return 21;
	case Yaku::Chiitoitsu: return 22;
	case Yaku::Honchantaiyaochuu: return 23;
	case Yaku::HonchantaiyaochuuF: return 24;
	case Yaku::Ikkitsuukan: return 25;
	case Yaku::IkkitsuukanF: return 26;
	case Yaku::Sanshokudoujun: return 27;
	case Yaku::SanshokudoujunF: return 28;
	case Yaku::Ryanpeikou: return 29;
	case Yaku::Junchantaiyaochuu: return 30;
	case Yaku::JunchantaiyaochuuF: return 31;
	case Yaku::Honitsu: return 32;
	case Yaku::HonitsuF: return 33;
	case Yaku::Nagashimangan: return 34;
	case Yaku::Chinitsu: return 35;
	case Yaku::ChinitsuF: return 36;
	case Yaku::Tenhou: return 37;
	case Yaku::Chihou: return 38;
	case Yaku::Daisangen: return 39;
	case Yaku::Suuankou: return 40;
	case Yaku::Tsuuiisou: return 41;
	case Yaku::Ryuuiisou: return 42;
	case Yaku::Chinroutou: return 43;
	case Yaku::Kokushimusou: return 44;
	case Yaku::Shousuushi: return 45;
	case Yaku::Suukantsu: return 46;
	case Yaku::Chuurenpoutou: return 47;
	case Yaku::Suuankoutanki: return 48;
	case Yaku::Kokushijuusanmenmachi: return 49;
	case Yaku::Chuurenkyuumenmachi: return 50;
	case Yaku::Daisuushi: return 51;
	default: return 0xFFFFFFFF;
	}
}

class Yakus {
	static constexpr uint64_t yakuToStatusMask(const Yaku yaku) noexcept { return 1ull << yakuToIndex(yaku); }
public:
	template <Yaku ...Ys>
	static constexpr Yakus Create() noexcept { Yakus ret; ret.status = (yakuToStatusMask(Ys) | ...); return ret; }
	
	template <Yaku Y>
	[[nodiscard]] constexpr bool has() const noexcept {
		static constexpr auto Msk = yakuToStatusMask(Y);
		return status & Msk;
	}

	template <Yaku Y>
	constexpr void add() noexcept {
		static constexpr auto Msk = yakuToStatusMask(Y);
		status |= Msk;
	}

	template <Yaku Y>
	constexpr void remove() noexcept {
		static constexpr auto Msk = yakuToStatusMask(Y);
		status ^= Msk;
	}

	template <Yaku Y>
	constexpr void exclude() noexcept {
		static constexpr auto Msk = ~yakuToStatusMask(Y);
		status &= Msk;
	}

	constexpr void add(const Yaku yaku) noexcept { status |= yakuToStatusMask(yaku); }
	constexpr void remove(const Yaku yaku) noexcept { status ^= yakuToStatusMask(yaku); }
	constexpr void exclude(const Yaku yaku) noexcept { status &= ~yakuToStatusMask(yaku); }
	constexpr void remove(const Yakus yakus) noexcept { status ^= yakus.status; }
	constexpr void exclude(const Yakus yakus) noexcept { status &= ~yakus.status; }
	constexpr void join(const Yakus yakus) noexcept { status |= yakus.status; }
	[[nodiscard]] constexpr bool has(const Yaku yaku) const noexcept { return status & yakuToStatusMask(yaku); }
	[[nodiscard]] constexpr bool match(const Yakus yakus) const noexcept { return status & yakus.status; }

	[[nodiscard]] constexpr bool contains(const Yakus yakus) const noexcept {
		return !static_cast<bool>(yakus.status & ~status);
	}

	[[nodiscard]] constexpr bool subset_of(const Yakus yakus) const noexcept { return yakus.contains(*this); }
	[[nodiscard]] constexpr bool empty() const noexcept { return !static_cast<bool>(status); }

	struct forward_iterator {
	private:
		uint64_t status = 0xFFFFFFFFFFFFFFFF;
		int index{};
	public:
		constexpr forward_iterator() noexcept = default;

		explicit constexpr forward_iterator(const uint64_t i_status) noexcept : status(i_status) { ++*this; }

		constexpr bool operator ==(const forward_iterator& r) const noexcept { return status == r.status; }

		constexpr bool operator !=(const forward_iterator& r) const noexcept { return status != r.status; }

		constexpr forward_iterator& operator++() noexcept {
			if (status){
				index = first(status), status &= ~(1ull << index);
			}
			else{
				status = 0xFFFFFFFFFFFFFFFF;
			}
			return *this;
		}
	
		constexpr forward_iterator operator++(int) noexcept {
			const auto ret = *this;
			++*this;
			return ret;
		}

		constexpr Yaku operator*() const noexcept { return reverse_table[index]; }

		[[nodiscard]] constexpr int getIndex() const noexcept { return index; }
	};

	[[nodiscard]] constexpr auto begin() const noexcept { return forward_iterator(status); }

	[[nodiscard]] constexpr auto end() const noexcept { return forward_iterator{}; }
private:
	static constexpr int first4(const uint8_t stat) noexcept {
		if (stat & 0b1000) return 3;
		if (stat & 0b0100) return 2;
		if (stat & 0b0010) return 1;
		return 0;
	}

	static constexpr int first(const uint8_t stat) noexcept {
		const auto high = stat >> 4;
		return high ? first4(high) | 4 : first4(stat & 0b1111);
	}

	static constexpr int first(const uint16_t stat) noexcept {
		const auto high = static_cast<uint8_t>(stat >> 8);
		return high ? first(high) | 8 : first(static_cast<uint8_t>(stat));
	}

	static constexpr int first(const uint32_t stat) noexcept {
		const auto high = static_cast<uint16_t>(stat >> 16);
		return high ? first(high) | 16 : first(static_cast<uint16_t>(stat));
	}

	static constexpr int first(const uint64_t stat) noexcept {
		const auto high = static_cast<uint32_t>(stat >> 32);
		return high ? first(high) | 32 : first(static_cast<uint32_t>(stat));
	}

	static constexpr Yaku reverse_table[] = {
		Yaku::Riichi, Yaku::Ippatsu, Yaku::Menzenchintsumo, Yaku::Tanyao, Yaku::YakuhaiJikaze,
		Yaku::YakuhaiBakaze, Yaku::YakuhaiHaku,Yaku::YakuhaiHatsu,Yaku::YakuhaiChun, Yaku::Pinhu,
		Yaku::Iipeikou, Yaku::Chankan, Yaku::Rinshankaihou, Yaku::Haiteiraoyue, Yaku::Houteiraoyui,
		Yaku::doubleRiichi, Yaku::Sanshokudoukou, Yaku::Sankantsu, Yaku::Toitoihou, Yaku::Sanankou,
		Yaku::Shousangen, Yaku::Honroutou, Yaku::Chiitoitsu, Yaku::Honchantaiyaochuu,
		Yaku::HonchantaiyaochuuF, Yaku::Ikkitsuukan, Yaku::IkkitsuukanF, Yaku::Sanshokudoujun,
		Yaku::SanshokudoujunF, Yaku::Ryanpeikou, Yaku::Junchantaiyaochuu, Yaku::JunchantaiyaochuuF,
		Yaku::Honitsu, Yaku::HonitsuF, Yaku::Nagashimangan, Yaku::Chinitsu, Yaku::ChinitsuF,
		Yaku::Tenhou, Yaku::Chihou, Yaku::Daisangen, Yaku::Suuankou, Yaku::Tsuuiisou, Yaku::Ryuuiisou,
		Yaku::Chinroutou, Yaku::Kokushimusou, Yaku::Shousuushi, Yaku::Suukantsu, Yaku::Chuurenpoutou,
		Yaku::Suuankoutanki, Yaku::Kokushijuusanmenmachi, Yaku::Chuurenkyuumenmachi, Yaku::Daisuushi
	};

	uint64_t status = 0;
};

std::string_view getYakuName(Yaku index) noexcept;
