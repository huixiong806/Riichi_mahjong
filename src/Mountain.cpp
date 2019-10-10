#include "Mountain.h"
#include <algorithm>
#include <random>

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
	std::vector<Single> all;
	for (int i = 1; i <= 9; ++i)
	{
		for (int j = 1; j <= 4; ++j)
		{
			if (i==5 && j==1 && rule.doraCount > 0)//红宝牌
			{
				all.emplace_back(i, 'm', true);
				all.emplace_back(i, 'p', true);
				all.emplace_back(i, 's', true);
			}
			else if(i==5 && j==2 && rule.doraCount == 4)//红宝牌5饼
			{
				all.emplace_back(i, 'm',false);
				all.emplace_back(i, 'p', true);
				all.emplace_back(i, 's', false);
			}
			else
			{
				all.emplace_back(i, 'm',false);
				all.emplace_back(i, 'p', false);
				all.emplace_back(i, 's', false);
			}
			if(i<=7)all.emplace_back(i, 'z', false);
		}
	}
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(all.begin(), all.end(), g);
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
