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
	Success,						//成功了，没有error
	NotYourTurn,					//非自己可以行动的情况
	AlreadyActed,					//已经行动过
	ActionRejected,					//行动被拒绝
	TileNotExist,					//打牌时不存在要打的东西
	Chii_WrongType,					//吃错误_类型不正确(必须为顺子)
	Chii_WrongColor,				//吃错误_花色不匹配
	Chii_CantBeJihai,				//吃错误_不能吃字牌
	Chii_WrongValue,				//吃错误_目标牌数值不正确
	Chii_WrongPosition,				//吃错误_位置不正确，只能吃上家牌
	Chii_WrongAkadora,				//吃错误_目标牌的红宝牌属性与顺子中的不同
	Chii_WrongTiles,				//吃错误_手牌中找不到对应的搭子
	Naki_AlreadyRiichi,				//鸣牌错误_立直后无法鸣牌
	Riichi_lessTilesLeft,			//立直错误_牌山剩余数量过少
	Riichi_Other,					//立直错误_其他立直错误
	CannotChooseNull,				//不准给空指令
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