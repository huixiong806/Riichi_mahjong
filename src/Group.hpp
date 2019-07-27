#pragma once
#include<vector>
enum class GroupType
{
	Quetou,//ȸͷ
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
	uint8_t state;//��λ��ʾ��/��/�ܵ������Ƶ���Դ(0,1,2,3��Ӧ�ԡ������ٸܡ����ܵ����¶���,4,5,6,7��Ӧ�Ӹܵ����¶���(4��Ч))����λ0��10��20�ֱ��ʾ��һ�ţ��ڶ��ţ������š�(���ӣ�����Ĭ��Ϊ0)
	//source��ʾ���������Ƶ���Դ(0,1,2,3��Ӧ���¶���,4,5,6,7Ҳ��Ӧ���¶��ϣ�����Ϊ�Ӹ�(4��Ч))
	static Group createKezi(Single a, Single b, Single target, int source);
	static Group createShunzi(Single a, Single b, Single target, int source);
	static Group createGangzi(Single a, Single b,Single c, Single target, int source);
	static Group createQuetou(Single a, Single b);
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
	//ͳһ���չ�������������������˳��ͬ�����������ֲ�ͬ����(����û�ã���Ϊ��ֻ������һ��ѡ��)
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
	if (a > b)std::swap(a, b);//ͳһ���չ�������������������˳��ͬ�����������ֲ�ͬ����
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