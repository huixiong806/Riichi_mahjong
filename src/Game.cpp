#include "Game.h"
//开始新局
void Game::startNewRound() {
	w = true;
	gameIsOver = false;
	roundIsOver = false;
	turn = east; //东家起手
	mountain.reset(rule); //牌山重置
	for (auto i = east, j = 0; j < 4; i = (i + 1) % 4, j++) {
		player[i].groupTile.clear();
		player[i].discardTile.clear();
		//mPlayer[i].score = mRule.startPoint;
		player[i].handTile = mountain.hand[j];
		std::sort(player[i].handTile.begin(), player[i].handTile.end());
		player[i].selfWind = static_cast<WindType>(j);
		player[i].lizhi = -1;
		player[i].lizhiXunmu = -1;
		player[i].setNowTile(Null, false);
		player[i].subround = -1;
	}
	//mLastTurn = -1;
	doraIndicatorCount = 1;
	waitMingpai = false;
	newTurn(east, false);
}

ActionResult Game::doAction(int index, Action act) {
	ActionResult res{};
	res.success = true;
	res.type = ErrorType::None;
	if (waitMingpai) {
		Single target;
		target = *player[turn].discardTile.rbegin();
		if (index == turn) {
			haveMingpai[index] = true;
			mingpaiAction[index] = Action();
		}
		if (haveMingpai[index] == false) {
			switch (act.type) {
			case ActionType::Null:
				res.success = false;
				res.type = ErrorType::CannotChooseNull;
				return res;
			case ActionType::Skip:
				haveMingpai[index] = true;
				mingpaiAction[index] = act;
				break;
			case ActionType::Rong: {
				std::vector<Single> allDora;
				std::vector<Single> allUra;
				TryToAgariResult result;
				for (auto i = 0; i < doraIndicatorCount; ++i) {
					allDora.push_back(mountain.getDora(i));
					allUra.push_back(mountain.getUra(i));
				}
				if (!player[index].canRong(target, prevailingWind, 1, w, allDora, allUra)) {
					res.success = false;
					res.type = ErrorType::ActionRejected;
					return res;
				}
				haveMingpai[index] = true;
				mingpaiAction[index] = act;
				break;
			}
			case ActionType::Chi:
				if (!player[index].canChi(target, act.group,
				                          (static_cast<int>(player[turn].selfWind) + 4 - player[index].selfWind) % 4)) {
					res.success = false;
					res.type = ErrorType::ActionRejected;
					return res;
				}
				//立直后不能鸣牌
				if (player[index].lizhiXunmu != -1) {
					res.success = false;
					res.type = ErrorType::ActionRejected;
					return res;
				}
				haveMingpai[index] = true;
				mingpaiAction[index] = act;
				break;
			case ActionType::Peng:
				if (!player[index].canPeng(target, act.group,
				                           (static_cast<int>(player[turn].selfWind) + 4 - player[index].selfWind) % 4)
				) {
					//std::cout<<"*********"<< std::endl;
					res.success = false;
					res.type = ErrorType::ActionRejected;
					return res;
				}
				//立直后不能鸣牌
				if (player[index].lizhiXunmu != -1) {
					res.success = false;
					res.type = ErrorType::ActionRejected;
					return res;
				}
				haveMingpai[index] = true;
				mingpaiAction[index] = act;
				break;
			case ActionType::Gang:
				if (!player[index].canGang(target, act.group,
				                           (static_cast<int>(player[turn].selfWind) + 4 - player[index].selfWind) % 4)
				) {
					res.success = false;
					res.type = ErrorType::ActionRejected;
					return res;
				}
				//立直后不能鸣牌
				if (player[index].lizhiXunmu != -1) {
					res.success = false;
					res.type = ErrorType::ActionRejected;
					return res;
				}
				haveMingpai[index] = true;
				mingpaiAction[index] = act;
				break;
			default:
				res.success = false;
				res.type = ErrorType::ActionRejected;
				return res;
			}
		}
		auto allActedFlag = true;
		for (auto i : haveMingpai) {
			if (!i) {
				allActedFlag = false;
				break;
			}
		}
		//std::cout << "***********" << std::endl;
		if (allActedFlag) { this->processMingpai(); }
	}
	else {
		if (act.type == ActionType::Null) return res;
		if (index != turn) {
			res.success = false;
			res.type = ErrorType::NotYourTurn;
			return res;
		}

		switch (act.type) {
		case ActionType::Dapai:
		case ActionType::Lizhi: {
			if (act.type == ActionType::Lizhi && mountain.remainCount() < 4) {
				res.success = false;
				res.type = ErrorType::ActionRejected;
			}
			if (act.type == ActionType::Lizhi) {
				if (!player[index].doLizhi(w, act.target)) {
					res.success = false;
					res.type = ErrorType::ActionRejected;
					return res;
				}
				lizhibang += 1000;
			}
			else if (!player[index].dapai(act.target)) {
				res.success = false;
				res.type = ErrorType::TileNotExist;
				//std::cout << "***********" << std::endl;
				return res;
			}
			if (act.type == ActionType::Dapai) { player[index].yifa = false; }
			if (player[index].lingshang == true) {
				doraIndicatorCount++;
				player[index].lingshang = false;
				//todo:判断四杠散了
			}
			waitMingpai = true;
			for (auto i = 0; i < 4; ++i) {
				haveMingpai[i] = false;
				mingpaiAction[i] = Action();
			}
			return res;
		}
		case ActionType::Zimo: {
			std::vector<Single> allDora;
			std::vector<Single> allUra;
			TryToAgariResult result;
			for (auto i = 0; i < doraIndicatorCount; ++i) {
				allDora.push_back(mountain.getDora(i));
				allUra.push_back(mountain.getUra(i));
			}
			result = player[index].zimo(prevailingWind, w, allDora, allUra);
			if (result.success) {
				result.result.fangchongID = -1;
				result.result.hupaiID = index;
				endThisRound(std::vector<AgariResult>({result.result}), false);
			}
			else {
				res.success = false;
				res.type = ErrorType::ActionRejected;
				//std::cout << "***********" << std::endl;
				return res;
			}
			break;
		}
		default:
			res.success = false;
			res.type = ErrorType::ActionRejected;
			return res;
		}
		//暂时不考虑鸣牌
		//一局结束判断
		//std::cout << "***********" << std::endl;
		if (mountain.remainCount() == 0) { endThisRound(std::vector<AgariResult>(), false); }
		else {
			//std::cout << "***********" << std::endl;
			this->newTurn((turn + 1) % 4, false);
		}
	}
	return res;
}

//玩家获取游戏信息
GameInfo Game::getGameInfo(int index) {
	GameInfo res;
	res.rule = rule; //规则
	//每个玩家的牌河，是否立直，以及副露牌组
	for (auto i = 0, j = east; i <= 3; ++i, j = (j + 1) % 4) {
		PlayerInfo info;
		info.discardTile = player[j].discardTile;
		info.groupTile = player[j].groupTile;
		info.lizhi = player[j].lizhi;
		info.lizhiXunmu = player[j].lizhiXunmu;
		info.yifa = player[j].yifa;
		info.score = player[j].score;
		res.playerInfo.push_back(info);
	}
	//手牌
	res.handTile = player[index].handTile;
	//剩余牌数
	res.remainTiles = mountain.remainCount();
	//宝牌指示牌
	for (auto i = 0; i < doraIndicatorCount; i++) { res.doraIndicator.push_back(mountain.getDoraIndicator(i)); }
	res.prevailingWind = prevailingWind; //场风
	res.selfWind = player[index].selfWind; //门风
	if (waitMingpai) {
		res.nowWind = player[turn].selfWind;
		res.nowTile = *player[turn].discardTile.rbegin();
	}
	else {
		res.nowWind = player[turn].selfWind;
		if (index == turn)
			res.nowTile = player[turn].nowTile;
		else
			res.nowTile = Null;
	}
	res.remainTiles = mountain.remainCount();
	res.lizhibangCount = lizhibang; //额外立直棒数量
	res.benchang = benchang; //本场
	res.mingpai = waitMingpai;
	res.round = round;
	res.w = w;
	return res;
}
