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
	std::vector<AgariResult> agariResult;
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
	Rule rule;//规则
	Mountain mountain;     //牌山
	WindType prevailingWind;     //场风
	std::vector<Player> player;      //0,1,2,3分别对应四个玩家。
	int east;//表示庄家的编号,庄家为1则东南西北玩家编号分别为1,2,3,0,以此类推。
	int lizhibang;    //额外立直棒的点数价值 
	int round;  //局数(1~4)
	int benchang;    //本场
	int turn;//表示正常顺序该谁摸牌了。
	//int mIndex;//表示该谁决策了(吃碰情况下mTurn和mIndex会不同)。
	RoundResult result;//一局的结果,调用next函数时如果本局结束则更新
	bool roundIsOver;//本局是否结束
	bool gameIsOver;//整个游戏是否结束
	//Single mLast=Null;//最后打出的牌
	//int mLastTurn;//最后打出的牌是谁打的。
	int doraIndicatorCount;//已翻开的宝牌指示牌数量
	bool waitMingpai;//是否等待鸣牌
	bool haveMingpai[4];//4人是否已经发出了关于鸣牌的抉择
	Action mingpaiAction[4];//鸣牌的操作暂存
	bool w;//是否处于w立,天和,地和可成立的状态
	//开始新局
	void startNewRound();
	//开始下一人摸牌
	void newTurn(int who,bool lingShang)
	{
		waitMingpai = false;
		turn = who;
		for (int i = 0; i < 4; ++i)
		{
			player[i].setNowTile(Null, false);
		}
		//mLastTurn = -1;
		if (mountain.zimo != Null)
		{
			player[who].setNowTile(mountain.zimo, false);//开局给东家设置自摸牌
			mountain.zimo = Null;
		}
		else
		{
			//正常摸牌
			if (lingShang)
			{
				player[who].setNowTile(mountain.nextLingshang(),true);
			}
			else
			{
				player[who].setNowTile(mountain.nextHand(), false);
			}
		}
		//std::cout << "***********" << std::endl;
		//巡数+1
		player[who].subround++;
		if (player[who].subround > 0)
			w = false;
	}
	//鸣牌后打牌
	void newTurnAfterMingpai(int who)
	{
		waitMingpai = false;
		turn = who;
		for (int i = 0; i < 4; ++i)
		{
			player[i].setNowTile(Null,false);
		}
		//巡数+1
		player[who].subround++;
	}
	//途中流局连庄
	void endThisRound(std::vector<AgariResult> res,bool tuzhongLiuju)
	{
		result = RoundResult();
		roundIsOver = true;
		gameIsOver = false;
		int top = 0;
		for (int i = 1; i < 4; ++i)
		{
			if (player[top].score < player[i].score || (player[top].score == player[i].score && ((player[top].selfWind + 4 - 1) % 4>(player[i].selfWind + 4 - 1) % 4)))
				top = i;
		}
		//有人和牌
		//TODO:分数计算 
		if (!res.empty())
		{
			result.liuju = false;
			result.lianzhuang=false;
			result.agariResult = res;
			//立直棒和本场费计算，按照头跳规则
			int targetPlayer;//获取点棒的人 
			if(res.size()==1)
				targetPlayer=res[0].hupaiID;
			else
			{
				targetPlayer=res[0].fangchongID;
				int minSpace=4;//最小间距先调整为4
				int a=player[res[0].fangchongID].selfWind;
				for (auto&item : res)
				{
					int b=player[item.hupaiID].selfWind;
					if(b<a)b+=4;
					int space=b-a;
					if(space<minSpace)
					{
						minSpace=space;
						targetPlayer=item.hupaiID;
					}
				}
			}
			player[targetPlayer].score+=lizhibang;
			player[targetPlayer].score+=benchang*300;
			//荣和的本场费计算 
			if(!res[0].zimo) 
				player[res[0].fangchongID].score-=benchang*300;
			else //自摸的本场费计算 
			{
				for(int i=0;i<4;++i)
					if(targetPlayer!=i)
						player[i].score-=benchang*100;
			}
			//分数计算 
			for (auto&item : res)
			{
				//和牌得失点 
				player[item.hupaiID].score+=item.scoreAdd;
				if(player[item.hupaiID].selfWind==WindType::EAST)
					result.lianzhuang=true;
				if(item.zimo)
				{
					for(int i=0;i<4;++i)
					{
						if(item.hupaiID!=i)
						{
							//std::cout<<item.scoreDecXian<<std::endl;
							if(player[i].selfWind==WindType::EAST)
								player[i].score-=item.scoreDecZhuang;
							else
								player[i].score-=item.scoreDecXian;
						}
					}
				}else player[item.fangchongID].score-=item.scoreDecFangchong;
			}
			if (prevailingWind == rule.gameType&&round == 4)
			{	
				//庄家没和牌或者和牌后是1位,游戏结束
				bool zhuangjiaHupai = false;
				for (auto&item : res)
					if (player[item.hupaiID].selfWind == WindType::EAST)
						zhuangjiaHupai = true;
				if (!zhuangjiaHupai||(zhuangjiaHupai&&top==east))
					gameIsOver = true;
				//但如果top的点数不足就要继续
				if (player[top].score < rule.endPoint)
					gameIsOver = false;
			}
			else if (prevailingWind == rule.gameType + 1)//北4局不考虑东入
			{
				//4局强制结束，分数超过返点也强制结束
				if (round == 4)gameIsOver = true;
				if (player[top].score >= rule.endPoint)
					gameIsOver = true;
			}
			return;
		}
		//*****以下为流局****** 
		//TODO：流局满贯结算 
		result.liuju = true;
		if (tuzhongLiuju)
		{
			result.lianzhuang = true;
			return;
		}
		result.lianzhuang = player[east].tingpai();		
		int bonus = 0,deBonus=0;
		int tingpaiCount = 0;
		for (int i = 0; i < 4; ++i)
			if (player[i].tingpai())
				tingpaiCount++;
		if (tingpaiCount ==1)bonus = 3000,deBonus=-1000;
		else if (tingpaiCount == 2)bonus = 1500, deBonus = -1500;
		else if (tingpaiCount == 3)bonus = 1000, deBonus = -3000;
		for (int i = 0; i < 4; ++i)
		{
			if (player[i].tingpai())player[i].score += bonus;
			else player[i].score += deBonus;
		}
		//游戏结束判断
		if (prevailingWind == rule.gameType&&round == 4)
		{
			//庄家没听牌或者收罚符后是1位,游戏结束
			bool zhuangjiaTingpai = result.lianzhuang;
			if (!zhuangjiaTingpai || (zhuangjiaTingpai&&top == east))
				gameIsOver = true;
			//但如果top的点数不足就要继续
			if (player[top].score < rule.endPoint)
				gameIsOver = false;
		}
		else if (prevailingWind == rule.gameType + 1)//北4局不考虑东入
		{
			//4局强制结束，分数超过返点也强制结束
			if (round == 4)gameIsOver = true;
			if (player[top].score >= rule.endPoint)
				gameIsOver = true;
		}
		if (gameIsOver)
			player[top].score += lizhibang;
	}
	//消去w立,一发,地和
	void resetBounusYaku()
	{
		w = false;
		for (int i = 0; i < 4; ++i)
			player[i].yifa = false;
	}
	//处理鸣牌
	void processMingpai()
	{
		std::vector<AgariResult> result;
		bool overflag = false;
		//荣
		for (int i = 0; i < 4; ++i)
		{
			
			if (mingpaiAction[i].type == ActionType::Rong)
			{
				std::vector<Single>allDora;
				std::vector<Single>allUra;
				for (int i = 0; i < doraIndicatorCount; ++i)
				{
					allDora.push_back(mountain.getDora(i));
					allUra.push_back(mountain.getUra(i));
				}
				AgariResult tmp=player[i].rong(*player[turn].discardTile.rbegin(), prevailingWind,1, w, allDora, allUra);
				tmp.hupaiID=i;
				tmp.fangchongID=turn;
				result.push_back(tmp);
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
			if (mingpaiAction[i].type == ActionType::Gang)
			{
				player[i].ronggang(mingpaiAction[i].group);
				player[turn].discardTile.pop_back();
				
				resetBounusYaku();
				newTurn(i, true);
				return;
			}
			if (mingpaiAction[i].type == ActionType::Peng)
			{
				player[i].peng(mingpaiAction[i].group);
				player[turn].discardTile.pop_back();
				resetBounusYaku();
				newTurnAfterMingpai(i);
				return;
			}
		}
		//吃
		for (int i = (turn + 1) % 4, j = 0; j < 3; ++j, i = (i + 1) % 4)
		{
			if (mingpaiAction[i].type == ActionType::Chi)
			{
				player[i].chi(mingpaiAction[i].group);
				player[turn].discardTile.pop_back();
				resetBounusYaku();
				newTurnAfterMingpai(i);
				return;
			}
		}
		//全部放弃鸣牌或者没法鸣牌
		if (mountain.remainCount() == 0)
		{
			endThisRound(std::vector<AgariResult>(), false);
		}
		else
		{
			this->newTurn((turn + 1) % 4, false);
		}
	}
public:
	//新游戏
	void startNewGame(Rule rule_ = DefualtRule)
	{
		player.resize(4);
		rule = rule_;//设置规则
		prevailingWind = WindType::EAST;//东场
		round = 1;//1局
		east = rand() % 4;//随机庄家
		benchang = 0;//0本场
		doraIndicatorCount = 0;
		gameIsOver = false;
		for(int i=0;i<4;++i)
			player[i].score=rule.startPoint;
		startNewRound();
	}
	//开始下一局
	void startNextRound()
	{
		//连庄，本场数+1，局数不变
		if (result.lianzhuang)
		{
			benchang++;
		}
		else//不连庄，进入下一局
		{
			east=(east+1)%4;//东家换成其下家
			if (round == 4)//4局的话换场风,局数重置
			{
				
				prevailingWind = (WindType)((prevailingWind + 1) % 4);
				round = 1;
			}
			else
			{
				round++;
			}
		}
		startNewRound();
	}
	//玩家获取游戏信息
	GameInfo getGameInfo(int index);
	bool roundOver()
	{
		return roundIsOver;
	}
	bool gameOver()
	{
		return gameIsOver;
	}
	RoundResult getRoundResult()
	{
		return result;
	}
	//下一步(返回操作的状态)(TODO)
	//注意:改mTurn,mLast,player的disabledHandTile等
	ActionResult doAction(int index, Action act);
};
//开始新局
void Game::startNewRound()
{
	w = true;
	gameIsOver = false;
	roundIsOver = false;
	turn = east;//东家起手
	mountain.reset(rule);//牌山重置
	for (int i = east, j = 0; j < 4; i = (i + 1) % 4, j++)
	{
		player[i].groupTile.clear();
		player[i].discardTile.clear();
		//mPlayer[i].score = mRule.startPoint;
		player[i].handTile = mountain.hand[j];
		std::sort(player[i].handTile.begin(), player[i].handTile.end());
		player[i].selfWind = (WindType)j;
		player[i].lizhi = -1;
		player[i].lizhiXunmu = -1;
		player[i].setNowTile(Null,false);
		player[i].subround = -1;
	}
	//mLastTurn = -1;
	doraIndicatorCount = 1;
	waitMingpai = false;
	newTurn(east, false);
}
ActionResult Game::doAction(int index, Action act)
{
	ActionResult res;
	res.success = true;
	res.type = ErrorType::None;
	if (waitMingpai)
	{
		Single target;
		target = *player[turn].discardTile.rbegin();
		if (index == turn)
		{
			haveMingpai[index] = true;
			mingpaiAction[index] = Action();
		}
		if (haveMingpai[index] == false)
		{
			switch (act.type)
			{
			case ActionType::Null:
				res.success = false;
				res.type = ErrorType::CannotChooseNull;
				return res;
				break;
			case ActionType::Skip:
				haveMingpai[index] = true;
				mingpaiAction[index] = act;
				break;
			case ActionType::Rong:
			{
				std::vector<Single>allDora;
				std::vector<Single>allUra;
				TryToAgariResult result;
				for (int i = 0; i < doraIndicatorCount; ++i)
				{
					allDora.push_back(mountain.getDora(i));
					allUra.push_back(mountain.getUra(i));
				}
				if (!player[index].canRong(target, prevailingWind, 1, w, allDora, allUra))
				{
					res.success = false;
					res.type = ErrorType::ActionRejected;
					return res;
				}
				haveMingpai[index] = true;
				mingpaiAction[index] = act;
				break;
			}
			case ActionType::Chi:
				if (!player[index].canChi(target, act.group, ((int)player[turn].selfWind + 4 - player[index].selfWind) % 4))
				{
					res.success = false;
					res.type = ErrorType::ActionRejected;
					return res;
				}
				//立直后不能鸣牌
				if (player[index].lizhiXunmu != -1)
				{
					res.success = false;
					res.type = ErrorType::ActionRejected;
					return res;
				}
				haveMingpai[index] = true;
				mingpaiAction[index] = act;
				break;
			case ActionType::Peng:
				if (!player[index].canPeng(target, act.group, ((int)player[turn].selfWind + 4 - player[index].selfWind) % 4))
				{
					//std::cout<<"*********"<< std::endl;
					res.success = false;
					res.type = ErrorType::ActionRejected;
					return res;
				}
				//立直后不能鸣牌
				if (player[index].lizhiXunmu != -1)
				{
					res.success = false;
					res.type = ErrorType::ActionRejected;
					return res;
				}
				haveMingpai[index] = true;
				mingpaiAction[index] = act;
				break;
			case ActionType::Gang:
				if (!player[index].canGang(target, act.group, ((int)player[turn].selfWind + 4 - player[index].selfWind) % 4))
				{
					res.success = false;
					res.type = ErrorType::ActionRejected;
					return res;
				}
				//立直后不能鸣牌
				if (player[index].lizhiXunmu != -1)
				{
					res.success = false;
					res.type = ErrorType::ActionRejected;
					return res;
				}
				haveMingpai[index] = true;
				mingpaiAction[index] = act;
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
			if (!haveMingpai[i])
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
		if (index != turn)
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
			if (act.type == ActionType::Lizhi&&mountain.remainCount() < 4)
			{
				res.success = false;
				res.type = ErrorType::ActionRejected;
			}
			if (act.type == ActionType::Lizhi)
			{
				if (!player[index].doLizhi(w,act.target))
				{
					res.success = false;
					res.type = ErrorType::ActionRejected;
					return res;
				}
				else
				{
					lizhibang+=1000;
				}
			}
			else if (!player[index].dapai(act.target))
			{
				res.success = false;
				res.type = ErrorType::TileNotExist;
				//std::cout << "***********" << std::endl;
				return res;
			}
			if (act.type == ActionType::Dapai)
			{
				player[index].yifa = false;
			}
			if (player[index].lingshang == true)
			{
				doraIndicatorCount++;
				player[index].lingshang = false;
				//todo:判断四杠散了
			}
			waitMingpai = true;
			for (int i = 0; i < 4; ++i)
			{
				haveMingpai[i] = false;
				mingpaiAction[i] = Action();
			}
			return res;
			break;
		}
		case ActionType::Zimo:
		{
			std::vector<Single>allDora;
			std::vector<Single>allUra;
			TryToAgariResult result;
			for (int i = 0; i < doraIndicatorCount; ++i)
			{
				allDora.push_back(mountain.getDora(i));
				allUra.push_back(mountain.getUra(i));
			}
			result = player[index].zimo(prevailingWind,w, allDora, allUra);
			if (result.success)
			{
				result.result.fangchongID=-1;
				result.result.hupaiID=index;
				endThisRound(std::vector<AgariResult>({result.result}),false);
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
		if (mountain.remainCount() == 0)
		{
			endThisRound(std::vector<AgariResult>(),false);
		}
		else
		{
			//std::cout << "***********" << std::endl;
			this->newTurn((turn + 1) % 4, false);
		}

	}
	return res;
}
//玩家获取游戏信息
GameInfo Game::getGameInfo(int index)
{
	GameInfo res;
	res.rule = rule;//规则
					 //每个玩家的牌河，是否立直，以及副露牌组
	for (int i = 0, j = east; i <= 3; ++i, j = (j + 1) % 4)
	{
		PlayerInfo info;
		info.discardTile = player[j].discardTile;
		info.groupTile = player[j].groupTile;
		info.lizhi = player[j].lizhi;
		info.lizhiXunmu = player[j].lizhiXunmu;
		info.yifa = player[j].yifa;
		info.score= player[j].score;
		res.playerInfo.push_back(info);
	}
	//手牌
	res.handTile = player[index].handTile;
	//剩余牌数
	res.remainTiles = mountain.remainCount();
	//宝牌指示牌
	for (int i = 0; i < doraIndicatorCount; i++)
	{
		res.doraIndicator.push_back(mountain.getDoraIndicator(i));
	}
	res.prevailingWind = prevailingWind;   //场风
	res.selfWind = player[index].selfWind;  //门风
	if (waitMingpai)
	{
		res.nowWind = player[turn].selfWind;
		res.nowTile = *player[turn].discardTile.rbegin();
	}
	else
	{
		res.nowWind = player[turn].selfWind;
		if (index == turn)
			res.nowTile = player[turn].nowTile;
		else
			res.nowTile = Null;
	}
	res.remainTiles = mountain.remainCount();
	res.lizhibangCount = lizhibang;  //额外立直棒数量
	res.benchang = benchang;//本场
	res.mingpai = waitMingpai;
	res.round = round;
	res.w = w;
	return res;
}
