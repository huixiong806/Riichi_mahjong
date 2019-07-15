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
		if (relativePosition != result.state%10)
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
	bool zimo()
	{
		return false;
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