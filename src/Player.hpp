#pragma once
#include"Single.hpp"
#include"Group.hpp"
#include"Game.hpp"
#include"MovementGenerator.hpp"
#include<vector>
#include<algorithm>
#include<string>
#include<map>
#include<set>
#include<cassert>
#include"Algorithms.hpp"
class Player
{
	friend class Game;
private:
	int subround; //巡数
	int score;
	WindType selfWind;   //门风
	std::vector<Single> handTile;//手牌
	std::vector<bool> disabledHandTile;//禁打手牌
	std::vector<Group> groupTile;//副露牌组
	Single nowTile;//自摸牌
	std::vector<Single>discardTile;//牌河
	bool lingshang;//玩家当前的自摸牌是岭上牌
	bool moqie = false;//表示牌河里最后一张牌是否为摸切
	int lizhi = -1;//立直宣言牌是牌河中的第几张，-1为未立直
	int lizhiXunmu = -1;//立直的巡目,从0开始,-1为未立直,-2为w立
	bool yifa;//当前是否为一发巡
public:
	void setInfo(
	int subround_, //巡数
	int score_,
	WindType selfWind_,   //门风
	const std::vector<Single>& handTile_,//手牌
	const std::vector<bool>& disabledHandTile_,//禁打手牌
	const std::vector<Group>& groupTile_,//副露牌组
	const Single& nowTile_,//自摸牌
	const std::vector<Single>&discardTile_,//牌河
	bool lingshang_,//玩家当前的自摸牌是岭上牌
	bool moqie_,//表示牌河里最后一张牌是否为摸切
	int lizhi_,//立直宣言牌是牌河中的第几张，-1为未立直
	int lizhiXunmu_,//立直的巡目,从0开始,-1为未立直,-2为w立
	bool yifa_//当前是否为一发巡
	)
	{
		subround = subround_;//巡数
		score = score_;
		selfWind = selfWind_;   //门风
		handTile = handTile_;//手牌
		disabledHandTile = disabledHandTile_;//禁打手牌
		groupTile = groupTile_;//副露牌组
		nowTile = nowTile_;//自摸牌
		discardTile = discardTile_;//牌河
		lingshang = lingshang_;//玩家当前的自摸牌是岭上牌
		moqie = moqie_;//表示牌河里最后一张牌是否为摸切
		lizhi = lizhi_;//立直宣言牌是牌河中的第几张，-1为未立直
		lizhiXunmu = lizhiXunmu_;//立直的巡目,从0开始,-1为未立直,-2为w立
		yifa = yifa;//当前是否为一发巡
	}
	void setNowTile(Single target,bool isLingshang)
	{
		lingshang = isLingshang;
		nowTile = target;
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
		if (relativePosition != result.state)
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
	//仅限荣杠
	bool canGang(Single target,Group result, int relativePosition)
	{
		//检查类型
		if (result.type != GroupType::Gang)
			return false;
		//检查颜色
		if (target.color() != result.color)
			return false;
		//检查数值
		if (target.value() != result.value)
			return false;
		//检查位置
		if (relativePosition != result.state)
			return false;
		//检查红宝牌是否正确
		if ((result.akadora & 1) != target.isAkadora())
			return false;
		int akadoraCount = (result.akadora & 2) + (result.akadora & 4) + (result.akadora & 8);
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
		if (tileCount < 3)return false;
		//红宝牌不够
		if (realAkadoraCount < akadoraCount)
			return false;
		//非红宝牌不够
		if ((tileCount - realAkadoraCount) < (3 - akadoraCount))
			return false;
		return true;
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
	void ronggang(Group result)
	{
		groupTile.push_back(result);
		Single other0 = Single(result.value, result.color, result.akadora & 2);
		Single other1 = Single(result.value, result.color, result.akadora & 4);
		Single other2 = Single(result.value, result.color, result.akadora & 8);
		bool ok0 = false, ok1 = false,ok2=false;
		for (auto& item : handTile)
		{
			if (!ok0&&other0 == item)
			{
				ok0 = true;
				item = Null;
			}
			else if (!ok1&&other1 == item)
			{
				ok1 = true;
				item = Null;
			}
			else if (!ok2&&other2 == item)
			{
				ok2 = true;
				item = Null;
			}
		}
		sort(handTile.begin(), handTile.end());
		while (handTile.back() == Null)handTile.pop_back();
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
		//立直必须摸切
		if (lizhiXunmu != -1)return false;
		for (int i=0;i<handTile.size();++i)
		{
			if (target == handTile[i])
			{
				discardTile.push_back(target);
				handTile[i] = nowTile;
				std::sort(handTile.begin(),handTile.end());//从小到大排序
				if (nowTile == Null)
					handTile.pop_back();
				nowTile = Null;
				moqie = false;
				return true;
			}
		}
		return false;
	}
	//返回false时不应变动私有变量
	/*
	state
	正常=0
	天地和,w立=1
	河底/海底=2
	*/
	TryToAgariResult zimo(WindType prevailingWind,int state, const std::vector<Single>&dora, const std::vector<Single>&ura)
	{
		return Algorithms::agari(AgariParameters(selfWind,prevailingWind,lizhiXunmu,yifa, state, 0, nowTile,handTile,groupTile, dora, ura));
	}
	AgariResult rong(Single target,WindType prevailingWind,int type, int state, const std::vector<Single>&dora, const std::vector<Single>&ura)
	{
		return Algorithms::agari(AgariParameters(selfWind, prevailingWind, lizhiXunmu, yifa, state, type, target, handTile, groupTile, dora, ura)).result;
	}
	bool canRong(Single target, WindType prevailingWind, int type, int state, const std::vector<Single>&dora, const std::vector<Single>&ura)
	{
		return Algorithms::agari(AgariParameters(selfWind, prevailingWind, lizhiXunmu, yifa, state, type, target, handTile, groupTile, dora, ura)).success;
	}
	//返回false时不应变动私有变量
	bool liuju()
	{
		return false;
	}
	bool tingpai()
	{
		return !Algorithms::tingpai(handTile).empty();
	}
	//返回false时不应变动私有变量
	bool doLizhi(int state, Single target)
	{
		//要打的牌是否存在?
		if (target == Null)return false;
		std::vector<Single> myTile = handTile;
		myTile.push_back(nowTile);
		bool success = false;
		for (int i = 0; i<myTile.size(); ++i)
		{
			if (target == myTile[i])
			{
				myTile[i] = Null;
				std::sort(myTile.begin(), myTile.end());//从小到大排序,Null一定被排到最大一个
				myTile.pop_back();
				success = true;
				break;
			}
		}
		if (!success)return false;
		//是否非暗杠副露？
		if (!groupTile.empty())
		{
			for (auto& item : groupTile)
			{
				//暗杠必须满足type=Gang且state=0
				if (item.type != GroupType::Gang|| item.state != 0)
					return false;
			}
		}
		//是否已经立直过了？
		if (lizhiXunmu != -1)
			return false;
		//点棒够不够?
		if (score < 1000)
			return false;
		//是否听牌?
		auto backup = handTile;
		handTile = myTile;
		bool isTingpai = tingpai();
		handTile = backup;
		if (isTingpai)
		{
			assert(dapai(target));
			lizhiXunmu = subround;
			yifa = true;
			lizhi = discardTile.size()-1;
			if (state == 1)
				lizhiXunmu = -2;
			score -= 1000;
			return true;
		}
		return false;
	}
};
