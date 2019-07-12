#pragma once
#include<vector>
enum class GroupType
{
	Shunzi, //顺子
	Kezi,  //刻子
	Gang,  //杠
};
struct Group
{
	GroupType type;
	char color;//'m','p','s','z'
	uint8_t value;//顺子的话value等于最小的那个
	uint8_t akadora;//从低到高分别表示三张牌(从小到大)是否为红宝牌
	uint8_t state;//低位表示吃/碰/杠的那张牌的来源(0,1,2,3对应自下对上,4,5,6,7也对应自下对上，并且为加杠(4无效))，高位0，10，20分别表示第一张，第二张，第三张。(刻子，杠子默认为0)
	std::string getString()
	{
		std::string res;
		if(type== GroupType::Shunzi)
		{
			if (state / 10 == 0)
			{
				if (state % 10 == 3)
				{
					res.push_back('(');
					res.push_back(((akadora & 1) ? value : 0) + '0');
					res.push_back(')');
				}
				res.push_back(((akadora & 2) ? value+1 : 0) + '0');
				if (state % 10 == 2)
				{
					res.push_back('(');
					res.push_back(((akadora & 1) ? value : 0) + '0');
					res.push_back(')');
				}
				res.push_back(((akadora & 4) ? value+2 : 0) + '0');
				if (state % 10 == 1)
				{
					res.push_back('(');
					res.push_back(((akadora & 1) ? value : 0) + '0');
					res.push_back(')');
				}
			}
			else if (state / 10 == 1)
			{
				if (state % 10 == 3)
				{
					res.push_back('(');
					res.push_back(((akadora & 2) ? value + 1 : 0) + '0');
					res.push_back(')');
				}
				res.push_back(((akadora & 1) ? value: 0) + '0');
				if (state % 10 == 2)
				{
					res.push_back('(');
					res.push_back(((akadora & 2) ? value + 1 : 0) + '0');
					res.push_back(')');
				}
				res.push_back(((akadora & 4) ? value + 2 : 0) + '0');
				if (state % 10 == 1)
				{
					res.push_back('(');
					res.push_back(((akadora & 2) ? value + 1 : 0) + '0');
					res.push_back(')');
				}
			}
			else if (state / 10 == 2)
			{
				if (state % 10 == 3)
				{
					res.push_back('(');
					res.push_back(((akadora & 4) ? value + 2 : 0) + '0');
					res.push_back(')');
				}
				res.push_back(((akadora & 1) ? value : 0) + '0');
				if (state % 10 == 2)
				{
					res.push_back('(');
					res.push_back(((akadora & 4) ? value + 2 : 0) + '0');
					res.push_back(')');
				}
				res.push_back(((akadora & 2) ? value + 1 : 0) + '0');
				if (state % 10 == 1)
				{
					res.push_back('(');
					res.push_back(((akadora & 4) ? value + 2 : 0) + '0');
					res.push_back(')');
				}
			}
			//if (state / 10 == 1 || state / 10 == 2)res.push_back(' ');
			//if (state / 10 == 3 || state / 10 == 2)res.push_back(' ');
		}
		else if (type == GroupType::Gang)
		{
			//暗杠
			if (state % 10 == 0)
			{
				res.push_back('O');
				res.push_back((akadora ? 0 : value) + '0');
				res.push_back(value + '0');
				res.push_back('O');
			}
			else
			{
				//明杠(第四张不带括号)加杠(第四张带括号，state/10==3)
				if (state % 10 == 3|| state % 10 == 7)
				{
					res.push_back('(');
					res.push_back(((akadora & 1) ? value : 0) + '0');
					res.push_back(')');
				}
				res.push_back(((akadora & 2) ? value : 0) + '0');
				if (state % 10 == 2||state % 10 == 6)
				{
					res.push_back('(');
					res.push_back(((akadora & 1) ? value : 0) + '0');
					res.push_back(')');
				}
				res.push_back(((akadora & 4) ? value : 0) + '0');
				if (state % 10 == 1||state % 10 == 5)
				{
					res.push_back('(');
					res.push_back(((akadora & 1) ? value : 0) + '0');
					res.push_back(')');
				}
				if (state % 10 > 4)
				{
					res.push_back('(');
					res.push_back(((akadora & 8) ? value : 0) + '0');
					res.push_back(')');
				}
				else res.push_back(((akadora & 8) ? value : 0) + '0');
			}
		}
		else if (type==GroupType::Kezi)
		{
			if (state % 10 == 3)
			{
				res.push_back('(');
				res.push_back(((akadora & 1) ? value : 0) + '0');
				res.push_back(')');
			}
			res.push_back(((akadora & 2) ? value : 0) + '0');
			if (state % 10 == 2)
			{
				res.push_back('(');
				res.push_back(((akadora & 1) ? value : 0) + '0');
				res.push_back(')');
			}
			res.push_back(((akadora & 4) ? value : 0) + '0');
			if (state % 10 == 1)
			{
				res.push_back('(');
				res.push_back(((akadora & 1) ? value : 0) + '0');
				res.push_back(')');
			}
		}
		res.push_back(color);
		return res;
	}
	Group() {}
	Group(uint8_t value_, char color_, GroupType type_, uint8_t akadora_,uint8_t state_) :type(type_), color(color_), value(value_),akadora(akadora_),state(state_) {}
	bool operator <(const Group& rhs)const
	{
		if(type < rhs.type)return true;
		if (color < rhs.color)return true;
		if (value < rhs.value)return true;
		return false;
	}
};