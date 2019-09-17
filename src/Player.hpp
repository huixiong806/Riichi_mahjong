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
	int subround; //Ѳ��
	int score;
	WindType selfWind;   //�ŷ�
	std::vector<Single> handTile;//����
	std::vector<bool> disabledHandTile;//��������
	std::vector<Group> groupTile;//��¶����
	Single nowTile;//������
	std::vector<Single>discardTile;//�ƺ�
	bool lingshang;//��ҵ�ǰ����������������
	bool moqie = false;//��ʾ�ƺ������һ�����Ƿ�Ϊ����
	int lizhi = -1;//��ֱ���������ƺ��еĵڼ��ţ�-1Ϊδ��ֱ
	int lizhiXunmu = -1;//��ֱ��ѲĿ,��0��ʼ,-1Ϊδ��ֱ,-2Ϊw��
	bool yifa;//��ǰ�Ƿ�Ϊһ��Ѳ
public:
	void setInfo(
	int subround_, //Ѳ��
	int score_,
	WindType selfWind_,   //�ŷ�
	const std::vector<Single>& handTile_,//����
	const std::vector<bool>& disabledHandTile_,//��������
	const std::vector<Group>& groupTile_,//��¶����
	const Single& nowTile_,//������
	const std::vector<Single>&discardTile_,//�ƺ�
	bool lingshang_,//��ҵ�ǰ����������������
	bool moqie_,//��ʾ�ƺ������һ�����Ƿ�Ϊ����
	int lizhi_,//��ֱ���������ƺ��еĵڼ��ţ�-1Ϊδ��ֱ
	int lizhiXunmu_,//��ֱ��ѲĿ,��0��ʼ,-1Ϊδ��ֱ,-2Ϊw��
	bool yifa_//��ǰ�Ƿ�Ϊһ��Ѳ
	)
	{
		subround = subround_;//Ѳ��
		score = score_;
		selfWind = selfWind_;   //�ŷ�
		handTile = handTile_;//����
		disabledHandTile = disabledHandTile_;//��������
		groupTile = groupTile_;//��¶����
		nowTile = nowTile_;//������
		discardTile = discardTile_;//�ƺ�
		lingshang = lingshang_;//��ҵ�ǰ����������������
		moqie = moqie_;//��ʾ�ƺ������һ�����Ƿ�Ϊ����
		lizhi = lizhi_;//��ֱ���������ƺ��еĵڼ��ţ�-1Ϊδ��ֱ
		lizhiXunmu = lizhiXunmu_;//��ֱ��ѲĿ,��0��ʼ,-1Ϊδ��ֱ,-2Ϊw��
		yifa = yifa;//��ǰ�Ƿ�Ϊһ��Ѳ
	}
	void setNowTile(Single target,bool isLingshang)
	{
		lingshang = isLingshang;
		nowTile = target;
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
		//��ֱ��������
		if (lizhiXunmu != -1)return false;
		for (int i=0;i<handTile.size();++i)
		{
			if (target == handTile[i])
			{
				discardTile.push_back(target);
				handTile[i] = nowTile;
				std::sort(handTile.begin(),handTile.end());//��С��������
				if (nowTile == Null)
					handTile.pop_back();
				nowTile = Null;
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
	��غ�,w��=1
	�ӵ�/����=2
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
	//����falseʱ��Ӧ�䶯˽�б���
	bool liuju()
	{
		return false;
	}
	bool tingpai()
	{
		return !Algorithms::tingpai(handTile).empty();
	}
	//����falseʱ��Ӧ�䶯˽�б���
	bool doLizhi(int state, Single target)
	{
		//Ҫ������Ƿ����?
		if (target == Null)return false;
		std::vector<Single> myTile = handTile;
		myTile.push_back(nowTile);
		bool success = false;
		for (int i = 0; i<myTile.size(); ++i)
		{
			if (target == myTile[i])
			{
				myTile[i] = Null;
				std::sort(myTile.begin(), myTile.end());//��С��������,Nullһ�����ŵ����һ��
				myTile.pop_back();
				success = true;
				break;
			}
		}
		if (!success)return false;
		//�Ƿ�ǰ��ܸ�¶��
		if (!groupTile.empty())
		{
			for (auto& item : groupTile)
			{
				//���ܱ�������type=Gang��state=0
				if (item.type != GroupType::Gang|| item.state != 0)
					return false;
			}
		}
		//�Ƿ��Ѿ���ֱ���ˣ�
		if (lizhiXunmu != -1)
			return false;
		//���������?
		if (score < 1000)
			return false;
		//�Ƿ�����?
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
