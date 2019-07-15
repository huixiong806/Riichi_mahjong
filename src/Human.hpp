#pragma once
#include"MovementGenerator.hpp"
#include<string>
#include<cassert>
#include<set>
#include<iostream>
const std::string windName[4] = { "��","��","��","��" };
const std::string actionName[11] = { "error","����","��","��","��","��","error","����","��ֱ","����","�α�" };
class Human:public ActionGenerator
{
private:
	void printActions(std::vector<Action>actions);
	std::vector<Action> getPengActions(const GameInfo& info);
	std::vector<Action> getChiActions(const GameInfo& info);
	std::vector<Action>getAllActionsMingpai(const GameInfo& info);
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
	return res;
}
void Human::printActions(std::vector<Action>actions)
{
	int num = 0;
	for (auto item : actions)
	{
		if (item.type == ActionType::Chi || item.type == ActionType::Peng || item.type == ActionType::Gang)
			std::cout << num << "." << actionName[(int)item.type] << " " << item.group.getString() << std::endl;
		else std::cout << num << "." << actionName[(int)item.type] << std::endl;
		num++;
	}
}
std::vector<Action> Human::getChiActions(const GameInfo& info)
{
	//ֻ�ܳ��ϼ���
	if (info.nowWind != (info.selfWind + 3) % 4)return std::vector<Action>();
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
		if (info.nowTile != Null)
		{
			std::cout << "��������Ϊ" << info.nowTile.getString() << std::endl;
			std::cout << "��ڼ�����?(����ѡ0)" << std::endl;
		}
		else
		{
			std::cout << "��ڼ�����?" << std::endl;
		}
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