#pragma once
#include"Single.h"
#include"Rules.h"
#include"ResultInspector.hpp"
#include<vector>
#include<string>
#include<algorithm>
class Mountain
{
private:
protected:
	std::vector<Single>mA;        //四家摸牌
	std::vector<Single>mB;        //岭上，dora指示牌
	std::vector<Single>mMountain;  //牌山序列
	int mHandPtr;  //手牌指针
	int mLingshangPtr;  //岭上牌指针
public:
	std::vector<Single>hand[4];  //四家初始手牌
	Single zimo;//庄家0巡的自摸牌
	Mountain();//构造函数
	virtual void reset(const Rule& rule);  //产生新牌山
	Single getDora(int index); //获取宝牌
	Single getUra(int index); //获取里宝牌
	Single getDoraIndicator(int index); //获取宝牌指示牌,下标范围0~4
	Single getUraIndicator(int index); //获取里宝牌指示牌,下标范围0~4
	Single nextLingshang();//获取下一张岭上牌(自动移海底牌)
	Single nextHand();//获取下一张手牌
	int remainCount(); //获取剩余牌数
	std::string getMountainString();//获得牌山序列
};