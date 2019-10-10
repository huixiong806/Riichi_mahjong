#pragma once
#include"Single.h"
#include"Rules.h"
#include"ResultInspector.hpp"
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