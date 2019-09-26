#pragma once
#include"Single.hpp"
#include"Rules.h"
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
Mountain::Mountain()
{
	mHandPtr = mLingshangPtr = 0;
}
std::string Mountain::getMountainString()//获得牌山序列
{
	std::string res;
	for (auto& item : mMountain)
	{
		int value = item.value();
		if (item.isAkadora())value = 0;
		res.push_back(value+'0');
		res.push_back(item.color());
	}
	return res;
}
Single Mountain::nextLingshang()//获取下一张岭上牌(自动移海底牌)
{
	Single res=mB[mLingshangPtr++];
	mB.push_back(mA[mA.size()-1]);
	mA.pop_back();
	return res;
}
Single Mountain::nextHand()//获取下一张手牌
{
	return mA[mHandPtr++];
}
int Mountain::remainCount() //获取剩余牌数
{
	return mA.size() - mHandPtr;
}
Single Mountain::getDora(int index) //获取宝牌
{
	return getDoraIndicator(index).next();
}
Single Mountain::getUra(int index) //获取里宝牌
{
	return getUraIndicator(index).next();
}
Single Mountain::getDoraIndicator(int index) //获取宝牌指示牌,下标范围0~4
{
	return mB[4 + index * 2];
}
Single Mountain::getUraIndicator(int index) //获取里宝牌指示牌,下标范围0~4
{
	return mB[5 + index * 2];
}
void Mountain::reset(const Rule& rule)
{
	mHandPtr = 0;
	mLingshangPtr = 0;
	mMountain.clear();
	mA.clear();
	mB.clear();
	for (int j = 0; j < 4; ++j)
		hand[j].clear();
	std::vector<Single>all;
	for (int i = 1; i <= 9; ++i)
	{
		for (int j = 1; j <= 4; ++j)
		{
			if (i==5 && j==1 && rule.doraCount > 0)//红宝牌
			{
				all.push_back(Single(i, 'm', true));
				all.push_back(Single(i, 'p', true));
				all.push_back(Single(i, 's', true));
			}
			else if(i==5 && j==2 && rule.doraCount == 4)//红宝牌5饼
			{
				all.push_back(Single(i, 'm',false));
				all.push_back(Single(i, 'p', true));
				all.push_back(Single(i, 's', false));
			}
			else
			{
				all.push_back(Single(i, 'm',false));
				all.push_back(Single(i, 'p', false));
				all.push_back(Single(i, 's', false));
			}
			if(i<=7)all.push_back(Single(i, 'z', false));
		}
	}
	std::random_shuffle(all.begin(), all.end());
	int ptr = 0;
	for (int i = 1; i <= 13; ++i)
		for (int j = 0; j < 4; ++j)
			hand[j].push_back(all[ptr++]);
	zimo=all[ptr++];
	for (int i = ptr; i < all.size(); ++i)
		mMountain.push_back(all[i]);
	for (int i = 1; i <= 69; ++i)
		mA.push_back(all[ptr++]);
	ptr = all.size();
	for (int i = 1; i <= 14; ++i)
		mB.push_back(all[--ptr]);
}
