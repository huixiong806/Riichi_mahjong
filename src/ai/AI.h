#pragma once
#include "ActionGenerator.hpp"
#include "Player.h"
class AI : public ActionGenerator {
private:
	void printActions(std::vector<Action> actions);
	std::vector<Action> getMinKanActions(const GameInfo& info);
	std::vector<Action> getPonActions(const GameInfo& info);
	std::vector<Action> getChiiActions(const GameInfo& info);
	std::vector<Action> getTsumoAction(const GameInfo& info);
	std::vector<Action> getRiichiAction(const GameInfo& info);
	std::vector<Action> getRonAction(const GameInfo& info);
	std::vector<Action> getAllNakiActions(const GameInfo& info);
	std::vector<Action> getAllNormalActions(const GameInfo& info);
	void printTile(const GameInfo& info, const Single& tile);
	void printInfo(const GameInfo& info);
	void printAction(const Action& action);
	bool isDora(const GameInfo& info, const Single& tile);
	//×¢Òâ£¬²»¿¼ÂÇakadora
	std::vector<Single> getShowedTiles(const GameInfo& info);
public:
	AI() { name = "No Name"; }
	AI(std::string name_) { name = name_; }

	auto generateAction(const GameInfo& info)->Action override;
};
