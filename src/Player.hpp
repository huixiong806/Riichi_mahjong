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