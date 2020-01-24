#include"Group.h"
#include <vector>
#include <algorithm>
Group Group_Null = {255, static_cast<char>('z' + 1), GroupType::Kantsu, 255, 255};

Group Group::createKantsu(Single a, Single b, Single c, Single target, int source) {
	if (!a.valueEqual(b) || !a.valueEqual(c) || !a.valueEqual(target))return Group_Null;
	Group res;
	res.type = GroupType::Kantsu;
	res.color = target.color();
	res.value = target.value();
	res.state = source;
	res.akadora = 0;
	std::vector<Single> temp;
	temp.push_back(a);
	temp.push_back(b);
	temp.push_back(c);
	//统一赤普共存的情况，避免它们因顺序不同而被当成两种不同杠子(根本没用，因为杠只可能有一种选择)
	std::sort(temp.begin(), temp.end());
	res.akadora |= static_cast<uint8_t>(target.isAkadora())&0b1;
	res.akadora |= (static_cast<uint8_t>(temp[0].isAkadora())&0b1) << 1;
	res.akadora |= (static_cast<uint8_t>(temp[1].isAkadora())&0b1) << 2;
	res.akadora |= (static_cast<uint8_t>(temp[2].isAkadora())&0b1) << 3;
	return res;
}

Group Group::createKoutsu(Single a, Single b, Single target, int source) {
	if (!a.valueEqual(b) || !b.valueEqual(target))return Group_Null;
	Group res;
	res.type = GroupType::Koutsu;
	res.color = target.color();
	res.value = target.value();
	res.state = source;
	res.state += 0;//第一张是目标
	res.akadora = 0;
	if (a > b)std::swap(a, b); //统一赤普共存的情况，避免它们因顺序不同而被当成两种不同刻子
	res.akadora |= static_cast<uint8_t>(target.isAkadora())&0b1;
	res.akadora |= (static_cast<uint8_t>(a.isAkadora())&0b1) << 1;
	res.akadora |= (static_cast<uint8_t>(b.isAkadora())&0b1) << 2;
	return res;
}

Group Group::createToitsu(Single a, Single b) {
	if (!a.valueEqual(b))return Group_Null;
	if (b.isAkadora() > a.isAkadora())
		std::swap(a, b);
	Group res;
	res.type = GroupType::Toitsu;
	res.color = a.color();
	res.value = a.value();
	res.akadora = static_cast<uint8_t>(a.isAkadora()) | static_cast<uint8_t>(b.isAkadora()) << 1;
	res.state = 0;
	return res;
}

Group Group::createShuntsu(Single a, Single b, Single target, int source) {
	if (!a.color() == b.color() || !b.color() == target.color())return Group_Null; // TODO: verify this
	if (b < a)std::swap(a, b);
	Group res;
	res.type = GroupType::Shuntsu;
	res.color = a.color();
	res.state = source;
	res.akadora = 0;
	//a<b<t
	if (a.value() == b.value() - 1 && b.value() == target.value() - 1) {
		res.value = a.value();
		res.state += 20;
		res.akadora |= static_cast<uint8_t>(a.isAkadora());
		res.akadora |= static_cast<uint8_t>(b.isAkadora()) << 1;
		res.akadora |= static_cast<uint8_t>(target.isAkadora()) << 2;
	}
		//a<t<b
	else if (a.value() == target.value() - 1 && target.value() == b.value() - 1) {
		res.value = a.value();
		res.state += 10;
		res.akadora |= static_cast<uint8_t>(a.isAkadora());
		res.akadora |= static_cast<uint8_t>(target.isAkadora()) << 1;
		res.akadora |= static_cast<uint8_t>(b.isAkadora()) << 2;
	}
		//t<a<b
	else if (target.value() == a.value() - 1 && a.value() == b.value() - 1) {
		res.value = target.value();
		res.state += 0;
		res.akadora |= static_cast<uint8_t>(target.isAkadora());
		res.akadora |= static_cast<uint8_t>(a.isAkadora()) << 1;
		res.akadora |= static_cast<uint8_t>(b.isAkadora()) << 2;
	}
	else return Group_Null;
	return res;
}
