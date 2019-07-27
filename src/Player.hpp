#pragma once
#include"Single.hpp"
#include"Group.hpp"
#include<vector>
#include<algorithm>
#include<string>
#include<map>
#include<set>
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
	Null,//��
	ShapeWrong,//�����������
	NoYaku,//����
	NotEnoughFan,//����
	CantQiangAnGang//�ǹ�ʿ��˫����������
};
struct AgariResult
{
	bool zimo;
	int fan;//-1=������-2=����������-3=�����������Դ�����,13���ۼ�����
	int fu;
	int dora;
	int akadora;
	int uradora;
	int scoreAdd;//��������ֱ���ͱ������ļ���
	int scoreDecFangchong;//����Ϊ0
	int scoreDecZhuang;//�ٺ�Ϊ0
	int scoreDecXian;//�ٺ�Ϊ0
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
//0~8 ����,9~17 ����,18~26 ����,27~33 ���������׷���
std::vector<int> getPool(const std::vector<Single>& tiles)
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
class Player
{
private:
	//�ж��Ƿ�Ϊ��ʿ��˫������
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
		//���Ը�����غ�
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
	TryToAgariResult agariSearch(int state, int type, const Single& target, const std::vector<Single>&dora, const std::vector<Single>&ura, int depth, std::vector<Single> remainTiles, std::vector<Group> mianzi)const
	{
		TryToAgariResult res;
		if (remainTiles.empty())
		{
			AgariResult result=AgariResult();
			mianzi.insert(mianzi.end(), groupTile.begin(), groupTile.end());
			if (mianzi.size() != 4)return TryToAgariResult(AgariFaildReason::ShapeWrong);
			//TODO:�ж�����
			res.result.zimo = type == 0;
			//��ǰ��������
			if (groupTile.empty() && type == 0)
			{
				result.fan += 1;
				result.yaku.push_back(Yaku::Menqianqingzimo);
			}
			result = getScore(res.result);
			if (result.fan == 0)
			{
				return TryToAgariResult(AgariFaildReason::NoYaku);
			}
			return TryToAgariResult(result);
		}
		if (depth == 0)
		{
			for (int i = 0; i<remainTiles.size()-1; ++i)
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
					res = std::max(res, agariSearch(state, type, target, dora, ura, depth + 1, newRemainTiles, mianzi));
					mianzi.pop_back();
				}
			}
			return res;
		}
		std::vector<int> pool = getPool(remainTiles);
		//�ж�����˳�ӺͿ���
		for (int i = 0; i <= 24; ++i)
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
					res = agariSearch(state, type, target, dora, ura, depth + 3, newRemainTiles, mianzi);
					mianzi.pop_back();
					mianzi.pop_back();
					mianzi.pop_back();
					//��ͬ˳
					mianzi.push_back(Group::createKezi(group[0][0], group[1][0], group[2][0], 0));
					mianzi.push_back(Group::createKezi(group[0][1], group[1][1], group[2][1], 0));
					mianzi.push_back(Group::createKezi(group[0][2], group[1][2], group[2][2], 0));
					res = std::max(res, agariSearch(state, type, target, dora, ura, depth + 3, newRemainTiles, mianzi));
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
				res = agariSearch(state, type, target, dora, ura, depth + 1, newRemainTiles, mianzi);
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
					res = agariSearch(state, type, target, dora, ura, depth + 1, newRemainTiles, mianzi);
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
	//�ж��Ƿ�Ϊ�߶Ժ�����
	TryToAgariResult qidui(int state, int type, const Single& target, const std::vector<Single>&dora, const std::vector<Single>&ura)const
	{
		AgariResult res= AgariResult();
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
		//�����Ժ��߶��Ӹ��ϵ�����:dora,ura,aka,��ֱ,һ��,����ֱ,��������,����,���۾�,��һɫ,��������,�ӵ�����,����ͷ,��һɫ
		//������:���,�غ�,��һɫ
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
		//����������
		if (res.fan < 0)
		{
			res = getScore(res);
			return TryToAgariResult(res);
		}
		//�����������ͣ���һ����Ҫ�����߶���
		res.fan += 2;
		res.fu = 25;
		res.yaku.push_back(Yaku::Qiduizi);
		//���dora��akadora��uradora
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
		//�����ֱ,����ֱ��һ��
		if (lizhiXunmu != -1)
		{
			//w��
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
		//�������
		if (type == 2)
		{
			res.fan += 1;
			res.yaku.push_back(Yaku::Qianggang);
		}
		//�����������
		if (type == 0)
		{
			res.fan += 1;
			res.yaku.push_back(Yaku::Menqianqingzimo);
		}
		//���ӵ�/����
		if (state == 2)
		{
			if (type == 0)
				res.yaku.push_back(Yaku::Haidilaoyue);
			else if (type == 1)res.yaku.push_back(Yaku::Hedilaoyu);
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
		res = getScore(res);
		return TryToAgariResult(res);
	}
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
	TryToAgariResult agari(int state, int type, const Single& target, const std::vector<Single>&dora, const std::vector<Single>&ura)
	{
		//��ʿ��˫���ж�
		AgariResult res;
		TryToAgariResult guoshi = guoshiwushuang(state, type, target);
		if (guoshi.success)
			return guoshi;
		//ֻ�й�ʿ��������
		if (state == 3)return TryToAgariResult(AgariFaildReason::CantQiangAnGang);
		//�߶��͵��ж�(�����ڽ������Ϊ�߶���)
		TryToAgariResult qiduizi = qidui(state, type, target, dora, ura);
		//һ���͵��ж�
		std::vector<Single>allTiles = handTile;
		allTiles.push_back(target);
		sort(allTiles.begin(), allTiles.end());
		TryToAgariResult normal = agariSearch(state, type, target, dora, ura, 0, allTiles, std::vector<Group>());
		return std::max(qiduizi, normal);
	}
public:
	int subround; //Ѳ��
	int score;
	WindType selfWind;   //�ŷ�
	std::vector<Single> handTile;//����
	std::vector<bool> disabledHandTile;//��������
	std::vector<Group> groupTile;//��¶����
	Single nowTile;//������
	std::vector<Single>discardTile;//�ƺ�
	bool lingshang;//��ҵ�ǰ����������������
	bool moqie=false;//��ʾ�ƺ������һ�����Ƿ�Ϊ����
	int lizhi=-1;//��ֱ���������ƺ��еĵڼ��ţ�-1Ϊδ��ֱ
	int lizhiXunmu=-1;//��ֱ��ѲĿ,��0��ʼ,-1Ϊδ��ֱ,-2Ϊw��
	bool yifa;//��ǰ�Ƿ�Ϊһ��Ѳ
	
	/*type
	����=0
	�ٺ�=1
	����=2
	������=3
	*/
	//ֻ�����㣬��������㷬�ͷ�
	AgariResult getScore(AgariResult inp)const
	{
		int east = selfWind == WindType::EAST;
		AgariResult res=inp;
		//������
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
		//�������� TODO:��scoreDec
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
	bool canRong(Single target)
	{
		return false;
	}
	//relativePosition 0,1,2,3�ֱ��ʾ�¶�����
	bool canChi(Single target, Group result, int relativePosition)
	{
		return false;
	}
	bool canPeng(Single target, Group result,int relativePosition)
	{
		//�������
		if (result.type != GroupType::Kezi)
			return false;
		//�����ɫ
		if (target.color() != result.color)
			return false;
		//�����ֵ
		if (target.value() != result.value)
			return false;
		//���λ��
		if (relativePosition != result.state)
			return false;
		//���챦���Ƿ���ȷ
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
		//��������
		if (tileCount < 2)return false;
		//�챦�Ʋ���
		if (realAkadoraCount < akadoraCount)
			return false;
		//�Ǻ챦�Ʋ���
		if ((tileCount - realAkadoraCount) < (2 - akadoraCount))
			return false;
		return true;
	}
	//�����ٸ�
	bool canGang(Single target,Group result, int relativePosition)
	{
		//�������
		if (result.type != GroupType::Gang)
			return false;
		//�����ɫ
		if (target.color() != result.color)
			return false;
		//�����ֵ
		if (target.value() != result.value)
			return false;
		//���λ��
		if (relativePosition != result.state)
			return false;
		//���챦���Ƿ���ȷ
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
		//��������
		if (tileCount < 3)return false;
		//�챦�Ʋ���
		if (realAkadoraCount < akadoraCount)
			return false;
		//�Ǻ챦�Ʋ���
		if ((tileCount - realAkadoraCount) < (3 - akadoraCount))
			return false;
		return true;
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
	//����falseʱ��Ӧ�䶯˽�б���
	bool dapai(Single target)//����
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
				std::sort(handTile.begin(),handTile.end());//��С��������
				if (nowTile == Null)
					handTile.pop_back();
				nowTile == Null;
				moqie = false;
				return true;
			}
		}
		return false;
	}
	//����falseʱ��Ӧ�䶯˽�б���
	/*
	state
	����=0
	��غ�=1
	�ӵ�/����=2
	*/
	TryToAgariResult zimo(int state, const std::vector<Single>&dora, const std::vector<Single>&ura)
	{
		return agari(state, 0, nowTile, dora, ura);;
	}
	//����falseʱ��Ӧ�䶯˽�б���
	bool liuju()
	{
		return false;
	}
	//����falseʱ��Ӧ�䶯˽�б���
	bool doLizhi()
	{
		return false;
	}
};