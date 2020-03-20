#pragma once

#include "Single.h"
#include "Group.h"
#include "Rules.h"
#include "Enums.h"
#include <vector>


struct Action {
	ActionType type;//表示决策的类型
	Single target; //表示打哪张牌,仅打牌时有效
	Group group; //仅吃碰杠有效
	bool operator<(const Action& rhs) const {
		if (static_cast<int>(type) < static_cast<int>(rhs.type))
			return true;
		if (target < rhs.target)return true;
		if (group < rhs.group)return true;
		return false;
	}

	Action() {
		type = ActionType::Null;
		target = Null;
	}

	Action(ActionType type_) {
		type = type_;
		target = Null;
		group = Group_Null;
	}

	Action(ActionType type_, Single target_, Group group_) {
		type = type_;
		target = target_;
		group = group_;
	}

	Action(ActionType type_, Single target_) {
		type = type_;
		target = target_;
	}

	Action(ActionType type_, Group group_) {
		type = type_;
		target = Null;
		group = group_;
	}
};

struct PlayerInfo {
	std::vector<Group> groupTile; //副露牌组
	DiscardedTiles discardTile; //牌河
	int riichi{}; //立直宣言牌是牌河中的第几张，-1为未立直
	int riichiJunme{}; //立直的巡目，-1为未立直，-2为w立
	int ippatsu{}; //是否处于一发巡
	int score{};
};
//游戏状态数据包，传递给决策器来决策
struct GameInfo {
	//规则
	Rule rule; 

	//按顺序分别为东南西北四家
	std::vector<PlayerInfo> playerInfo; 

	//场风
	WindType prevailingWind; 

	//门风
	WindType selfWind; 

	//nowWind 的意义取决于 gameState，表示打出牌(gameState 为 GAME_STATE_WATING_FOR_MINGPAI)/该打牌(gameState == GAME_STATE_NORMAL)的人的门风。
	WindType nowWind;

	//手牌
	Tiles handTile;

	//宝牌指示牌,先翻出来的下标小
	Tiles doraIndicator;

	//决策者的自摸牌或是可以鸣牌/荣和的对象
	Single nowTile; 

	/*
		gameState 为 GAME_STATE_NORMAL 表示自己该打牌了，分两种情况
			自摸牌为Null说明是刚吃、碰过牌后的打牌。
			自摸牌不为Null说明是刚摸牌后的打牌
		gameState 为 GAME_STATE_WATING_FOR_MINGPAI 表示该处理某人打出的牌了
			如果 nowWind==selfWind 的话，自己什么也干不了(因为不能鸣自己的牌)，需要给空指令。
			如果 nowWind!=selfWind 的话，gameState 为 WATING_FOR_MINGPAI 时自己必须选择一样操作(鸣牌，和牌或者跳过)
	*/
	GameState gameState{}; 

	//当前自摸牌是否为岭上牌
	bool rinshan{}; 

	//表示打出去这张牌是否为摸切
	bool tsumogiri{}; 

	//额外立直棒数量
	int riichibouCount{}; 

	//本场数
	int honba{}; 

	//巡数
	int subround{}; 

	//剩余牌数
	int remainTiles{}; 

	//局数(如东2局就是2)
	int round{}; 

	//是否处于w立,天和,地和可成立的状态
	bool w{}; 

	//是否处于立直振听状态
	bool riichiFuriten{};

	//是否处于同巡振听状态
	bool doujunFuriten{};

	//是否处于舍张振听状态
	bool sutehaiFuriten{};
};

class ActionGenerator {
protected:
	std::string name;
public:
    virtual ~ActionGenerator() noexcept = default;
	virtual auto generateAction(const GameInfo& info)->Action = 0;
	virtual auto getName()->std::string& { return name; }
};
