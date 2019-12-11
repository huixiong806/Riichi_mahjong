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
	std::vector<Single> handTile; //手牌
	std::vector<bool> disabledHandTile; //禁打手牌
	std::vector<Group> groupTile; //副露牌组
	Single nowTile; //自摸牌
	std::vector<Single> discardTile; //牌河
	bool rinshan{}; //玩家当前的自摸牌是岭上牌
	bool tsumogiri = false; //表示牌河里最后一张牌是否为摸切
	int riichi = -1; //立直宣言牌是牌河中的第几张，-1为未立直
	int riichiJunme = -1; //立直的巡目,从0开始,-1为未立直,-2为w立
	bool ippatsu{}; //当前是否为一发巡
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
		const std::vector<Single>& discardTile_, //牌河
		bool rinshan_, //玩家当前的自摸牌是岭上牌
		bool tsumogiri_, //表示牌河里最后一张牌是否为摸切
		int riichi_, //立直宣言牌是牌河中的第几张，-1为未立直
		int riichiJunme_, //立直的巡目,从0开始,-1为未立直,-2为w立
		bool ippatsu_ //当前是否为一发巡
	);

	void setNowTile(Single target, bool isLingshang) {
		rinshan = isLingshang;
		nowTile = target;
	}

	//relativePosition 0,1,2,3分别表示下对上自
	bool canChi(Single target, Group result, int relativePosition)const { return false; }

	bool canPeng(Single target, Group result, int relativePosition)const;

	//仅限荣杠
	bool canGang(Single target, Group result, int relativePosition)const;

	void peng(Group result);

	void ronggang(Group result);

	void chi(Group result) { }

	bool canDapai(Single target)const;
	//返回false时不应变动私有变量
	void dapai(Single target);

	//返回false时不应变动私有变量
	/*
	state
	正常=0
	天地和,w立=1
	河底/海底=2
	*/
	TryToAgariResult tsumo(WindType prevailingWind, BonusYakuState state, const std::vector<Single>& dora,
	                      const std::vector<Single>& ura) {
		return Algorithms::agari(AgariParameters(selfWind, prevailingWind, riichiJunme, ippatsu, state, 0, nowTile,
		                                         handTile, groupTile, dora, ura));
	}
	bool canTsumo(WindType prevailingWind, BonusYakuState state, const std::vector<Single>& dora,
		const std::vector<Single>& ura)const {
		return Algorithms::agari(AgariParameters(selfWind, prevailingWind, riichiJunme, ippatsu, state, 0, nowTile,
			handTile, groupTile, dora, ura)).success;
	}

	AgariResult rong(Single target, WindType prevailingWind, int type, BonusYakuState state, const std::vector<Single>& dora,
	                 const std::vector<Single>& ura) {
		return Algorithms::agari(AgariParameters(selfWind, prevailingWind, riichiJunme, ippatsu, state, type, target,
		                                         handTile, groupTile, dora, ura)).result;
	}

	bool canRong(Single target, WindType prevailingWind, int type, BonusYakuState state, const std::vector<Single>& dora,
	             const std::vector<Single>& ura)const {
		return Algorithms::agari(AgariParameters(selfWind, prevailingWind, riichiJunme, ippatsu, state, type, target,
		                                         handTile, groupTile, dora, ura)).success;
	}
	/*
	state
	正常=0
	天地和,w立=1
	河底/海底=2
	*/
	bool canLizhi(BonusYakuState state, Single target)const;
	void doLizhi(BonusYakuState state, Single target);
	//返回false时不应变动私有变量
	bool ryuukyoku()const { return false; }
	bool tenpai()const { return !Algorithms::tenpai(handTile).empty(); }
	
};
