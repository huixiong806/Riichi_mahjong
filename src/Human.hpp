#pragma once
#include"MovementGenerator.hpp"
#include"Player.hpp"
#include<string>
#include<cassert>
#include<set>
#include<iostream>
#include<iomanip>
const std::string windName[4] = { "东","南","西","北" };
const std::string actionName[11] = { "error","跳过","error","吃","碰","杠","和","立直","自摸","流局","拔北" };
class Human:public ActionGenerator
{
private:
	void printActions(std::vector<Action>actions);
	std::vector<Action> getRongGangActions(const GameInfo& info);
	std::vector<Action> getPengActions(const GameInfo& info);
	std::vector<Action> getChiActions(const GameInfo& info);
	std::vector<Action> getZimoAction(const GameInfo& info);
	std::vector<Action> getLizhiAction(const GameInfo& info);
	std::vector<Action> getRongAction(const GameInfo& info);
	std::vector<Action>getAllActionsMingpai(const GameInfo& info);
	std::vector<Action>getAllActionsNormal(const GameInfo& info);
	void printInfo(const GameInfo& info);
public:
	Human() { name = "No Name"; }
	Human(std::string name_)
	{
		name = name_;
	}
	virtual Action generateAction(const GameInfo& info);
};
std::vector<Action> Human::getAllActionsMingpai(const GameInfo& info)
{
	std::vector<Action> res;
	res.push_back(ActionType::Skip);
	std::vector<Action> temp = getPengActions(info);
	res.insert(res.end(), temp.begin(), temp.end());
	temp = getRongGangActions(info);
	res.insert(res.end(), temp.begin(), temp.end());
	temp = getRongAction(info);
	res.insert(res.end(), temp.begin(), temp.end());
	return res;
}
std::vector<Action> Human::getAllActionsNormal(const GameInfo& info)
{
	std::vector<Action> res;
	std::vector<Action> temp = getZimoAction(info);
	if (info.nowTile != Null)
		res.push_back(Action(ActionType::Dapai,info.nowTile));
	if (info.playerInfo[(int)info.selfWind].lizhiXunmu == -1)
		for (int i = 0; i < info.handTile.size(); ++i)
			res.push_back(Action(ActionType::Dapai, info.handTile[i]));
	res.insert(res.end(), temp.begin(), temp.end());
	temp = getLizhiAction(info);
	res.insert(res.end(), temp.begin(), temp.end());
	return res;
}
void Human::printActions(std::vector<Action>actions)
{
	int num = 0;
	for (int i=0;i<actions.size();++i)
	{
		if (actions[i].type == ActionType::Dapai)
			std::cout << num <<" ";
		else
		{
			if (i>0 && actions[i - 1].type== ActionType::Dapai)
			{
				std::cout << "打牌(0摸切)" << std::endl;
			}
			if (actions[i].type == ActionType::Chi || actions[i].type == ActionType::Peng || actions[i].type == ActionType::Gang)
				std::cout << num << "." << actionName[(int)actions[i].type] << " " << actions[i].group.getString() << std::endl;
			else if (actions[i].type == ActionType::Lizhi)
			{
				std::cout << num << ".切"<<actions[i].target.getDisplay()<<"立直"<< std::endl;
			}
			else
				std::cout << num << "." << actionName[(int)actions[i].type] << std::endl;
		}
		num++;
	}
	if (actions[actions.size() - 1].type == ActionType::Dapai)
	{
		std::cout << "打牌(0摸切)" << std::endl;
	}
}
std::vector<Action> Human::getRongAction(const GameInfo& info)
{
	std::vector<Action> res;
	int state = 0;
	if (info.w)state = 1;
	else if (info.remainTiles == 0)state = 2;
	bool result = Algorithms::agari(AgariParameters(info.selfWind, info.prevailingWind, info.playerInfo[info.selfWind].lizhiXunmu, info.playerInfo[info.selfWind].yifa, state, 1, info.nowTile, info.handTile, info.playerInfo[info.selfWind].groupTile, std::vector<Single>(), std::vector<Single>())).success;
	if (result)
		res.push_back(Action(ActionType::Rong));
	return res;
}
std::vector<Action> Human::getZimoAction(const GameInfo& info)
{
	std::vector<Action> res;
	int state=0;
	if (info.w)state = 1;
	else if (info.remainTiles == 0)state = 2;
	bool result = Algorithms::agari(AgariParameters(info.selfWind, info.prevailingWind, info.playerInfo[info.selfWind].lizhiXunmu, info.playerInfo[info.selfWind].yifa, state, 0, info.nowTile, info.handTile, info.playerInfo[info.selfWind].groupTile, std::vector<Single>(), std::vector<Single>())).success;
	if (result)
		res.push_back(Action(ActionType::Zimo));
	return res;
}
std::vector<Action> Human::getLizhiAction(const GameInfo& info)
{
	std::set<Action> res;
	if (info.playerInfo[(int)info.selfWind].lizhiXunmu != -1)
		return std::vector<Action>();
	auto allTiles = info.handTile;
	allTiles.push_back(info.nowTile);
	for (auto& target : allTiles)
	{
		Player player;
		player.setInfo(0, info.playerInfo[(int)info.selfWind].score, info.selfWind, info.handTile,
			{}, info.playerInfo[(int)info.selfWind].groupTile, info.nowTile, {}, info.lingshang, 0, info.playerInfo[(int)info.selfWind].lizhi,
			info.playerInfo[(int)info.selfWind].lizhiXunmu, info.playerInfo[(int)info.selfWind].yifa);
		int state = 0;
		if (info.w)state = 1;
		else if (info.remainTiles == 0)state = 2;
		if (player.doLizhi(state, target))
		{
			res.insert(Action(ActionType::Lizhi, target));
		}
	}
	std::vector<Action> ret;
	for (auto& item : res)
		ret.push_back(item);
	return ret;
}
std::vector<Action> Human::getChiActions(const GameInfo& info)
{
	//只能吃上家牌
	if (info.nowWind != (info.selfWind + 3) % 4)return std::vector<Action>();
	if (info.playerInfo[(int)info.selfWind].lizhiXunmu != -1)
		return std::vector<Action>();
}
std::vector<Action> Human::getRongGangActions(const GameInfo& info)
{
	std::vector<Action>res;
	if (info.playerInfo[(int)info.selfWind].lizhiXunmu!=-1)
		return res;
	Single target = info.nowTile;
	std::vector<Single>candidate;
	for (auto& item : info.handTile)
		if (item.valueEqual(target))
			candidate.push_back(target);
	if (candidate.size() < 3)return res;
	assert(candidate.size() == 3);
	res.push_back(Action(ActionType::Gang, Group::createGangzi(candidate[0], candidate[1], candidate[2], target, (info.nowWind - info.selfWind + 4) % 4)));
	return res;
}
std::vector<Action> Human::getPengActions(const GameInfo& info)
{
	std::vector<Action>res;
	if (info.playerInfo[(int)info.selfWind].lizhiXunmu != -1)
		return res;
	Single target = info.nowTile;
	std::vector<Single>candidate;
	for (auto& item : info.handTile)
		if (item.valueEqual(target))
			candidate.push_back(target);
	if (candidate.size() < 2)return res;
	if (candidate.size() == 2)
	{
		res.push_back(Action(ActionType::Peng, Group::createKezi(candidate[0], candidate[1], target, (info.nowWind - info.selfWind + 4) % 4)));
		return res;
	}
	assert(candidate.size() == 3);
	std::set<Group>temp;
	temp.emplace(Group::createKezi(candidate[0], candidate[1], target, (info.nowWind - info.selfWind + 4) % 4));
	temp.emplace(Group::createKezi(candidate[1], candidate[2], target, (info.nowWind - info.selfWind + 4) % 4));
	temp.emplace(Group::createKezi(candidate[0], candidate[2], target, (info.nowWind - info.selfWind + 4) % 4));
	for (auto item : temp)
	{
		res.emplace_back(Action(ActionType::Peng, item));
	}
	return res;
}
void Human::printInfo(const GameInfo& info)
{
	std::cout << std::endl;
	std::cout << name << std::endl;
	std::cout << windName[(int)info.prevailingWind] << info.round << "局 " << info.benchang << "本场 剩余" << info.remainTiles << "张" << std::endl;
	std::cout << "自风为" << windName[(int)info.selfWind] << std::endl;
	//<< "  场风役牌:" << Single((int)info.prevailingWind + 1, 'z', false).getString() << "  自风役牌:" << Single((int)info.selfWind + 1, 'z', false).getString() << std::endl;
	std::cout << "宝牌指示牌 ";
	for (auto& item : info.doraIndicator)
	{
		std::cout << item.getString() << " ";
	}
	std::cout << std::endl;
	std::cout << "信息:" << std::endl;
	for (int wind = 0; wind <= 3; ++wind)
	{
		std::cout << windName[wind] << "家  " << std::setw(7) << std::left << info.playerInfo[wind].score << std::setw(0) << "|";
		for (auto& item : info.playerInfo[wind].groupTile)
		{
			std::cout << item.getString() << " ";
		}
		std::cout << std::endl;
		for (auto& item : info.playerInfo[wind].discardTile)
		{
			std::cout << item.getDisplay() << " ";
		}
		std::cout << std::endl << std::endl;
	}
	auto hand = info.handTile;
	hand.push_back(info.nowTile);
	std::cout << "当前标准型" << Algorithms::getDistance14Standard(hand) << "向听，七对" << Algorithms::getDistance14Qidui(hand) << "向听"<<std::endl;
	std::cout << windName[info.selfWind] << "| ";
	
	for (auto& item : info.handTile)
		std::cout << item.getDisplay() << " ";
	
	std::cout << std::endl;
	std::cout << "ID| ";
	for (int j = 0; j < (13 - info.playerInfo[info.selfWind].groupTile.size() * 3); ++j)
	{
		if (j < 9)std::cout << " ";
		std::cout << j + 1 << " ";
	}
	std::cout << std::endl;
}
Action Human::generateAction(const GameInfo& info)
{
	Action res;
	res.type = ActionType::Null;
	if (info.nowWind != info.selfWind&&info.mingpai == false)return res;
	if (info.nowWind == info.selfWind&&info.mingpai == true)return res;
	if (info.mingpai == false && info.selfWind == info.nowWind)
		printInfo(info);
	if (info.mingpai == false)
	{
		std::vector<Action>actions;
		if (info.nowTile != Null)
		{
			std::cout << "摸 " << info.nowTile.getDisplay() << std::endl << std::endl;
			actions = getAllActionsNormal(info);
			std::cout << "请选择:" << std::endl;
			printActions(actions);
			int index;
			std::cin >> index;
			res = actions[index];

		}
		else
		{
			std::cout << "打第几张牌?(0为摸切)" << std::endl;
			int index;
			std::cin >> index;
			if (index <= info.handTile.size())
			{
				res.type = ActionType::Dapai;
				if (index == 0)
					res.target = info.nowTile;
				else res.target = info.handTile[index - 1];
			}
		}
		std::cout << std::endl;
	}
	else
	{
		
		std::vector<Action>actions = getAllActionsMingpai(info);
		if (actions.size() != 1)
			printInfo(info);
		std::cout << windName[(int)info.nowWind] << "家打出了" << info.nowTile.getDisplay() << std::endl;
		if (actions.size() == 1)
			res = actions[0];
		else
		{
			std::cout << "请选择一种操作:" << std::endl;
			printActions(actions);
			int opt = 0;
			std::cin >> opt;
			res = actions[opt];
		}
	}
	return res;
}
