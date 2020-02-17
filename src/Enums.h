#pragma once
enum WindType {
	EAST,
	SOUTH,
	WEST,
	NORTH
};
enum class BonusYakuState {
	Normal,		//普通
	FirstTurn,	//天地和、W立
	LastTurn,	//河底、海底
};
enum class AgariWays {
	Tsumo,		//自摸
	Ron,		//荣和
	Chankan,	//抢杠
	ChanAnkan	//抢暗杠
};
enum class ActionType {
	Null,		//空指令
	Skip,		//跳过
	DiscardTile,//正常打牌
	Chii,		//吃
	Pon,		//碰
	Kan,		//杠
	Ron,		//荣和
	Riichi,		//立直
	Tsumo,		//自摸
	Ryuukyoku,	//流局
	Peinuki,	//拔北
};

enum class ErrorType {
	None,				//成功了，没有error
	NotYourTurn,		//非自己可以行动的情况
	AlreadyActed,		//已经行动过
	ActionRejected,		//行动被拒绝
	TileNotExist,		//打牌时不存在要打的东西
	CantRiichi,			//不能立直
	CannotChooseNull,	//不准给空指令
};

enum class GameState :bool
{
	OneAct,
	WaitingForNaki
};

enum class AgariFaildReason {
	Null,			//空
	ShapeWrong,		//不满足和牌型
	NoYaku,			//无役
	NotEnoughHan,	//番缚
	AgariTypeWrong	//非国士无双不能抢暗杠
};