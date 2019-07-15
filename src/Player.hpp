#pragma once
#include"Single.hpp"
#include"Group.hpp"
#include<vector>
#include<algorithm>
#include<string>
struct AgariResult
{

};
class Player
{
public:
	int subround; //巡数
	int score;
	WindType selfWind;   //门风
	std::vector<Single> handTile;//手牌
	std::vector<bool> disabledHandTile;//禁打手牌
	std::vector<Group> groupTile;//副露牌组
	Single nowTile;//自摸牌
	std::vector<Single>discardTile;//牌河
	bool lingshang;//玩家当前的自摸牌是岭上牌
	bool moqie=false;//表示牌河里最后一张牌是否为摸切
	int lizhi=-1;//立直宣言牌是牌河中的第几张，-1为未立直
	void setNowTile(Single target,bool isLingshang)
	{
		lingshang = isLingshang;
		nowTile = target;
	}
	bool canRong(Single target)
	{
		return false;
	}
	//relativePosition 0,1,2,3分别表示下对上自
	bool canChi(Single target, Group result, int relativePosition)
	{
		return false;
	}
	bool canPeng(Single target, Group result,int relativePosition)
	{
		//检查类型
		if (result.type != GroupType::Kezi)
			return false;
		//检查颜色
		if (target.color() != result.color)
			return false;
		//检查数值
		if (target.value() != result.value)
			return false;
		//检查位置
		if (relativePosition != result.state%10)
			return false;
		//检查红宝牌是否正确
		if ((result.akadora & 1) != target.isAkadora())
			return false;
		int akadoraCount = (result.akadora&2)+(result.akadora&4);
		int realAkadoraCount = 0;
		int tileCount = 0;
		for (auto& item : handTile)
		{
			if (item.color() == result.color&&item.value() == result.value)
			{
				tileCount++; 
				if (item.isAkadora())
					realAkadoraCount++;
			}
		}
		//牌数不够
		if (tileCount < 2)return false;
		//红宝牌不够
		if (realAkadoraCount < akadoraCount)
			return false;
		//非红宝牌不够
		if ((tileCount - realAkadoraCount) < (2 - akadoraCount))
			return false;
		return true;
	}
	bool canGang(Single target,Group result, int relativePosition)
	{
		return false;
	}
	AgariResult rong(Single target)
	{
		return AgariResult();
	}
	void peng(Group result)
	{
		groupTile.push_back(result);
		Single other0 = Single(result.value, result.color, result.akadora & 2);
		Single other1 = Single(result.value, result.color, result.akadora & 4);
		bool ok0=false, ok1=false;
		for (auto& item : handTile)
		{
			if (!ok0&&other0 == item)
			{
				ok0 = true;
				item = Null;
			}else if (!ok1&&other1 == item)
			{
				ok1 = true;
				item = Null;
			}
		}
		sort(handTile.begin(), handTile.end());
		while (handTile.back() == Null)handTile.pop_back();
	}
	void gang(Group result)
	{

	}
	void chi(Group result)
	{

	}
	//返回false时不应变动私有变量
	bool dapai(Single target)//打牌
	{
		if (target == Null)return false;
		if (nowTile == target)
		{
			discardTile.push_back(nowTile);
			moqie = true;
			nowTile = Null;
			return true;
		}
		for (int i=0;i<handTile.size();++i)
		{
			if (target == handTile[i])
			{
				discardTile.push_back(target);
				handTile[i] = nowTile;
				std::sort(handTile.begin(),handTile.end());//从小到大排序
				if (nowTile == Null)
					handTile.pop_back();
				nowTile == Null;
				moqie = false;
				return true;
			}
		}
		return false;
	}
	//返回false时不应变动私有变量
	bool zimo()
	{
		return false;
	}
	//返回false时不应变动私有变量
	bool liuju()
	{
		return false;
	}
	//返回false时不应变动私有变量
	bool doLizhi()
	{
		return false;
	}
};