#pragma once
#include"Mountain.h"
#include"Player.h"
#include"Enums.h"
#include<vector>

//返回一局的结果
struct RoundResult {
	bool ryuukyoku{}; //是否流局
	bool renchan{}; //庄家是否连庄
	std::vector<AgariResult> agariResult;
};

class Game {
private:
	//int mIndex;//表示本局的编号
	Rule rule; //规则
	Mountain mountain; //牌山
	WindType prevailingWind; //场风
	std::vector<Player> player; //0,1,2,3分别对应四个玩家。
	int east; //表示庄家的编号,庄家为1则东南西北玩家编号分别为1,2,3,0,以此类推。
	int riichibouValue; //额外立直棒的点数价值 
	int round; //局数(1~4)
	int honba; //本场
	int turn; //表示正常顺序该谁摸牌了。
	//int mIndex;//表示该谁决策了(吃碰情况下mTurn和mIndex会不同)。
	RoundResult result; //一局的结果,调用next函数时如果本局结束则更新
	bool roundIsOver; //本局是否结束
	bool gameIsOver; //整个游戏是否结束
	//Single mLast=Null;//最后打出的牌
	//int mLastTurn;//最后打出的牌是谁打的。
	int doraIndicatorCount; //已翻开的宝牌指示牌数量
	GameState gameState; //游戏状态
	bool isReady[4]; //玩家是否已经做出了决策
	Action playerAction[4]; //玩家决策暂存
	bool w; //是否处于w立,天和,地和可成立的状态
	//开始新局
	void startNewRound();
	//开始下一人摸牌
	void newTurn(int who, bool lingShang);
	
	//鸣牌(除了杠)后打牌
	void newTurnAfterNari(int who);

	//途中流局连庄
	void endThisRound(std::vector<AgariResult> res, bool tochuuRyuukyoku);

	//消去w立,一发,地和
	void resetBounusYaku();

	//处理鸣牌
	void processNari();

public:
	/*
	  结束本turn，进入下一个
	  gameState == GAME_STATE_WATING_FOR_MINGPAI  处理鸣牌
	  gameState == GAME_STATE_NORMAL              处理玩家打牌/立直/自摸等操作
	  如果某玩家没有设置好本轮的操作，返回false
	*/
	bool endThisTurn();

	//新游戏
	void startNewGame(Rule rule_ = DefualtRule);

	//开始下一局
	void startNextRound();

	//获取某个玩家能看到的游戏信息
	GameInfo getGameInfo(int index)const;

	//一局是否结束？
	bool roundOver()const { return roundIsOver; }

	//游戏是否结束？
	bool gameOver()const { return gameIsOver; }

	RoundResult getRoundResult() { return result; }
	/*
	    Action分为两类
		gameState == GAME_STATE_NORMAL             轮到自己时的action
		gameState == GAME_STATE_WATING_FOR_MINGPAI 别人打出牌后的action
		返回值为设置Action是否成功，以及失败的话失败的原因
	*/
	//注意:改mTurn,mLast,player的disabledHandTile等
	ActionCheckingResult setPlayerAction(int index, Action act);
};
