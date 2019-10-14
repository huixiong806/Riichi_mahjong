#pragma once
#include <string_view>

//三位16进制数，第一位为番数(役满记d,两倍役满记f,食下役按照应有的番数算)，第二位:0=非食下役/食下役的门清版，1=门清限定，2=食下役的非门清版,第三位为编号
enum class Yaku {
	Lizhi = 0x110,
	Yifa = 0x111,
	Menqianqingzimo = 0x112,
	Duanyaojiu = 0x100,
	Yipaizifeng = 0x101,
	Yipaichangfeng = 0x102,
	Yipaibai = 0x103,
	Yipaifa = 0x104,
	Yipaizhong = 0x105,
	Pinghu = 0x113,
	Yibeikou = 0x114,
	Qianggang = 0x106,
	Lingshangkaihua = 0x107,
	Haidilaoyue = 0x108,
	Hedilaoyu = 0x109,

	Lianglizhi = 0x210,
	Sansetongke = 0x200,
	Sangangzi = 0x201,
	Duiduihu = 0x202,
	Sananke = 0x203,
	Xiaosanyuan = 0x204,
	Hunlaotou = 0x205,
	Qiduizi = 0x211,

	Hunquandaiyaojiu = 0x206,
	HunquandaiyaojiuF = 0x126,
	Yiqitongguan = 0x207,
	YiqitongguanF = 0x127,
	Sansetongshun = 0x208,
	SansetongshunF = 0x128,

	Erbeikou = 0x310,
	Chunquantaiyaojiu = 0x300,
	ChunquantaiyaojiuF = 0x220,
	Hunyise = 0x301,
	HunyiseF = 0x221,

	Liujumanguan = 0x500,
	Qingyise = 0x600,
	QingyiseF = 0x520,


	Tianhu = 0xd00,
	Dihu = 0xd01,
	Dasanyuan = 0xd02,
	Sianke = 0xd10,
	Ziyise = 0xd03,
	Lvyise = 0xd04,
	Qinglaotou = 0xd05,
	Guoshiwushuang = 0xd11,
	Xiaosixi = 0xd06,
	Sigangzi = 0xd07,
	Jiulianbaodeng = 0xd12,

	Siankedanqi = 0xf10,
	Guoshiwushuangshisanmian = 0xf11,
	Chunzhengjiulianbaodeng = 0xf12,
	Dasixi = 0xf00,
};

constexpr unsigned int yakuToIndex(const Yaku yaku) noexcept {
	switch (yaku) {
	case Yaku::Lizhi: return 0;
	case Yaku::Yifa: return 1;
	case Yaku::Menqianqingzimo: return 2;
	case Yaku::Duanyaojiu: return 3;
	case Yaku::Yipaizifeng: return 4;
	case Yaku::Yipaichangfeng: return 5;
	case Yaku::Yipaibai: return 6;
	case Yaku::Yipaifa: return 7;
	case Yaku::Yipaizhong: return 8;
	case Yaku::Pinghu: return 9;
	case Yaku::Yibeikou: return 10;
	case Yaku::Qianggang: return 11;
	case Yaku::Lingshangkaihua: return 12;
	case Yaku::Haidilaoyue: return 13;
	case Yaku::Hedilaoyu: return 14;
	case Yaku::Lianglizhi: return 15;
	case Yaku::Sansetongke: return 16;
	case Yaku::Sangangzi: return 17;
	case Yaku::Duiduihu: return 18;
	case Yaku::Sananke: return 19;
	case Yaku::Xiaosanyuan: return 20;
	case Yaku::Hunlaotou: return 21;
	case Yaku::Qiduizi: return 22;
	case Yaku::Hunquandaiyaojiu: return 23;
	case Yaku::HunquandaiyaojiuF: return 24;
	case Yaku::Yiqitongguan: return 25;
	case Yaku::YiqitongguanF: return 26;
	case Yaku::Sansetongshun: return 27;
	case Yaku::SansetongshunF: return 28;
	case Yaku::Erbeikou: return 29;
	case Yaku::Chunquantaiyaojiu: return 30;
	case Yaku::ChunquantaiyaojiuF: return 31;
	case Yaku::Hunyise: return 32;
	case Yaku::HunyiseF: return 33;
	case Yaku::Liujumanguan: return 34;
	case Yaku::Qingyise: return 35;
	case Yaku::QingyiseF: return 36;
	case Yaku::Tianhu: return 37;
	case Yaku::Dihu: return 38;
	case Yaku::Dasanyuan: return 39;
	case Yaku::Sianke: return 40;
	case Yaku::Ziyise: return 41;
	case Yaku::Lvyise: return 42;
	case Yaku::Qinglaotou: return 43;
	case Yaku::Guoshiwushuang: return 44;
	case Yaku::Xiaosixi: return 45;
	case Yaku::Sigangzi: return 46;
	case Yaku::Jiulianbaodeng: return 47;
	case Yaku::Siankedanqi: return 48;
	case Yaku::Guoshiwushuangshisanmian: return 49;
	case Yaku::Chunzhengjiulianbaodeng: return 50;
	case Yaku::Dasixi: return 51;
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
		Yaku::Lizhi, Yaku::Yifa, Yaku::Menqianqingzimo, Yaku::Duanyaojiu, Yaku::Yipaizifeng,
		Yaku::Yipaichangfeng, Yaku::Yipaibai, Yaku::Yipaifa, Yaku::Yipaizhong, Yaku::Pinghu,
		Yaku::Yibeikou, Yaku::Qianggang, Yaku::Lingshangkaihua, Yaku::Haidilaoyue, Yaku::Hedilaoyu,
		Yaku::Lianglizhi, Yaku::Sansetongke, Yaku::Sangangzi, Yaku::Duiduihu, Yaku::Sananke,
		Yaku::Xiaosanyuan, Yaku::Hunlaotou, Yaku::Qiduizi, Yaku::Hunquandaiyaojiu,
		Yaku::HunquandaiyaojiuF, Yaku::Yiqitongguan, Yaku::YiqitongguanF, Yaku::Sansetongshun,
		Yaku::SansetongshunF, Yaku::Erbeikou, Yaku::Chunquantaiyaojiu, Yaku::ChunquantaiyaojiuF,
		Yaku::Hunyise, Yaku::HunyiseF, Yaku::Liujumanguan, Yaku::Qingyise, Yaku::QingyiseF,
		Yaku::Tianhu, Yaku::Dihu, Yaku::Dasanyuan, Yaku::Sianke, Yaku::Ziyise, Yaku::Lvyise,
		Yaku::Qinglaotou, Yaku::Guoshiwushuang, Yaku::Xiaosixi, Yaku::Sigangzi, Yaku::Jiulianbaodeng,
		Yaku::Siankedanqi, Yaku::Guoshiwushuangshisanmian, Yaku::Chunzhengjiulianbaodeng, Yaku::Dasixi
	};

	uint64_t status = 0;
};

std::string_view getYakuName(Yaku index) noexcept;
