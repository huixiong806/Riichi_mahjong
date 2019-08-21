#pragma once
#include"Single.hpp"
#include"Group.hpp"
#include<vector>
#include<algorithm>
#include<string>
#include<map>
#include<set>
#include<cassert>
#include"Algorithms.h"
//三位16进制数，第一位为番数(役满记d,两倍役满记f)，第二位:0=非食下役/食下役的门清版，1=门清限定，2=食下役的非门清版,第三位为编号
enum class Yaku
{
	Lizhi = 0x110,
	Yifa = 0x111,
	Menqianqingzimo = 0x112,
	Duanyaojiu = 0x100,
	Yipaizifeng = 0x101,
	Yipaichangfeng = 0x102,
	Yipaibai = 0x103,
	Yipaifa = 0x104,
	Yipaizhong = 0x105,
	Pinghu = 0x113,
	Yibeikou = 0x114,
	Qianggang = 0x106,
	Lingshangkaihua = 0x107,
	Haidilaoyue = 0x108,
	Hedilaoyu = 0x109,

	Lianglizhi = 0x210,
	Sansetongke = 0x200,
	Sangangzi = 0x201,
	Duiduihu = 0x202,
	Sananke = 0x203,
	Xiaosanyuan = 0x204,
	Hunlaotou = 0x205,
	Qiduizi = 0x211,

	Hunquandaiyaojiu = 0x206,
	HunquandaiyaojiuF = 0x226,
	Yiqitongguan = 0x207,
	YiqitongguanF = 0x227,
	Sansetongshun = 0x208,
	SansetongshunF = 0x228,

	Erbeikou = 0x310,
	Chunquantaiyaojiu = 0x300,
	ChunquantaiyaojiuF = 0x320,
	Hunyise = 0x301,
	HunyiseF = 0x321,

	Liujumanguan = 0x500,
	Qingyise = 0x600,
	QingyiseF = 0x620,
	

	Tianhu=0xd00,
	Dihu=0xd01,
	Dasanyuan=0xd02,
	Sianke=0xd10,
	Ziyise=0xd03,
	Lvyise=0xd04,
	Qinglaotou=0xd05,
	Guoshiwushuang=0xd11,
	Xiaosixi=0xd06,
	Sigangzi=0xd07,
	Jiulianbaodeng=0xd12,

	Siankedanqi=0xf10,
	Guoshiwushuangshisanmian=0xf11,
	Chunzhengjiulianbaodeng=0xf12,
	Dasixi=0xf00,
};
enum class AgariFaildReason
{
	Null,//空
	ShapeWrong,//不满足和牌型
	NoYaku,//无役
	NotEnoughFan,//番缚
	CantQiangAnGang//非国士无双不能抢暗杠
};
struct AgariResult
{
	bool zimo;
	int fan;//-1=役满，-2=两倍役满，-3=三倍役满，以此类推,13番累计役满
	int fu;
	int dora;
	int akadora;
	int uradora;
	int scoreAdd;//不包含立直棒和本场棒的计算
	int scoreDecFangchong;//自摸为0
	int scoreDecZhuang;//荣和为0
	int scoreDecXian;//荣和为0
	//int nukidora;
	AgariResult()
	{
		zimo = fan = fu = dora = akadora = uradora = scoreAdd = scoreDecFangchong = scoreDecZhuang = scoreDecXian = 0;
	}
	std::vector<Yaku> yaku;
	bool operator <(const AgariResult& rhs)const
	{
		return scoreAdd < rhs.scoreAdd;
	}
	bool operator ==(const AgariResult& rhs)const
	{
		return scoreAdd == rhs.scoreAdd;
	}
};
struct TryToAgariResult
{
	AgariResult result;
	bool success;
	AgariFaildReason reason;
	bool operator <(const TryToAgariResult& rhs)const
	{

		if (!success && !rhs.success)
			return reason < rhs.reason;
		if (success && !rhs.success)
			return false;
		if (!success&&rhs.success)
			return true;
		return result < rhs.result;
	}
	TryToAgariResult()
	{
		success = false;
		result = AgariResult();
	}
	TryToAgariResult(AgariFaildReason reason_)
	{
		success = false;
		reason = reason_;
	}
	TryToAgariResult(AgariResult result_)
	{
		success = true;
		result = result_;
	}
};

class Player
{
private:
	//判断是否为国士无双和牌型
	TryToAgariResult guoshiwushuang(int state, int type, const Single& target)const
	{
		AgariResult res=AgariResult();
		int guoshi = 0;
		if (groupTile.empty())
		{
			std::set<Single>all;
			for (auto& item : handTile)
				if (item.value() == 1 || item.value() == 9 || item.color() == 'z')
					all.insert(item);
			//国士
			if (all.size() == 12)
			{
				if (target.value() == 1 || target.value() == 9 || target.color() == 'z')
				{
					all.insert(target);
					if (all.size() == 13)
						guoshi = 1;
				}
			}
			//国士十三面
			else if (all.size() == 13)
			{
				if (target.value() == 1 || target.value() == 9 || target.color() == 'z')
					guoshi = 2;
			}
		}
		//可以复合天地和
		if (state == 1 && type == 0)
		{
			if (selfWind == WindType::EAST)
				res.yaku.push_back(Yaku::Tianhu);
			else
				res.yaku.push_back(Yaku::Dihu);
			res.fan -= 1;
		}
		if (guoshi == 1)
		{
			res.yaku.push_back(Yaku::Guoshiwushuang);
			res.fan -= 1;
		}
		else if (guoshi == 2)
		{
			res.yaku.push_back(Yaku::Guoshiwushuangshisanmian);
			res.fan -= 2;
		}
		else
		{
			return TryToAgariResult(AgariFaildReason::ShapeWrong);
		}
		res.zimo = (type == 0);
		res = getScore(res);
		return TryToAgariResult(res);
	}
	//判断是否为标准和牌型，返回(点数最大的)结果,depth=0表示枚举雀头，dep=4为最深层
	/*type
	自摸=0
	荣和=1
	抢杠=2
	抢暗杠=3
	state
	正常=0
	天地和=1
	河底/海底=2
	*/
	TryToAgariResult agariSearch(WindType prevailingWind, int state, int type, const Single& target, const std::vector<Single>&dora, const std::vector<Single>&ura, int depth, std::vector<Single> remainTiles, std::vector<Group> mianzi)const;
	//判断是否为七对和牌型
	TryToAgariResult qidui(int state, int type, const Single& target, const std::vector<Single>&dora, const std::vector<Single>&ura)const;
	TryToAgariResult agari(WindType prevailingWind,int state, int type, const Single& target, const std::vector<Single>&dora, const std::vector<Single>&ura)
	{
		//国士无双的判定
		AgariResult res;
		TryToAgariResult guoshi = guoshiwushuang(state, type, target);
		if (guoshi.success)
			return guoshi;
		//只有国士能抢暗杠
		if (state == 3)return TryToAgariResult(AgariFaildReason::CantQiangAnGang);
		//七对型的判定(两杯口将被拆解为七对型)
		TryToAgariResult qiduizi = qidui(state, type, target, dora, ura);
		//一般型的判定
		std::vector<Single>allTiles = handTile;
		allTiles.push_back(target);
		sort(allTiles.begin(), allTiles.end());
		TryToAgariResult normal = agariSearch(prevailingWind,state, type, target, dora, ura, 0, allTiles, std::vector<Group>());
		return std::max(qiduizi, normal);
	}
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
	int lizhiXunmu=-1;//立直的巡目,从0开始,-1为未立直,-2为w立
	bool yifa;//当前是否为一发巡
	
	/*type
	自摸=0
	荣和=1
	抢杠=2
	抢暗杠=3
	*/
	//只计算打点，不负责计算番和符
	AgariResult getScore(AgariResult inp)const
	{
		int east = selfWind == WindType::EAST;
		AgariResult res=inp;
		//役满型
		if (inp.fan < 0)
		{
			res.scoreAdd = (-inp.fan)*16000*(2+east);
			if (!inp.zimo)
				res.scoreDecFangchong = res.scoreAdd;
			else if (east)
				res.scoreDecXian = (-inp.fan) * 16000;
			else
			{
				res.scoreDecXian = (-inp.fan) * 8000;
				res.scoreDecZhuang = (-inp.fan) * 16000;
			}
			return res;
		}
		//非役满型 TODO:算scoreDec
		if (inp.fan<1)res.scoreAdd= 0;
		if (inp.fan >= 13)res.scoreAdd = 16000 * (2 + east);
		else if (inp.fan >= 11)res.scoreAdd = 12000 * (2 + east);
		else if (inp.fan >= 8)res.scoreAdd = 8000 * (2 + east);
		else if (inp.fan >= 6)res.scoreAdd = 6000 * (2 + east);
		else if (inp.fan == 5)res.scoreAdd = 4000 * (2 + east);
		else if (!inp.zimo)res.scoreAdd = (std::min(((inp.fu << (inp.fan + 4)) - 10), 7999)*(2 + east) / 200 + 1) * 100;
		else if (east)res.scoreAdd = (std::min(((inp.fu << (inp.fan + 4)) - 10), 7999) / 200 + 1) * 300;
		else res.scoreAdd = (std::min(((inp.fu << (inp.fan + 4)) - 10), 7999) / 200 + 1) * 100 + (std::min(((inp.fu << (inp.fan + 4)) - 10), 7999) / 400 + 1) * 200;
		return res;
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
		return agari(prevailingWind, state, 0, nowTile, dora, ura);
	}
	AgariResult rong(Single target,WindType prevailingWind,int type, int state, const std::vector<Single>&dora, const std::vector<Single>&ura)
	{
		return agari(prevailingWind, state, type, target, dora, ura).result;
	}
	bool canRong(Single target, WindType prevailingWind, int type, int state, const std::vector<Single>&dora, const std::vector<Single>&ura)
	{
		return agari(prevailingWind, state, type, target, dora, ura).success;
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
//判断是否为标准和牌型，返回(点数最大的)结果,depth=0表示枚举雀头，dep=4为最深层
/*type
自摸=0
荣和=1
抢杠=2
抢暗杠=3
state
正常=0
天地和=1
河底/海底=2
*/
TryToAgariResult Player::agariSearch(WindType prevailingWind, int state, int type, const Single& target, const std::vector<Single>&dora, const std::vector<Single>&ura, int depth, std::vector<Single> remainTiles, std::vector<Group> mianzi)const
{
	TryToAgariResult res;
	if (remainTiles.empty())
	{
		AgariResult result = AgariResult();
		int menqingCount = mianzi.size() - 1;//非副露的面子数
		mianzi.insert(mianzi.end(), groupTile.begin(), groupTile.end());
		if (mianzi.size() != 5)return TryToAgariResult(AgariFaildReason::ShapeWrong);
		//TODO:判断役种
		res.result.zimo = type == 0;
		//役满型 天和、地和
		if (state == 1 && type == 0)
		{
			if (selfWind == WindType::EAST)
				res.result.yaku.push_back(Yaku::Tianhu);
			else
				res.result.yaku.push_back(Yaku::Dihu);
			res.result.fan -= 1;
		}
		//满足役满型
		if (res.result.fan < 0)
		{
			res.result = getScore(res.result);
			return res;
		}
		//检查dora，akadora和uradora
		auto myHandTile = handTile;
		myHandTile.push_back(target);
		for (auto& doraneko : dora)
			for (auto& item : myHandTile)
				if (doraneko.valueEqual(item))
					res.result.dora++;
		for (auto& doraneko : ura)
			for (auto& item : myHandTile)
				if (doraneko.valueEqual(item))
					res.result.uradora++;
		for (auto& item : myHandTile)
			if (item.isAkadora())
				res.result.akadora++;
		res.result.fan += res.result.dora + res.result.akadora + res.result.uradora;
		//平和和符数计算
		res.result.fu = 20;
		bool pinghu = true;
		//首先判断雀头是否为役牌
		if (mianzi[0].color == 'z')
		{
			if (mianzi[0].value >= 5)
			{
				pinghu = false;
				res.result.fu += 2;
			}
			if (mianzi[0].value == prevailingWind + 1)
			{
				res.result.fu += 2;
				pinghu = false;
			}
			if (mianzi[0].value == selfWind + 1)
			{
				res.result.fu += 2;
				pinghu = false;
			}
		}
		//判断是否有刻子
		for (int i = 1; i <= 4; ++i)
		{
			if (mianzi[i].type != GroupType::Shunzi)
			{
				pinghu = false;
				if (mianzi[i].type == GroupType::Kezi)
					res.result.fu += 2 * (mianzi[i].isyaojiu() ? 2 : 1)*(i <= menqingCount ? 2 : 1);
				if (mianzi[i].type == GroupType::Gang)
					res.result.fu += 8 * (mianzi[i].isyaojiu() ? 2 : 1)*(i <= menqingCount ? 2 : 1);
			}
		}
		//和牌方式是否为两面
		bool liangmianOnly = true;//是否只能被看成两面
		bool liangmian = false;//是否可以被看成两面
		for (int i = 1; i <= menqingCount; ++i)
		{
			if (mianzi[i].type == GroupType::Shunzi)
			{
				if (target.valueEqual(Single(mianzi[i].value, mianzi[i].color, false)))
					liangmian = true;
				if (target.valueEqual(Single(mianzi[i].value + 2, mianzi[i].color, false)))
					liangmian = true;
			}
			if (mianzi[i].type == GroupType::Quetou)
			{
				if (target.valueEqual(Single(mianzi[i].value, mianzi[i].color, false)))
					liangmianOnly = false;
			}
			if (mianzi[i].type == GroupType::Kezi || mianzi[i].type == GroupType::Gang)
			{
				if (target.valueEqual(Single(mianzi[i].value, mianzi[i].color, false)))
					liangmianOnly = false;
			}
		}
		if (liangmian == false)
			pinghu = false;
		//为平和型
		if (pinghu)
		{
			//门清，附加平和一役
			if (menqingCount == 4)
			{
				res.result.fan++;
				res.result.yaku.push_back(Yaku::Pinghu);
				if (type == 1)res.result.fu += 10;
				//assert(res.result.fu == 20);
			}
			else //非门清,30符固定
				res.result.fu = 30;
		}
		else
		{
			if (liangmianOnly == false)
				res.result.fu += 2;
			//门清荣和+10符,自摸+2符
			if (menqingCount == 4)
			{
				if (type >= 1)
					res.result.fu += 10;
				else res.result.fu += 2;
			}
			else //副露自摸+2符
			{
				if (type == 0)
					res.result.fu += 2;
			}
		}
		//立直,两立直和一发
		if (lizhiXunmu != -1)
		{
			//w立
			if (lizhiXunmu == -2)
			{
				res.result.fan += 2;
				res.result.yaku.push_back(Yaku::Lianglizhi);
			}
			else
			{
				res.result.fan += 1;
				res.result.yaku.push_back(Yaku::Lizhi);
			}
			if (yifa == true)
			{
				res.result.fan += 1;
				res.result.yaku.push_back(Yaku::Yifa);
			}
		}
		//检查抢杠
		if (type == 2)
		{
			res.result.fan += 1;
			res.result.yaku.push_back(Yaku::Qianggang);
		}
		//检查门清自摸
		if (type == 0 && menqingCount == 4)
		{
			res.result.fan += 1;
			res.result.yaku.push_back(Yaku::Menqianqingzimo);
		}
		//检查河底/海底
		if (state == 2)
		{
			if (type == 0)
				res.result.yaku.push_back(Yaku::Haidilaoyue);
			else if (type == 1)res.result.yaku.push_back(Yaku::Hedilaoyu);
		}
		result = getScore(res.result);
		//std::cout << "*" << std::endl;
		if (result.fan == 0)
		{
			return TryToAgariResult(AgariFaildReason::NoYaku);
		}

		return TryToAgariResult(result);
	}
	if (depth == 0)
	{
		for (int i = 0; i<remainTiles.size() - 1; ++i)
		{
			if (remainTiles[i] == remainTiles[i + 1])
			{
				std::vector<Single> newRemainTiles;
				for (int j = 0; j < remainTiles.size(); ++j)
				{
					if (j != i && j != i + 1)
						newRemainTiles.push_back(remainTiles[j]);
				}
				mianzi.push_back(Group::createQuetou(remainTiles[i], remainTiles[i + 1]));
				res = std::max(res, agariSearch(prevailingWind, state, type, target, dora, ura, depth + 1, newRemainTiles, mianzi));
				mianzi.pop_back();
			}
		}
		return res;
	}
	std::vector<int> pool = Algorithms::getPool(remainTiles);
	//判断顺子和刻子
	for (int i = 0; i <= 33; ++i)
	{
		//唯一的分歧点，三连刻和三同顺
		if (i != 7 && i != 8 && i != 16 && i != 17 && i <= 24)
		{
			if (pool[i] >= 3 && pool[i + 1] >= 3 && pool[i + 2] >= 3)
			{
				Single standard[3];
				if (i < 7)
				{
					standard[0] = Single(i + 1, 'm', 0);
					standard[1] = Single(i + 2, 'm', 0);
					standard[2] = Single(i + 3, 'm', 0);
				}
				else if (i < 16)
				{
					standard[0] = Single(i - 8, 'p', 0);
					standard[1] = Single(i - 7, 'p', 0);
					standard[2] = Single(i - 6, 'p', 0);
				}
				else
				{
					standard[0] = Single(i - 17, 's', 0);
					standard[1] = Single(i - 16, 's', 0);
					standard[2] = Single(i - 15, 's', 0);
				}
				std::vector<Single> newRemainTiles;
				std::vector<Single> group[3];
				int count[3] = { 3,3,3 };
				for (auto& item : remainTiles)
				{
					bool equalToStandard = false;
					for (int j = 0; j < 3; ++j)
					{
						if (item.valueEqual(standard[j]) && count[j] > 0)
						{
							count[j]--;
							group[j].push_back(item);
							equalToStandard = true;
						}
					}
					if (!equalToStandard)
						newRemainTiles.push_back(item);
				}
				//三连刻
				mianzi.push_back(Group::createKezi(group[0][0], group[0][1], group[0][2], 0));
				mianzi.push_back(Group::createKezi(group[1][0], group[1][1], group[1][2], 0));
				mianzi.push_back(Group::createKezi(group[2][0], group[2][1], group[2][2], 0));
				res = agariSearch(prevailingWind, state, type, target, dora, ura, depth + 3, newRemainTiles, mianzi);
				mianzi.pop_back();
				mianzi.pop_back();
				mianzi.pop_back();
				//三同顺
				mianzi.push_back(Group::createKezi(group[0][0], group[1][0], group[2][0], 0));
				mianzi.push_back(Group::createKezi(group[0][1], group[1][1], group[2][1], 0));
				mianzi.push_back(Group::createKezi(group[0][2], group[1][2], group[2][2], 0));
				res = std::max(res, agariSearch(prevailingWind, state, type, target, dora, ura, depth + 3, newRemainTiles, mianzi));
				return res;
			}
		}
		//刻子
		if (pool[i] >= 3)
		{
			std::vector<Single> newRemainTiles;
			std::vector<Single> kezi;
			int count = 3;
			Single standard;
			if (i <= 8)standard = Single(i + 1, 'm', 0);
			else if (i <= 17)standard = Single(i - 8, 'p', 0);
			else if (i <= 26)standard = Single(i - 17, 's', 0);
			else standard = Single(i - 26, 'z', 0);
			for (auto& item : remainTiles)
			{
				if (item.valueEqual(standard) && count>0)
				{
					kezi.push_back(item);
					count--;
					continue;
				}
				else
				{
					newRemainTiles.push_back(item);
				}
			}
			mianzi.push_back(Group::createKezi(kezi[0], kezi[1], kezi[2], 0));
			res = agariSearch(prevailingWind, state, type, target, dora, ura, depth + 1, newRemainTiles, mianzi);
			return res;
		}
		//顺子
		if (i != 7 && i != 8 && i != 16 && i != 17 && i <= 24)
		{
			if (pool[i] > 0 && pool[i + 1] > 0 && pool[i + 2] > 0)
			{
				Single standard[3];
				if (i < 7)
				{
					standard[0] = Single(i + 1, 'm', 0);
					standard[1] = Single(i + 2, 'm', 0);
					standard[2] = Single(i + 3, 'm', 0);
				}
				else if (i < 16)
				{
					standard[0] = Single(i - 8, 'p', 0);
					standard[1] = Single(i - 7, 'p', 0);
					standard[2] = Single(i - 6, 'p', 0);
				}
				else
				{
					standard[0] = Single(i - 17, 's', 0);
					standard[1] = Single(i - 16, 's', 0);
					standard[2] = Single(i - 15, 's', 0);
				}
				std::vector<Single> newRemainTiles;
				std::vector<Single> group;
				group.resize(3);
				int count[3] = { 1,1,1 };
				for (auto& item : remainTiles)
				{
					bool equalToStandard = false;
					for (int j = 0; j < 3; ++j)
					{
						if (item.valueEqual(standard[j]) && count[j] > 0)
						{
							count[j]--;
							group[j] = item;
							equalToStandard = true;
						}
					}
					if (!equalToStandard)
						newRemainTiles.push_back(item);
				}
				//顺子
				mianzi.push_back(Group::createShunzi(group[0], group[1], group[2], 0));
				res = agariSearch(prevailingWind, state, type, target, dora, ura, depth + 1, newRemainTiles, mianzi);
				return res;
			}
		}
		//存在浮牌
		if (pool[i] > 0)
		{
			return TryToAgariResult(AgariFaildReason::ShapeWrong);
		}
	}
	assert(0);
	return TryToAgariResult(AgariFaildReason::ShapeWrong);
}
TryToAgariResult Player::qidui(int state, int type, const Single& target, const std::vector<Single>&dora, const std::vector<Single>&ura)const
{
	AgariResult res = AgariResult();
	std::map<int, int>all;
	for (auto& item : handTile)
		all[item.color() * 100 + item.value()]++;
	all[target.color() * 100 + target.value()]++;
	if (all.size() != 7)return TryToAgariResult(AgariFaildReason::ShapeWrong);
	for (auto& item : all)
	{
		if (item.second != 2)
		{
			return TryToAgariResult(AgariFaildReason::ShapeWrong);
		}
	}
	//检查可以和七对子复合的役种:dora,ura,aka,立直,一发,两立直,门清自摸,抢杠,断幺九,清一色,海底摸月,河底捞鱼,混老头,混一色
	//役满型:天和,地和,字一色
	if (state == 1 && type == 0)
	{
		if (selfWind == WindType::EAST)
			res.yaku.push_back(Yaku::Tianhu);
		else
			res.yaku.push_back(Yaku::Dihu);
		res.fan -= 1;
	}
	bool ziyise = true;
	for (auto& item : handTile)
	{
		if (item.color() != 'z')
		{
			ziyise = false;
			break;
		}
	}
	if (target.color() != 'z')
		ziyise = false;
	if (ziyise == true)
	{
		res.fan -= 1;
		res.yaku.push_back(Yaku::Ziyise);
	}
	res.zimo = type == 0;
	//满足役满型
	if (res.fan < 0)
	{
		res = getScore(res);
		return TryToAgariResult(res);
	}
	//不满足役满型，则一定需要计算七对型
	res.fan += 2;
	res.fu = 25;
	res.yaku.push_back(Yaku::Qiduizi);
	//检查dora，akadora和uradora
	auto myHandTile = handTile;
	myHandTile.push_back(target);
	for (auto& doraneko : dora)
		for (auto& item : myHandTile)
			if (doraneko.valueEqual(item))
				res.dora++;
	for (auto& doraneko : ura)
		for (auto& item : myHandTile)
			if (doraneko.valueEqual(item))
				res.uradora++;
	for (auto& item : myHandTile)
		if (item.isAkadora())
			res.akadora++;
	res.fan += res.dora + res.akadora + res.uradora;
	//检查立直,两立直和一发
	if (lizhiXunmu != -1)
	{
		//w立
		if (lizhiXunmu == -2)
		{
			res.fan += 2;
			res.yaku.push_back(Yaku::Lianglizhi);
		}
		else
		{
			res.fan += 1;
			res.yaku.push_back(Yaku::Lizhi);
		}
		if (yifa == true)
		{
			res.fan += 1;
			res.yaku.push_back(Yaku::Yifa);
		}
	}
	//检查抢杠
	if (type == 2)
	{
		res.fan += 1;
		res.yaku.push_back(Yaku::Qianggang);
	}
	//检查门清自摸
	if (type == 0)
	{
		res.fan += 1;
		res.yaku.push_back(Yaku::Menqianqingzimo);
	}
	//检查河底/海底
	if (state == 2)
	{
		if (type == 0)
			res.yaku.push_back(Yaku::Haidilaoyue);
		else if (type == 1)res.yaku.push_back(Yaku::Hedilaoyu);
	}
	//检查清一色，混一色，断幺九，混老头
	bool qingyise = true, hunyise = true, duanyao = true, hunlaotou = true;
	char color = '0';
	for (auto& item : myHandTile)
	{
		if (item.color() == 'z')
		{
			duanyao = false;
			qingyise = false;
		}
		else
		{
			if (color == '0')
				color = item.color();
			else if (color != item.color())
			{
				qingyise = false;
				hunyise = false;
			}
		}
		if (item.value() == 1 || item.value() == 9)
			duanyao = false;
		if (item.value() != 1 && item.value() != 9)
			hunlaotou = false;
	}
	if (qingyise)
	{
		res.fan += 6;
		res.yaku.push_back(Yaku::Qingyise);
	}
	else if (hunyise)
	{
		res.fan += 3;
		res.yaku.push_back(Yaku::Hunyise);
	}
	if (duanyao)
	{
		res.fan += 1;
		res.yaku.push_back(Yaku::Duanyaojiu);
	}
	if (hunlaotou)
	{
		res.fan += 2;
		res.yaku.push_back(Yaku::Hunlaotou);
	}
	res = getScore(res);
	return TryToAgariResult(res);
}