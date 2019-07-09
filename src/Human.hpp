#pragma once
#include"MovementGenerator.hpp"
#include<string>
#include<iostream>
const std::string windName[4] = { "东","南","西","北" };
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
		std::cout << "昵称:" << name << std::endl;
		std::cout << windName[(int)info.prevailingWind] << info.round<<"局 "<<info.benchang<<"本场 剩余"<<info.remainTiles<<"张"<< std::endl;
		std::cout << "自风为" << windName[(int)info.selfWind]<<"  场风役牌:"<<Single((int)info.prevailingWind+1,'z',false).getString()<< "  自风役牌:" << Single((int)info.selfWind+1, 'z', false).getString() << std::endl;
		std::cout << "宝牌指示牌 ";
		for (auto& item:info.doraIndicator)
		{
			std::cout << item.getString()<<" ";
		}
		std::cout << std::endl;
		std::cout << "牌河:" << std::endl;
		std::cout << "东家 " << std::endl;
		for (auto& item:info.playerInfo[0].discardTile)
		{
			std::cout<<item.getString()<<" ";
		}
		std::cout << std::endl;
		std::cout << "南家 " << std::endl;
		for (auto& item : info.playerInfo[1].discardTile)
		{
			std::cout << item.getString() << " ";
		}
		std::cout << std::endl;
		std::cout << "西家 " << std::endl;
		for (auto& item : info.playerInfo[2].discardTile)
		{
			std::cout << item.getString() << " ";
		}
		std::cout << std::endl;
		std::cout << "北家 " << std::endl;
		for (auto& item : info.playerInfo[3].discardTile)
		{
			std::cout << item.getString() << " ";
		}
		std::cout << std::endl;
		std::cout << "手牌:" << std::endl;
		auto hand = info.handTile;
		for (auto& item:info.handTile)
		{
			std::cout << item.getString() << " ";
		}
		std::cout << std::endl;
		if (info.mingpai == false)
		{
			std::cout << "摸到的牌为" << info.nowTile.getString() << std::endl;
			std::cout << "打第几张牌?(摸切选0)" << std::endl;
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
			std::cout << windName[(int)info.nowWind] << "家打出了" << info.nowTile.getString() << std::endl;
			std::cout << "请选择一种操作:" << std::endl;
			std::cout << "0.跳过 1.吃 2.碰 3.杠 4.和" << std::endl;
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
