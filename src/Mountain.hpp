#pragma once
#include"Single.hpp"
#include"Rules.h"
#include<vector>
#include<string>
#include<algorithm>
class Mountain
{
private:
protected:
	std::vector<Single>mA;        //�ļ�����
	std::vector<Single>mB;        //���ϣ�doraָʾ��
	std::vector<Single>mMountain;  //��ɽ����
	int mHandPtr;  //����ָ��
	int mLingshangPtr;  //������ָ��
public:
	std::vector<Single>hand[4];  //�ļҳ�ʼ����
	Single zimo;//ׯ��0Ѳ��������
	Mountain();//���캯��
	virtual void reset(const Rule& rule);  //��������ɽ
	Single getDora(int index); //��ȡ����
	Single getUra(int index); //��ȡ�ﱦ��
	Single getDoraIndicator(int index); //��ȡ����ָʾ��,�±귶Χ0~4
	Single getUraIndicator(int index); //��ȡ�ﱦ��ָʾ��,�±귶Χ0~4
	Single nextLingshang();//��ȡ��һ��������(�Զ��ƺ�����)
	Single nextHand();//��ȡ��һ������
	int remainCount(); //��ȡʣ������
	std::string getMountainString();//�����ɽ����
};
Mountain::Mountain()
{
	mHandPtr = mLingshangPtr = 0;
}
std::string Mountain::getMountainString()//�����ɽ����
{
	std::string res;
	for (auto& item : mMountain)
	{
		int value = item.value();
		if (item.isAkadora())value = 0;
		res.push_back(value+'0');
		res.push_back(item.color());
	}
	return res;
}
Single Mountain::nextLingshang()//��ȡ��һ��������(�Զ��ƺ�����)
{
	Single res=mB[mLingshangPtr++];
	mB.push_back(mA[mA.size()-1]);
	mA.pop_back();
	return res;
}
Single Mountain::nextHand()//��ȡ��һ������
{
	return mA[mHandPtr++];
}
int Mountain::remainCount() //��ȡʣ������
{
	return mA.size() - mHandPtr;
}
Single Mountain::getDora(int index) //��ȡ����
{
	return getDoraIndicator(index).next();
}
Single Mountain::getUra(int index) //��ȡ�ﱦ��
{
	return getUraIndicator(index).next();
}
Single Mountain::getDoraIndicator(int index) //��ȡ����ָʾ��,�±귶Χ0~4
{
	return mB[4 + index * 2];
}
Single Mountain::getUraIndicator(int index) //��ȡ�ﱦ��ָʾ��,�±귶Χ0~4
{
	return mB[5 + index * 2];
}
void Mountain::reset(const Rule& rule)
{
	mHandPtr = 0;
	mLingshangPtr = 0;
	mMountain.clear();
	mA.clear();
	mB.clear();
	for (int j = 0; j < 4; ++j)
		hand[j].clear();
	std::vector<Single>all;
	for (int i = 1; i <= 9; ++i)
	{
		for (int j = 1; j <= 4; ++j)
		{
			if (i==5 && j==1 && rule.doraCount > 0)//�챦��
			{
				all.push_back(Single(i, 'm', true));
				all.push_back(Single(i, 'p', true));
				all.push_back(Single(i, 's', true));
			}
			else if(i==5 && j==2 && rule.doraCount == 4)//�챦��5��
			{
				all.push_back(Single(i, 'm',false));
				all.push_back(Single(i, 'p', true));
				all.push_back(Single(i, 's', false));
			}
			else
			{
				all.push_back(Single(i, 'm',false));
				all.push_back(Single(i, 'p', false));
				all.push_back(Single(i, 's', false));
			}
			if(i<=7)all.push_back(Single(i, 'z', false));
		}
	}
	std::random_shuffle(all.begin(), all.end());
	int ptr = 0;
	for (int i = 1; i <= 13; ++i)
		for (int j = 0; j < 4; ++j)
			hand[j].push_back(all[ptr++]);
	zimo=all[ptr++];
	for (int i = ptr; i < all.size(); ++i)
		mMountain.push_back(all[i]);
	for (int i = 1; i <= 69; ++i)
		mA.push_back(all[ptr++]);
	ptr = all.size();
	for (int i = 1; i <= 14; ++i)
		mB.push_back(all[--ptr]);
}
