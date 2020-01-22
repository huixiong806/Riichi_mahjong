#pragma once
enum WindType {
	EAST,
	SOUTH,
	WEST,
	NORTH
};
enum class BonusYakuState {
	Normal,//普通
	FirstTurn,//天地和、W立
	LastTurn,//河底、海底
};
enum class AgariWays {
	Tsumo,
	Ron,
	Chankan,
	ChanAnkan
};
enum class ActionType {
	Null,//空指令
	Skip,
	DiscardTile,//正常打牌
	Chii,
	Pon,
	Kan,//杠
	Ron,
	Riichi,
	Tsumo,
	Ryuukyoku,
	Peinuki,
};

enum class ErrorType {
	None,//成功了，没有error
	NotYourTurn,
	AlreadyActed,
	ActionRejected,
	TileNotExist,//打牌时不存在要打的东西
	CantRiichi,//不能立直
	CannotChooseNull,//不准给空指令
};

enum class GameState :bool
{
	OneAct,
	WaitingForNaki
};

enum class AgariFaildReason {
	Null,//空
	ShapeWrong,//不满足和牌型
	NoYaku,//无役
	NotEnoughHan,//番缚
	AgariTypeWrong //非国士无双不能抢暗杠
};