#pragma once
#include<vector>
enum class GroupType
{
	Shunzi, //˳��
	Kezi,  //����
	Gang,  //��
};
struct Group
{
	GroupType type;
	char color;//'m','p','s','z'
	uint8_t value;//˳�ӵĻ�value������С���Ǹ�
	uint8_t akadora;//�ӵ͵��߷ֱ��ʾ������(��С����)�Ƿ�Ϊ�챦��
	uint8_t state;//��λ��ʾ��/��/�ܵ������Ƶ���Դ(0,1,2,3��Ӧ���¶���,4,5,6,7Ҳ��Ӧ���¶��ϣ�����Ϊ�Ӹ�(4��Ч))����λ0��10��20�ֱ��ʾ��һ�ţ��ڶ��ţ������š�(���ӣ�����Ĭ��Ϊ0)
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
			//����
			if (state % 10 == 0)
			{
				res.push_back('O');
				res.push_back((akadora ? 0 : value) + '0');
				res.push_back(value + '0');
				res.push_back('O');
			}
			else
			{
				//����(�����Ų�������)�Ӹ�(�����Ŵ����ţ�state/10==3)
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