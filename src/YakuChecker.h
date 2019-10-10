#pragma once
#include"ResultInspector.hpp"
#include "Algorithms.h"
class YakuChecker
{
private:
	bool qingyise;
	bool menqing;
	int menqingCount;
	void addYaku(AgariResult& result,Yaku yaku);
public:
	YakuChecker();
	//役满型
	bool tianhu(const AgariParameters& par);
	bool dihu(const AgariParameters& par);
	bool ziyise(const AgariParameters& par, const std::vector<Group>& mianzi);
	bool sigangzi(const AgariParameters& par, const std::vector<Group>& mianzi);
	TryToAgariResult getResult(const AgariParameters& par, const std::vector<Group>& menqingMianzi);
};