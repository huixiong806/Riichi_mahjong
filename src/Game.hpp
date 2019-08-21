#pragma once
#include"Mountain.hpp"
#include"Player.hpp"
#include"MovementGenerator.hpp"
#include<vector>
#include<memory>
#include<iostream>
//返回一局的结果
struct RoundResult
{
	bool liuju;//是否流局
	bool lianzhuang;//庄家是否连庄
	std::vector<std::pair<int,AgariResult>> agariResult;
};
struct ActionResult
{
	bool success;
	ErrorType type;
};
class Game
{
private:
	//int mIndex;//表示本局的编号
	Rule mRule;//规则
	Mountain mMountain;     //牌山
	WindType mPrevailingWind;     //场风
	std::vector<Player> mPlayer;      //0,1,2,3分别对应四个玩家。
	int mEast;//表示庄家的编号,庄家为1则东南西北玩家编号分别为1,2,3,0,以此类推。
	int mLizhibang;    //额外立直棒数量
	int mRound;  //局数(1~4)
	int mBenchang;    //本场
	int mTurn;//表示正常顺序该谁摸牌了。
	//int mIndex;//表示该谁决策了(吃碰情况下mTurn和mIndex会不同)。
	RoundResult mResult;//一局的结果,调用next函数时如果本局结束则更新
	bool mRoundover;//本局是否结束
	bool mGameover;//整个游戏是否结束
	//Single mLast=Null;//最后打出的牌
	//int mLastTurn;//最后打出的牌是谁打的。
	int mDoraIndicatorCount;//已翻开的宝牌指示牌数量
	bool mWaitMingpai;//是否等待鸣牌
	bool mHaveMingpai[4];//4人是否已经发出了关于鸣牌的抉择
	Action mMingpaiAction[4];//鸣牌的操作暂存
	bool mW;//是否处于w立,天和,地和可成立的状态
	//开始新局
	void startNewRound();
	//开始下一人摸牌
	void newTurn(int who,bool lingShang)
	{
		mWaitMingpai = false;
		mTurn = who;
		for (int i = 0; i < 4; ++i)
		{
			mPlayer[i].setNowTile(Null, false);
		}
		//mLastTurn = -1;
		if (mMountain.zimo != Null)
		{
			mPlayer[who].setNowTile(mMountain.zimo, false);//开局给东家设置自摸牌
			mMountain.zimo = Null;
		}
		else
		{
			//正常摸牌
			if (lingShang)
			{
				mPlayer[who].setNowTile(mMountain.nextLingshang(),true);
			}
			else
			{
				mPlayer[who].setNowTile(mMountain.nextHand(), false);
			}
		}
		//std::cout << "***********" << std::endl;
		//巡数+1
		mPlayer[who].subround++;
		if (mPlayer[who].subround > 0)
			mW = false;
	}
	//鸣牌后打牌
	void newTurnAfterMingpai(int who)
	{
		mWaitMingpai = false;
		mTurn = who;
		for (int i = 0; i < 4; ++i)
		{
			mPlayer[i].setNowTile(Null,false);
		}
		//巡数+1
		mPlayer[who].subround++;
	}
	//途中流局连庄
	void endThisRound(std::vector<std::pair<int, AgariResult>> res,bool tuzhongLiuju)
	{
		mResult = RoundResult();
		mRoundover = true;
		mGameover = false;
		int top = 0;
		for (int i = 1; i < 4; ++i)
		{
			if (mPlayer[top].score < mPlayer[i].score || (mPlayer[top].score == mPlayer[i].score && ((mPlayer[top].selfWind + 4 - 1) % 4>(mPlayer[i].selfWind + 4 - 1) % 4)))
				top = i;
		}
		//有人和牌
		//TODO:分数计算
		if (!res.empty())
		{
			mResult.liuju = false;
			mResult.agariResult = res;
			//游戏结束判断
			if (mPrevailingWind == mRule.gameType&&mRound == 4)
			{	
				//庄家没和牌或者和牌后是1位,游戏结束
				bool zhuangjiaHupai = false;
				for (auto&item : res)
					if (mPlayer[item.first].selfWind == WindType::EAST)
						zhuangjiaHupai = true;
				if (!zhuangjiaHupai||(zhuangjiaHupai&&top==mEast))
					mGameover = true;
				//但如果top的点数不足就要继续
				if (mPlayer[top].score < mRule.endPoint)
					mGameover = false;
			}
			else if (mPrevailingWind == mRule.gameType + 1)//北4局不考虑东入
			{
				//4局强制结束，分数超过返点也强制结束
				if (mRound == 4)mGameover = true;
				if (mPlayer[top].score >= mRule.endPoint)
					mGameover = true;
			}
			return;
		}
		//流局
		mResult.liuju = true;
		if (tuzhongLiuju)
		{
			mResult.lianzhuang = true;
			return;
		}
		mResult.lianzhuang = mPlayer[mEast].tingpai();		
		int bonus = 0,deBonus=0;
		int tingpaiCount = 0;
		for (int i = 0; i < 4; ++i)
			if (mPlayer[i].tingpai())
				tingpaiCount++;
		if (tingpaiCount ==1)bonus = 3000,deBonus=-1000;
		else if (tingpaiCount == 2)bonus = 1500, deBonus = -1500;
		else if (tingpaiCount == 3)bonus = 1000, deBonus = -3000;
		for (int i = 0; i < 4; ++i)
		{
			if (mPlayer[i].tingpai())mPlayer[i].score += bonus;
			else mPlayer[i].score += deBonus;
		}
		//游戏结束判断
		if (mPrevailingWind == mRule.gameType&&mRound == 4)
		{
			//庄家没听牌或者收罚符后是1位,游戏结束
			bool zhuangjiaTingpai = mResult.lianzhuang;
			if (!zhuangjiaTingpai || (zhuangjiaTingpai&&top == mEast))
				mGameover = true;
			//但如果top的点数不足就要继续
			if (mPlayer[top].score < mRule.endPoint)
				mGameover = false;
		}
		else if (mPrevailingWind == mRule.gameType + 1)//北4局不考虑东入
		{
			//4局强制结束，分数超过返点也强制结束
			if (mRound == 4)mGameover = true;
			if (mPlayer[top].score >= mRule.endPoint)
				mGameover = true;
		}
		if (mGameover)
			mPlayer[top].score += mLizhibang;
	}
	//消去w立,一发,地和
	void resetBounusYaku()
	{
		mW = false;
		for (int i = 0; i < 4; ++i)
			mPlayer[i].yifa = false;
	}
	//处理鸣牌
	void processMingpai()
	{
		std::vector<std::pair<int,AgariResult>> result;
		bool overflag = false;
		//荣
		for (int i = 0; i < 4; ++i)
		{
			
			if (mMingpaiAction[i].type == ActionType::Rong)
			{
				std::vector<Single>allDora;
				std::vector<Single>allUra;
				for (int i = 0; i < mDoraIndicatorCount; ++i)
				{
					allDora.push_back(mMountain.getDora(i));
					allUra.push_back(mMountain.getUra(i));
				}
				result.push_back(std::make_pair(i,mPlayer[i].rong(*mPlayer[mTurn].discardTile.rbegin(), mPrevailingWind,1, mW, allDora, allUra)));
				overflag = true;
				//std::cout << "*" << std::endl;
			}
			
		}
		if (overflag)
		{
			endThisRound(result,false);
			return;
		}
		//杠和碰
		for (int i = 0; i < 4; ++i)
		{
			if (mMingpaiAction[i].type == ActionType::Gang)
			{
				mPlayer[i].ronggang(mMingpaiAction[i].group);
				mPlayer[mTurn].discardTile.pop_back();
				
				resetBounusYaku();
				newTurn(i, true);
				return;
			}
			if (mMingpaiAction[i].type == ActionType::Peng)
			{
				mPlayer[i].peng(mMingpaiAction[i].group);
				mPlayer[mTurn].discardTile.pop_back();
				resetBounusYaku();
				newTurnAfterMingpai(i);
				return;
			}
		}
		//吃
		for (int i = (mTurn + 1) % 4, j = 0; j < 3; ++j, i = (i + 1) % 4)
		{
			if (mMingpaiAction[i].type == ActionType::Chi)
			{
				mPlayer[i].chi(mMingpaiAction[i].group);
				mPlayer[mTurn].discardTile.pop_back();
				resetBounusYaku();
				newTurnAfterMingpai(i);
				return;
			}
		}
		//全部放弃鸣牌或者没法鸣牌
		if (mMountain.remainCount() == 0)
		{
			endThisRound(std::vector<std::pair<int,AgariResult>>(), false);
		}
		else
		{
			this->newTurn((mTurn + 1) % 4, false);
		}
	}
public:
	//新游戏
	void startNewGame(Rule rule = DefualtRule)
	{
		mPlayer.resize(4);
		mRule = rule;//设置规则
		mPrevailingWind = WindType::EAST;//东场
		mRound = 1;//1局
		mEast = rand() % 4;//随机庄家
		mBenchang = 0;//0本场
		mDoraIndicatorCount = 0;
		mGameover = false;
		startNewRound();
	}
	//开始下一局
	void startNextRound()
	{
		//连庄，本场数+1，局数不变
		if (mResult.lianzhuang)
		{
			mBenchang++;
		}
		else//不连庄，进入下一局
		{
			mEast=(mEast+1)%4;//东家换成其下家
			if (mRound == 4)//4局的话换场风,局数重置
			{
				
				mPrevailingWind = (WindType)((mPrevailingWind + 1) % 4);
				mRound = 1;
			}
			else
			{
				mRound++;
			}
		}
		startNewRound();
	}
	//玩家获取游戏信息
	GameInfo getGameInfo(int index);
	bool roundOver()
	{
		return mRoundover;
	}
	bool gameOver()
	{
		return mGameover;
	}
	RoundResult getRoundResult()
	{
		return mResult;
	}
	//下一步(返回操作的状态)(TODO)
	//注意:改mTurn,mLast,player的disabledHandTile等
	ActionResult doAction(int index, Action act);
};
//开始新局
void Game::startNewRound()
{
	mW = true;
	mGameover = false;
	mRoundover = false;
	mTurn = mEast;//东家起手
	mMountain.reset(mRule);//牌山重置
	for (int i = mEast, j = 0; j < 4; i = (i + 1) % 4, j++)
	{
		mPlayer[i].groupTile.clear();
		mPlayer[i].discardTile.clear();
		mPlayer[i].score = mRule.startPoint;
		mPlayer[i].handTile = mMountain.hand[j];
		std::sort(mPlayer[i].handTile.begin(), mPlayer[i].handTile.end());
		mPlayer[i].selfWind = (WindType)j;
		mPlayer[i].lizhi = -1;
		mPlayer[i].lizhiXunmu = -1;
		mPlayer[i].setNowTile(Null,false);
		mPlayer[i].subround = -1;
	}
	//mLastTurn = -1;
	mDoraIndicatorCount = 1;
	mWaitMingpai = false;
	newTurn(mEast, false);
}
ActionResult Game::doAction(int index, Action act)
{
	ActionResult res;
	res.success = true;
	res.type = ErrorType::None;
	if (mWaitMingpai)
	{
		Single target;
		target = *mPlayer[mTurn].discardTile.rbegin();
		if (index == mTurn)
		{
			mHaveMingpai[index] = true;
			mMingpaiAction[index] = Action();
		}
		if (mHaveMingpai[index] == false)
		{
			switch (act.type)
			{
			case ActionType::Null:
				res.success = false;
				res.type = ErrorType::CannotChooseNull;
				return res;
				break;
			case ActionType::Skip:
				mHaveMingpai[index] = true;
				mMingpaiAction[index] = act;
				break;
			case ActionType::Rong:
			{
				std::vector<Single>allDora;
				std::vector<Single>allUra;
				TryToAgariResult result;
				for (int i = 0; i < mDoraIndicatorCount; ++i)
				{
					allDora.push_back(mMountain.getDora(i));
					allUra.push_back(mMountain.getUra(i));
				}
				if (!mPlayer[index].canRong(target, mPrevailingWind, 1, mW, allDora, allUra))
				{
					res.success = false;
					res.type = ErrorType::ActionRejected;
					return res;
				}
				mHaveMingpai[index] = true;
				mMingpaiAction[index] = act;
				break;
			}
			case ActionType::Chi:
				if (!mPlayer[index].canChi(target, act.group, ((int)mPlayer[mTurn].selfWind + 4 - mPlayer[index].selfWind) % 4))
				{
					res.success = false;
					res.type = ErrorType::ActionRejected;
					return res;
				}
				//立直后不能鸣牌
				if (mPlayer[index].lizhiXunmu != -1)
				{
					res.success = false;
					res.type = ErrorType::ActionRejected;
					return res;
				}
				mHaveMingpai[index] = true;
				mMingpaiAction[index] = act;
				break;
			case ActionType::Peng:
				if (!mPlayer[index].canPeng(target, act.group, ((int)mPlayer[mTurn].selfWind + 4 - mPlayer[index].selfWind) % 4))
				{
					//std::cout<<"*********"<< std::endl;
					res.success = false;
					res.type = ErrorType::ActionRejected;
					return res;
				}
				//立直后不能鸣牌
				if (mPlayer[index].lizhiXunmu != -1)
				{
					res.success = false;
					res.type = ErrorType::ActionRejected;
					return res;
				}
				mHaveMingpai[index] = true;
				mMingpaiAction[index] = act;
				break;
			case ActionType::Gang:
				if (!mPlayer[index].canGang(target, act.group, ((int)mPlayer[mTurn].selfWind + 4 - mPlayer[index].selfWind) % 4))
				{
					res.success = false;
					res.type = ErrorType::ActionRejected;
					return res;
				}
				//立直后不能鸣牌
				if (mPlayer[index].lizhiXunmu != -1)
				{
					res.success = false;
					res.type = ErrorType::ActionRejected;
					return res;
				}
				mHaveMingpai[index] = true;
				mMingpaiAction[index] = act;
				break;
			default:
				res.success = false;
				res.type = ErrorType::ActionRejected;
				return res;
				break;
			}
		}
		bool allActedFlag = true;
		for (int i = 0; i < 4; ++i)
		{
			if (!mHaveMingpai[i])
			{
				allActedFlag = false;
				break;
			}
		}
		//std::cout << "***********" << std::endl;
		if (allActedFlag)
		{
			this->processMingpai();
		}
	}
	else
	{
		if (act.type == ActionType::Null) return res;
		if (index != mTurn)
		{
			res.success = false;
			res.type = ErrorType::NotYourTurn;
			return res;
		}
		
		switch (act.type)
		{
		case ActionType::Dapai:
		case ActionType::Lizhi:
		{
			if (act.type == ActionType::Lizhi&&mMountain.remainCount() < 4)
			{
				res.success = false;
				res.type = ErrorType::ActionRejected;
			}
			if (act.type == ActionType::Lizhi)
			{
				if (!mPlayer[index].doLizhi(mW,act.target))
				{
					res.success = false;
					res.type = ErrorType::ActionRejected;
					return res;
				}
				else
				{
					mLizhibang+=1000;
				}
			}
			else if (!mPlayer[index].dapai(act.target))
			{
				res.success = false;
				res.type = ErrorType::TileNotExist;
				//std::cout << "***********" << std::endl;
				return res;
			}
			if (act.type == ActionType::Dapai)
			{
				mPlayer[index].yifa = false;
			}
			if (mPlayer[index].lingshang == true)
			{
				mDoraIndicatorCount++;
				mPlayer[index].lingshang = false;
				//todo:判断四杠散了
			}
			mWaitMingpai = true;
			for (int i = 0; i < 4; ++i)
			{
				mHaveMingpai[i] = false;
				mMingpaiAction[i] = Action();
			}
			return res;
			break;
		}
		case ActionType::Zimo:
		{
			std::vector<Single>allDora;
			std::vector<Single>allUra;
			TryToAgariResult result;
			for (int i = 0; i < mDoraIndicatorCount; ++i)
			{
				allDora.push_back(mMountain.getDora(i));
				allUra.push_back(mMountain.getUra(i));
			}
			result = mPlayer[index].zimo(mPrevailingWind,mW, allDora, allUra);
			if (result.success)
			{
				endThisRound(std::vector<std::pair<int, AgariResult>>({ std::make_pair(index,result.result)}),false);
			}
			else
			{
				res.success = false;
				res.type = ErrorType::ActionRejected;
				//std::cout << "***********" << std::endl;
				return res;
			}
			break;
		}
		default:
			res.success = false;
			res.type = ErrorType::ActionRejected;
			return res;
			break;
		}
		//暂时不考虑鸣牌
		//一局结束判断
		//std::cout << "***********" << std::endl;
		if (mMountain.remainCount() == 0)
		{
			endThisRound(std::vector<std::pair<int,AgariResult>>(),false);
		}
		else
		{
			//std::cout << "***********" << std::endl;
			this->newTurn((mTurn + 1) % 4, false);
		}

	}
	return res;
}
//玩家获取游戏信息
GameInfo Game::getGameInfo(int index)
{
	GameInfo res;
	res.rule = mRule;//规则
					 //每个玩家的牌河，是否立直，以及副露牌组
	for (int i = 0, j = mEast; i <= 3; ++i, j = (j + 1) % 4)
	{
		PlayerInfo info;
		info.discardTile = mPlayer[j].discardTile;
		info.groupTile = mPlayer[j].groupTile;
		info.lizhi = mPlayer[j].lizhi;
		info.lizhiXunmu = mPlayer[j].lizhiXunmu;
		info.yifa = mPlayer[j].yifa;
		info.score= mPlayer[j].score;
		res.playerInfo.push_back(info);
	}
	//手牌
	res.handTile = mPlayer[index].handTile;
	//剩余牌数
	res.remainTiles = mMountain.remainCount();
	//宝牌指示牌
	for (int i = 0; i < mDoraIndicatorCount; i++)
	{
		res.doraIndicator.push_back(mMountain.getDoraIndicator(i));
	}
	res.prevailingWind = mPrevailingWind;   //场风
	res.selfWind = mPlayer[index].selfWind;  //门风
	if (mWaitMingpai)
	{
		res.nowWind = mPlayer[mTurn].selfWind;
		res.nowTile = *mPlayer[mTurn].discardTile.rbegin();
	}
	else
	{
		res.nowWind = mPlayer[mTurn].selfWind;
		if (index == mTurn)
			res.nowTile = mPlayer[mTurn].nowTile;
		else
			res.nowTile = Null;
	}
	res.remainTiles = mMountain.remainCount();
	res.lizhibangCount = mLizhibang;  //额外立直棒数量
	res.benchang = mBenchang;//本场
	res.mingpai = mWaitMingpai;
	res.round = mRound;
	res.w = mW;
	return res;
}