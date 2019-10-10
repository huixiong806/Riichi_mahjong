#pragma once

#include "Single.h"
#include "Group.h"
#include "Rules.h"
#include <vector>

enum class ActionType {
	Null,
	//空指令
	Skip,
	Dapai,
	//正常打牌
	Chi,
	Peng,
	Gang,
	//杠
	Rong,
	Lizhi,
	Zimo,
	Liuju,
	BaBei,
};

enum class ErrorType {
	None,
	//成功了，没有error
	NotYourTurn,
	AlreadyActed,
	ActionRejected,
	TileNotExist,
	//打牌时不存在要打的东西
	CannotChooseNull,
	//不准给空指令
};

struct Action {
	ActionType type;
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
	std::vector<Single> discardTile; //牌河
	int lizhi{}; //立直宣言牌是牌河中的第几张，-1为未立直
	int lizhiXunmu{}; //立直的巡目
	int yifa{}; //是否处于一发巡
	int score{};
};

struct GameInfo {
	Rule rule;  //规则
	std::vector<PlayerInfo> playerInfo; //按顺序分别为东南西北四家
	WindType prevailingWind; //场风
	WindType selfWind; //门风
	WindType nowWind;
	/*
	  表示打出牌(mingpai=true)/该打牌(mingpai=false)的人的门风。
	  如果为自己的话，mingpai=true表示自己什么也干不了(因为不能鸣自己的牌)，需要给空指令。
	  mingpai=false表示自己该打牌了，mingpai=false并且自摸牌为Null说明是刚吃、碰过牌后的打牌。
	  如果为别人的话，mingpai=true时自己必须选择一样操作，mingpai=false时需要给空指令
	*/
	std::vector<Single> handTile; //手牌
	std::vector<Single> doraIndicator; //宝牌指示牌
	Single nowTile; //决策者的自摸牌或是可以鸣牌/荣和的对象
	bool mingpai{}; //为true表示别人刚打完牌，你可以选择鸣牌、荣和或者跳过
	bool lingshang{}; //当前自摸牌是否为岭上牌(仅mingpai=false时有效)
	bool moqie{}; //表示打出去这张牌是否为摸切(仅mingpai=true时有效)
	int lizhibangCount{}; //额外立直棒数量
	int benchang{}; //本场数
	int subround{}; //巡数
	int remainTiles{}; //剩余牌数
	int round{}; //局数(如东2局就是2)
	bool w{}; //是否处于w立,天和,地和可成立的状态
};

class ActionGenerator {
protected:
	std::string name;
public:
    virtual ~ActionGenerator() noexcept = default;
	virtual auto generateAction(const GameInfo& info)->Action = 0;
	virtual auto getName()->std::string& { return name; }
};
