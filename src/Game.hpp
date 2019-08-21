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
	//int mIndex;//��ʾ���ֵı��
	Rule mRule;//����
	Mountain mMountain;     //��ɽ
	WindType mPrevailingWind;     //����
	std::vector<Player> mPlayer;      //0,1,2,3�ֱ��Ӧ�ĸ���ҡ�
	int mEast;//��ʾׯ�ҵı��,ׯ��Ϊ1����������ұ�ŷֱ�Ϊ1,2,3,0,�Դ����ơ�
	int mLizhibang;    //������ֱ������
	int mRound;  //����(1~4)
	int mBenchang;    //����
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
	bool mW;//�Ƿ���w��,���,�غͿɳ�����״̬
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
		if (mPlayer[who].subround > 0)
			mW = false;
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
	//;��������ׯ
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
		//���˺���
		//TODO:��������
		if (!res.empty())
		{
			mResult.liuju = false;
			mResult.agariResult = res;
			//��Ϸ�����ж�
			if (mPrevailingWind == mRule.gameType&&mRound == 4)
			{	
				//ׯ��û���ƻ��ߺ��ƺ���1λ,��Ϸ����
				bool zhuangjiaHupai = false;
				for (auto&item : res)
					if (mPlayer[item.first].selfWind == WindType::EAST)
						zhuangjiaHupai = true;
				if (!zhuangjiaHupai||(zhuangjiaHupai&&top==mEast))
					mGameover = true;
				//�����top�ĵ��������Ҫ����
				if (mPlayer[top].score < mRule.endPoint)
					mGameover = false;
			}
			else if (mPrevailingWind == mRule.gameType + 1)//��4�ֲ����Ƕ���
			{
				//4��ǿ�ƽ�����������������Ҳǿ�ƽ���
				if (mRound == 4)mGameover = true;
				if (mPlayer[top].score >= mRule.endPoint)
					mGameover = true;
			}
			return;
		}
		//����
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
		//��Ϸ�����ж�
		if (mPrevailingWind == mRule.gameType&&mRound == 4)
		{
			//ׯ��û���ƻ����շ�������1λ,��Ϸ����
			bool zhuangjiaTingpai = mResult.lianzhuang;
			if (!zhuangjiaTingpai || (zhuangjiaTingpai&&top == mEast))
				mGameover = true;
			//�����top�ĵ��������Ҫ����
			if (mPlayer[top].score < mRule.endPoint)
				mGameover = false;
		}
		else if (mPrevailingWind == mRule.gameType + 1)//��4�ֲ����Ƕ���
		{
			//4��ǿ�ƽ�����������������Ҳǿ�ƽ���
			if (mRound == 4)mGameover = true;
			if (mPlayer[top].score >= mRule.endPoint)
				mGameover = true;
		}
		if (mGameover)
			mPlayer[top].score += mLizhibang;
	}
	//��ȥw��,һ��,�غ�
	void resetBounusYaku()
	{
		mW = false;
		for (int i = 0; i < 4; ++i)
			mPlayer[i].yifa = false;
	}
	//��������
	void processMingpai()
	{
		std::vector<std::pair<int,AgariResult>> result;
		bool overflag = false;
		//��
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
		//�ܺ���
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
		//��
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
		//ȫ���������ƻ���û������
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
	mW = true;
	mGameover = false;
	mRoundover = false;
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
				//��ֱ��������
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
				//��ֱ��������
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
				//��ֱ��������
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
				//todo:�ж��ĸ�ɢ��
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
		//��ʱ����������
		//һ�ֽ����ж�
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
		info.lizhiXunmu = mPlayer[j].lizhiXunmu;
		info.yifa = mPlayer[j].yifa;
		info.score= mPlayer[j].score;
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
	res.lizhibangCount = mLizhibang;  //������ֱ������
	res.benchang = mBenchang;//����
	res.mingpai = mWaitMingpai;
	res.round = mRound;
	res.w = mW;
	return res;
}