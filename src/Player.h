#pragma once
#include"Single.h"
#include"Group.h"
#include"ActionGenerator.hpp"
#include"Algorithms.h"
#include<vector>
#include<algorithm>
#include<cassert>
class Player {
private:
	int subround{}; //巡数
	int score{};
	WindType selfWind; //门风
	Tiles handTile; //手牌
	std::vector<bool> disabledHandTile; //禁打手牌(食替)
	std::vector<Group> groupTile; //副露牌组
	Single nowTile; //自摸牌
	DiscardedTiles discardedTile; //牌河(包含被鸣走的)
	bool rinshan{}; //玩家当前的自摸牌是岭上牌
	bool tsumogiri = false; //表示牌河里最后一张牌是否为摸切
	int riichi = -1; //立直宣言牌是牌河中的第几张，-1为未立直
	int riichiJunme = -1; //立直的巡目,从0开始,-1为未立直,-2为w立
	bool ippatsu{}; //当前是否为一发巡
	bool riichiFuriten;//立直振听
	bool doujunFuriten;//同巡振听
	bool sutehaiFuriten;//舍张振听
	friend class Game;
public:
	void setInfo(
		int subround_, //巡数
		int score_,//分数
		WindType selfWind_, //门风
		const std::vector<Single>& handTile_, //手牌
		const std::vector<bool>& disabledHandTile_, //禁打手牌
		const std::vector<Group>& groupTile_, //副露牌组
		const Single& nowTile_, //自摸牌
		const DiscardedTiles& discardTile_, //牌河
		bool rinshan_, //玩家当前的自摸牌是岭上牌
		bool tsumogiri_, //表示牌河里最后一张牌是否为摸切
		int riichi_, //立直宣言牌是牌河中的第几张，-1为未立直
		int riichiJunme_, //立直的巡目,从0开始,-1为未立直,-2为w立
		bool ippatsu_ //当前是否为一发巡
	);

	//设置当前的自摸牌
	void setNowTile(Single target, bool isLingshang) {
		rinshan = isLingshang;
		nowTile = target;
	}

	//检测玩家给出的吃操作的正确性，relativePosition 0,1,2,3分别表示自下对上
	ActionCheckingResult canChii(Single target, Group result, int relativePosition)const;

	//检测玩家给出的碰操作的正确性，relativePosition 0,1,2,3分别表示自下对上
	bool canPon(Single target, Group result, int relativePosition)const;

	//检测玩家给出的明杠操作的正确性，relativePosition 0,1,2,3分别表示自下对上
	bool canMinKan(Single target, Group result, int relativePosition)const;


	//检测玩家给出的打牌操作的正确性
	bool canDiscardTile(Single target)const;

	//检测玩家给出的自摸操作的正确性
	bool canTsumo(WindType prevailingWind, BonusYakuState state, const Tiles& dora, const Tiles& ura)const {
		return Algorithms::agari(AgariParameters(selfWind, prevailingWind, riichiJunme, ippatsu, rinshan, state, AgariWays::Tsumo, nowTile,
			handTile, groupTile, dora, ura)).success;
	}

	//检查玩家给出的荣和操作的正确性
	bool canRon(Single target, WindType prevailingWind, AgariWays type, BonusYakuState state, const Tiles& dora, const Tiles& ura)const {
		if (doujunFuriten || sutehaiFuriten || riichiFuriten)
			return false;
		return Algorithms::agari(AgariParameters(selfWind, prevailingWind, riichiJunme, ippatsu, rinshan, state, type, target,
			handTile, groupTile, dora, ura)).success;
	}
	//检查玩家给出的立直操作的正确性
	bool canRiichi(BonusYakuState state, Single target)const;
	
	//检查玩家给出的九种九牌流局操作是否正确
	bool canRyuukyouku(BonusYakuState state)const;

	//执行碰操作
	void pon(Group result);

	//执行明杠操作
	void minkan(Group result);

	//TODO:执行吃操作
	void chii(Group result);

	//执行立直操作
	void doRiichi(BonusYakuState state, Single target);

	//执行打牌操作
	void discardTile(Single target);

	//执行自摸操作
	TryToAgariResult tsumo(WindType prevailingWind, BonusYakuState state, const Tiles& dora, const Tiles& ura) {
		return Algorithms::agari(AgariParameters(selfWind, prevailingWind, riichiJunme, ippatsu, rinshan, state, AgariWays::Tsumo, nowTile,
			handTile, groupTile, dora, ura));
	}

	//执行荣和操作
	AgariResult ron(Single target, WindType prevailingWind, AgariWays type, BonusYakuState state, const Tiles& dora, const Tiles& ura) {
		return Algorithms::agari(AgariParameters(selfWind, prevailingWind, riichiJunme, ippatsu, rinshan, state, type, target,
			handTile, groupTile, dora, ura)).result;
	}

	//是否听牌
	bool tenpai()const { return !Algorithms::tenpai(handTile).empty(); }
	//更新舍张振听
	void updateSutehaiFuriten();
	//设置立直振听
	void setRiichiFuriten();
	//设置同巡振听
	void setDoujunFuriten();
	//取消同巡振听
	void resetDoujunFuriten();
};
