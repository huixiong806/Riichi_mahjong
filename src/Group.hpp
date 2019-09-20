#pragma once
#include<vector>
enum class GroupType
{
	Quetou,//雀头
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
	uint8_t state;//低位表示吃/碰/杠的那张牌的来源(0,1,2,3对应吃、碰、荣杠、暗杠的自下对上,4,5,6,7对应加杠的自下对上(4无效))，高位0，10，20分别表示第一张，第二张，第三张。(刻子，杠子默认为0)
	//source表示碰的那张牌的来源(0,1,2,3对应自下对上,4,5,6,7也对应自下对上，并且为加杠(4无效))
	static Group createKezi(Single a, Single b, Single target, int source);
	static Group createShunzi(Single a, Single b, Single target, int source);
	static Group createGangzi(Single a, Single b,Single c, Single target, int source);
	static Group createQuetou(Single a, Single b);
	//是否为幺九
	bool isyaojiu()
	{
		if (color == 'z')return true;
		if (value == 1 || value == 9)return true;
		return false;
	}
	const std::string getString()const
	{
		std::string res;
		if(type== GroupType::Shunzi)
		{
			if (state / 10 == 0)
			{
				if (state % 10 == 3)
				{
					res.push_back('(');
					res.push_back(((akadora & 1) ? 0 : value) + '0');
					res.push_back(')');
				}
				res.push_back(((akadora & 2) ? 0: value + 1) + '0');
				if (state % 10 == 2)
				{
					res.push_back('(');
					res.push_back(((akadora & 1) ? 0 : value) + '0');
					res.push_back(')');
				}
				res.push_back(((akadora & 4) ? 0:value+2) + '0');
				if (state % 10 == 1)
				{
					res.push_back('(');
					res.push_back(((akadora & 1) ? 0:value) + '0');
					res.push_back(')');
				}
			}
			else if (state / 10 == 1)
			{
				if (state % 10 == 3)
				{
					res.push_back('(');
					res.push_back(((akadora & 2) ? 0:value + 1) + '0');
					res.push_back(')');
				}
				res.push_back(((akadora & 1) ?0:value) + '0');
				if (state % 10 == 2)
				{
					res.push_back('(');
					res.push_back(((akadora & 2) ? 0:value + 1) + '0');
					res.push_back(')');
				}
				res.push_back(((akadora & 4) ? 0:value + 2) + '0');
				if (state % 10 == 1)
				{
					res.push_back('(');
					res.push_back(((akadora & 2) ? 0:value + 1) + '0');
					res.push_back(')');
				}
			}
			else if (state / 10 == 2)
			{
				if (state % 10 == 3)
				{
					res.push_back('(');
					res.push_back(((akadora & 4) ? 0:value + 2) + '0');
					res.push_back(')');
				}
				res.push_back(((akadora & 1) ? 0:value) + '0');
				if (state % 10 == 2)
				{
					res.push_back('(');
					res.push_back(((akadora & 4) ? 0:value + 2) + '0');
					res.push_back(')');
				}
				res.push_back(((akadora & 2) ? 0:value + 1) + '0');
				if (state % 10 == 1)
				{
					res.push_back('(');
					res.push_back(((akadora & 4) ? 0:value + 2) + '0');
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
					res.push_back(((akadora & 1) ? 0:value) + '0');
					res.push_back(')');
				}
				res.push_back(((akadora & 2) ? 0:value) + '0');
				if (state % 10 == 2||state % 10 == 6)
				{
					res.push_back('(');
					res.push_back(((akadora & 1) ? 0:value) + '0');
					res.push_back(')');
				}
				res.push_back(((akadora & 4) ? 0:value) + '0');
				if (state % 10 == 1||state % 10 == 5)
				{
					res.push_back('(');
					res.push_back(((akadora & 1) ? 0:value) + '0');
					res.push_back(')');
				}
				if (state % 10 > 4)
				{
					res.push_back('(');
					res.push_back(((akadora & 8) ? 0:value) + '0');
					res.push_back(')');
				}
				else res.push_back(((akadora & 8) ? 0:value) + '0');
			}
		}
		else if (type==GroupType::Kezi)
		{
			if (state % 10 == 3)
			{
				res.push_back('(');
				res.push_back(((akadora & 1) ? 0:value) + '0');
				res.push_back(')');
			}
			res.push_back(((akadora & 2) ? 0:value) + '0');
			if (state % 10 == 2)
			{
				res.push_back('(');
				res.push_back(((akadora & 1) ? 0:value) + '0');
				res.push_back(')');
			}
			res.push_back(((akadora & 4) ? 0:value) + '0');
			if (state % 10 == 1)
			{
				res.push_back('(');
				res.push_back(((akadora & 1) ? 0:value) + '0');
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
	bool operator !=(const Group& rhs)const
	{
		return !(*this == rhs);
	}
	bool operator ==(const Group& rhs)const
	{
		if (type != rhs.type)return false;
		if (color != rhs.color)return false;
		if (value != rhs.value)return false;
		if (akadora != rhs.akadora)return false;
		if (state != rhs.state)return false;
		return true;
	}
};
Group Group_Null = { 255,(char)('z' + 1),GroupType::Gang,255,255 };
Group Group::createGangzi(Single a, Single b, Single c, Single target, int source)
{
	if (!a.valueEqual(b) || !a.valueEqual(c) || !a.valueEqual(target))return Group_Null;
	Group res;
	res.type = GroupType::Gang;
	res.color = target.color();
	res.value = target.value();
	res.state = source;
	res.akadora = 0;
	std::vector<Single>temp;
	temp.push_back(a);
	temp.push_back(b);
	temp.push_back(c);
	//统一赤普共存的情况，避免它们因顺序不同而被当成两种不同杠子(根本没用，因为杠只可能有一种选择)
	std::sort(temp.begin(), temp.end());
	res.akadora |= target.isAkadora();
	res.akadora |= temp[0].isAkadora() << 1;
	res.akadora |= temp[1].isAkadora() << 2;
	res.akadora |= temp[2].isAkadora() << 3;
	return res;
}
Group Group::createKezi(Single a, Single b, Single target, int source)
{
	if (!a.valueEqual(b) || !b.valueEqual(target))return Group_Null;
	Group res;
	res.type = GroupType::Kezi;
	res.color = target.color();
	res.value = target.value();
	res.state = source;
	res.akadora = 0;
	if (a > b)std::swap(a, b);//统一赤普共存的情况，避免它们因顺序不同而被当成两种不同刻子
	res.akadora |= target.isAkadora();
	res.akadora |= a.isAkadora() << 1;
	res.akadora |= b.isAkadora() << 2;
	return res;
}
Group Group::createQuetou(Single a, Single b)
{
	if (!a.valueEqual(b))return Group_Null;
	if (b.isAkadora() > a.isAkadora())
		std::swap(a, b);
	Group res;
	res.type = GroupType::Quetou;
	res.color = a.color();
	res.value = a.value();
	res.akadora = a.isAkadora() | b.isAkadora() << 1;
	res.state = 0;
	return res;
}
Group Group::createShunzi(Single a, Single b, Single target, int source)
{
	if (!a.color()==b.color() || !b.color()==target.color())return Group_Null;
	if (b < a)std::swap(a, b);
	Group res;
	res.type = GroupType::Shunzi;
	res.color = a.color();
	res.state = source;
	res.akadora = 0;
	//a<b<t
	if (a.value() == b.value() - 1&&b.value()==target.value()-1)
	{
		res.value = a.value();
		res.state += 20;
		res.akadora |= a.isAkadora();
		res.akadora |= b.isAkadora() << 1;
		res.akadora |= target.isAkadora() << 2;
	}
	//a<t<b
	else if (a.value() == target.value() - 1 && target.value() == b.value() - 1)
	{
		res.value = a.value();
		res.state += 10;
		res.akadora |= a.isAkadora();
		res.akadora |= target.isAkadora() << 1;
		res.akadora |= b.isAkadora() << 2;
	}
	//t<a<b
	else if(target.value() == a.value() - 1 && a.value() == b.value() - 1)
	{
		res.value = target.value();
		res.state += 0;
		res.akadora |= target.isAkadora();
		res.akadora |= a.isAkadora() << 1;
		res.akadora |= b.isAkadora() << 2;
	}
	else return Group_Null;
	return res;
}