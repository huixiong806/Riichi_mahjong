#include "Mountain.h"
#include <algorithm>
#include <random>

Mountain::Mountain() { handPtr = rinshanPtr = 0; }

std::string Mountain::getMountainString() {//获得牌山序列
	std::string res;
	for (auto& item : mountain) {
		int value = item.value();
		if (item.isAkadora())value = 0;
		res.push_back(value + '0');
		res.push_back(item.color());
	}
	return res;
}

Single Mountain::nextRinshan() {//获取下一张岭上牌(自动移海底牌)
	const auto res = B[rinshanPtr++];
	B.push_back(A[A.size() - 1]);
	A.pop_back();
	return res;
}

Single Mountain::nextHand() {//获取下一张手牌
	return A[handPtr++];
}

int Mountain::remainCount()const {//获取剩余牌数
	return A.size() - handPtr;
}

Single Mountain::getDora(int index)const { //获取宝牌
	return getDoraIndicator(index).next();
}

Single Mountain::getUra(int index)const { //获取里宝牌
	return getUraIndicator(index).next();
}

std::vector<Single> Mountain::getDoras(int count) const{
	std::vector<Single> res;
	for (int i = 0; i < count; ++i)
		res.push_back(getDora(i));
	return res;
}

std::vector<Single> Mountain::getUras(int count) const{
	std::vector<Single> res;
	for (int i = 0; i < count; ++i)
		res.push_back(getUra(i));
	return res;
}

Single Mountain::getDoraIndicator(int index)const{//获取宝牌指示牌,下标范围0~4
	return B[4 + index * 2];
}

Single Mountain::getUraIndicator(int index)const{//获取里宝牌指示牌,下标范围0~4
	return B[5 + index * 2];
}

std::vector<Single> Mountain::getDoraIndicators(int count)const{
	std::vector<Single> res;
	for (int i = 0; i < count;++i)
		res.push_back(getDoraIndicator(i));
	return res;
}

std::vector<Single> Mountain::getUraIndicators(int count)const{
	std::vector<Single> res;
	for (int i = 0; i < count; ++i)
		res.push_back(getUraIndicator(i));
	return res;
}

void Mountain::reset(const Rule& rule) {
	handPtr = 0;
	rinshanPtr = 0;
	mountain.clear();
	A.clear();
	B.clear();
	for (auto& j : hand)
		j.clear();
	std::vector<Single> all;
	for (auto i = 1; i <= 9; ++i) {
		for (auto j = 1; j <= 4; ++j) {
			if (i == 5 && j == 1 && rule.doraCount > 0) //红宝牌
			{
				all.emplace_back(i, 'm', true);
				all.emplace_back(i, 'p', true);
				all.emplace_back(i, 's', true);
			}
			else if (i == 5 && j == 2 && rule.doraCount == 4) //红宝牌5饼
			{
				all.emplace_back(i, 'm', false);
				all.emplace_back(i, 'p', true);
				all.emplace_back(i, 's', false);
			}
			else {
				all.emplace_back(i, 'm', false);
				all.emplace_back(i, 'p', false);
				all.emplace_back(i, 's', false);
			}
			if (i <= 7)all.emplace_back(i, 'z', false);
		}
	}
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(all.begin(), all.end(), g);
	auto ptr = 0;
	for (auto i = 1; i <= 13; ++i)
		for (auto& j : hand)
			j.push_back(all[ptr++]);
	tsumohai = all[ptr++];
	for (auto i = ptr; i < all.size(); ++i)
		mountain.push_back(all[i]);
	for (auto i = 1; i <= 69; ++i)
		A.push_back(all[ptr++]);
	ptr = all.size();
	for (auto i = 1; i <= 14; ++i)
		B.push_back(all[--ptr]);
}

void Mountain::DEBUG_RESET(const Rule & rule,uint64_t seed)
{
	handPtr = 0;
	rinshanPtr = 0;
	mountain.clear();
	A.clear();
	B.clear();
	for (auto& j : hand)
		j.clear();
	std::vector<Single> all;
	for (auto j = 4; j >= 1; --j) {
		for (auto ii = 1; ii <= 9; ++ii) {
			auto i = ii;
			if (i == 5 && j == 1 && rule.doraCount > 0) //红宝牌
			{
				all.emplace_back(i, 'm', true);
				all.emplace_back(i, 'p', true);
				all.emplace_back(i, 's', true);
			}
			else if (i == 5 && j == 2 && rule.doraCount == 4) //红宝牌5饼
			{
				all.emplace_back(i, 'm', false);
				all.emplace_back(i, 'p', true);
				all.emplace_back(i, 's', false);
			}
			else {
				all.emplace_back(i, 'm', false);
				all.emplace_back(i, 'p', false);
				all.emplace_back(i, 's', false);
			}
			if (i <= 7)all.emplace_back(i, 'z', false);
		}
	}
	//std::mt19937 g(seed);
	//std::shuffle(all.begin(), all.end(), g);
	auto ptr = 0;
	for (auto& j : hand)
		for (auto i = 1; i <= 13; ++i)
			j.push_back(all[ptr++]);
	tsumohai = all[ptr++];
	for (auto i = ptr; i < all.size(); ++i)
		mountain.push_back(all[i]);
	for (auto i = 1; i <= 69; ++i)
		A.push_back(all[ptr++]);
	ptr = all.size();
	for (auto i = 1; i <= 14; ++i)
		B.push_back(all[--ptr]);
}
