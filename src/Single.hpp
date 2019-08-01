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
	char color()const;
	//��ȡֵ
	uint8_t value()const;
	//�Ƿ�Ϊ�챦��
	bool isAkadora()const;
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
	//�ж������ƵĴ�С�������Ƿ�Ϊ�챦�ƣ���<��<��<��<Null,������ͬ������º챦�Ƹ���
	bool operator <(const Single& rhs)const
	{
		return (int)mColor * 1000 + mValue*10+ (int)mAkadora < (int)rhs.mColor * 1000 + rhs.mValue*10+ (int)rhs.mAkadora;
	}
	//�ж������ƵĴ�С�������Ƿ�Ϊ�챦�ƣ���<��<��<��<Null,������ͬ������º챦�Ƹ���
	bool operator >(const Single& rhs)const
	{
		return (int)mColor * 1000 + mValue * 10 + (int)mAkadora > (int)rhs.mColor * 1000 + rhs.mValue * 10 + (int)rhs.mAkadora;
	}
	std::string getString()const
	{
		std::string res;
		if(mAkadora)res.push_back('0');
		else res.push_back(mValue+'0');
		res.push_back(mColor);
		return res;
	}
	bool isyaojiu()
	{
		if (mColor == 'z')return true;
		if (mValue == 1 || mValue == 9)return true;
		return false;
	}
};
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
//Null��Զ������
const Single Null = { 1,'z'+1,true };
Single::Single()
{
	*this = Null;
}