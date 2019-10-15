#include <string>
#include <cassert>
#include <set>
#include <iostream>
#include <iomanip>
#include "Tester.h"

const std::string windName[4] = {"东", "南", "西", "北"};
const std::string actionName[11] = {"error", "跳过", "error", "吃", "碰", "杠", "和", "立直", "自摸", "流局", "拔北"};

std::vector<Action> Tester::getAllActionsMingpai(const GameInfo& info) {
	std::vector<Action> res;
	res.emplace_back(ActionType::Skip);
	auto temp = getRongAction(info);
	res.insert(res.end(), temp.begin(), temp.end());
	return res;
}

std::vector<Action> Tester::getAllActionsNormal(const GameInfo& info) {
	std::vector<Action> res;
	auto temp = getZimoAction(info);
	if (info.nowTile != Null)
		res.emplace_back(ActionType::Dapai, info.nowTile);
	if (info.playerInfo[static_cast<int>(info.selfWind)].lizhiXunmu == -1)
		for (auto i : info.handTile)
			res.emplace_back(ActionType::Dapai, i);
	res.insert(res.end(), temp.begin(), temp.end());
	temp = getLizhiAction(info);
	res.insert(res.end(), temp.begin(), temp.end());
	return res;
}

void Tester::printActions(std::vector<Action> actions) {
	auto num = 0;
	for (auto i = 0; i < actions.size(); ++i) {
		if (actions[i].type == ActionType::Dapai)
			std::cout << num << " ";
		else {
			if (i > 0 && actions[i - 1].type == ActionType::Dapai) { std::cout << "打牌(0摸切)" << std::endl; }
			if (actions[i].type == ActionType::Chi || actions[i].type == ActionType::Peng || actions[i].type ==
				ActionType::Gang)
				std::cout << num << "." << actionName[static_cast<int>(actions[i].type)] << " " << actions[i]
				                                                                                   .group.getString() <<
					std::endl;
			else if (actions[i].type == ActionType::Lizhi) {
				std::cout << num << ".切" << actions[i].target.getDisplay() << "立直" << std::endl;
			}
			else
				std::cout << num << "." << actionName[static_cast<int>(actions[i].type)] << std::endl;
		}
		num++;
	}
	if (actions[actions.size() - 1].type == ActionType::Dapai) { std::cout << "打牌(0摸切)" << std::endl; }
}

std::vector<Action> Tester::getRongAction(const GameInfo& info) {
	std::vector<Action> res;
	auto state = 0;
	if (info.w)state = 1;
	else if (info.remainTiles == 0)state = 2;
	const auto result = Algorithms::agari(AgariParameters(info.selfWind, info.prevailingWind,
	                                                      info.playerInfo[info.selfWind].lizhiXunmu,
	                                                      info.playerInfo[info.selfWind].yifa, state, 1, info.nowTile,
	                                                      info.handTile, info.playerInfo[info.selfWind].groupTile,
	                                                      std::vector<Single>(), std::vector<Single>())).success;
	if (result)
		res.emplace_back(ActionType::Rong);
	return res;
}

std::vector<Action> Tester::getZimoAction(const GameInfo& info) {
	std::vector<Action> res;
	auto state = 0;
	if (info.w)state = 1;
	else if (info.remainTiles == 0)state = 2;
	const auto result = Algorithms::agari(AgariParameters(info.selfWind, info.prevailingWind,
	                                                      info.playerInfo[info.selfWind].lizhiXunmu,
	                                                      info.playerInfo[info.selfWind].yifa, state, 0, info.nowTile,
	                                                      info.handTile, info.playerInfo[info.selfWind].groupTile,
	                                                      std::vector<Single>(), std::vector<Single>())).success;
	if (result)
		res.emplace_back(ActionType::Zimo);
	return res;
}

std::vector<Action> Tester::getLizhiAction(const GameInfo& info) {
	std::set<Action> res;
	if (info.playerInfo[static_cast<int>(info.selfWind)].score < 1000)
		return std::vector<Action>();
	if (info.playerInfo[static_cast<int>(info.selfWind)].lizhiXunmu != -1)
		return std::vector<Action>();
	if (info.remainTiles < 4)
		return std::vector<Action>();
	auto allTiles = info.handTile;
	allTiles.push_back(info.nowTile);
	for (auto& target : allTiles) {
		Player player;
		player.setInfo(0, info.playerInfo[static_cast<int>(info.selfWind)].score, info.selfWind, info.handTile,
			{}, info.playerInfo[static_cast<int>(info.selfWind)].groupTile, info.nowTile, {}, info.lingshang,
			false, info.playerInfo[static_cast<int>(info.selfWind)].lizhi,
			info.playerInfo[static_cast<int>(info.selfWind)].lizhiXunmu,
			info.playerInfo[static_cast<int>(info.selfWind)].yifa);
		auto state = 0;
		if (info.w)state = 1;
		else if (info.remainTiles == 0)state = 2;
		if (player.doLizhi(state, target)) { res.insert(Action(ActionType::Lizhi, target)); }
	}
	std::vector<Action> ret;
	for (auto& item : res)
	ret.push_back(item);
	return ret;
}
std::vector<Single> Tester::getShowedTiles(const GameInfo& info)
{
	std::vector<Single> res;
	for (auto wind = 0; wind <= 3; ++wind) {
		for (auto& item : info.playerInfo[wind].groupTile) {
			if (item.type == GroupType::Gang){
				for(int i=0;i<4;++i)
					res.push_back(Single(item.value,item.color,false));
			}else if (item.type == GroupType::Kezi) {
				for (int i = 0; i < 3; ++i)
					res.push_back(Single(item.value, item.color, false));
			}else if (item.type == GroupType::Shunzi) {
				for (int i = 0; i < 3; ++i)
					res.push_back(Single(item.value+i, item.color, false));
			}
		}
		for (auto& item : info.playerInfo[wind].discardTile) { 
			res.push_back(item);
		}
	}
	for (auto& item : info.handTile) {
		res.push_back(item);
	}
	for(auto& item: info.doraIndicator)
		res.push_back(item);
	res.push_back(info.nowTile);
	return res;
}
Action Tester::generateAction(const GameInfo& info) {
	Action res;
	res.type = ActionType::Null;
	if (info.nowWind != info.selfWind && info.mingpai == false)return res;
	if (info.nowWind == info.selfWind && info.mingpai == true)return res;
	//if (info.mingpai == false && info.selfWind == info.nowWind)
	//	printInfo(info);
	if (info.mingpai == false) {
		if (info.nowTile != Null) {
			auto actions = getAllActionsNormal(info);
			bool lizhi = false;
			bool zimo = false;
			auto templ = info.handTile;
			templ.push_back(info.nowTile);
			int nowXiangting = Algorithms::getDistance(CompactSingles(templ));
			int maxCount = -1;
			std::vector<int>pool=Algorithms::getPool(getShowedTiles(info));
			Action temp;
			Action res;
			/*
			for (auto& item : info.handTile)
				std::cout << item.getDisplay() << " ";
			std::cout << info.nowTile.getDisplay() << " ";
			std::cout << std::endl;*/
			for (auto& act : actions) {
				int count = 0;
				if (act.type == ActionType::Zimo) {
					res = act;
					zimo = true;
					break;
				}
				else if (act.type == ActionType::Lizhi) {
					if (!lizhi) {
						lizhi = true;
						res = act;
					}
				}
				else if (act.type == ActionType::Dapai) {
					//std::cout << "打" << act.target.getDisplay() << ",";
					auto allTiles = info.handTile;
					allTiles.push_back(info.nowTile);
					for (auto& item : allTiles) {
						if (item == act.target) {
							auto allKindsOfTiles = Algorithms::allKindsOfTiles();
							//表示摸下一张牌后的最小向听数
							int minXiangtingThis = 999;
							//表示摸下一张牌后的最大向听数
							int maxXiangtingThis = -2;
							for (auto& pai : allKindsOfTiles) {
								item = pai;
								int xiangting = Algorithms::getDistance(CompactSingles(allTiles));
								if (xiangting < minXiangtingThis)
									minXiangtingThis =xiangting;
								if (xiangting > maxXiangtingThis)
									maxXiangtingThis = xiangting;
							}
							for (auto& pai : allKindsOfTiles) {
								item = pai;
								int xiangting = Algorithms::getDistance(CompactSingles(allTiles));
								if (xiangting == minXiangtingThis) {
									count += 4 - pool[Algorithms::getTileIndex(pai)];
								}
							}
							if (maxXiangtingThis > nowXiangting) {
								//std::cout << "向听倒退" << std::endl;
								break;
							}
							//std::cout << maxXiangtingThis << "向听,枚数:" << count << std::endl;
							if (count > maxCount) {
								maxCount = count;
								temp = act;
							}
							break;
						}
					}
				}
			}
			if(zimo){
				return res;
			}
			else if (lizhi) {
				return res;
			}
			else {
				return temp;
			}
		}
		else {
		}
	}
	else {
		auto actions = getAllActionsMingpai(info);
		if (actions.size() == 1)
			res = actions[0];
		else {
			res = actions[1];
		}
	}
	return res;
}
