#pragma once
#include "Single.h"
#include "Rules.h"
#include "ResultInspector.hpp"
#include <vector>
#include <string>

class Mountain {
private:
protected:
	std::vector<Single> mA; //四家摸牌
	std::vector<Single> mB; //岭上，dora指示牌
	std::vector<Single> mMountain; //牌山序列
	int mHandPtr; //手牌指针
	int mRinshanPtr; //岭上牌指针
public:
	virtual ~Mountain() noexcept = default;
	std::vector<Single> hand[4]; //四家初始手牌
	Single tsumohai; //庄家0巡的自摸牌
	Mountain(); //构造函数
	virtual void reset(const Rule& rule); //产生新牌山
	void DEBUG_RESET(const Rule& rule); //产生作弊的牌山，方便debug
	Single getDora(int index)const; //获取宝牌
	Single getUra(int index)const; //获取里宝牌
	std::vector<Single> getDoras(int count)const; //获取宝牌组,count表示获得前几个
	std::vector<Single> getUras(int count)const; //获取里宝牌组,count表示获得前几个
	Single getDoraIndicator(int index)const; //获取宝牌指示牌,下标范围0~4
	Single getUraIndicator(int index)const; //获取里宝牌指示牌,下标范围0~4
	std::vector<Single> getDoraIndicators(int count)const; //获取宝牌指示牌组,count表示获得前几个
	std::vector<Single> getUraIndicators(int count)const; //获取里宝牌指示牌组,count表示获得前几个
	Single nextRinshan(); //获取下一张岭上牌(自动移海底牌)
	Single nextHand(); //获取下一张手牌
	int remainCount()const; //获取剩余牌数
	std::string getMountainString(); //获得牌山序列
};
