#include"Group.h"
#include<algorithm>
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
	res.akadora |= (uint8_t)a.isAkadora() << 1;
	res.akadora |= (uint8_t)b.isAkadora() << 2;
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
	res.akadora = a.isAkadora() |((uint8_t)b.isAkadora() << 1);
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
		res.akadora |= (uint8_t)a.isAkadora();
		res.akadora |= (uint8_t)b.isAkadora() << 1;
		res.akadora |= (uint8_t)target.isAkadora() << 2;
	}
	//a<t<b
	else if (a.value() == target.value() - 1 && target.value() == b.value() - 1)
	{
		res.value = a.value();
		res.state += 10;
		res.akadora |= (uint8_t)a.isAkadora();
		res.akadora |= (uint8_t)target.isAkadora() << 1;
		res.akadora |= (uint8_t)b.isAkadora() << 2;
	}
	//t<a<b
	else if(target.value() == a.value() - 1 && a.value() == b.value() - 1)
	{
		res.value = target.value();
		res.state += 0;
		res.akadora |= (uint8_t)target.isAkadora();
		res.akadora |= (uint8_t)a.isAkadora() << 1;
		res.akadora |= (uint8_t)b.isAkadora() << 2;
	}
	else return Group_Null;
	return res;
}