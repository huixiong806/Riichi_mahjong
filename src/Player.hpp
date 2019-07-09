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
		return false;
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
		
	}
	void gang(Group result)
	{

	}
	void chi(Group result)
	{

	}
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
	bool zimo()
	{
		return false;
	}
	bool liuju()
	{
		return false;
	}
	bool doLizhi()
	{
		return false;
	}
};