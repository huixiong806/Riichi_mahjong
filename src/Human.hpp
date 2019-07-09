#pragma once
#include"MovementGenerator.hpp"
#include<string>
#include<iostream>
const std::string windName[4] = { "��","��","��","��" };
class Human:public ActionGenerator
{
private:

public:
	Human() { name = "No Name"; }
	Human(std::string name_)
	{
		name = name_;
	}
	virtual Action generateAction(const GameInfo&& info)
	{
		Action res;
		res.type = ActionType::Null;
		if (info.nowWind != info.selfWind&&info.mingpai == false)return res;
		if (info.nowWind == info.selfWind&&info.mingpai == true)return res;
		std::cout << "�ǳ�:" << name << std::endl;
		std::cout << windName[(int)info.prevailingWind] << info.round<<"�� "<<info.benchang<<"���� ʣ��"<<info.remainTiles<<"��"<< std::endl;
		std::cout << "�Է�Ϊ" << windName[(int)info.selfWind]<<"  ��������:"<<Single((int)info.prevailingWind+1,'z',false).getString()<< "  �Է�����:" << Single((int)info.selfWind+1, 'z', false).getString() << std::endl;
		std::cout << "����ָʾ�� ";
		for (auto& item:info.doraIndicator)
		{
			std::cout << item.getString()<<" ";
		}
		std::cout << std::endl;
		std::cout << "�ƺ�:" << std::endl;
		std::cout << "���� " << std::endl;
		for (auto& item:info.playerInfo[0].discardTile)
		{
			std::cout<<item.getString()<<" ";
		}
		std::cout << std::endl;
		std::cout << "�ϼ� " << std::endl;
		for (auto& item : info.playerInfo[1].discardTile)
		{
			std::cout << item.getString() << " ";
		}
		std::cout << std::endl;
		std::cout << "���� " << std::endl;
		for (auto& item : info.playerInfo[2].discardTile)
		{
			std::cout << item.getString() << " ";
		}
		std::cout << std::endl;
		std::cout << "���� " << std::endl;
		for (auto& item : info.playerInfo[3].discardTile)
		{
			std::cout << item.getString() << " ";
		}
		std::cout << std::endl;
		std::cout << "����:" << std::endl;
		auto hand = info.handTile;
		for (auto& item:info.handTile)
		{
			std::cout << item.getString() << " ";
		}
		std::cout << std::endl;
		if (info.mingpai == false)
		{
			std::cout << "��������Ϊ" << info.nowTile.getString() << std::endl;
			std::cout << "��ڼ�����?(����ѡ0)" << std::endl;
			int index;
			std::cin >> index;
			res.type = ActionType::Dapai;
			if (index == 0)
				res.target = info.nowTile;
			else res.target = info.handTile[index - 1];
			std::cout << std::endl;
		}
		else
		{
			std::cout << windName[(int)info.nowWind] << "�Ҵ����" << info.nowTile.getString() << std::endl;
			std::cout << "��ѡ��һ�ֲ���:" << std::endl;
			std::cout << "0.���� 1.�� 2.�� 3.�� 4.��" << std::endl;
			int opt=0;
			std::cin >> opt;
			if (opt == 0)
			{
				res.type = ActionType::Skip;
			}
			else if (opt == 1)
			{

			}
			else if (opt == 2)
			{

			}
			else if (opt == 3)
			{

			}
			else
			{
				res.type = ActionType::Rong;
			}
		}
		return res;
	}
};
