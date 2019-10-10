#include"Single.h"
Single Single::next()
{
	if (mColor == 'z'&&mValue == 7)return Single(1, mColor, false);
	if (mValue == 9)return Single(1, mColor, false);
	return Single(mValue + 1, mColor, false);
}
char Single::color()const
{
	return mColor;
}
uint8_t Single::value()const
{
	return mValue;
}
bool Single::isAkadora()const
{
	return mAkadora;
}
Single::Single()
{
	*this = Null;
}
std::string Single::getString()const
{
	std::string res;
	if (mAkadora)res.push_back('0');
	else res.push_back(mValue + '0');
	res.push_back(mColor);
	return res;
}
bool Single::isyaojiu()
{
	if (mColor == 'z')return true;
	if (mValue == 1 || mValue == 9)return true;
	return false;
}
std::string Single::getDisplay()const
{
	std::string res;
	if (mColor != 'z')
	{
		res.push_back((mAkadora ? 0 : mValue) + '0');
		res.push_back(mColor);
	}
	else
	{
		const std::string zi[8] = { "??","东","南","西","北","白","发","中" };
		res = zi[mValue];
	}
	return res;
}
//Null永远是最大的
const Single Null = { 1,'z' + 1,true };