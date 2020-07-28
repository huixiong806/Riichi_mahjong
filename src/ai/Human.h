#pragma once
#include "ActionGenerator.hpp"
#include "Player.h"

class Human : public ActionGenerator {
private:
	void printActions(std::vector<Action> actions);
	std::vector<Action> getMinKanActions(const GameInfo& info);
	std::vector<Action> getPonActions(const GameInfo& info);
	std::vector<Action> getChiiActions(const GameInfo& info);
	std::vector<Action> getTsumoAction(const GameInfo& info);
	std::vector<Action> getRyuukyokuAction(const GameInfo& info);
	std::vector<Action> getRiichiAction(const GameInfo& info);
	std::vector<Action> getRonAction(const GameInfo& info);
	std::vector<Action> getAllNakiActions(const GameInfo& info);
	std::vector<Action> getAllNormalActions(const GameInfo& info);
	void printTile(const GameInfo& info,const Single& tile);
	void printInfo(const GameInfo& info);
	bool isDora(const GameInfo& info,const Single& tile);
public:
	Human() { name = "No Name"; }
	Human(std::string name_) { name = name_; }

	auto generateAction(const GameInfo& info)->Action override;
};
