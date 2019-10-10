#pragma once
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
	std::string getDisplay()const;
	std::string getString()const;
	bool isyaojiu();
};
//Null��Զ������
extern const Single Null;
