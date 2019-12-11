#pragma once
#include "ActionGenerator.hpp"
#include "Player.h"

class Human : public ActionGenerator {
private:
	void printActions(std::vector<Action> actions);
	std::vector<Action> getRongGangActions(const GameInfo& info);
	std::vector<Action> getPengActions(const GameInfo& info);
	std::vector<Action> getChiActions(const GameInfo& info);
	std::vector<Action> getZimoAction(const GameInfo& info);
	std::vector<Action> getLizhiAction(const GameInfo& info);
	std::vector<Action> getRongAction(const GameInfo& info);
	std::vector<Action> getAllActionsMingpai(const GameInfo& info);
	std::vector<Action> getAllActionsNormal(const GameInfo& info);
	void printInfo(const GameInfo& info);
public:
	Human() { name = "No Name"; }
	Human(std::string name_) { name = name_; }

	auto generateAction(const GameInfo& info)->Action override;
};
