#include"Single.h"

std::string Single::getString() const {
	std::string res;
	if (mAkadora)res.push_back('0');
	else res.push_back(mValue + '0');
	res.push_back(mColor);
	return res;
}

std::string Single::getDisplay() const {
	std::string res;
	if (mColor != 'z') {
		res.push_back((mAkadora ? 0 : mValue) + '0');
		res.push_back(mColor);
	}
	else {
		const std::string zi[8] = {"??", "东", "南", "西", "北", "白", "发", "中"};
		res = zi[mValue];
	}
	return res;
}
