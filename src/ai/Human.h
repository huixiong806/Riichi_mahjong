#pragma once
#include "ActionGenerator.hpp"
#include "Player.h"

class Human : public ActionGenerator {
private:
	void printActions(std::vector<Action> actions);
	std::vector<Action> getMinKanActions(const GameInfo& info);
	std::vector<Action> getPonActions(const GameInfo& info);
	std::vector<Action> getChiActions(const GameInfo& info);
	std::vector<Action> getTsumoAction(const GameInfo& info);
	std::vector<Action> getRiichiAction(const GameInfo& info);
	std::vector<Action> getRonAction(const GameInfo& info);
	std::vector<Action> getAllNakiActions(const GameInfo& info);
	std::vector<Action> getAllNormalActions(const GameInfo& info);
	void printInfo(const GameInfo& info);
public:
	Human() { name = "No Name"; }
	Human(std::string name_) { name = name_; }

	auto generateAction(const GameInfo& info)->Action override;
};
