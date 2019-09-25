#pragma once
#include "Single.hpp"
#include "Group.hpp"
#include<set>
#include<map>
#include<cassert>
#include<queue>
#include<algorithm>
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


	Tianhu = 0xd00,
	Dihu = 0xd01,
	Dasanyuan = 0xd02,
	Sianke = 0xd10,
	Ziyise = 0xd03,
	Lvyise = 0xd04,
	Qinglaotou = 0xd05,
	Guoshiwushuang = 0xd11,
	Xiaosixi = 0xd06,
	Sigangzi = 0xd07,
	Jiulianbaodeng = 0xd12,

	Siankedanqi = 0xf10,
	Guoshiwushuangshisanmian = 0xf11,
	Chunzhengjiulianbaodeng = 0xf12,
	Dasixi = 0xf00,
};
struct AgariResult
{
	bool zimo;
	int fangchongID;//放铳人ID 
	int hupaiID;//和牌人ID 
	int fan;//-1=役满，-2=两倍役满，-3=三倍役满，以此类推,13番累计役满
	int fu;
	int dora;
	int akadora;
	int uradora;
	int scoreAdd;//不包含立直棒和本场棒的计算
	int scoreDecFangchong;//自摸无效 
	int scoreDecZhuang;//荣和无效 
	int scoreDecXian;//荣和无效 
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
enum class AgariFaildReason
{
	Null,//空
	ShapeWrong,//不满足和牌型
	NoYaku,//无役
	NotEnoughFan,//番缚
	CantQiangAnGang//非国士无双不能抢暗杠
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
		if (!success && rhs.success)
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
struct AgariParameters
{
	WindType selfWind;
	WindType prevailingWind;
	int lizhiXunmu;
	bool yifa;
	int state;
	int type;
	Single target;
	std::vector<Single> handTile;
	std::vector<Group> groupTile;
	std::vector<Single> dora;
	std::vector<Single> ura;
	AgariParameters(
		WindType selfWind_,
		WindType prevailingWind_,
		int lizhiXunmu_,
		bool yifa_,
		int state_,
		int type_,
		const Single& target_,
		const std::vector<Single>& handTile_,
		const std::vector<Group>& groupTile_,
		const std::vector<Single>& dora_,
		const std::vector<Single>& ura_
	)
	{
		selfWind = selfWind_;
		prevailingWind = prevailingWind_;
		lizhiXunmu = lizhiXunmu_;
		yifa = yifa_;
		state = state_;
		type = type_;
		target = target_;
		handTile = handTile_;
		groupTile = groupTile_;
		dora = dora_;
		ura = ura_;
	}
};
class Algorithms
{
private:
	//判断是否为国士无双和牌型
	static bool guoshiWithoutYaku(const Single& target, const std::vector<Single>& handTile);
	//判断是否为七对和牌型
	static bool qiduiWithoutYaku(const Single& target, const std::vector<Single>& handTile);
	//判断是否为标准和牌型
	static bool agariSearchWithoutYaku(std::vector<int> pool);
	//判断是否形式和牌,不考虑役
	static bool agariWithoutYaku(const Single& target, const std::vector<Single>& handTile);
	//试图和牌，并计算国士无双型和牌点数
	static TryToAgariResult guoshiwushuang(const AgariParameters& par);
	//试图和牌，并计算标准型和牌点数，返回(点数最大的)结果,depth=0表示枚举雀头，dep=4为最深层
	static TryToAgariResult agariSearch(const AgariParameters& par, int depth, std::vector<Single> remainTiles, std::vector<Group> mianzi);
	//试图和牌，并计算七对型和牌点数
	static TryToAgariResult qidui(const AgariParameters& par);
	//构造给定数量的雀头和面子的牌型，存到queue中
	static void constructTarget(int quetou, int mianzi, std::queue<int>& q,int shape,int target,bool z);
	//5的幂
	static const int pow5[10];
	//到给定目标的距离，参数分别为原形状、目标雀头数*5+目标面子数(雀头最多只能有一个) 原形状为9位5进制数，从低到高表示手牌中数字1~9有几个
	static uint8_t distanceToTarget[1953125][10];
	//到给定目标的距离(字牌)，参数分别为原形状、目标雀头数*5+目标面子数(雀头最多只能有一个) 原形状为9位5进制数，从低到高表示手牌中东南西北白发中有几个
	static uint8_t distanceToTargetZ[78125][10];
public:
	
	//试图和牌，返回点数最大的结果
	static TryToAgariResult agari(const AgariParameters par);
	/*type
	自摸=0
	荣和=1
	抢杠=2
	抢暗杠=3
	*/
	//根据和牌结果计算打点
	static AgariResult getScore(WindType selfWind, AgariResult inp);
	//0~8 万字,9~17 饼子,18~26 索子,27~33 东南西北白发中
	static std::vector<int> getPool(const std::vector<Single>& tiles);
	//快速判断是否形式听牌。不允许虚听，允许空听。返回听牌种类
	static std::vector<Single> tingpai(std::vector<Single> handTile);
	//shape为压缩后的牌型参数，value为要获取个数的数字。函数对单花色牌的压缩型进行操作。
	static int getNumberCount(int shape, int value);
	static void setNumberCount(int& shape, int value, int count);
	static void addNumberCount(int& shape, int value, int count);
	//预处理，计算单花色离目标的距离(仅允许插入和删除两个操作)
	static void preprocessDistance();
	//计算14张手牌的标准型向听数(0为一向听，-1为和牌)
	static int getDistance14Standard(std::vector<Single>& handTile);
	//计算14张手牌的七对型向听数(0为一向听，-1为和牌)
	static int getDistance14Qidui(std::vector<Single>& handTile);
	//获得单花色向听数
	static int getDistanceSingle(int shape,int mianzi,int quetou,bool z);
}; 
uint8_t Algorithms::distanceToTarget[1953125][10];
uint8_t Algorithms::distanceToTargetZ[78125][10];
const int Algorithms::pow5[10] = { 1,5,25,125,625,3125,15625,78125,390625,1953125 };
//shape为压缩后的牌型参数，value为要获取个数的数字
int Algorithms::getNumberCount(int shape, int value)
{
	return (shape / pow5[value-1])%5;
}
//count不得超过4
void Algorithms::setNumberCount(int& shape, int value, int count)
{
	shape = (shape / pow5[value]) * pow5[value]+ count* pow5[value - 1]+shape % pow5[value-1];
}
void Algorithms::addNumberCount(int& shape, int value, int count)
{
	shape += pow5[value - 1];
}
std::vector<Single> Algorithms::tingpai(std::vector<Single> handTile)
{
	std::vector<Single> res;
	for (int i = 1; i <= 9; ++i)
	{
		if (agariWithoutYaku(Single(i, 'm', 0), handTile))
			res.push_back(Single(i, 'm', 0));
		if (agariWithoutYaku(Single(i, 'p', 0), handTile))
			res.push_back(Single(i, 'p', 0));
		if (agariWithoutYaku(Single(i, 's', 0), handTile))
			res.push_back(Single(i, 's', 0));
		if (i <= 7 && agariWithoutYaku(Single(i, 'z', 0), handTile))
			res.push_back(Single(i, 'z', 0));
	}
	return res;
}
std::vector<int> Algorithms::getPool(const std::vector<Single>& tiles)
{
	std::vector<int> res;
	res.resize(34);
	for (auto& item : tiles)
	{
		if (item.color() == 'm')
		{
			res[item.value() - 1]++;
		}
		else if (item.color() == 'p')
		{
			res[item.value() + 8]++;
		}
		else if (item.color() == 's')
		{
			res[item.value() + 17]++;
		}
		else if (item.color() == 'z')
		{
			res[item.value() + 26]++;
		}
		else
		{
			assert(0);
		}
	}
	return res;
}
AgariResult Algorithms::getScore(WindType selfWind, AgariResult inp)
{
	int east = selfWind == WindType::EAST;
	AgariResult res = inp;
	//役满型 ***暂时不考虑包牌*** 
	if (inp.fan < 0)
	{
		res.scoreAdd = (-inp.fan) * 16000 * (2 + east);
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
	//非役满型
	if (inp.fan < 1)res.scoreAdd = 0;
	if (inp.fan >= 13)
	{
		res.scoreAdd = 16000 * (2 + east);
		res.scoreDecZhuang = 16000;
		res.scoreDecXian = 8000 * (1 + east);
	}
	else if (inp.fan >= 11)
	{
		res.scoreAdd = 12000 * (2 + east);
		res.scoreDecZhuang = 12000;
		res.scoreDecXian = 6000 * (1 + east);
	}
	else if (inp.fan >= 8)
	{
		res.scoreAdd = 8000 * (2 + east);
		res.scoreDecZhuang = 8000;
		res.scoreDecXian = 4000 * (1 + east);
	}
	else if (inp.fan >= 6)
	{
		res.scoreAdd = 6000 * (2 + east);
		res.scoreDecZhuang = 6000;
		res.scoreDecXian = 3000 * (1 + east);
	}
	else if (inp.fan == 5)
	{
		res.scoreAdd = 4000 * (2 + east);
		res.scoreDecZhuang = 4000;
		res.scoreDecXian = 2000 * (1 + east);
	}
	else if (!inp.zimo)res.scoreAdd = (std::min(((inp.fu << (inp.fan + 4)) - 10), 7999) * (2 + east) / 200 + 1) * 100;
	else if (east)
	{
		res.scoreAdd = (std::min(((inp.fu << (inp.fan + 4)) - 10), 7999) / 200 + 1) * 300;
		res.scoreDecXian = (std::min(((inp.fu << (inp.fan + 4)) - 10), 7999) / 200 + 1) * 100;
	}
	else
	{
		res.scoreAdd = (std::min(((inp.fu << (inp.fan + 4)) - 10), 7999) / 200 + 1) * 100 + (std::min(((inp.fu << (inp.fan + 4)) - 10), 7999) / 400 + 1) * 200;
		res.scoreDecXian = (std::min(((inp.fu << (inp.fan + 4)) - 10), 7999) / 400 + 1) * 100;
		res.scoreDecZhuang = (std::min(((inp.fu << (inp.fan + 4)) - 10), 7999) / 200 + 1) * 100;
	}
	res.scoreDecFangchong = res.scoreAdd;//放铳失点等于和牌得点 
	return res;
}
TryToAgariResult Algorithms::agari(const AgariParameters par)
{
	//国士无双的判定
	AgariResult res;
	TryToAgariResult guoshi = guoshiwushuang(par);
	if (guoshi.success)
		return guoshi;
	//只有国士能抢暗杠
	if (par.state == 3)return TryToAgariResult(AgariFaildReason::CantQiangAnGang);
	//七对型的判定(两杯口将被拆解为七对型)
	TryToAgariResult qiduizi = qidui(par);
	//一般型的判定
	std::vector<Single>allTiles = par.handTile;
	allTiles.push_back(par.target);
	sort(allTiles.begin(), allTiles.end());
	TryToAgariResult normal = agariSearch(par, 0, allTiles, std::vector<Group>());
	return std::max(qiduizi, normal);
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
TryToAgariResult Algorithms::agariSearch(const AgariParameters& par, int depth, std::vector<Single> remainTiles, std::vector<Group> mianzi)
{
	TryToAgariResult bestResult;
	if (remainTiles.empty())
	{
		AgariResult result = AgariResult();
		int menqingCount = mianzi.size() - 1;//非副露的面子数
		mianzi.insert(mianzi.end(), par.groupTile.begin(), par.groupTile.end());
		if (mianzi.size() != 5)return TryToAgariResult(AgariFaildReason::ShapeWrong);
		//TODO:判断役种
		result.zimo = par.type == 0;
		//役满型 天和、地和
		if (par.state == 1 && par.type == 0)
		{
			if (par.selfWind == WindType::EAST)
				result.yaku.push_back(Yaku::Tianhu);
			else
				result.yaku.push_back(Yaku::Dihu);
			result.fan -= 1;
		}
		//满足役满型
		if (result.fan < 0)
		{
			return AgariResult(getScore(par.selfWind, result));
		}
		//检查dora，akadora和uradora
		auto myHandTile = par.handTile;
		myHandTile.push_back(par.target);
		for (auto& doraneko : par.dora)
			for (auto& item : myHandTile)
				if (doraneko.valueEqual(item))
					result.dora++;
		for (auto& doraneko : par.ura)
			for (auto& item : myHandTile)
				if (doraneko.valueEqual(item))
					result.uradora++;
		for (auto& item : myHandTile)
			if (item.isAkadora())
				result.akadora++;
		result.fan += result.dora + result.akadora + result.uradora;
		//平和和符数计算
		result.fu = 20;
		bool pinghu = true;
		//首先判断雀头是否为役牌
		if (mianzi[0].color == 'z')
		{
			if (mianzi[0].value >= 5)
			{
				pinghu = false;
				result.fu += 2;
			}
			if (mianzi[0].value == par.prevailingWind + 1)
			{
				result.fu += 2;
				pinghu = false;
			}
			if (mianzi[0].value == par.selfWind + 1)
			{
				result.fu += 2;
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
					result.fu += 2 * (mianzi[i].isyaojiu() ? 2 : 1) * (i <= menqingCount ? 2 : 1);
				if (mianzi[i].type == GroupType::Gang)
					result.fu += 8 * (mianzi[i].isyaojiu() ? 2 : 1) * (i <= menqingCount ? 2 : 1);
			}
		}
		//和牌方式是否为两面
		bool liangmianOnly = true;//是否只能被看成两面
		bool liangmian = false;//是否可以被看成两面
		for (int i = 1; i <= menqingCount; ++i)
		{
			if (mianzi[i].type == GroupType::Shunzi)
			{
				if (par.target.valueEqual(Single(mianzi[i].value, mianzi[i].color, false)))
					liangmian = true;
				if (par.target.valueEqual(Single(mianzi[i].value + 2, mianzi[i].color, false)))
					liangmian = true;
			}
			if (mianzi[i].type == GroupType::Quetou)
			{
				if (par.target.valueEqual(Single(mianzi[i].value, mianzi[i].color, false)))
					liangmianOnly = false;
			}
			if (mianzi[i].type == GroupType::Kezi || mianzi[i].type == GroupType::Gang)
			{
				if (par.target.valueEqual(Single(mianzi[i].value, mianzi[i].color, false)))
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
				result.fan++;
				result.yaku.push_back(Yaku::Pinghu);
				if (par.type == 1)result.fu += 10;
				//assert(res.result.fu == 20);
			}
			else //非门清,30符固定
				result.fu = 30;
		}
		else
		{
			if (liangmianOnly == false)
				result.fu += 2;
			//门清荣和+10符,自摸+2符
			if (menqingCount == 4)
			{
				if (par.type >= 1)
					result.fu += 10;
				else result.fu += 2;
			}
			else //副露自摸+2符
			{
				if (par.type == 0)
					result.fu += 2;
			}
		}
		if (result.fu % 10 != 0)result.fu = result.fu - result.fu % 10 + 10;
		//立直,两立直和一发
		if (par.lizhiXunmu != -1)
		{
			//w立
			if (par.lizhiXunmu == -2)
			{
				result.fan += 2;
				result.yaku.push_back(Yaku::Lianglizhi);
			}
			else
			{
				result.fan += 1;
				result.yaku.push_back(Yaku::Lizhi);
			}
			if (par.yifa == true)
			{
				result.fan += 1;
				result.yaku.push_back(Yaku::Yifa);
			}
		}
		//检查抢杠
		if (par.type == 2)
		{
			result.fan += 1;
			result.yaku.push_back(Yaku::Qianggang);
		}
		//检查门清自摸
		if (par.type == 0 && menqingCount == 4)
		{
			result.fan += 1;
			result.yaku.push_back(Yaku::Menqianqingzimo);
		}
		//检查河底/海底
		if (par.state == 2)
		{
			if (par.type == 0)
				result.yaku.push_back(Yaku::Haidilaoyue);
			else if (par.type == 1)result.yaku.push_back(Yaku::Hedilaoyu);
		}
		result = getScore(par.selfWind, result);
		//std::cout << "*" << std::endl;
		if (result.fan == 0)
		{
			return TryToAgariResult(AgariFaildReason::NoYaku);
		}

		return TryToAgariResult(result);
	}
	if (depth == 0)
	{
		for (int i = 0; i < remainTiles.size() - 1; ++i)
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
				bestResult = std::max(bestResult, agariSearch(par, depth + 1, newRemainTiles, mianzi));
				mianzi.pop_back();
			}
		}
		return bestResult;
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
				bestResult = std::max(bestResult,agariSearch(par, depth + 3, newRemainTiles, mianzi));
				mianzi.pop_back();
				mianzi.pop_back();
				mianzi.pop_back();
				//三同顺
				mianzi.push_back(Group::createKezi(group[0][0], group[1][0], group[2][0], 0));
				mianzi.push_back(Group::createKezi(group[0][1], group[1][1], group[2][1], 0));
				mianzi.push_back(Group::createKezi(group[0][2], group[1][2], group[2][2], 0));
				bestResult = std::max(bestResult, agariSearch(par, depth + 3, newRemainTiles, mianzi));
				return bestResult;
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
				if (item.valueEqual(standard) && count > 0)
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
			bestResult = agariSearch(par, depth + 1, newRemainTiles, mianzi);
			return bestResult;
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
				bestResult = agariSearch(par, depth + 1, newRemainTiles, mianzi);
				return bestResult;
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
TryToAgariResult Algorithms::qidui(const AgariParameters& par)
{
	AgariResult res = AgariResult();
	std::map<int, int>all;
	for (auto& item : par.handTile)
		all[item.color() * 100 + item.value()]++;
	all[par.target.color() * 100 + par.target.value()]++;
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
	if (par.state == 1 && par.type == 0)
	{
		if (par.selfWind == WindType::EAST)
			res.yaku.push_back(Yaku::Tianhu);
		else
			res.yaku.push_back(Yaku::Dihu);
		res.fan -= 1;
	}
	bool ziyise = true;
	for (auto& item : par.handTile)
	{
		if (item.color() != 'z')
		{
			ziyise = false;
			break;
		}
	}
	if (par.target.color() != 'z')
		ziyise = false;
	if (ziyise == true)
	{
		res.fan -= 1;
		res.yaku.push_back(Yaku::Ziyise);
	}
	res.zimo = par.type == 0;
	//满足役满型
	if (res.fan < 0)
	{
		res = getScore(par.selfWind,res);
		return TryToAgariResult(res);
	}
	//不满足役满型，则一定需要计算七对型
	res.fan += 2;
	res.fu = 25;
	res.yaku.push_back(Yaku::Qiduizi);
	//检查dora，akadora和uradora
	auto myHandTile = par.handTile;
	myHandTile.push_back(par.target);
	for (auto& doraneko : par.dora)
		for (auto& item : myHandTile)
			if (doraneko.valueEqual(item))
				res.dora++;
	for (auto& doraneko : par.ura)
		for (auto& item : myHandTile)
			if (doraneko.valueEqual(item))
				res.uradora++;
	for (auto& item : myHandTile)
		if (item.isAkadora())
			res.akadora++;
	res.fan += res.dora + res.akadora + res.uradora;
	//检查立直,两立直和一发
	if (par.lizhiXunmu != -1)
	{
		//w立
		if (par.lizhiXunmu == -2)
		{
			res.fan += 2;
			res.yaku.push_back(Yaku::Lianglizhi);
		}
		else
		{
			res.fan += 1;
			res.yaku.push_back(Yaku::Lizhi);
		}
		if (par.yifa == true)
		{
			res.fan += 1;
			res.yaku.push_back(Yaku::Yifa);
		}
	}
	//检查抢杠
	if (par.type == 2)
	{
		res.fan += 1;
		res.yaku.push_back(Yaku::Qianggang);
	}
	//检查门清自摸
	if (par.type == 0)
	{
		res.fan += 1;
		res.yaku.push_back(Yaku::Menqianqingzimo);
	}
	//检查河底/海底
	if (par.state == 2)
	{
		if (par.type == 0)
			res.yaku.push_back(Yaku::Haidilaoyue);
		else if (par.type == 1)res.yaku.push_back(Yaku::Hedilaoyu);
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
	res = getScore(par.selfWind,res);
	return TryToAgariResult(res);
}
//判断是否为国士无双和牌型
TryToAgariResult Algorithms::guoshiwushuang(const AgariParameters& par)
{
	AgariResult res = AgariResult();
	int guoshi = 0;
	if (par.groupTile.empty())
	{
		std::set<Single>all;
		for (auto& item : par.handTile)
			if (item.value() == 1 || item.value() == 9 || item.color() == 'z')
				all.insert(item);
		//国士
		if (all.size() == 12)
		{
			if (par.target.value() == 1 || par.target.value() == 9 || par.target.color() == 'z')
			{
				all.insert(par.target);
				if (all.size() == 13)
					guoshi = 1;
			}
		}
		//国士十三面
		else if (all.size() == 13)
		{
			if (par.target.value() == 1 || par.target.value() == 9 || par.target.color() == 'z')
				guoshi = 2;
		}
	}
	//可以复合天地和
	if (par.state == 1 && par.type == 0)
	{
		if (par.selfWind == WindType::EAST)
		{
			if(guoshi != 0)
				guoshi = 2;
			res.yaku.push_back(Yaku::Tianhu);
		}
		else
			res.yaku.push_back(Yaku::Dihu);
		if(guoshi>0)
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
	res.zimo = (par.type == 0);
	res = getScore(par.selfWind,res);
	return TryToAgariResult(res);
}
bool Algorithms::guoshiWithoutYaku(const Single& target, const std::vector<Single>& handTile)
{
	bool res = false;
	int guoshi = 0;
	if (handTile.size() == 13)
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
	return guoshi > 0;
}
bool Algorithms::qiduiWithoutYaku(const Single& target, const std::vector<Single>& handTile)
{
	bool res = false;
	std::map<int, int>all;
	for (auto& item : handTile)
		all[item.color() * 100 + item.value()]++;
	all[target.color() * 100 + target.value()]++;
	if (all.size() != 7)return false;
	for (auto& item : all)
		if (item.second != 2)
			return false;
	return true;
}
bool Algorithms::agariSearchWithoutYaku(std::vector<int> pool)
{
	//判断是否虚和(一种牌拿5张)
	for (int i = 0; i <= 33; ++i)
		if (pool[i] >= 5)
			return false;
	//判断雀头
	for (int i = 0; i <= 33; ++i)
	{
		if (pool[i] >= 2)
		{
			std::vector<int> tempPool = pool;
			tempPool[i] -= 2;
			//刻子
			for (int i = 0; i <= 33; ++i)
				if (tempPool[i] >= 3)
					tempPool[i] -= 3;
			//顺子
			for (int i = 0; i <= 33; ++i)
				if (i != 7 && i != 8 && i != 16 && i != 17 && i <= 24)
					if (tempPool[i] > 0 && tempPool[i + 1] > 0 && tempPool[i + 2] > 0)
					{
						tempPool[i]--;
						tempPool[i + 1]--;
						tempPool[i + 2]--;
					}
			//存在浮牌
			bool res = true;
			for (int i = 0; i <= 33; ++i)
				if (tempPool[i] > 0)
				{
					res = false;
					break;
				}
			if (res)return true;
		}
	}
	return false;
}
bool Algorithms::agariWithoutYaku(const Single& target, const std::vector<Single>& handTile)
{
	//国士无双型的判定
	bool guoshi = guoshiWithoutYaku(target, handTile);
	if (guoshi)return true;
	//七对型的判定(两杯口将被拆解为七对型)
	bool qiduizi = qiduiWithoutYaku(target, handTile);
	if (qiduizi)
		return true;
	//一般型的判定
	std::vector<Single>allTiles = handTile;
	allTiles.push_back(target);
	sort(allTiles.begin(), allTiles.end());
	auto pool = getPool(allTiles);
	bool normal = agariSearchWithoutYaku(pool);
	return std::max(qiduizi, normal);
}
void Algorithms::constructTarget(int quetou,int mianzi, std::queue<int>&q,int shape, int target,bool z)
{
	int nextShape;
	if (quetou > 0)
	{
		for(int value=1;value<= (z ? 7 : 9);++value)
		{
			nextShape = shape;
			int count = getNumberCount(shape, value);
			if (count <= 2)
			{
				setNumberCount(nextShape, value, count + 2);
				constructTarget(quetou - 1, mianzi, q, nextShape,target,z);
			}
		}
	}
	if (mianzi > 0)
	{
		//刻子
		for (int value = 1; value <= (z?7:9); ++value)
		{
			nextShape = shape;
			int count = getNumberCount(shape, value);
			if (count <= 1)
			{
				setNumberCount(nextShape, value, count + 3);
				constructTarget(quetou, mianzi-1, q, nextShape,target,z);
			}
		}
		//顺子
		if (!z)
		{
			for (int value = 1; value <= 7; ++value)
			{
				nextShape = shape;
				int count0 = getNumberCount(shape, value);
				int count1 = getNumberCount(shape, value + 1);
				int count2 = getNumberCount(shape, value + 2);
				if (count0 <= 3 && count1 <= 3 && count2 <= 3)
				{
					setNumberCount(nextShape, value, count0 + 1);
					setNumberCount(nextShape, value + 1, count1 + 1);
					setNumberCount(nextShape, value + 2, count2 + 1);
					constructTarget(quetou, mianzi - 1, q, nextShape, target,z);
				}
			}
		}
	}
	if (mianzi > 0 || quetou > 0)return;
	q.push(shape);
	if(!z)distanceToTarget[shape][target] = 0;
	else distanceToTargetZ[shape][target] = 0;
}
//预处理，计算单花色离目标的距离(仅允许插入和删除两个操作)
void Algorithms::preprocessDistance()
{
	for (int j = 0; j < 10; ++j)
	{
		for (int i = 0; i < pow5[9]; ++i)
			distanceToTarget[i][j] = 255;
		for (int i = 0; i < pow5[7]; ++i)
			distanceToTargetZ[i][j] = 255;
	}
		
	//枚举目标
	for (int target = 0; target < 10; ++target)
	{
		int quetou = target / 5;
		int mianzi = target % 5;
		//构造目标(同时也是BFS的起点)
		std::queue<int>q;
		constructTarget(quetou, mianzi, q,0,target,0);
		while (!q.empty())
		{
			int now = q.front();
			q.pop();
			for (int value = 1; value <= 9; ++value)
			{
				int next;
				int count = getNumberCount(now, value);
				if (count > 0)
				{
					next = now;
					setNumberCount(next, value,count - 1);
					if (distanceToTarget[next][target] == 255)
					{
						distanceToTarget[next][target] = distanceToTarget[now][target] + 1;
						q.push(next);
					}
				}
				if (count < 4)
				{
					next = now;
					setNumberCount(next, value, count + 1);
					if (distanceToTarget[next][target] == 255)
					{
						distanceToTarget[next][target] = distanceToTarget[now][target] + 1;
						q.push(next);
					}
				}
			}
		}
		//字牌
		constructTarget(quetou, mianzi, q, 0, target, 1);
		while (!q.empty())
		{
			int now = q.front();
			q.pop();
			for (int value = 1; value <= 7; ++value)
			{
				int next;
				int count = getNumberCount(now, value);
				if (count > 0)
				{
					next = now;
					setNumberCount(next, value, count - 1);
					if (distanceToTargetZ[next][target] == 255)
					{
						distanceToTargetZ[next][target] = distanceToTargetZ[now][target] + 1;
						q.push(next);
					}
				}
				if (count < 4)
				{
					next = now;
					setNumberCount(next, value, count + 1);
					if (distanceToTargetZ[next][target] == 255)
					{
						distanceToTargetZ[next][target] = distanceToTargetZ[now][target] + 1;
						q.push(next);
					}
				}
			}
		}
	}
}
//获得单花色向听数
int Algorithms::getDistanceSingle(int shape,int mianzi,int quetou, bool z)
{
	if (!z)return distanceToTarget[shape][mianzi+quetou*5];
	return distanceToTargetZ[shape][mianzi + quetou * 5];
}
//计算14张手牌的向听数(0为一向听，-1为和牌)
int Algorithms::getDistance14Standard(std::vector<Single>& handTile)
{	
	int shape[5] = { 0,0,0,0,0 };//1~4分别表示mpsz，其中z的格式为5位16进制数,从低到高分别为出现了0,1,2,3,4张的牌共有几种,高位无意义
	for (auto& item:handTile)
	{
		if (item.color() == 'm')
			addNumberCount(shape[1], item.value(),1);
		if (item.color() == 'p')
			addNumberCount(shape[2], item.value(), 1);
		if (item.color() == 's')
			addNumberCount(shape[3], item.value(), 1);
		if (item.color() == 'z')
			addNumberCount(shape[4], item.value(), 1);
	}
	//计算标准型向听数
	int dp[5][5][2];//a当前已经完成的花色数，b当前已经完成的面子数，c当前已经完成的雀头数,dp表示最小距离
	//int pre[5][5][2];
	for (int i = 0; i < 5; ++i)
		for (int j = 0; j < 5; ++j)
			for (int k = 0; k < 2; ++k)
			{
				dp[i][j][k] = 99;//此距离最大为18，表示8向听
				//pre[i][j][k] = -1;
			}
	dp[0][0][0] = 0;//设定边界值，0花色0面子0雀头距离为0
	for (int a = 1; a < 5; ++a)
		for (int b = 0; b < 5; ++b)
			for (int k = 0; k < 5&&b-k>=0; ++k)//枚举这一种花色要达成的面子数
			{
				dp[a][b][0] = std::min(dp[a][b][0], dp[a - 1][b - k][0] + getDistanceSingle(shape[a], k, 0, a == 4));
				dp[a][b][1] = std::min(dp[a][b][1], dp[a - 1][b - k][0] + getDistanceSingle(shape[a], k, 1, a == 4));
				dp[a][b][1] = std::min(dp[a][b][1], dp[a - 1][b - k][1] + getDistanceSingle(shape[a], k, 0, a == 4));
				/*
				if (dp[a][b][0] > dp[a - 1][b - k][0] + getDistanceSingle(shape[a], k, 0, a == 4))
				{
					dp[a][b][0] = dp[a - 1][b - k][0] + getDistanceSingle(shape[a], k, 0, a == 4);
					pre[a][b][0] = (b - k);
				}
				if (dp[a][b][1]> dp[a - 1][b - k][0] + getDistanceSingle(shape[a], k, 1, a == 4))
				{
					dp[a][b][1] = dp[a - 1][b - k][0] + getDistanceSingle(shape[a], k, 1, a == 4);
					pre[a][b][1] = (b - k);
				}
				if (dp[a][b][1]> dp[a - 1][b - k][1] + getDistanceSingle(shape[a], k, 0, a == 4))
				{
					dp[a][b][1]= dp[a - 1][b - k][1] + getDistanceSingle(shape[a], k, 0, a == 4);
					pre[a][b][1] = (b - k)+5;
				}*/
			}
	/*
	int a = 4, b = 4, c = 1;
	std::cout << a << " " << b << " " << c << " " << dp[a][b][c] << std::endl;
	while (1)
	{
		int pa = a, pb = b, pc = c;
		a--;
		b = pre[pa][pb][pc] % 5;
		c = pre[pa][pb][pc] / 5;
		if (a >= 0 && b >= 0 && c >= 0)
			std::cout << a << " " << b << " " << c << " " << dp[a][b][c] << std::endl;
		else break;
	}
	*/
	/*
	for (int k = 0; k < 2; ++k)
	{
		for (int i = 0; i < 5; ++i)
		{
			for (int j = 0; j < 5; ++j)
				std::cout << dp[i][j][k] << " ";
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}*/
	return dp[4][4][1]/2-1;
}
//计算14张手牌的向听数(0为一向听，-1为和牌)
int Algorithms::getDistance14Qidui(std::vector<Single>& handTile)
{
	int shape[5] = { 0,0,0,0,0 };//1~4分别表示mpsz，其中z的格式为5位16进制数,从低到高分别为出现了0,1,2,3,4张的牌共有几种,高位无意义
	int count[8];//字牌个数统计
	memset(count, 0, sizeof(count));
	for (auto& item : handTile)
	{
		if (item.color() == 'm')
			addNumberCount(shape[1], item.value(), 1);
		if (item.color() == 'p')
			addNumberCount(shape[2], item.value(), 1);
		if (item.color() == 's')
			addNumberCount(shape[3], item.value(), 1);
		if (item.color() == 'z')
			count[item.value()]++;
	}
	for (int i = 1; i <= 7; ++i)
		shape[4] += 1 << (count[i] << 2);
	//计算七对子向听数
	int duiziCount = 0;
	for (int i = 1; i <= 3; ++i)
		for (int j = 1; j <= 9; ++j)
			if (getNumberCount(shape[i], j) >= 2)
				duiziCount++;
	duiziCount += (shape[4] >> 8) % 0xF;
	return 6 - duiziCount;
}