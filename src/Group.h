#pragma once
#include<vector>
#include<string>
#include"Single.h"
enum class GroupType
{
	Quetou,//ȸͷ
	Shunzi, //˳��
	Kezi,  //����
	Gang,  //��
};
class Group
{
public:
	GroupType type;
	char color;//'m','p','s','z'
	uint8_t value;//˳�ӵĻ�value������С���Ǹ�
	uint8_t akadora;//�ӵ͵��߷ֱ��ʾ������(��С����)�Ƿ�Ϊ�챦��
	uint8_t state;//��λ��ʾ��/��/�ܵ������Ƶ���Դ(0,1,2,3��Ӧ�ԡ������ٸܡ����ܵ����¶���,4,5,6,7��Ӧ�Ӹܵ����¶���(4��Ч))����λ0��10��20�ֱ��ʾ��һ�ţ��ڶ��ţ������š�(���ӣ�����Ĭ��Ϊ0)
	//source��ʾ���������Ƶ���Դ(0,1,2,3��Ӧ���¶���,4,5,6,7Ҳ��Ӧ���¶��ϣ�����Ϊ�Ӹ�(4��Ч))
	static Group createKezi(Single a, Single b, Single target, int source);
	static Group createShunzi(Single a, Single b, Single target, int source);
	static Group createGangzi(Single a, Single b,Single c, Single target, int source);
	static Group createQuetou(Single a, Single b);
	//�Ƿ�Ϊ����
	bool isgreen()
	{
		if (color == 'z')return value == 6;
		else if (color == 's')
		{
			if (type != GroupType::Shunzi)
				return value == 2 || value == 3 || value == 4 || value == 6 || value == 8;
			else return value == 2;
		}
		return false;
	}
	//�Ƿ�Ϊ��ͷ����(������19)
	bool islaotou()
	{
		if (type == GroupType::Shunzi)return false;
		if (color == 'z')return true;
		if (value == 1 || value == 9)return true;
		return false;
	}
	//�Ƿ�Ϊ�۾�����(����˳�ӣ���������)
	bool isyaojiu()
	{
		if (type == GroupType::Shunzi)
		{
			if (value == 7 || value ==  1)return true;
			return false;
		}
		if (color == 'z')return true;
		if (value == 1 || value == 9)return true;
		return false;
	}
	const std::string getString()const
	{
		std::string res;
		if (state % 10 == 0 && type != GroupType::Gang)
		{
			if (type == GroupType::Shunzi)
			{
				res.push_back(value + '0');
				res.push_back(value + '1');
				res.push_back(value + '2');
			}
			else if (type == GroupType::Quetou)
			{
				res.push_back(value + '0');
				res.push_back(value + '0');
			}
			else
			{
				res.push_back(value + '0');
				res.push_back(value + '0');
				res.push_back(value + '0');
			}
			res.push_back(color);
			return res;
		}
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
extern Group Group_Null;