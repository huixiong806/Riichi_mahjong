#pragma once
#include "ActionGenerator.hpp"
#include "Player.h"

class Tester : public ActionGenerator {
private:
	void printActions(std::vector<Action> actions);
	std::vector<Action> getTsumoAction(const GameInfo& info);
	std::vector<Action> getRiichiAction(const GameInfo& info);
	std::vector<Action> getRonAction(const GameInfo& info);
	std::vector<Action> getAllNakiActions(const GameInfo& info);
	std::vector<Action> getAllNormalActions(const GameInfo& info);
	//注意，不考虑akadora
	std::vector<Single> getShowedTiles(const GameInfo& info);
public:
	Tester() { name = "No Name"; }
	Tester(std::string name_) { name = name_; }

	auto generateAction(const GameInfo& info)->Action override;
};
