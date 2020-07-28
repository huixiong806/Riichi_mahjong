#include"AI.h"
#include <string>
#include <cassert>
#include <set>
#include <iostream>
#include <iomanip>
#include "Human.h"
#include "InformationStream.hpp";
std::vector<Action> AI::getAllNakiActions(const GameInfo& info) {
	std::vector<Action> res;
	res.emplace_back(ActionType::Skip);
	auto temp = getChiiActions(info);
	res.insert(res.end(), temp.begin(), temp.end());
	temp = getPonActions(info);
	res.insert(res.end(), temp.begin(), temp.end());
	temp = getMinKanActions(info);
	res.insert(res.end(), temp.begin(), temp.end());
	temp = getRonAction(info);
	res.insert(res.end(), temp.begin(), temp.end());
	return res;
}

std::vector<Action> AI::getAllNormalActions(const GameInfo& info) {
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



void AI::printActions(std::vector<Action> actions) {
	auto num = 0;
	for (auto i = 0; i < actions.size(); ++i) {
		if (actions[i].type == ActionType::DiscardTile)
			std::cout << num << " ";
		else {
			if (i > 0 && actions[i - 1].type == ActionType::DiscardTile)
				std::cout << BookManager::lang.mj_discardTile << "(0" + BookManager::lang.mj_tsumogiri + ")" << std::endl;
			if (actions[i].type == ActionType::Chii || actions[i].type == ActionType::Pon || actions[i].type ==
				ActionType::Kan)
				std::cout << num << "." << BookManager::lang.ct_action[static_cast<int>(actions[i].type)] << " " << actions[i]
				.group.getString() <<
				std::endl;
			else if (actions[i].type == ActionType::Riichi) {
				std::cout << num << "." << BookManager::lang.mj_discard << actions[i].target.getDisplay() << BookManager::lang.mj_riichi << std::endl;
			}
			else
				std::cout << num << "." << BookManager::lang.ct_action[static_cast<int>(actions[i].type)] << std::endl;
		}
		num++;
	}
	if (actions[actions.size() - 1].type == ActionType::DiscardTile)
		std::cout << BookManager::lang.mj_discardTile << "(0" + BookManager::lang.mj_tsumogiri + ")" << std::endl;
}
void AI::printAction(const Action& action) {
	auto num = 0;
	if (action.type == ActionType::DiscardTile)
		std::cout << BookManager::lang.mj_discard << " " << action.target.getDisplay();
	else {
		if (action.type == ActionType::Chii || action.type == ActionType::Pon || action.type ==
			ActionType::Kan)
			std::cout << BookManager::lang.ct_action[static_cast<int>(action.type)] << " " << action.group.getString();
		else if (action.type == ActionType::Riichi) {
			std::cout<< BookManager::lang.mj_discard << action.target.getDisplay() << BookManager::lang.mj_riichi;
		}
		else
			std::cout<< BookManager::lang.ct_action[static_cast<int>(action.type)];
	}
}
std::vector<Action> AI::getRonAction(const GameInfo& info) {
	std::vector<Action> res;
	if (info.riichiFuriten || info.doujunFuriten || info.sutehaiFuriten)
		return res;
	BonusYakuState state = BonusYakuState::Normal;
	if (info.w)BonusYakuState::FirstTurn;
	else if (info.remainTiles == 0)BonusYakuState::LastTurn;
	const auto result = Algorithms::agari(AgariParameters(info.selfWind, info.prevailingWind,
		info.playerInfo[info.selfWind].riichiJunme,
		info.playerInfo[info.selfWind].ippatsu,
		info.rinshan, state, AgariWays::Ron, info.nowTile,
		info.handTile, info.playerInfo[info.selfWind].groupTile,
		std::vector<Single>(), std::vector<Single>())).success;
	if (result)
		res.emplace_back(ActionType::Ron);
	return res;
}

std::vector<Action> AI::getTsumoAction(const GameInfo& info) {
	std::vector<Action> res;
	BonusYakuState state = BonusYakuState::Normal;
	if (info.w)BonusYakuState::FirstTurn;
	else if (info.remainTiles == 0)BonusYakuState::LastTurn;
	const auto result = Algorithms::agari(AgariParameters(info.selfWind, info.prevailingWind,
		info.playerInfo[info.selfWind].riichiJunme,
		info.playerInfo[info.selfWind].ippatsu,
		info.rinshan, state, AgariWays::Tsumo, info.nowTile,
		info.handTile, info.playerInfo[info.selfWind].groupTile,
		std::vector<Single>(), std::vector<Single>())).success;
	if (result)
		res.emplace_back(ActionType::Tsumo);
	return res;
}

std::vector<Action> AI::getRiichiAction(const GameInfo& info) {
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

std::vector<Action> AI::getChiiActions(const GameInfo& info) {
	std::vector<Action> res;
	//只能吃上家牌
	if (info.nowWind != (info.selfWind + 3) % 4)return res;
	if (info.playerInfo[static_cast<int>(info.selfWind)].riichiJunme != -1)
		return res;
	const auto target = info.nowTile;
	//不能吃字牌
	if (target.color() == 'z')return res;
	std::set<Group> resultGroup;
	//假设被吃的牌是5，i=0表示34吃5，i=1表示46吃5，i=2表示67吃5
	for (int i = 0; i < 3; ++i) {
		int delta[2];//以target为中心，要寻找的搭子的两张牌的偏移量
		if (i == 0) {
			delta[0] = -2;
			delta[1] = -1;
		}
		else if (i == 1) {
			delta[0] = -1;
			delta[1] = 1;
		}
		else if (i == 2) {
			delta[0] = 1;
			delta[1] = 2;
		}
		//寻找搭子候选
		std::vector<Single> candidate[2];
		int found[2] = { 0 };
		for (auto& item : info.handTile) {
			if (item.color() == target.color()) {
				if (item.value() == target.value() + delta[0]) {
					found[0]++;
					candidate[0].push_back(item);
				}
				else if (item.value() == target.value() + delta[1]) {
					found[1]++;
					candidate[1].push_back(item);
				}
			}
		}
		if (found[0] < 1 || found[1] < 1)
			continue;
		for (auto& candidata0 : candidate[0])
			for (auto& candidata1 : candidate[1])
				resultGroup.insert(Group::createShuntsu(candidata0, candidata1, target, (info.nowWind - info.selfWind + 4) % 4));
	}
	for (auto& group : resultGroup)
		res.emplace_back(ActionType::Chii, group);
	return res;
}

std::vector<Action> AI::getMinKanActions(const GameInfo& info) {
	std::vector<Action> res;
	if (info.playerInfo[static_cast<int>(info.selfWind)].riichiJunme != -1)
		return res;
	const auto target = info.nowTile;
	std::vector<Single> candidate;
	for (auto& item : info.handTile)
		if (item.valueEqual(target))
			candidate.push_back(item);
	if (candidate.size() < 3)return res;
	assert(candidate.size() == 3);
	res.emplace_back(ActionType::Kan,
		Group::createKantsu(candidate[0], candidate[1], candidate[2], target,
		(info.nowWind - info.selfWind + 4) % 4));
	return res;
}

std::vector<Action> AI::getPonActions(const GameInfo& info) {
	std::vector<Action> res;
	if (info.playerInfo[static_cast<int>(info.selfWind)].riichiJunme != -1)
		return res;
	const auto target = info.nowTile;
	std::vector<Single> candidate;
	for (auto& item : info.handTile)
		if (item.valueEqual(target))
			candidate.push_back(item);
	if (candidate.size() < 2)return res;
	if (candidate.size() == 2) {
		res.emplace_back(ActionType::Pon,
			Group::createKoutsu(candidate[0], candidate[1], target, (info.nowWind - info.selfWind + 4) % 4));
		return res;
	}
	assert(candidate.size() == 3);
	std::set<Group> temp;
	temp.emplace(Group::createKoutsu(candidate[0], candidate[1], target, (info.nowWind - info.selfWind + 4) % 4));
	temp.emplace(Group::createKoutsu(candidate[1], candidate[2], target, (info.nowWind - info.selfWind + 4) % 4));
	temp.emplace(Group::createKoutsu(candidate[0], candidate[2], target, (info.nowWind - info.selfWind + 4) % 4));
	for (const auto item : temp) { res.emplace_back(Action(ActionType::Pon, item)); }
	return res;
}
bool AI::isDora(const GameInfo& info, const Single& tile) {
	if (tile.isAkadora())return true;
	for (auto& item : info.doraIndicator) {
		if (tile.valueEqual(item.next()))
			return true;
	}
	return false;
}
void AI::printTile(const GameInfo& info, const Single& tile) {
	if (isDora(info, tile)) {
		InfoPrinter::printColorString(tile.getDisplay(), Color(0, 255, 0));
	}
	else {
		InfoPrinter::printColorString(tile.getDisplay(), Color(0, 255, 255));
	}
}
void AI::printInfo(const GameInfo& info) {
	//system("cls");
	std::cout << name << std::endl;
	std::cout << BookManager::lang.mj_wind[static_cast<int>(info.prevailingWind)]
		<< info.round << BookManager::lang.mj_round << " "
		<< info.honba << BookManager::lang.mj_honba << " "
		<< BookManager::lang.mj_remain << info.remainTiles << BookManager::lang.mj_tileMeasureWord << std::endl;
	//宝牌指示牌
	std::cout << BookManager::lang.mj_doraIndicator << " ";
	for (auto& item : info.doraIndicator) {
		printTile(info, item);
		std::cout << " ";
	}
	std::cout << std::endl;
	/*
	for (auto wind = 0; wind <= 3; ++wind) {
		//x家
		if (wind == info.nowWind)
			InfoPrinter::printColorString(BookManager::lang.mj_wind[wind] + BookManager::lang.mj_direction, Color(255, 255, 0));
		else
			std::cout << BookManager::lang.mj_wind[wind] + BookManager::lang.mj_direction;
		//分数以及是否立直
		std::cout << "  " << std::setw(7) << std::left << info.playerInfo[wind].score << std::setw(0)
			<< (info.playerInfo[wind].riichiJunme != -1 ? ("(" + BookManager::lang.mj_riichi + ")") : ("")) << "|";
		//副露牌
		for (auto& item : info.playerInfo[wind].groupTile) { std::cout << item.getString() << " "; }
		std::cout << std::endl;
		//牌河
		for (auto& item : info.playerInfo[wind].discardTile) {
			printTile(info, item);
			std::cout << " ";
		}
		std::cout << std::endl;
	}*/
	auto hand = info.handTile;
	hand.push_back(info.nowTile);
	//向听数
	std::cout << BookManager::lang.mj_standardType << Algorithms::getDistanceStandard(CompactSingles(hand)) << BookManager::lang.mj_syanten<<" "
			  << BookManager::lang.mj_chiitoiType << Algorithms::getDistanceChiitoi(CompactSingles(hand)) << BookManager::lang.mj_syanten << " "
			  << BookManager::lang.mj_kokushiType << Algorithms::getDistanceKokushi(CompactSingles(hand)) << BookManager::lang.mj_syanten << std::endl;
	InfoPrinter::printColorString(BookManager::lang.mj_wind[static_cast<int>(info.selfWind)], Color(255, 255, 0));
	std::cout << "| ";
	for (auto& item : info.handTile) {
		printTile(info, item);
		std::cout << " ";
	}
	std::cout << std::endl;
	std::cout << "ID| ";
	const int add = info.gameState == GameState::OneAct && info.nowTile == Null;
	for (auto j = 0; j < (13 + add - info.playerInfo[info.selfWind].groupTile.size() * 3); ++j) {
		if (j < 9)std::cout << " ";
		std::cout << j + 1 << " ";
	}
	std::cout << std::endl;
}

std::vector<Single> AI::getShowedTiles(const GameInfo& info) {
	std::vector<Single> res;
	for (auto wind = 0; wind <= 3; ++wind) {
		for (auto& item : info.playerInfo[wind].groupTile) {
			if (item.type == GroupType::Kantsu) {
				for (int i = 0; i < 4; ++i)
					res.push_back(Single(item.value, item.color, false));
			}
			else if (item.type == GroupType::Koutsu) {
				for (int i = 0; i < 3; ++i)
					res.push_back(Single(item.value, item.color, false));
			}
			else if (item.type == GroupType::Shuntsu) {
				for (int i = 0; i < 3; ++i)
					res.push_back(Single(item.value + i, item.color, false));
			}
		}
		for (auto& item : info.playerInfo[wind].discardTile) {
			if(item.exist)
				res.push_back(item.tile);
		}
	}
	for (auto& item : info.handTile) {
		res.push_back(item);
	}
	for (auto& item : info.doraIndicator)
		res.push_back(item);
	res.push_back(info.nowTile);
	return res;
}
/*
hand 手牌
remainDepth 剩余深度，为0则是叶子节点
*/
void AI::dfs14(Tiles hand,int remainDepth)
{
	if (remainDepth == 0)
	{

	}
	for (int i = 0; i < hand.size();++i) {
		Tiles newHand;
		for (int j = 0; j < hand.size(); ++j)
			if (i != j)newHand.push_back(hand[j]);
		//枚举打出去的牌，dfs13
		dfs13(newHand,remainDepth);
	}
}
/*
hand 手牌
remainDepth 剩余深度，为0则其子(dfs14)节点是叶子节点
*/
void AI::dfs13(Tiles hand, int remainDepth)
{

}
Action AI::generateAction(const GameInfo& info)
{
	Action res;
	res.type = ActionType::Null;
	if (info.nowWind != info.selfWind && info.gameState == GameState::OneAct)return res;
	if (info.nowWind == info.selfWind && info.gameState == GameState::WaitingForNaki)return res;
	if (info.gameState == GameState::OneAct && info.selfWind == info.nowWind)
		printInfo(info);
	if (info.gameState == GameState::OneAct) {
		if (info.nowTile != Null) {
			//摸 x
			std::cout << BookManager::lang.mj_get << " ";
			printTile(info, info.nowTile);
			std::cout << std::endl << std::endl;
			auto actions = getAllNormalActions(info);
			bool riichi = false;
			bool tsumo = false;
			auto templ = info.handTile;
			templ.push_back(info.nowTile);
			int nowXiangting = Algorithms::getDistance(CompactSingles(templ));
			int maxCount = -1;
			std::vector<int>pool = Algorithms::getPool(getShowedTiles(info));
			Action temp;

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
									minXiangtingThis = xiangting;
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
							std::cout << "D " << act.target.getDisplay() << ",";
							std::cout << maxXiangtingThis << "ST,count:" << count << std::endl;
							if (count > maxCount) {
								maxCount = count;
								temp = act;
							}
							break;
						}
					}
				}
			}
			if (tsumo) {
			}
			else if (riichi) {
			}
			else {
				res=temp;
			}
			printAction(res);
			getchar();
		}
		else {
		}	
	}
	else {
		auto actions = getAllNakiActions(info);
		if (actions.size() != 1)
			printInfo(info);
		if (actions.size() == 1)
			res = actions[0];
		else {
			std::cout << BookManager::lang.mj_wind[static_cast<int>(info.nowWind)] << BookManager::lang.ct_hasDiscard;
			printTile(info, info.nowTile);
			std::cout << std::endl;
			res = actions[0];
			printAction(res);
			getchar();
		}
	}
	return res;
}
