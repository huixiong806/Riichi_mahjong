#pragma once
#include<string>
#include<iostream>
#include"ActionGenerator.hpp"

class AutoFurikomu : public ActionGenerator {
private:

public:
	AutoFurikomu() { name = "放铳人"; }
	AutoFurikomu(std::string name_) { name = name_; }

	Action generateAction(const GameInfo& info) override {
		Action res;
		res.type = ActionType::Null;
		if (info.nowWind != info.selfWind && info.gameState == GameState::OneAct)return res;
		if (info.nowWind == info.selfWind && info.gameState == GameState::WaitingForNaki)return res;
		if (info.gameState== GameState::OneAct) { res.type = ActionType::Skip; }
		else {
			res.type = ActionType::DiscardTile;
			res.target = info.nowTile;
		}

		return res;
	}
};
