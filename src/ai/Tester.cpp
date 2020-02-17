#include <string>
#include <cassert>
#include <set>
#include <iostream>
#include <iomanip>
#include "Tester.h"

const std::string windName[4] = {"东", "南", "西", "北"};
const std::string actionName[11] = {"error", "跳过", "error", "吃", "碰", "杠", "和", "立直", "自摸", "流局", "拔北"};

std::vector<Action> Tester::getAllNakiActions(const GameInfo& info) {
	std::vector<Action> res;
	res.emplace_back(ActionType::Skip);
	auto temp = getRonAction(info);
	res.insert(res.end(), temp.begin(), temp.end());
	return res;
}

std::vector<Action> Tester::getAllNormalActions(const GameInfo& info) {
	std::vector<Action> res;
	auto temp = getTsumoAction(info);
	if (info.nowTile != Null)
		res.emplace_back(ActionType::DiscardTile, info.nowTile);
	if (info.playerInfo[static_cast<int>(info.selfWind)].riichiJunme == -1)
		for (auto i : info.handTile)
			res.emplace_back(ActionType::DiscardTile, i);
	res.insert(res.end(), temp.begin(), temp.end());
	temp = getRiichiAction(info);
	res.insert(res.end(), temp.begin(), temp.end());
	return res;
}

void Tester::printActions(std::vector<Action> actions) {
	auto num = 0;
	for (auto i = 0; i < actions.size(); ++i) {
		if (actions[i].type == ActionType::DiscardTile)
			std::cout << num << " ";
		else {
			if (i > 0 && actions[i - 1].type == ActionType::DiscardTile) { std::cout << "打牌(0摸切)" << std::endl; }
			if (actions[i].type == ActionType::Chii || actions[i].type == ActionType::Pon || actions[i].type ==
				ActionType::Kan)
				std::cout << num << "." << actionName[static_cast<int>(actions[i].type)] << " " << actions[i]
				                                                                                   .group.getString() <<
					std::endl;
			else if (actions[i].type == ActionType::Riichi) {
				std::cout << num << ".切" << actions[i].target.getDisplay() << "立直" << std::endl;
			}
			else
				std::cout << num << "." << actionName[static_cast<int>(actions[i].type)] << std::endl;
		}
		num++;
	}
	if (actions[actions.size() - 1].type == ActionType::DiscardTile) { std::cout << "打牌(0摸切)" << std::endl; }
}

std::vector<Action> Tester::getRonAction(const GameInfo& info) {
	std::vector<Action> res;
	BonusYakuState state = BonusYakuState::Normal;
	if (info.w)BonusYakuState::FirstTurn;
	else if (info.remainTiles == 0)BonusYakuState::LastTurn;
	const auto result = Algorithms::agari(AgariParameters(info.selfWind, info.prevailingWind,
	                                                      info.playerInfo[info.selfWind].riichiJunme,
	                                                      info.playerInfo[info.selfWind].ippatsu,info.rinshan, state, AgariWays::Ron, info.nowTile,
	                                                      info.handTile, info.playerInfo[info.selfWind].groupTile,
	                                                      std::vector<Single>(), std::vector<Single>())).success;
	if (result)
		res.emplace_back(ActionType::Ron);
	return res;
}

std::vector<Action> Tester::getTsumoAction(const GameInfo& info) {
	std::vector<Action> res;
	BonusYakuState state = BonusYakuState::Normal;
	if (info.w)BonusYakuState::FirstTurn;
	else if (info.remainTiles == 0)BonusYakuState::LastTurn;
	const auto result = Algorithms::agari(AgariParameters(info.selfWind, info.prevailingWind,
	                                                      info.playerInfo[info.selfWind].riichiJunme,
	                                                      info.playerInfo[info.selfWind].ippatsu, info.rinshan, state, AgariWays::Tsumo, info.nowTile,
	                                                      info.handTile, info.playerInfo[info.selfWind].groupTile,
	                                                      std::vector<Single>(), std::vector<Single>())).success;
	if (result)
		res.emplace_back(ActionType::Tsumo);
	return res;
}

std::vector<Action> Tester::getRiichiAction(const GameInfo& info) {
	std::set<Action> res;
	if (info.playerInfo[static_cast<int>(info.selfWind)].score < 1000)
		return std::vector<Action>();
	if (info.playerInfo[static_cast<int>(info.selfWind)].riichiJunme != -1)
		return std::vector<Action>();
	if (info.remainTiles < 4)
		return std::vector<Action>();
	auto allTiles = info.handTile;
	allTiles.push_back(info.nowTile);
	for (auto& target : allTiles) {
		Player player;
		player.setInfo(0, info.playerInfo[static_cast<int>(info.selfWind)].score, info.selfWind, info.handTile,
			{}, info.playerInfo[static_cast<int>(info.selfWind)].groupTile, info.nowTile, {}, info.rinshan,
			false, info.playerInfo[static_cast<int>(info.selfWind)].riichi,
			info.playerInfo[static_cast<int>(info.selfWind)].riichiJunme,
			info.playerInfo[static_cast<int>(info.selfWind)].ippatsu);
		BonusYakuState state = BonusYakuState::Normal;
		if (info.w)state = BonusYakuState::FirstTurn;
		else if (info.remainTiles == 0)state = BonusYakuState::LastTurn;
		if (player.canRiichi(state, target)) { res.insert(Action(ActionType::Riichi, target)); }
	}
	std::vector<Action> ret;
	for (auto& item : res)
	ret.push_back(item);
	return ret;
}
std::vector<Single> Tester::getShowedTiles(const GameInfo& info){
	std::vector<Single> res;
	for (auto wind = 0; wind <= 3; ++wind) {
		for (auto& item : info.playerInfo[wind].groupTile) {
			if (item.type == GroupType::Kantsu){
				for(int i=0;i<4;++i)
					res.push_back(Single(item.value,item.color,false));
			}else if (item.type == GroupType::Koutsu) {
				for (int i = 0; i < 3; ++i)
					res.push_back(Single(item.value, item.color, false));
			}else if (item.type == GroupType::Shuntsu) {
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
	if (info.nowWind != info.selfWind && info.gameState == GameState::OneAct)return res;
	if (info.nowWind == info.selfWind && info.gameState == GameState::WaitingForNaki)return res;
	//if (info.mingpai == false && info.selfWind == info.nowWind)
	//	printInfo(info);
	if (info.gameState == GameState::OneAct) {
		if (info.nowTile != Null) {
			auto actions = getAllNormalActions(info);
			bool riichi = false;
			bool tsumo = false;
			auto templ = info.handTile;
			templ.push_back(info.nowTile);
			int nowXiangting = Algorithms::getDistance(CompactSingles(templ));
			int maxCount = -1;
			std::vector<int>pool=Algorithms::getPool(getShowedTiles(info));
			Action temp;
			Action res;
			
			/*for (auto& item : info.handTile)
				std::cout << item.getDisplay() << " ";
			std::cout << info.nowTile.getDisplay() << " ";
			std::cout << std::endl;*/
			for (auto& act : actions) {
				int count = 0;
				if (act.type == ActionType::Tsumo) {
					res = act;
					tsumo = true;
					break;
				}
				else if (act.type == ActionType::Riichi) {
					if (!riichi) {
						riichi = true;
						res = act;
					}
				}
				else if (act.type == ActionType::DiscardTile) {
					
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
							//std::cout << "打" << act.target.getDisplay() << ",";
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
			if(tsumo){
				return res;
			}
			else if (riichi) {
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
		auto actions = getAllNakiActions(info);
		if (actions.size() == 1)
			res = actions[0];
		else {
			res = actions[1];
		}
	}
	return res;
}
