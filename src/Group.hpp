#pragma once
#include<vector>
enum class GroupType
{
	Quetou, //ȸͷ
	Shunzi, //˳��
	Kezi,  //����
	Gang,  //��
	Angang //����
};
struct Group
{
	GroupType type;
	char color;//'m','p','s','z'
	uint8_t value;//˳�ӵĻ�value������С���Ǹ�
	uint8_t akadora;//�ӵ͵��߷ֱ��ʾ�������Ƿ�Ϊ�챦��
	Group() {}
	Group(uint8_t value_, char color_, GroupType type_, uint8_t akadora_) :type(type_), color(color_), value(value_),akadora(akadora_) {}
	bool operator <(const Group& rhs)const
	{
		if(type < rhs.type)return true;
		if (color < rhs.color)return true;
		if (value < rhs.value)return true;
		return false;
	}
};