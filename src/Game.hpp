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
	Rule mRule;//����
	Mountain mMountain;     //��ɽ
	WindType mPrevailingWind;     //����
	std::vector<Player> mPlayer;      //0,1,2,3�ֱ��Ӧ�ĸ���ҡ�
	int mEast;//��ʾׯ�ҵı��,ׯ��Ϊ1����������ұ�ŷֱ�Ϊ1,2,3,0,�Դ����ơ�
	int mLizhibangCount;    //������ֱ������
	int mRound;  //����(1~4)
	int mBenchang;    //������
	int mTurn;//��ʾ����˳���˭�����ˡ�
	//int mIndex;//��ʾ��˭������(���������mTurn��mIndex�᲻ͬ)��
	RoundResult mResult;//һ�ֵĽ��,����next����ʱ������ֽ��������
	bool mRoundover;//�����Ƿ����
	bool mGameover;//������Ϸ�Ƿ����
	//Single mLast=Null;//���������
	//int mLastTurn;//�����������˭��ġ�
	int mDoraIndicatorCount;//�ѷ����ı���ָʾ������
	bool mWaitMingpai;//�Ƿ�ȴ�����
	bool mHaveMingpai[4];//4���Ƿ��Ѿ������˹������Ƶľ���
	Action mMingpaiAction[4];//���ƵĲ����ݴ�
	//��ʼ�¾�
	void startNewRound();
	//��ʼ��һ������
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
			mPlayer[who].setNowTile(mMountain.zimo, false);//���ָ���������������
			mMountain.zimo = Null;
		}
		else
		{
			//��������
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
		//Ѳ��+1
		mPlayer[who].subround++;
	}
	//���ƺ����
	void newTurnAfterMingpai(int who)
	{
		mWaitMingpai = false;
		mTurn = who;
		for (int i = 0; i < 4; ++i)
		{
			mPlayer[i].setNowTile(Null,false);
		}
		//Ѳ��+1
		mPlayer[who].subround++;
	}
	void endThisRound(std::vector<AgariResult> res)
	{
		//TODO:����Ƿ�����
		mRoundover = true;
		mResult.liuju = true;
		mResult.lianzhuang = false;
		//��Ϸ�����ж�
		if (mPrevailingWind == mRule.gameType&&mRound == 4)
		{
			mGameover = true;
		}
	}
	//��������
	void processMingpai()
	{
		std::vector<AgariResult> result;
		bool overflag = false;
		//��
		for (int i = 0; i < 4; ++i)
		{
			if (mMingpaiAction[i].type == ActionType::Rong)
			{
				result.push_back(mPlayer[i].rong(*mPlayer[mTurn].discardTile.rbegin()));
				overflag = true;
			}
		}
		if (overflag)
		{
			endThisRound(result);
			return;
		}
		//�ܺ���
		for (int i = 0; i < 4; ++i)
		{
			if (mMingpaiAction[i].type == ActionType::Gang)
			{
				mPlayer[i].gang(mMingpaiAction[i].group);
				mPlayer[mTurn].discardTile.pop_back();
				newTurn(i, true);
				return;
			}
			if (mMingpaiAction[i].type == ActionType::Peng)
			{
				mPlayer[i].peng(mMingpaiAction[i].group);
				mPlayer[mTurn].discardTile.pop_back();
				newTurnAfterMingpai(i);
				return;
			}
		}
		//��
		for (int i = (mTurn + 1) % 4, j = 0; j < 3; ++j, i = (i + 1) % 4)
		{
			if (mMingpaiAction[i].type == ActionType::Chi)
			{
				mPlayer[i].chi(mMingpaiAction[i].group);
				mPlayer[mTurn].discardTile.pop_back();
				newTurnAfterMingpai(i);
				return;
			}
		}
		//ȫ���������ƻ���û������
		if (mMountain.remainCount() == 0)
		{
			endThisRound(std::vector<AgariResult>());
		}
		else
		{
			this->newTurn((mTurn + 1) % 4, false);
		}
	}
public:
	
	//����Ϸ
	void startNewGame(Rule rule = DefualtRule)
	{
		mPlayer.resize(4);
		mRule = rule;//���ù���
		mPrevailingWind = WindType::EAST;//����
		mRound = 1;//1��
		mEast = rand() % 4;//���ׯ��
		mBenchang = 0;//0����
		mDoraIndicatorCount = 0;
		mGameover = false;
		startNewRound();
	}
	//��ʼ��һ��
	void startNextRound()
	{
		//��ׯ��������+1����������
		mGameover = false;
		if (mResult.lianzhuang)
		{
			mBenchang++;
		}
		else//����ׯ��������һ��
		{
			mEast=(mEast+1)%4;//���һ������¼�
			if (mRound == 4)//4�ֵĻ�������,��������
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
	//��һ�ȡ��Ϸ��Ϣ
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
	//��һ��(���ز�����״̬)(TODO)
	//ע��:��mTurn,mLast,player��disabledHandTile��
	ActionResult doAction(int index, Action act);
};
//��ʼ�¾�
void Game::startNewRound()
{
	mTurn = mEast;//��������
	mMountain.reset(mRule);//��ɽ����
	for (int i = mEast, j = 0; j < 4; i = (i + 1) % 4, j++)
	{
		mPlayer[i].groupTile.clear();
		mPlayer[i].discardTile.clear();
		mPlayer[i].score = mRule.startPoint;
		mPlayer[i].handTile = mMountain.hand[j];
		std::sort(mPlayer[i].handTile.begin(), mPlayer[i].handTile.end());
		mPlayer[i].selfWind = (WindType)j;
		mPlayer[i].lizhi = -1;
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
				if (!mPlayer[index].canRong(target))
				{
					res.success = false;
					res.type = ErrorType::ActionRejected;
					return res;
				}
				mHaveMingpai[index] = true;
				mMingpaiAction[index] = act;
				break;
			case ActionType::Chi:
				if (!mPlayer[index].canChi(target, act.group, ((int)mPlayer[mTurn].selfWind + 4 - mPlayer[index].selfWind) % 4))
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
			if (!mPlayer[index].dapai(act.target))
			{
				res.success = false;
				res.type = ErrorType::TileNotExist;
				//std::cout << "***********" << std::endl;
				return res;
			}
			if (mPlayer[index].lingshang == true)
			{
				mDoraIndicatorCount++;
				mPlayer[index].lingshang = false;
				//todo:�ж��ĸ�ɢ��
			}
			mWaitMingpai = true;
			for (int i = 0; i < 4; ++i)
			{
				mHaveMingpai[i] = false;
				mMingpaiAction[i] = Action();
			}
			return res;
			//std::cout << "***********" << std::endl;
			break;
		default:
			res.success = false;
			res.type = ErrorType::ActionRejected;
			return res;
			break;
		}
		//��ʱ����������
		//һ�ֽ����ж�
		//std::cout << "***********" << std::endl;
		if (mMountain.remainCount() == 0)
		{
			endThisRound(std::vector<AgariResult>());
		}
		else
		{
			//std::cout << "***********" << std::endl;
			this->newTurn((mTurn + 1) % 4, false);
		}

	}
	return res;
}
//��һ�ȡ��Ϸ��Ϣ
GameInfo Game::getGameInfo(int index)
{
	GameInfo res;
	res.rule = mRule;//����
					 //ÿ����ҵ��ƺӣ��Ƿ���ֱ���Լ���¶����
	for (int i = 0, j = mEast; i <= 3; ++i, j = (j + 1) % 4)
	{
		PlayerInfo info;
		info.discardTile = mPlayer[j].discardTile;
		info.groupTile = mPlayer[j].groupTile;
		info.lizhi = mPlayer[j].lizhi;
		res.playerInfo.push_back(info);
	}
	//����
	res.handTile = mPlayer[index].handTile;
	//ʣ������
	res.remainTiles = mMountain.remainCount();
	//����ָʾ��
	for (int i = 0; i < mDoraIndicatorCount; i++)
	{
		res.doraIndicator.push_back(mMountain.getDoraIndicator(i));
	}
	res.prevailingWind = mPrevailingWind;   //����
	res.selfWind = mPlayer[index].selfWind;  //�ŷ�
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
	res.lizhibangCount = mLizhibangCount;  //������ֱ������
	res.benchang = mBenchang;//����
	res.mingpai = mWaitMingpai;
	res.round = mRound;
	return res;
}