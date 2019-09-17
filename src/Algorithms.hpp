#pragma once
#include "Single.hpp"
#include "Group.hpp"
#include<set>
#include<map>
#include<cassert>
//��λ16����������һλΪ����(������d,����������f)���ڶ�λ:0=��ʳ����/ʳ���۵�����棬1=�����޶���2=ʳ���۵ķ������,����λΪ���
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
	int fangchongID;//�����ID 
	int hupaiID;//������ID 
	int fan;//-1=������-2=����������-3=�����������Դ�����,13���ۼ�����
	int fu;
	int dora;
	int akadora;
	int uradora;
	int scoreAdd;//��������ֱ���ͱ������ļ���
	int scoreDecFangchong;//������Ч 
	int scoreDecZhuang;//�ٺ���Ч 
	int scoreDecXian;//�ٺ���Ч 
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
	Null,//��
	ShapeWrong,//�����������
	NoYaku,//����
	NotEnoughFan,//����
	CantQiangAnGang//�ǹ�ʿ��˫����������
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
	����=0
	�ٺ�=1
	����=2
	������=3
	state
	����=0
	��غ�=1
	�ӵ�/����=2
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
	//�ж��Ƿ�Ϊ��ʿ��˫������
	static bool guoshiWithoutYaku(const Single& target,const std::vector<Single>&handTile)
	{
		bool res = false;
		int guoshi = 0;
		if (handTile.size()==13)
		{
			std::set<Single>all;
			for (auto& item : handTile)
				if (item.value() == 1 || item.value() == 9 || item.color() == 'z')
					all.insert(item);
			//��ʿ
			if (all.size() == 12)
			{
				if (target.value() == 1 || target.value() == 9 || target.color() == 'z')
				{
					all.insert(target);
					if (all.size() == 13)
						guoshi = 1;
				}
			}
			//��ʿʮ����
			else if (all.size() == 13)
			{
				if (target.value() == 1 || target.value() == 9 || target.color() == 'z')
					guoshi = 2;
			}
		}
		return guoshi > 0;
	}
	static bool qiduiWithoutYaku(const Single& target, const std::vector<Single>&handTile)
	{
		bool res=false;
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
	//�жϱ�׼�͵���ʽ����
	static bool agariSearchWithoutYaku(std::vector<int> pool, int depth, int remainTiles)
	{
		bool res=false;
		if (remainTiles == 0)
			return true;
		if (depth == 0)
		{
			//�ж��Ƿ����(һ������5��)
			for (int i = 0; i <= 33; ++i)
				if (pool[i] >= 5)
					return false;
			//�ж�ȸͷ
			for (int i = 0; i <= 33; ++i)
			{
				if (pool[i]>=2)
				{
					pool[i] -= 2;
					res = std::max(res, agariSearchWithoutYaku(pool, depth + 1, remainTiles-2));
					if (res)return true;
					else pool[i] += 2;
				}
			}
			return false;
		}
		//�ж�˳�ӺͿ���
		for (int i = 0; i <= 33; ++i)
		{
			if (pool[i] >= 3)
			{
				pool[i] -= 3;
				remainTiles -= 3;
				res = std::max(res, agariSearchWithoutYaku(pool,depth + 1, remainTiles-3));
				return res;
			}
			//˳��
			if (i != 7 && i != 8 && i != 16 && i != 17 && i <= 24)
			{
				if (pool[i] > 0 && pool[i + 1] > 0 && pool[i + 2] > 0)
				{
					pool[i]--;
					pool[i + 1]--;
					pool[i + 2]--;
					res = std::max(res, agariSearchWithoutYaku(pool, depth + 1, remainTiles - 3));
					return res;
				}
			}
			//���ڸ���
			if (pool[i] > 0)
				return false;
		}
		assert(0);
		return false;
	}
	//�ж��Ƿ���ʽ����,��������
	static bool agariWithoutYaku(const Single& target, const std::vector<Single>&handTile)
	{
		//��ʿ��˫�͵��ж�
		bool guoshi = guoshiWithoutYaku(target,handTile);
		if (guoshi)return true;
		//�߶��͵��ж�(�����ڽ������Ϊ�߶���)
		bool qiduizi = qiduiWithoutYaku(target, handTile);
		if (qiduizi)
			return true;
		//һ���͵��ж�
		std::vector<Single>allTiles = handTile;
		allTiles.push_back(target);
		sort(allTiles.begin(), allTiles.end());
		auto pool = getPool(allTiles);
		bool normal = agariSearchWithoutYaku(pool, 0, allTiles.size());
		return std::max(qiduizi, normal);
	}
	//�ж��Ƿ�δ��ʿ��˫������
	static TryToAgariResult guoshiwushuang(const AgariParameters& par);
	//�ж��Ƿ�Ϊ��׼�����ͣ�����(��������)���,depth=0��ʾö��ȸͷ��dep=4Ϊ�����
	static TryToAgariResult agariSearch(const AgariParameters& par, int depth, std::vector<Single> remainTiles, std::vector<Group> mianzi);
	//�ж��Ƿ�Ϊ�߶Ժ�����
	static TryToAgariResult qidui(const AgariParameters& par);
public:
	static TryToAgariResult agari(const AgariParameters par);
	/*type
	����=0
	�ٺ�=1
	����=2
	������=3
	*/
	//ֻ�����㣬��������㷬�ͷ�
	static AgariResult getScore(WindType selfWind,AgariResult inp)
	{
		int east = selfWind == WindType::EAST;
		AgariResult res = inp;
		//������ ***��ʱ�����ǰ���*** 
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
		//��������
		if (inp.fan < 1)res.scoreAdd = 0;
		if (inp.fan >= 13)
		{
			res.scoreAdd = 16000 * (2 + east);
			res.scoreDecZhuang = 16000;
			res.scoreDecXian = 8000*(1 + east);
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
		res.scoreDecFangchong = res.scoreAdd;//���ʧ����ں��Ƶõ� 
		return res;
	}
	//0~8 ����,9~17 ����,18~26 ����,27~33 ���������׷���
	static std::vector<int> getPool(const std::vector<Single>& tiles)
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
	//�����ж��Ƿ���ʽ���ơ����������������������������������
	static std::vector<Single> tingpai(std::vector<Single> handTile)
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
};
TryToAgariResult Algorithms::agari(const AgariParameters par)
{
	//��ʿ��˫���ж�
	AgariResult res;
	TryToAgariResult guoshi = guoshiwushuang(par);
	if (guoshi.success)
		return guoshi;
	//ֻ�й�ʿ��������
	if (par.state == 3)return TryToAgariResult(AgariFaildReason::CantQiangAnGang);
	//�߶��͵��ж�(�����ڽ������Ϊ�߶���)
	TryToAgariResult qiduizi = qidui(par);
	//һ���͵��ж�
	std::vector<Single>allTiles = par.handTile;
	allTiles.push_back(par.target);
	sort(allTiles.begin(), allTiles.end());
	TryToAgariResult normal = agariSearch(par, 0, allTiles, std::vector<Group>());
	return std::max(qiduizi, normal);
}
//�ж��Ƿ�Ϊ��׼�����ͣ�����(��������)���,depth=0��ʾö��ȸͷ��dep=4Ϊ�����
/*type
����=0
�ٺ�=1
����=2
������=3
state
����=0
��غ�=1
�ӵ�/����=2
*/
TryToAgariResult Algorithms::agariSearch(const AgariParameters& par, int depth, std::vector<Single> remainTiles, std::vector<Group> mianzi)
{
	TryToAgariResult res;
	if (remainTiles.empty())
	{
		AgariResult result = AgariResult();
		int menqingCount = mianzi.size() - 1;//�Ǹ�¶��������
		mianzi.insert(mianzi.end(), par.groupTile.begin(), par.groupTile.end());
		if (mianzi.size() != 5)return TryToAgariResult(AgariFaildReason::ShapeWrong);
		//TODO:�ж�����
		res.result.zimo = par.type == 0;
		//������ ��͡��غ�
		if (par.state == 1 && par.type == 0)
		{
			if (par.selfWind == WindType::EAST)
				res.result.yaku.push_back(Yaku::Tianhu);
			else
				res.result.yaku.push_back(Yaku::Dihu);
			res.result.fan -= 1;
		}
		//����������
		if (res.result.fan < 0)
		{
			res.result = getScore(par.selfWind,res.result);
			return res;
		}
		//���dora��akadora��uradora
		auto myHandTile = par.handTile;
		myHandTile.push_back(par.target);
		for (auto& doraneko : par.dora)
			for (auto& item : myHandTile)
				if (doraneko.valueEqual(item))
					res.result.dora++;
		for (auto& doraneko : par.ura)
			for (auto& item : myHandTile)
				if (doraneko.valueEqual(item))
					res.result.uradora++;
		for (auto& item : myHandTile)
			if (item.isAkadora())
				res.result.akadora++;
		res.result.fan += res.result.dora + res.result.akadora + res.result.uradora;
		//ƽ�ͺͷ�������
		res.result.fu = 20;
		bool pinghu = true;
		//�����ж�ȸͷ�Ƿ�Ϊ����
		if (mianzi[0].color == 'z')
		{
			if (mianzi[0].value >= 5)
			{
				pinghu = false;
				res.result.fu += 2;
			}
			if (mianzi[0].value == par.prevailingWind + 1)
			{
				res.result.fu += 2;
				pinghu = false;
			}
			if (mianzi[0].value == par.selfWind + 1)
			{
				res.result.fu += 2;
				pinghu = false;
			}
		}
		//�ж��Ƿ��п���
		for (int i = 1; i <= 4; ++i)
		{
			if (mianzi[i].type != GroupType::Shunzi)
			{
				pinghu = false;
				if (mianzi[i].type == GroupType::Kezi)
					res.result.fu += 2 * (mianzi[i].isyaojiu() ? 2 : 1) * (i <= menqingCount ? 2 : 1);
				if (mianzi[i].type == GroupType::Gang)
					res.result.fu += 8 * (mianzi[i].isyaojiu() ? 2 : 1) * (i <= menqingCount ? 2 : 1);
			}
		}
		//���Ʒ�ʽ�Ƿ�Ϊ����
		bool liangmianOnly = true;//�Ƿ�ֻ�ܱ���������
		bool liangmian = false;//�Ƿ���Ա���������
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
		//Ϊƽ����
		if (pinghu)
		{
			//���壬����ƽ��һ��
			if (menqingCount == 4)
			{
				res.result.fan++;
				res.result.yaku.push_back(Yaku::Pinghu);
				if (par.type == 1)res.result.fu += 10;
				//assert(res.result.fu == 20);
			}
			else //������,30���̶�
				res.result.fu = 30;
		}
		else
		{
			if (liangmianOnly == false)
				res.result.fu += 2;
			//�����ٺ�+10��,����+2��
			if (menqingCount == 4)
			{
				if (par.type >= 1)
					res.result.fu += 10;
				else res.result.fu += 2;
			}
			else //��¶����+2��
			{
				if (par.type == 0)
					res.result.fu += 2;
			}
		}
		//��ֱ,����ֱ��һ��
		if (par.lizhiXunmu != -1)
		{
			//w��
			if (par.lizhiXunmu == -2)
			{
				res.result.fan += 2;
				res.result.yaku.push_back(Yaku::Lianglizhi);
			}
			else
			{
				res.result.fan += 1;
				res.result.yaku.push_back(Yaku::Lizhi);
			}
			if (par.yifa == true)
			{
				res.result.fan += 1;
				res.result.yaku.push_back(Yaku::Yifa);
			}
		}
		//�������
		if (par.type == 2)
		{
			res.result.fan += 1;
			res.result.yaku.push_back(Yaku::Qianggang);
		}
		//�����������
		if (par.type == 0 && menqingCount == 4)
		{
			res.result.fan += 1;
			res.result.yaku.push_back(Yaku::Menqianqingzimo);
		}
		//���ӵ�/����
		if (par.state == 2)
		{
			if (par.type == 0)
				res.result.yaku.push_back(Yaku::Haidilaoyue);
			else if (par.type == 1)res.result.yaku.push_back(Yaku::Hedilaoyu);
		}
		result = getScore(par.selfWind,res.result);
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
				res = std::max(res, agariSearch(par, depth + 1, newRemainTiles, mianzi));
				mianzi.pop_back();
			}
		}
		return res;
	}
	std::vector<int> pool = Algorithms::getPool(remainTiles);
	//�ж�˳�ӺͿ���
	for (int i = 0; i <= 33; ++i)
	{
		//Ψһ�ķ���㣬�����̺���ͬ˳
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
				//������
				mianzi.push_back(Group::createKezi(group[0][0], group[0][1], group[0][2], 0));
				mianzi.push_back(Group::createKezi(group[1][0], group[1][1], group[1][2], 0));
				mianzi.push_back(Group::createKezi(group[2][0], group[2][1], group[2][2], 0));
				res = agariSearch(par, depth + 3, newRemainTiles, mianzi);
				mianzi.pop_back();
				mianzi.pop_back();
				mianzi.pop_back();
				//��ͬ˳
				mianzi.push_back(Group::createKezi(group[0][0], group[1][0], group[2][0], 0));
				mianzi.push_back(Group::createKezi(group[0][1], group[1][1], group[2][1], 0));
				mianzi.push_back(Group::createKezi(group[0][2], group[1][2], group[2][2], 0));
				res = std::max(res, agariSearch(par, depth + 3, newRemainTiles, mianzi));
				return res;
			}
		}
		//����
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
			res = agariSearch(par, depth + 1, newRemainTiles, mianzi);
			return res;
		}
		//˳��
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
				//˳��
				mianzi.push_back(Group::createShunzi(group[0], group[1], group[2], 0));
				res = agariSearch(par, depth + 1, newRemainTiles, mianzi);
				return res;
			}
		}
		//���ڸ���
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
	//�����Ժ��߶��Ӹ��ϵ�����:dora,ura,aka,��ֱ,һ��,����ֱ,��������,����,���۾�,��һɫ,��������,�ӵ�����,����ͷ,��һɫ
	//������:���,�غ�,��һɫ
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
	//����������
	if (res.fan < 0)
	{
		res = getScore(par.selfWind,res);
		return TryToAgariResult(res);
	}
	//�����������ͣ���һ����Ҫ�����߶���
	res.fan += 2;
	res.fu = 25;
	res.yaku.push_back(Yaku::Qiduizi);
	//���dora��akadora��uradora
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
	//�����ֱ,����ֱ��һ��
	if (par.lizhiXunmu != -1)
	{
		//w��
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
	//�������
	if (par.type == 2)
	{
		res.fan += 1;
		res.yaku.push_back(Yaku::Qianggang);
	}
	//�����������
	if (par.type == 0)
	{
		res.fan += 1;
		res.yaku.push_back(Yaku::Menqianqingzimo);
	}
	//���ӵ�/����
	if (par.state == 2)
	{
		if (par.type == 0)
			res.yaku.push_back(Yaku::Haidilaoyue);
		else if (par.type == 1)res.yaku.push_back(Yaku::Hedilaoyu);
	}
	//�����һɫ����һɫ�����۾ţ�����ͷ
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
//�ж��Ƿ�Ϊ��ʿ��˫������
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
		//��ʿ
		if (all.size() == 12)
		{
			if (par.target.value() == 1 || par.target.value() == 9 || par.target.color() == 'z')
			{
				all.insert(par.target);
				if (all.size() == 13)
					guoshi = 1;
			}
		}
		//��ʿʮ����
		else if (all.size() == 13)
		{
			if (par.target.value() == 1 || par.target.value() == 9 || par.target.color() == 'z')
				guoshi = 2;
		}
	}
	//���Ը�����غ�
	if (par.state == 1 && par.type == 0)
	{
		if (par.selfWind == WindType::EAST)
		{
			guoshi = 2;
			res.yaku.push_back(Yaku::Tianhu);
		}
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
	res.zimo = (par.type == 0);
	res = getScore(par.selfWind,res);
	return TryToAgariResult(res);
}