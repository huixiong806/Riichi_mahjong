#include"Single.h"
#include"BookManager.h"
#include"Algorithms.h"
std::string Single::getString() const {
	std::string res;
	if (mAkadora)res.push_back('0');
	else res.push_back(mValue + '0');
	res.push_back(mColor);
	return res;
}

std::string Single::getDisplay() const {
	std::string res= BookManager::lang.p_pai[Algorithms::getTileIndex(*this)];
	if (isAkadora()) {
		if(color()=='m')
			res= BookManager::lang.p_pai[34];
		else if(color()=='p')
			res = BookManager::lang.p_pai[35];
		else if (color() == 's')
			res = BookManager::lang.p_pai[36];
	}
	return res;
}
