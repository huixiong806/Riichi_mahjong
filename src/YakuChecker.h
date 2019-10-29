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
	YakuChecker() noexcept: menqingCount(0), menqing(false), par(*reinterpret_cast<AgariParameters*>(this)),
	                        mianzi(*reinterpret_cast<std::vector<Group>*>(this)) { }

private:
	// operation buffers
	mutable Yakus exec{}, result{};
	// parameters
	const int menqingCount;
	const bool menqing;
	const AgariParameters& par;
	const std::vector<Group>& mianzi;

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
	[[nodiscard]] bool tianhu() const noexcept;
	[[nodiscard]] bool dihu() const noexcept;
	[[nodiscard]] bool ziyise() const noexcept;
	[[nodiscard]] bool sigangzi() const noexcept;
	[[nodiscard]] bool xiaosixi() const noexcept;
	[[nodiscard]] bool dasixi() const noexcept;
	[[nodiscard]] bool qinglaotou() const noexcept;
	[[nodiscard]] bool dasanyuan() const noexcept;
	[[nodiscard]] bool lvyise() const noexcept;
	[[nodiscard]] bool sianke() const noexcept;
	[[nodiscard]] bool siankedanqi() const noexcept;
	[[nodiscard]] bool jiulianbaodengShape() const noexcept;
	[[nodiscard]] bool chunzhengjiulianbaodengShape() const noexcept;
	[[nodiscard]] bool jiulianbaodeng() const noexcept;
	[[nodiscard]] bool chunzhengjiulianbaodeng() const noexcept;
	//非役满型
	[[nodiscard]] bool qingyise() const noexcept;
	[[nodiscard]] bool qingyiseF() const noexcept;
	[[nodiscard]] bool hunyise() const noexcept;
	[[nodiscard]] bool hunyiseF() const noexcept;
	//返回值第一项为是否满足平和，第二项为符数
	[[nodiscard]] std::pair<bool,int> pinghuAndFuCount() const noexcept;
	[[nodiscard]] TryToAgariResult getResult();

	template <Yaku Y>
	[[nodiscard]] bool UnSyncCheck() noexcept {
		static constexpr auto Id = yakuToIndex(Y);
		return (this->*yakuCheckFunc[Id])();
	}
};

TryToAgariResult CheckYakuForStandardType(const AgariParameters& par, const std::vector<Group>& mianzi);
