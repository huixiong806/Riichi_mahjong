#pragma once
enum WindType
{
	EAST,
	SOUTH,
	WEST,
	NORTH
};
struct Rule
{
	int doraCount=3;  //��������
	int fanfu=1;      //����
	int startPoint=25000; //��ʼ����
	int endPoint=30000;   //һλ��Ҫ����
	WindType gameType= WindType::SOUTH;//��Ϸ���ͣ������/��ׯ��/ȫׯ��
};
const Rule DefualtRule = Rule();