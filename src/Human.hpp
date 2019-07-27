#pragma once
#include"MovementGenerator.hpp"
#include"Player.hpp"
#include<string>
#include<cassert>
#include<set>
#include<iostream>
const std::string windName[4] = { "��","��","��","��" };
const std::string actionName[11] = { "error","����","��","��","��","��","error","��ֱ","����","����","�α�" };
class Human:public ActionGenerator
{
private:
	void printActions(std::vector<Action>actions);
	std::vector<Action> getRongGangActions(const GameInfo& info);
	std::vector<Action> getPengActions(const GameInfo& info);
	std::vector<Action> getChiActions(const GameInfo& info);
	std::vector<Action> getZimoAction(const GameInfo& info);
	std::vector<Action>getAllActionsMingpai(const GameInfo& info);
	std::vector<Action>getAllActionsNormal(const GameInfo& info);
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
	return res;
}
std::vector<Action> Human::getAllActionsNormal(const GameInfo& info)
{
	std::vector<Action> res;
	std::vector<Action> temp = getZimoAction(info);
	if (info.nowTile != Null)
		res.push_back(Action(ActionType::Dapai,info.nowTile));
	for (int i = 0; i < info.handTile.size(); ++i)
		res.push_back(Action(ActionType::Dapai, info.handTile[i]));
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
				std::cout << "Ϊ�����Ӧ����(0Ϊ����)" << std::endl;
			}
			if (actions[i].type == ActionType::Chi || actions[i].type == ActionType::Peng || actions[i].type == ActionType::Gang)
				std::cout << num << "." << actionName[(int)actions[i].type] << " " << actions[i].group.getString() << std::endl;
			else std::cout << num << "." << actionName[(int)actions[i].type] << std::endl;
		}
		num++;
	}
	if (actions[actions.size() - 1].type == ActionType::Dapai)
	{
		std::cout << "Ϊ�����Ӧ����(0Ϊ����)" << std::endl;
	}
}
std::vector<Action> Human::getZimoAction(const GameInfo& info)
{
	std::vector<Action> res;
	Player player;
	player.groupTile = info.playerInfo[(int)info.selfWind].groupTile;
	player.lizhi = info.playerInfo[(int)info.selfWind].lizhi;
	player.yifa = info.playerInfo[(int)info.selfWind].yifa;
	player.lingshang = info.lingshang;
	player.handTile = info.handTile;
	player.nowTile = info.nowTile;
	int state=0;
	if (info.w)state = 1;
	else if (info.remainTiles == 0)state = 2;
	auto result=player.zimo(state,std::vector<Single>(), std::vector<Single>());
	if (result.success)
	{
		res.push_back(Action(ActionType::Zimo));
	}
	return res;
}
std::vector<Action> Human::getChiActions(const GameInfo& info)
{
	//ֻ�ܳ��ϼ���
	if (info.nowWind != (info.selfWind + 3) % 4)return std::vector<Action>();
}
std::vector<Action> Human::getRongGangActions(const GameInfo& info)
{
	std::vector<Action>res;
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
Action Human::generateAction(const GameInfo& info)
{
	Action res;
	res.type = ActionType::Null;
	if (info.nowWind != info.selfWind&&info.mingpai == false)return res;
	if (info.nowWind == info.selfWind&&info.mingpai == true)return res;
	std::cout << "�ǳ�:" << name << std::endl;
	std::cout << windName[(int)info.prevailingWind] << info.round << "�� " << info.benchang << "���� ʣ��" << info.remainTiles << "��" << std::endl;
	std::cout << "�Է�Ϊ" << windName[(int)info.selfWind] << "  ��������:" << Single((int)info.prevailingWind + 1, 'z', false).getString() << "  �Է�����:" << Single((int)info.selfWind + 1, 'z', false).getString() << std::endl;
	std::cout << "����ָʾ�� ";
	for (auto& item : info.doraIndicator)
	{
		std::cout << item.getString() << " ";
	}
	std::cout << std::endl;
	std::cout << "�ƺ�:" << std::endl;
	std::cout << "���� " << std::endl;
	for (auto& item : info.playerInfo[0].discardTile)
	{
		std::cout << item.getString() << " ";
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
	for (int i = 0; i < 4; ++i)
	{
		std::cout<<windName[i]<<" ";
		if (i == info.selfWind)
		{
			auto hand = info.handTile;
			for (auto& item : info.handTile)
			{
				std::cout << item.getString() << " ";
			}
		}
		else
		{
			for (int j = 0; j < (13 - info.playerInfo[i].groupTile.size() * 3); ++j)
			{
				std::cout << "??" << " ";
			}
		}
		for (auto& item : info.playerInfo[i].groupTile)
		{
			std::cout << item.getString() << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	if (info.mingpai == false)
	{
		std::vector<Action>actions;
		if (info.nowTile != Null)
		{
			std::cout << "��������Ϊ" << info.nowTile.getString() << std::endl;
			actions = getAllActionsNormal(info);
			std::cout << "��ѡ��һ�ֲ���:" << std::endl;
			printActions(actions);
			int index;
			std::cin >> index;
			res = actions[index];
		}
		else
		{
			std::cout << "��ڼ�����?(0Ϊ����)" << std::endl;
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
		std::cout << windName[(int)info.nowWind] << "�Ҵ����" << info.nowTile.getString() << std::endl;	
		std::vector<Action>actions = getAllActionsMingpai(info);
		if (actions.size() == 1)
		{
			res = actions[0];
		}
		else
		{
			std::cout << "��ѡ��һ�ֲ���:" << std::endl;
			printActions(actions);
			int opt = 0;
			std::cin >> opt;
			res = actions[opt];
		}
	}
	return res;
}