#pragma once
#include<vector>
#include<string>
class Single
{
private:
	char mColor; //'m','p','s','z'
	uint8_t mValue;
	bool mAkadora; //是否为红宝牌
public:
	Single();
	Single(uint8_t value_, char color_,bool akadora_) :mColor(color_), mValue(value_),mAkadora(akadora_) {}
	//获取颜色
	char color()const;
	//获取值
	uint8_t value()const;
	//是否为红宝牌
	bool isAkadora()const;
	//获取顺序上的下一张牌(默认为非红宝牌)
	Single next();
	//判断两张牌是否相等，不考虑是否为红宝牌
	bool valueEqual(const Single& rhs)const
	{
		return mColor == rhs.mColor&&mValue == rhs.mValue;
	}
	//判断两张牌是否相等，考虑是否为红宝牌
	bool operator ==(const Single& rhs)const
	{
		return (mColor == rhs.mColor&&mValue == rhs.mValue&&rhs.mAkadora==mAkadora);
	}
	bool operator !=(const Single& rhs)const
	{
		return !(*this == rhs);
	}
	//判断两张牌的大小，考虑是否为红宝牌，万<饼<索<字<Null,数字相同的情况下红宝牌更大
	bool operator <(const Single& rhs)const
	{
		return (int)mColor * 1000 + mValue*10+ (int)mAkadora < (int)rhs.mColor * 1000 + rhs.mValue*10+ (int)rhs.mAkadora;
	}
	//判断两张牌的大小，考虑是否为红宝牌，万<饼<索<字<Null,数字相同的情况下红宝牌更大
	bool operator >(const Single& rhs)const
	{
		return (int)mColor * 1000 + mValue * 10 + (int)mAkadora > (int)rhs.mColor * 1000 + rhs.mValue * 10 + (int)rhs.mAkadora;
	}
	std::string getDisplay()const;
	std::string getString()const;
	bool isyaojiu();
};
//Null永远是最大的
extern const Single Null;
