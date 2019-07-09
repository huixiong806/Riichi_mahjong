#pragma once
#include"Mountain.hpp"
#include<vector>
#include<string>
class Single
{
private:
	char mColor; //'m','p','s','z'
	uint8_t mValue;
	bool mAkadora; //�Ƿ�Ϊ�챦��
public:
	Single();
	Single(uint8_t value_, char color_,bool akadora_) :mColor(color_), mValue(value_),mAkadora(akadora_) {}
	//��ȡ��ɫ
	char color();
	//��ȡֵ
	uint8_t value();
	//�Ƿ�Ϊ�챦��
	bool isAkadora();
	//��ȡ˳���ϵ���һ����(Ĭ��Ϊ�Ǻ챦��)
	Single next();
	//�ж��������Ƿ���ȣ��������Ƿ�Ϊ�챦��
	bool valueEqual(const Single& rhs)const
	{
		return mColor == rhs.mColor&&mValue == rhs.mValue;
	}
	//�ж��������Ƿ���ȣ������Ƿ�Ϊ�챦��
	bool operator ==(const Single& rhs)const
	{
		return (mColor == rhs.mColor&&mValue == rhs.mValue&&rhs.mAkadora==mAkadora);
	}
	bool operator !=(const Single& rhs)const
	{
		return !(*this == rhs);
	}
	//�ж������ƵĴ�С���������Ƿ�Ϊ�챦�ƣ���<��<��<��
	bool operator <(const Single& rhs)const
	{
		return (int)mColor * 100 + mValue < (int)rhs.mColor * 100 + rhs.mValue;
	}
	std::string getString()const
	{
		std::string res;
		if(mAkadora)res.push_back('0');
		else res.push_back(mValue+'0');
		res.push_back(mColor);
		return res;
	}
};
Single Single::next()
{
	if (mColor == 'z'&&mValue == 7)return Single(1, mColor, false);
	if (mValue == 9)return Single(1, mColor, false);
	return Single(mValue + 1, mColor, false);
}
char Single::color()
{
	return mColor;
}
uint8_t Single::value()
{
	return mValue;
}
bool Single::isAkadora()
{
	return mAkadora;
}
const Single Null = { 1,'z'+1,false };
Single::Single()
{
	*this = Null;
}