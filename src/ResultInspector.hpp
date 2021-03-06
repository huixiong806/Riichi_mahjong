#pragma once
#include<vector>
#include"Rules.h"
#include"Single.h"
#include"Group.h"
#include"Yaku.h"
#include"Enums.h"
struct AgariResult {
	bool tsumo;//自摸
	int furikomiID{}; //放铳人ID 
	int agariID{}; //和牌人ID 
	int han; //翻数，-1=役满，-2=两倍役满，-3=三倍役满，以此类推,13番累计役满
	int fu;//符数
	int dora;//宝牌数
	int akadora;//红宝牌数
	int uradora;//里宝牌数
	int scoreAdd; //不包含立直棒和本场棒的计算
	int scoreDecFurikomi; //自摸无效 
	int scoreDecOya; //荣和无效,庄家和牌无效
	int scoreDecKodomo; //荣和无效
	//int nukidora;
	AgariResult() {
		tsumo = han = fu = dora = akadora = uradora = scoreAdd = scoreDecFurikomi = scoreDecOya = scoreDecKodomo = 0;
	}

	Yakus yaku;
	bool operator <(const AgariResult& rhs) const { return scoreAdd < rhs.scoreAdd; }
	bool operator ==(const AgariResult& rhs) const { return scoreAdd == rhs.scoreAdd; }
};


struct TryToAgariResult {
	AgariResult result;
	bool success;
	AgariFaildReason reason;

	bool operator <(const TryToAgariResult& rhs) const {

		if (!success && !rhs.success)
			return reason < rhs.reason;
		if (success && !rhs.success)
			return false;
		if (!success && rhs.success)
			return true;
		return result < rhs.result;
	}

	TryToAgariResult() {
		success = false;
		result = AgariResult();
	}

	TryToAgariResult(AgariFaildReason reason_) {
		success = false;
		reason = reason_;
	}

	TryToAgariResult(const AgariResult& result_) {
		success = true;
		result = result_;
	}
};

struct AgariParameters {
	WindType selfWind;
	WindType prevailingWind;
	int riichiJunme;
	bool ippatsu;
	bool rinshan;
	BonusYakuState state;
	AgariWays type;
	Single target;
	std::vector<Single> handTile;
	std::vector<Group> groupTile;
	std::vector<Single> dora;
	std::vector<Single> ura;

	AgariParameters(
		WindType selfWind_,
		WindType prevailingWind_,
		int lizhiXunmu_,
		bool yifa_,
		bool rinshan_,
		BonusYakuState state_,
		AgariWays type_,
		const Single& target_,
		const std::vector<Single>& handTile_,
		const std::vector<Group>& groupTile_,
		const std::vector<Single>& dora_,
		const std::vector<Single>& ura_
	) {
		selfWind = selfWind_;
		prevailingWind = prevailingWind_;
		riichiJunme = lizhiXunmu_;
		ippatsu = yifa_;
		state = state_;
		type = type_;
		target = target_;
		handTile = handTile_;
		groupTile = groupTile_;
		dora = dora_;
		ura = ura_;
		rinshan = rinshan_;
	}
};
