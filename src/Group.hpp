#pragma once
#include<vector>
enum class GroupType
{
	Quetou, //雀头
	Shunzi, //顺子
	Kezi,  //刻子
	Gang,  //杠
	Angang //暗杠
};
struct Group
{
	GroupType type;
	char color;//'m','p','s','z'
	uint8_t value;//顺子的话value等于最小的那个
	uint8_t akadora;//从低到高分别表示三张牌是否为红宝牌
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