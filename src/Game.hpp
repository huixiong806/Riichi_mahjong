#pragma once
#include"Mountain.hpp"
#include"Player.hpp"
#include"MovementGenerator.hpp"
#include<vector>
#include<memory>
#include<iostream>
//����һ�ֵĽ��
struct RoundResult
{
	bool liuju;//�Ƿ�����
	bool lianzhuang;//ׯ���Ƿ���ׯ
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
	//int mIndex;//��ʾ���ֵı��
	Rule rule;//����
	Mountain mountain;     //��ɽ
	WindType prevailingWind;     //����
	std::vector<Player> player;      //0,1,2,3�ֱ��Ӧ�ĸ���ҡ�
	int east;//��ʾׯ�ҵı��,ׯ��Ϊ1����������ұ�ŷֱ�Ϊ1,2,3,0,�Դ����ơ�
	int lizhibang;    //������ֱ���ĵ�����ֵ 
	int round;  //����(1~4)
	int benchang;    //����
	int turn;//��ʾ����˳���˭�����ˡ�
	//int mIndex;//��ʾ��˭������(���������mTurn��mIndex�᲻ͬ)��
	RoundResult result;//һ�ֵĽ��,����next����ʱ������ֽ��������
	bool roundIsOver;//�����Ƿ����
	bool gameIsOver;//������Ϸ�Ƿ����
	//Single mLast=Null;//���������
	//int mLastTurn;//�����������˭��ġ�
	int doraIndicatorCount;//�ѷ����ı���ָʾ������
	bool waitMingpai;//�Ƿ�ȴ�����
	bool haveMingpai[4];//4���Ƿ��Ѿ������˹������Ƶľ���
	Action mingpaiAction[4];//���ƵĲ����ݴ�
	bool w;//�Ƿ���w��,���,�غͿɳ�����״̬
	//��ʼ�¾�
	void startNewRound();
	//��ʼ��һ������
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
			player[who].setNowTile(mountain.zimo, false);//���ָ���������������
			mountain.zimo = Null;
		}
		else
		{
			//��������
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
		//Ѳ��+1
		player[who].subround++;
		if (player[who].subround > 0)
			w = false;
	}
	//���ƺ����
	void newTurnAfterMingpai(int who)
	{
		waitMingpai = false;
		turn = who;
		for (int i = 0; i < 4; ++i)
		{
			player[i].setNowTile(Null,false);
		}
		//Ѳ��+1
		player[who].subround++;
	}
	//;��������ׯ
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
		//���˺���
		//TODO:�������� 
		if (!res.empty())
		{
			result.liuju = false;
			result.lianzhuang=false;
			result.agariResult = res;
			//��ֱ���ͱ����Ѽ��㣬����ͷ������
			int targetPlayer;//��ȡ������� 
			if(res.size()==1)
				targetPlayer=res[0].hupaiID;
			else
			{
				targetPlayer=res[0].fangchongID;
				int minSpace=4;//��С����ȵ���Ϊ4
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
			//�ٺ͵ı����Ѽ��� 
			if(!res[0].zimo) 
				player[res[0].fangchongID].score-=benchang*300;
			else //�����ı����Ѽ��� 
			{
				for(int i=0;i<4;++i)
					if(targetPlayer!=i)
						player[i].score-=benchang*100;
			}
			//�������� 
			for (auto&item : res)
			{
				//���Ƶ�ʧ�� 
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
				//ׯ��û���ƻ��ߺ��ƺ���1λ,��Ϸ����
				bool zhuangjiaHupai = false;
				for (auto&item : res)
					if (player[item.hupaiID].selfWind == WindType::EAST)
						zhuangjiaHupai = true;
				if (!zhuangjiaHupai||(zhuangjiaHupai&&top==east))
					gameIsOver = true;
				//�����top�ĵ��������Ҫ����
				if (player[top].score < rule.endPoint)
					gameIsOver = false;
			}
			else if (prevailingWind == rule.gameType + 1)//��4�ֲ����Ƕ���
			{
				//4��ǿ�ƽ�����������������Ҳǿ�ƽ���
				if (round == 4)gameIsOver = true;
				if (player[top].score >= rule.endPoint)
					gameIsOver = true;
			}
			return;
		}
		//*****����Ϊ����****** 
		//TODO������������� 
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
		//��Ϸ�����ж�
		if (prevailingWind == rule.gameType&&round == 4)
		{
			//ׯ��û���ƻ����շ�������1λ,��Ϸ����
			bool zhuangjiaTingpai = result.lianzhuang;
			if (!zhuangjiaTingpai || (zhuangjiaTingpai&&top == east))
				gameIsOver = true;
			//�����top�ĵ��������Ҫ����
			if (player[top].score < rule.endPoint)
				gameIsOver = false;
		}
		else if (prevailingWind == rule.gameType + 1)//��4�ֲ����Ƕ���
		{
			//4��ǿ�ƽ�����������������Ҳǿ�ƽ���
			if (round == 4)gameIsOver = true;
			if (player[top].score >= rule.endPoint)
				gameIsOver = true;
		}
		if (gameIsOver)
			player[top].score += lizhibang;
	}
	//��ȥw��,һ��,�غ�
	void resetBounusYaku()
	{
		w = false;
		for (int i = 0; i < 4; ++i)
			player[i].yifa = false;
	}
	//��������
	void processMingpai()
	{
		std::vector<AgariResult> result;
		bool overflag = false;
		//��
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
		//�ܺ���
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
		//��
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
		//ȫ���������ƻ���û������
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
	//����Ϸ
	void startNewGame(Rule rule_ = DefualtRule)
	{
		player.resize(4);
		rule = rule_;//���ù���
		prevailingWind = WindType::EAST;//����
		round = 1;//1��
		east = rand() % 4;//���ׯ��
		benchang = 0;//0����
		doraIndicatorCount = 0;
		gameIsOver = false;
		for(int i=0;i<4;++i)
			player[i].score=rule.startPoint;
		startNewRound();
	}
	//��ʼ��һ��
	void startNextRound()
	{
		//��ׯ��������+1����������
		if (result.lianzhuang)
		{
			benchang++;
		}
		else//����ׯ��������һ��
		{
			east=(east+1)%4;//���һ������¼�
			if (round == 4)//4�ֵĻ�������,��������
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
	//��һ�ȡ��Ϸ��Ϣ
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
	//��һ��(���ز�����״̬)(TODO)
	//ע��:��mTurn,mLast,player��disabledHandTile��
	ActionResult doAction(int index, Action act);
};
//��ʼ�¾�
void Game::startNewRound()
{
	w = true;
	gameIsOver = false;
	roundIsOver = false;
	turn = east;//��������
	mountain.reset(rule);//��ɽ����
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
				//��ֱ��������
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
				//��ֱ��������
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
				//��ֱ��������
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
				//todo:�ж��ĸ�ɢ��
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
		//��ʱ����������
		//һ�ֽ����ж�
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
//��һ�ȡ��Ϸ��Ϣ
GameInfo Game::getGameInfo(int index)
{
	GameInfo res;
	res.rule = rule;//����
					 //ÿ����ҵ��ƺӣ��Ƿ���ֱ���Լ���¶����
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
	//����
	res.handTile = player[index].handTile;
	//ʣ������
	res.remainTiles = mountain.remainCount();
	//����ָʾ��
	for (int i = 0; i < doraIndicatorCount; i++)
	{
		res.doraIndicator.push_back(mountain.getDoraIndicator(i));
	}
	res.prevailingWind = prevailingWind;   //����
	res.selfWind = player[index].selfWind;  //�ŷ�
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
	res.lizhibangCount = lizhibang;  //������ֱ������
	res.benchang = benchang;//����
	res.mingpai = waitMingpai;
	res.round = round;
	res.w = w;
	return res;
}
