#pragma once
#include<string>
#include"Single.h"

enum class GroupType {
	Toitsu,
	//雀头
	Shuntsu,
	//顺子
	Koutsu,
	//刻子
	Kantsu,
	//杠
};

class Group {
public:
	GroupType type;
	char color{}; //'m','p','s','z'
	uint8_t value{}; //顺子的话value等于最小的那个
	uint8_t akadora{}; //从低到高分别表示三张牌(从小到大)是否为红宝牌
	uint8_t state{};
	//低位表示吃/碰/杠的那张牌的来源(0,1,2,3对应吃、碰、荣杠、暗杠的自下对上,4,5,6,7对应加杠的自下对上(4无效))，高位0，10，20分别表示第一张，第二张，第三张。(刻子，杠子默认为0)
	//source表示碰的那张牌的来源(0,1,2,3对应自下对上,4,5,6,7也对应自下对上，并且为加杠(4无效))
	static Group createKoutsu(Single a, Single b, Single target, int source);
	static Group createShuntsu(Single a, Single b, Single target, int source);
	static Group createKantsu(Single a, Single b, Single c, Single target, int source);
	static Group createToitsu(Single a, Single b);
	//是否为绿牌
	[[nodiscard]] bool isgreen()const {
		if (color == 'z')return value == 6;
		if (color == 's') {
			if (type != GroupType::Shuntsu)
				return value == 2 || value == 3 || value == 4 || value == 6 || value == 8;
			return value == 2;
		}
		return false;
	}

	//是否为老头牌组(非字牌19)
	[[nodiscard]] bool is19AndNotZ()const {
		if (type == GroupType::Shuntsu)return false;
		if (color == 'z')return false;
		if (value == 1 || value == 9)return true;
		return false;
	}

	//是否为幺九牌组(允许123/789顺子，允许字牌)
	[[nodiscard]] bool is19Z()const {
		if (type == GroupType::Shuntsu) {
			if (value == 7 || value == 1)return true;
			return false;
		}
		if (color == 'z')return true;
		if (value == 1 || value == 9)return true;
		return false;
	}

	[[nodiscard]] std::string getString() const {
		std::string res;
		if (state % 10 == 0 && type != GroupType::Kantsu) {
			if (type == GroupType::Shuntsu) {
				res.push_back(value + '0');
				res.push_back(value + '1');
				res.push_back(value + '2');
			}
			else if (type == GroupType::Toitsu) {
				res.push_back(value + '0');
				res.push_back(value + '0');
			}
			else {
				res.push_back(value + '0');
				res.push_back(value + '0');
				res.push_back(value + '0');
			}
			res.push_back(color);
			return res;
		}
		if (type == GroupType::Shuntsu) {
			if (state / 10 == 0) {
				if (state % 10 == 3) {
					res.push_back('(');
					res.push_back(((akadora & 1) ? 0 : value) + '0');
					res.push_back(')');
				}
				res.push_back(((akadora & 2) ? 0 : value + 1) + '0');
				if (state % 10 == 2) {
					res.push_back('(');
					res.push_back(((akadora & 1) ? 0 : value) + '0');
					res.push_back(')');
				}
				res.push_back(((akadora & 4) ? 0 : value + 2) + '0');
				if (state % 10 == 1) {
					res.push_back('(');
					res.push_back(((akadora & 1) ? 0 : value) + '0');
					res.push_back(')');
				}
			}
			else if (state / 10 == 1) {
				if (state % 10 == 3) {
					res.push_back('(');
					res.push_back(((akadora & 2) ? 0 : value + 1) + '0');
					res.push_back(')');
				}
				res.push_back(((akadora & 1) ? 0 : value) + '0');
				if (state % 10 == 2) {
					res.push_back('(');
					res.push_back(((akadora & 2) ? 0 : value + 1) + '0');
					res.push_back(')');
				}
				res.push_back(((akadora & 4) ? 0 : value + 2) + '0');
				if (state % 10 == 1) {
					res.push_back('(');
					res.push_back(((akadora & 2) ? 0 : value + 1) + '0');
					res.push_back(')');
				}
			}
			else if (state / 10 == 2) {
				if (state % 10 == 3) {
					res.push_back('(');
					res.push_back(((akadora & 4) ? 0 : value + 2) + '0');
					res.push_back(')');
				}
				res.push_back(((akadora & 1) ? 0 : value) + '0');
				if (state % 10 == 2) {
					res.push_back('(');
					res.push_back(((akadora & 4) ? 0 : value + 2) + '0');
					res.push_back(')');
				}
				res.push_back(((akadora & 2) ? 0 : value + 1) + '0');
				if (state % 10 == 1) {
					res.push_back('(');
					res.push_back(((akadora & 4) ? 0 : value + 2) + '0');
					res.push_back(')');
				}
			}
			//if (state / 10 == 1 || state / 10 == 2)res.push_back(' ');
			//if (state / 10 == 3 || state / 10 == 2)res.push_back(' ');
		}
		else if (type == GroupType::Kantsu) {
			//暗杠
			if (state % 10 == 0) {
				res.push_back('O');
				res.push_back((akadora ? 0 : value) + '0');
				res.push_back(value + '0');
				res.push_back('O');
			}
			else {
				//明杠(第四张不带括号)加杠(第四张带括号，state/10==3)
				if (state % 10 == 3 || state % 10 == 7) {
					res.push_back('(');
					res.push_back(((akadora & 1) ? 0 : value) + '0');
					res.push_back(')');
				}
				res.push_back(((akadora & 2) ? 0 : value) + '0');
				if (state % 10 == 2 || state % 10 == 6) {
					res.push_back('(');
					res.push_back(((akadora & 1) ? 0 : value) + '0');
					res.push_back(')');
				}
				res.push_back(((akadora & 4) ? 0 : value) + '0');
				if (state % 10 == 1 || state % 10 == 5) {
					res.push_back('(');
					res.push_back(((akadora & 1) ? 0 : value) + '0');
					res.push_back(')');
				}
				if (state % 10 > 4) {
					res.push_back('(');
					res.push_back(((akadora & 8) ? 0 : value) + '0');
					res.push_back(')');
				}
				else res.push_back(((akadora & 8) ? 0 : value) + '0');
			}
		}
		else if (type == GroupType::Koutsu) {
			if (state % 10 == 3) {
				res.push_back('(');
				res.push_back(((akadora & 1) ? 0 : value) + '0');
				res.push_back(')');
			}
			res.push_back(((akadora & 2) ? 0 : value) + '0');
			if (state % 10 == 2) {
				res.push_back('(');
				res.push_back(((akadora & 1) ? 0 : value) + '0');
				res.push_back(')');
			}
			res.push_back(((akadora & 4) ? 0 : value) + '0');
			if (state % 10 == 1) {
				res.push_back('(');
				res.push_back(((akadora & 1) ? 0 : value) + '0');
				res.push_back(')');
			}
		}
		res.push_back(color);
		return res;
	}

	Group() = default;

	Group(uint8_t value_, char color_, GroupType type_, uint8_t akadora_, uint8_t state_) : type(type_), color(color_),
	                                                                                        value(value_),
	                                                                                        akadora(akadora_),
	                                                                                        state(state_) {}

	bool operator <(const Group& rhs) const {
		if (type < rhs.type)return true;
		if (color < rhs.color)return true;
		if (value < rhs.value)return true;
		return false;
	}

	bool operator !=(const Group& rhs) const { return !(*this == rhs); }

	bool operator ==(const Group& rhs) const {
		if (type != rhs.type)return false;
		if (color != rhs.color)return false;
		if (value != rhs.value)return false;
		if (akadora != rhs.akadora)return false;
		if (state != rhs.state)return false;
		return true;
	}
};

extern Group Group_Null;
