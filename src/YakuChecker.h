#pragma once
#include "ResultInspector.hpp"
#include "Algorithms.h"
#include "Yaku.h"
#include <array>
#include <functional>

class YakuChecker {
private:
	bool menqing{};
	int menqingCount{};
	const AgariParameters& par;
	const std::vector<Group>& mianzi;
	std::array<std::pair<Yaku, bool>, 52> yaku;
	static const std::array<std::pair<Yaku, std::function<bool()>>, 52> yakuCheckFunc;
	void addYaku(AgariResult& result, Yaku yaku);
public:
		YakuChecker(const AgariParameters& i_par, const std::vector<Group>& i_menqingMianzi);
	//役满型
	bool tianhu();
	bool dihu();
	bool ziyise();
	bool sigangzi();
	bool xiaosixi();
	bool dasixi();
	bool qinglaotou();
	bool dasanyuan();
	bool lvyise();
	bool sianke();
	bool siankedanqi();
	bool jiulianbaodeng();
	bool chunzhengjiulianbaodeng();
	//非役满型
	bool qingyise();
	bool qingyiseF();
	TryToAgariResult getResult();
};

TryToAgariResult CheckYakuForStandardType(const AgariParameters& par, const std::vector<Group>& mianzi);
