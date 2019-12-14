#pragma once
#include "ResultInspector.hpp"
#include "Algorithms.h"
#include "Yaku.h"
#include <array>
#include <type_traits>

// NOTE: This class is NOT thread-safe
class YakuChecker {
protected:
	// list of checks
	using InternalInvoke = bool (YakuChecker::*)() const noexcept;
	inline static std::array<InternalInvoke, 52> yakuCheckFunc;
	// fake constructor for implementing static initialization
	YakuChecker() noexcept: menchinCount(0), menchin(false), par(*reinterpret_cast<AgariParameters*>(this)),
	                        mentsu(*reinterpret_cast<std::vector<Group>*>(this)) { }

private:
	// operation buffers
	mutable Yakus exec{}, result{};
	// parameters
	const int menchinCount;
	const bool menchin;
	const AgariParameters& par;
	const std::vector<Group>& mentsu;

	template <Yaku Y, class Invoke>
	[[nodiscard]] bool UnSyncCacheInvoke(const Invoke invoke) const noexcept {
		static_assert(noexcept(std::declval<Invoke>()));
		if (!exec.has<Y>()) {
			const auto res = invoke();
			exec.add<Y>();
			if (res) { result.add<Y>(); }
			return res;
		}
		return result.has<Y>();
	}

	template <Yaku Y, Yaku ... Ys, class Invoke>
	[[nodiscard]] bool UnSyncUnifiedCacheInvoke(const Invoke invoke) const noexcept {
		static_assert(noexcept(std::declval<Invoke>()));
		if (!exec.contains(Yakus::Create<Y, Ys...>())) { invoke(); }
		return result.has<Y>();
	}

	template <Yaku Y>
	void UnSyncCacheSet(const bool val) const noexcept { if (val) { result.add<Y>(); } }
public:
	YakuChecker(const AgariParameters& i_par, const std::vector<Group>& i_menqingMianzi) noexcept;
	//役满型
	[[nodiscard]] bool tenhou() const noexcept;
	[[nodiscard]] bool chihou() const noexcept;
	[[nodiscard]] bool tsuuiisou() const noexcept;
	[[nodiscard]] bool suukantsu() const noexcept;
	[[nodiscard]] bool shousuushi() const noexcept;
	[[nodiscard]] bool daisuushi() const noexcept;
	[[nodiscard]] bool chinroutou() const noexcept;
	[[nodiscard]] bool daisangen() const noexcept;
	[[nodiscard]] bool ryuuiisou() const noexcept;
	[[nodiscard]] bool suuankou() const noexcept;
	[[nodiscard]] bool suuankoutanki() const noexcept;
	[[nodiscard]] bool chuurenpoutouShape() const noexcept;
	[[nodiscard]] bool chuurenkyuumenmachiShape() const noexcept;
	[[nodiscard]] bool chuurenpoutou() const noexcept;
	[[nodiscard]] bool chuurenkyuumenmachi() const noexcept;
	//非役满型
	[[nodiscard]] bool chinitsu() const noexcept;
	[[nodiscard]] bool chinitsuF() const noexcept;
	[[nodiscard]] bool honitsu() const noexcept;
	[[nodiscard]] bool honitsuF() const noexcept;
	//返回值第一项为是否满足平和，第二项为符数
	[[nodiscard]] std::pair<bool,int> pinghuAndFuCount() const noexcept;
	[[nodiscard]] TryToAgariResult getResult();

	template <Yaku Y>
	[[nodiscard]] bool UnSyncCheck() noexcept {
		static constexpr auto Id = yakuToIndex(Y);
		return (this->*yakuCheckFunc[Id])();
	}
};

TryToAgariResult CheckYakuForStandardType(const AgariParameters& par, const std::vector<Group>& mentsu);
