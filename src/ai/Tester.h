#pragma once
#include "ActionGenerator.hpp"
#include "Player.hpp"

class Tester : public ActionGenerator {
private:
	void printActions(std::vector<Action> actions);
	std::vector<Action> getZimoAction(const GameInfo& info);
	std::vector<Action> getLizhiAction(const GameInfo& info);
	std::vector<Action> getRongAction(const GameInfo& info);
	std::vector<Action> getAllActionsMingpai(const GameInfo& info);
	std::vector<Action> getAllActionsNormal(const GameInfo& info);
	//注意，不考虑akadora
	std::vector<Single> getShowedTiles(const GameInfo& info);
public:
	Tester() { name = "No Name"; }
	Tester(std::string name_) { name = name_; }

	auto generateAction(const GameInfo& info)->Action override;
};
