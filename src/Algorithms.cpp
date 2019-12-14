#include <set>
#include <map>
#include <cassert>
#include "Algorithms.h"
#include "YakuChecker.h"
uint8_t Algorithms::distanceToTarget[1953125][10];
uint8_t Algorithms::distanceToTargetZ[78125][10];

const int Algorithms::pow5[10] = {1, 5, 25, 125, 625, 3125, 15625, 78125, 390625, 1953125};
/*
//shape为压缩后的牌型参数，value为要获取个数的数字
int Algorithms::getNumberCount(int shape, int value) { return (shape / pow5[value - 1]) % 5; }
//count不得超过4
void Algorithms::setNumberCount(int& shape, int value, int count) {
	shape = (shape / pow5[value]) * pow5[value] + count * pow5[value - 1] + shape % pow5[value - 1];
}

void Algorithms::addNumberCount(int& shape, int value, int count) { shape += pow5[value - 1]; }
*/
//0~8 万字,9~17 饼子,18~26 索子,27~33 东南西北白发中
int Algorithms::getTileIndex(Single& tile)
{
	if (tile.color() == 'm')return tile.value() - 1;
	if (tile.color() == 'p')return tile.value() + 8;
	if (tile.color() == 's')return tile.value() + 17;
	if (tile.color() == 'z')return tile.value() + 26;
	return -1;
}
//获取所有类型的牌
std::vector<Single> Algorithms::allKindsOfTiles()
{
	std::vector<Single> res;
	for (auto i = 1; i <= 9; ++i) {
		res.emplace_back(i, 'm', 0);
		res.emplace_back(i, 'p', 0);
		res.emplace_back(i, 's', 0);
		if (i <= 7)
			res.emplace_back(i, 'z', 0);
	}
	return res;
}
std::vector<Single> Algorithms::tenpai(const std::vector<Single>& handTile) {
	std::vector<Single> res;
	for (auto i = 1; i <= 9; ++i) {
		if (agariWithoutYaku(Single(i, 'm', false), handTile))
			res.emplace_back(i, 'm', 0);
		if (agariWithoutYaku(Single(i, 'p', false), handTile))
			res.emplace_back(i, 'p', 0);
		if (agariWithoutYaku(Single(i, 's', false), handTile))
			res.emplace_back(i, 's', 0);
		if (i <= 7 && agariWithoutYaku(Single(i, 'z', false), handTile))
			res.emplace_back(i, 'z', 0);
	}
	return res;
}

std::vector<int> Algorithms::getPool(const std::vector<Single>& tiles) {
	std::vector<int> res;
	res.resize(34);
	for (auto& item : tiles) {
		if (item.color() == 'm') { res[item.value() - 1]++; }
		else if (item.color() == 'p') { res[item.value() + 8]++; }
		else if (item.color() == 's') { res[item.value() + 17]++; }
		else if (item.color() == 'z') { res[item.value() + 26]++; }
		else { assert(0); }
	}
	return res;
}

AgariResult Algorithms::getScore(WindType selfWind, AgariResult inp) {
	const int east = selfWind == EAST;
	auto res = inp;
	//役满型 ***暂时不考虑包牌*** 
	if (inp.han < 0) {
		res.scoreAdd = (-inp.han) * 16000 * (2 + east);
		if (!inp.tsumo)
			res.scoreDecFurikomi = res.scoreAdd;
		else if (east)
			res.scoreDecKodomo = (-inp.han) * 16000;
		else {
			res.scoreDecKodomo = (-inp.han) * 8000;
			res.scoreDecOya = (-inp.han) * 16000;
		}
		return res;
	}
	//非役满型
	if (inp.han < 1)res.scoreAdd = 0;
	if (inp.han >= 13) {
		res.scoreAdd = 16000 * (2 + east);
		res.scoreDecOya = 16000;
		res.scoreDecKodomo = 8000 * (1 + east);
	}
	else if (inp.han >= 11) {
		res.scoreAdd = 12000 * (2 + east);
		res.scoreDecOya = 12000;
		res.scoreDecKodomo = 6000 * (1 + east);
	}
	else if (inp.han >= 8) {
		res.scoreAdd = 8000 * (2 + east);
		res.scoreDecOya = 8000;
		res.scoreDecKodomo = 4000 * (1 + east);
	}
	else if (inp.han >= 6) {
		res.scoreAdd = 6000 * (2 + east);
		res.scoreDecOya = 6000;
		res.scoreDecKodomo = 3000 * (1 + east);
	}
	else if (inp.han == 5) {
		res.scoreAdd = 4000 * (2 + east);
		res.scoreDecOya = 4000;
		res.scoreDecKodomo = 2000 * (1 + east);
	}
	else if (!inp.tsumo)res.scoreAdd = (std::min(((inp.fu << (inp.han + 4)) - 10), 7999) * (2 + east) / 200 + 1) * 100;
	else if (east) {
		res.scoreAdd = (std::min(((inp.fu << (inp.han + 4)) - 10), 7999) / 200 + 1) * 300;
		res.scoreDecKodomo = (std::min(((inp.fu << (inp.han + 4)) - 10), 7999) / 200 + 1) * 100;
	}
	else {
		res.scoreAdd = (std::min(((inp.fu << (inp.han + 4)) - 10), 7999) / 200 + 1) * 100 + (std::min(
			((inp.fu << (inp.han + 4)) - 10), 7999) / 400 + 1) * 200;
		res.scoreDecKodomo = (std::min(((inp.fu << (inp.han + 4)) - 10), 7999) / 400 + 1) * 100;
		res.scoreDecOya = (std::min(((inp.fu << (inp.han + 4)) - 10), 7999) / 200 + 1) * 100;
	}
	res.scoreDecFurikomi = res.scoreAdd; //放铳失点等于和牌得点 
	return res;
}

TryToAgariResult Algorithms::agari(const AgariParameters& par) {
	//国士无双的判定
	const auto kokushi = guoshiwushuang(par);
	if (kokushi.success)
		return kokushi;
	//只有国士能抢暗杠
	if (par.type == AgariWays::ChanAnkan)return TryToAgariResult(AgariFaildReason::AgariTypeWrong);
	//七对型的判定(两杯口将被拆解为七对型)
	const auto qiduizi = chiitoi(par);
	//一般型的判定
	auto allTiles = par.handTile;
	allTiles.push_back(par.target);
	sort(allTiles.begin(), allTiles.end());
	const auto normal = agariSearch(par, 0, allTiles, std::vector<Group>());
	return std::max(qiduizi, normal);
}

//标准型和牌役种判断
TryToAgariResult Algorithms::YakuCheckForStandard(const AgariParameters& par, std::vector<Group>& mentsu) {
	auto allMentsu = mentsu;
	allMentsu.insert(allMentsu.end(), par.groupTile.begin(), par.groupTile.end());
	return CheckYakuForStandardType(par, allMentsu);
}

//判断是否为标准和牌型，返回(点数最大的)结果,depth=0表示枚举雀头，dep=4为最深层
/*
type
自摸=0
荣和=1
抢杠=2
抢暗杠=3

state
正常=0
天地和=1
河底/海底=2
*/
TryToAgariResult Algorithms::agariSearch(const AgariParameters& par, int depth, const std::vector<Single>& remainTiles,
                                         std::vector<Group> mentsu) {
	TryToAgariResult bestResult;
	if (remainTiles.empty()) { return YakuCheckForStandard(par, mentsu); }
	if (depth == 0) {
		std::vector<Single> newRemainTiles { remainTiles.size() - 2 };
		for (auto i = 0u; i < remainTiles.size() - 1; ++i) {
			if (remainTiles[i] == remainTiles[i + 1]) {
				newRemainTiles.clear();
				for (auto j = 0u; j < remainTiles.size(); ++j) {
					if (j != i && j != i + 1)
						newRemainTiles.push_back(remainTiles[j]);
				}
				mentsu.push_back(Group::createToitsu(remainTiles[i], remainTiles[i + 1]));
				bestResult = std::max(bestResult, agariSearch(par, depth + 1,  newRemainTiles , mentsu));
				mentsu.pop_back();
			}
		}
		return bestResult;
	}
	auto pool = getPool(remainTiles);
	//判断顺子和刻子
	for (auto i = 0; i <= 33; ++i) {
		//唯一的分歧点，三连刻和三同顺
		if (i != 7 && i != 8 && i != 16 && i != 17 && i <= 24) {
			if (pool[i] >= 3 && pool[i + 1] >= 3 && pool[i + 2] >= 3) {
				Single standard[3];
				if (i < 7) {
					standard[0] = Single(i + 1, 'm', false);
					standard[1] = Single(i + 2, 'm', false);
					standard[2] = Single(i + 3, 'm', false);
				}
				else if (i < 16) {
					standard[0] = Single(i - 8, 'p', false);
					standard[1] = Single(i - 7, 'p', false);
					standard[2] = Single(i - 6, 'p', false);
				}
				else {
					standard[0] = Single(i - 17, 's', false);
					standard[1] = Single(i - 16, 's', false);
					standard[2] = Single(i - 15, 's', false);
				}
				std::vector<Single> newRemainTiles;
				std::vector<Single> group[3];
				int count[3] = {3, 3, 3};
				for (auto& item : remainTiles) {
					auto equalToStandard = false;
					for (auto j = 0; j < 3; ++j) {
						if (item.valueEqual(standard[j]) && count[j] > 0) {
							count[j]--;
							group[j].push_back(item);
							equalToStandard = true;
						}
					}
					if (!equalToStandard)
						newRemainTiles.push_back(item);
				}
				//三连刻
				mentsu.push_back(Group::createKoutsu(group[0][0], group[0][1], group[0][2], 0));
				mentsu.push_back(Group::createKoutsu(group[1][0], group[1][1], group[1][2], 0));
				mentsu.push_back(Group::createKoutsu(group[2][0], group[2][1], group[2][2], 0));
				bestResult = std::max(bestResult, agariSearch(par, depth + 3, newRemainTiles, mentsu));
				mentsu.pop_back();
				mentsu.pop_back();
				mentsu.pop_back();
				//三同顺
				mentsu.push_back(Group::createShuntsu(group[0][0], group[1][0], group[2][0], 0));
				mentsu.push_back(Group::createShuntsu(group[0][1], group[1][1], group[2][1], 0));
				mentsu.push_back(Group::createShuntsu(group[0][2], group[1][2], group[2][2], 0));
				bestResult = std::max(bestResult, agariSearch(par, depth + 3, newRemainTiles, mentsu));
				return bestResult;
			}
		}
		//刻子
		if (pool[i] >= 3) {
			std::vector<Single> newRemainTiles;
			std::vector<Single> kezi;
			auto count = 3;
			Single standard;
			if (i <= 8)standard = Single(i + 1, 'm', false);
			else if (i <= 17)standard = Single(i - 8, 'p', false);
			else if (i <= 26)standard = Single(i - 17, 's', false);
			else standard = Single(i - 26, 'z', false);
			for (auto& item : remainTiles) {
				if (item.valueEqual(standard) && count > 0) {
					kezi.push_back(item);
					count--;
					continue;
				}
				newRemainTiles.push_back(item);
			}
			mentsu.push_back(Group::createKoutsu(kezi[0], kezi[1], kezi[2], 0));
			bestResult = agariSearch(par, depth + 1, newRemainTiles, mentsu);
			return bestResult;
		}
		//顺子
		if (i != 7 && i != 8 && i != 16 && i != 17 && i <= 24) {
			if (pool[i] > 0 && pool[i + 1] > 0 && pool[i + 2] > 0) {
				Single standard[3];
				if (i < 7) {
					standard[0] = Single(i + 1, 'm', false);
					standard[1] = Single(i + 2, 'm', false);
					standard[2] = Single(i + 3, 'm', false);
				}
				else if (i < 16) {
					standard[0] = Single(i - 8, 'p', false);
					standard[1] = Single(i - 7, 'p', false);
					standard[2] = Single(i - 6, 'p', false);
				}
				else {
					standard[0] = Single(i - 17, 's', false);
					standard[1] = Single(i - 16, 's', false);
					standard[2] = Single(i - 15, 's', false);
				}
				std::vector<Single> newRemainTiles;
				std::vector<Single> group;
				group.resize(3);
				int count[3] = {1, 1, 1};
				for (auto& item : remainTiles) {
					auto equalToStandard = false;
					for (auto j = 0; j < 3; ++j) {
						if (item.valueEqual(standard[j]) && count[j] > 0) {
							count[j]--;
							group[j] = item;
							equalToStandard = true;
						}
					}
					if (!equalToStandard)
						newRemainTiles.push_back(item);
				}
				//顺子
				mentsu.push_back(Group::createShuntsu(group[0], group[1], group[2], 0));
				bestResult = agariSearch(par, depth + 1, newRemainTiles, mentsu);
				return bestResult;
			}
		}
		//存在浮牌
		if (pool[i] > 0) { return TryToAgariResult(AgariFaildReason::ShapeWrong); }
	}
	assert(0);
	return TryToAgariResult(AgariFaildReason::ShapeWrong);
}

TryToAgariResult Algorithms::chiitoi(const AgariParameters& par) {
	auto res = AgariResult();
	std::map<int, int> all;
	for (auto& item : par.handTile)
		all[item.color() * 100 + item.value()]++;
	all[par.target.color() * 100 + par.target.value()]++;
	if (all.size() != 7)return TryToAgariResult(AgariFaildReason::ShapeWrong);
	for (auto& item : all) { if (item.second != 2) { return TryToAgariResult(AgariFaildReason::ShapeWrong); } }
	//检查可以和七对子复合的役种:dora,ura,aka,立直,一发,两立直,门清自摸,断幺九,清一色,海底摸月,河底捞鱼,混老头,混一色
	//役满型:天和,地和,字一色
	if (par.state == BonusYakuState::FirstTurn && par.type == AgariWays::Tsumo) {
		if (par.selfWind == EAST)
			res.yaku.add<Yaku::Tenhou>();
		else
			res.yaku.add<Yaku::Chihou>();
		res.han -= 1;
	}
	auto tsuuiisou = true;
	for (auto& item : par.handTile) {
		if (item.color() != 'z') {
			tsuuiisou = false;
			break;
		}
	}
	if (par.target.color() != 'z')
		tsuuiisou = false;
	if (tsuuiisou == true) {
		res.han -= 1;
		res.yaku.add<Yaku::Tsuuiisou>();
	}
	res.tsumo = par.type == AgariWays::Tsumo;
	//满足役满型
	if (res.han < 0) {
		res = getScore(par.selfWind, res);
		return TryToAgariResult(res);
	}
	//不满足役满型，则一定需要计算七对型
	res.han += 2;
	res.fu = 25;
	res.yaku.add<Yaku::Chiitoitsu>();
	//检查dora，akadora和uradora
	auto myHandTile = par.handTile;
	myHandTile.push_back(par.target);
	for (auto& doraneko : par.dora)
		for (auto& item : myHandTile)
			if (doraneko.valueEqual(item))
				res.dora++;
	for (auto& doraneko : par.ura)
		for (auto& item : myHandTile)
			if (doraneko.valueEqual(item))
				res.uradora++;
	for (auto& item : myHandTile)
		if (item.isAkadora())
			res.akadora++;
	res.han += res.dora + res.akadora + res.uradora;
	//检查立直,两立直和一发
	if (par.riichiJunme != -1) {
		//w立
		if (par.riichiJunme == -2) {
			res.han += 2;
			res.yaku.add<Yaku::doubleRiichi>();
		}
		else {
			res.han += 1;
			res.yaku.add<Yaku::Riichi>();
		}
		if (par.ippatsu == true) {
			res.han += 1;
			res.yaku.add<Yaku::Ippatsu>();
		}
	}
	//检查门清自摸
	if (par.type == AgariWays::Tsumo) {
		res.han += 1;
		res.yaku.add<Yaku::Menzenchintsumo>();
	}
	//检查河底/海底
	if (par.state == BonusYakuState::LastTurn) {
		if (par.type == AgariWays::Tsumo)
			res.yaku.add<Yaku::Haiteiraoyue>();
		else if (par.type == AgariWays::Ron)res.yaku.add<Yaku::Houteiraoyui>();

	}
	//检查清一色，混一色，断幺九，混老头
	auto chinitsu = true, honitsu = true, tanyao = true, honraotou = true;
	auto color = '0';
	for (auto& item : myHandTile) {
		if (item.color() == 'z') {
			tanyao = false;
			chinitsu = false;
		}
		else {
			if (color == '0')
				color = item.color();
			else if (color != item.color()) {
				chinitsu = false;
				honitsu = false;
			}
		}
		if (item.value() == 1 || item.value() == 9)
			tanyao = false;
		if (item.value() != 1 && item.value() != 9)
			honraotou = false;
	}
	if (chinitsu) {
		res.han += 6;
		res.yaku.add<Yaku::Chinitsu>();
	}
	else if (honitsu) {
		res.han += 3;
		res.yaku.add<Yaku::Honitsu>();
	}
	if (tanyao) {
		res.han += 1;
		res.yaku.add<Yaku::Tanyao>();
	}
	if (honraotou) {
		res.han += 2;
		res.yaku.add<Yaku::Honroutou>();
	}
	res = getScore(par.selfWind, res);
	return TryToAgariResult(res);
}

//判断是否为国士无双和牌型
TryToAgariResult Algorithms::guoshiwushuang(const AgariParameters& par) {
	auto res = AgariResult();
	auto kokushi = 0;
	if (par.groupTile.empty()) {
		std::set<Single> all;
		for (auto& item : par.handTile)
			if (item.value() == 1 || item.value() == 9 || item.color() == 'z')
				all.insert(item);
		//国士
		if (all.size() == 12) {
			if (par.target.value() == 1 || par.target.value() == 9 || par.target.color() == 'z') {
				all.insert(par.target);
				if (all.size() == 13)
					kokushi = 1;
			}
		}
			//国士十三面
		else if (all.size() == 13) {
			if (par.target.value() == 1 || par.target.value() == 9 || par.target.color() == 'z')
				kokushi = 2;
		}
	}
	//可以复合天地和
	if (par.state == BonusYakuState::FirstTurn && par.type == AgariWays::Tsumo) {
		if (par.selfWind == EAST) {
			if (kokushi != 0)
				kokushi = 2;
			res.yaku.add<Yaku::Tenhou>();
		}
		else
			res.yaku.add<Yaku::Chihou>();
		if (kokushi > 0)
			res.han -= 1;
	}
	if (kokushi == 1) {
		res.yaku.add<Yaku::Kokushimusou>();
		res.han -= 1;
	}
	else if (kokushi == 2) {
		res.yaku.add<Yaku::Kokushijuusanmenmachi>();
		res.han -= 2;
	}
	else { return TryToAgariResult(AgariFaildReason::ShapeWrong); }
	res.tsumo = (par.type == AgariWays::Tsumo);
	res = getScore(par.selfWind, res);
	return TryToAgariResult(res);
}
/*
bool Algorithms::guoshiWithoutYaku(const Single& target, const std::vector<Single>& handTile) {
	auto allTiles = handTile;
	allTiles.push_back(target);
	return getDistanceGuoshi(allTiles) == -1;
}

bool Algorithms::qiduiWithoutYaku(const Single& target, const std::vector<Single>& handTile) {
	auto res = false;
	std::map<int, int> all;
	for (auto& item : handTile)
		all[item.color() * 100 + item.value()]++;
	all[target.color() * 100 + target.value()]++;
	if (all.size() != 7)return false;
	for (auto& item : all)
		if (item.second != 2)
			return false;
	return true;
}
bool Algorithms::agariSearchWithoutYaku(std::vector<int> pool) {
	//判断雀头
	for (auto i = 0; i <= 33; ++i) {
		if (pool[i] >= 2) {
			auto tempPool = pool;
			tempPool[i] -= 2;
			//刻子
			for (auto j = 0; j <= 33; ++j)
				if (tempPool[j] >= 3)
					tempPool[j] -= 3;
			//顺子
			for (auto j = 0; j <= 33; ++j)
				if (j != 7 && j != 8 && j != 16 && j != 17 && j <= 24)
					if (tempPool[j] > 0 && tempPool[j + 1] > 0 && tempPool[j + 2] > 0) {
						tempPool[j]--;
						tempPool[j + 1]--;
						tempPool[j + 2]--;
					}
			//存在浮牌
			auto res = true;
			for (auto j = 0; j <= 33; ++j)
				if (tempPool[j] > 0) {
					res = false;
					break;
				}
			if (res)return true;
		}
	}
	return false;
}
*/
bool Algorithms::agariWithoutYaku(const Single& target, const std::vector<Single>& handTile) {
	CompactSingles state;
	for (int i = 0; i <= handTile.size();++i) {
		const auto& item = i == handTile.size() ? target : handTile[i];
		if (item.color() == 'm')
			state.colors[0].add(item.value(), 1);
		if (item.color() == 'p')
			state.colors[1].add(item.value(), 1);
		if (item.color() == 's')
			state.colors[2].add(item.value(), 1);
		if (item.color() == 'z')
			state.colors[3].add(item.value(), 1);
	}
	//判断是否虚和(一种牌拿5张)
	for (int i = 1; i <= 9; ++i) {
		if (state.colors[0].get(i) >= 5)return false;
		if (state.colors[1].get(i) >= 5)return false;
		if (state.colors[2].get(i) >= 5)return false;
		if (state.colors[3].get(i) >= 5)return false;
	}
	//国士无双型的判定
	const auto kokushi = getDistanceKokushi(state) == -1;
	if (kokushi)return true;
	//七对型的判定(两杯口将被拆解为七对型)
	const auto chiitoi = getDistanceChiitoi(state) == -1;
	if (chiitoi)
		return true;
	//一般型的判定
	const auto standard = getDistanceStandard(state) == -1;
	return standard;
}

void Algorithms::constructTarget(int quetou, int mentsu, std::queue<SparseSinglesOfColor>& q, SparseSinglesOfColor shape, int target, bool z) {
	SparseSinglesOfColor nextShape;
	if (quetou > 0) {
		for (auto value = 1; value <= (z ? 7 : 9); ++value) {
			nextShape = shape;
			const auto count = shape.get(value); 
			if (count <= 2) {
				nextShape.add(value,2);
				constructTarget(quetou - 1, mentsu, q, nextShape, target, z);
			}
		}
	}
	if (mentsu > 0) {
		//刻子
		for (auto value = 1; value <= (z ? 7 : 9); ++value) {
			nextShape = shape;
			if (nextShape.get(value) <= 1) {
				nextShape.add(value, 3);
				constructTarget(quetou, mentsu - 1, q, nextShape, target, z);
			}
		}
		//顺子
		if (!z) {
			for (auto value = 1; value <= 7; ++value) {
				nextShape = shape;
				const auto count0 = shape.get(value);
				const auto count1 = shape.get(value+1);
				const auto count2 = shape.get(value+2);
				if (count0 <= 3 && count1 <= 3 && count2 <= 3) {
					nextShape.add(value, 1);
					nextShape.add(value+1, 1);
					nextShape.add(value+2, 1);
					constructTarget(quetou, mentsu - 1, q, nextShape, target, z);
				}
			}
		}
	}
	if (mentsu > 0 || quetou > 0)return;
	q.push(shape);
	if (!z)distanceToTarget[CompactSinglesOfColor(shape).raw()][target] = 0;
	else distanceToTargetZ[CompactSinglesOfColor(shape).raw()][target] = 0;
}

//预处理，计算单花色离目标的距离(仅允许插入和删除两个操作)
void Algorithms::preprocessDistance() {
	for (auto j = 0; j < 10; ++j) {
		for (auto i = 0; i < pow5[9]; ++i)
			distanceToTarget[i][j] = 255;
		for (auto i = 0; i < pow5[7]; ++i)
			distanceToTargetZ[i][j] = 255;
	}

	//枚举目标
	for (auto target = 0; target < 10; ++target) {
		const auto jyantou = target / 5;
		const auto mentsu = target % 5;
		//构造目标(同时也是BFS的起点)
		std::queue<SparseSinglesOfColor> q;
		constructTarget(jyantou, mentsu, q, SparseSinglesOfColor(), target, false);
		//BFS
		while (!q.empty()) {
			const auto now = q.front();
			q.pop();
			for (auto value = 1; value <= 9; ++value) {
				SparseSinglesOfColor next;
				const auto count = now.get(value);
				if (count > 0) {
					next = now;
					next.add(value, - 1);
					if (distanceToTarget[CompactSinglesOfColor(next).raw()][target] == 255) {
						distanceToTarget[CompactSinglesOfColor(next).raw()][target] = distanceToTarget[CompactSinglesOfColor(now).raw()][target] + 1;
						q.push(next);
					}
				}
				if (count < 4) {
					next = now;
					next.add(value, +1);
					if (distanceToTarget[CompactSinglesOfColor(next).raw()][target] == 255) {
						distanceToTarget[CompactSinglesOfColor(next).raw()][target] = distanceToTarget[CompactSinglesOfColor(now).raw()][target] + 1;
						q.push(next);
					}
				}
			}
		}
		//字牌
		constructTarget(jyantou, mentsu, q, SparseSinglesOfColor(), target, true);
		while (!q.empty()) {
			const auto now = q.front();
			q.pop();
			for (auto value = 1; value <= 7; ++value) {
				SparseSinglesOfColor next;
				const auto count = now.get(value); 
				if (count > 0) {
					next = now;
					next.add(value, -1);
					if (distanceToTargetZ[CompactSinglesOfColor(next).raw()][target] == 255) {
						distanceToTargetZ[CompactSinglesOfColor(next).raw()][target] = distanceToTargetZ[CompactSinglesOfColor(now).raw()][target] + 1;
						q.push(next);
					}
				}
				if (count < 4) {
					next = now;
					next.add(value, +1);
					if (distanceToTargetZ[CompactSinglesOfColor(next).raw()][target] == 255) {
						distanceToTargetZ[CompactSinglesOfColor(next).raw()][target] = distanceToTargetZ[CompactSinglesOfColor(now).raw()][target] + 1;
						q.push(next);
					}
				}
			}
		}
	}
}

//获得单花色向听数
int Algorithms::getDistanceSingle(int shape, int mentsu, int jyantou, bool z) {
	if (!z)return distanceToTarget[shape][mentsu + jyantou * 5];
	return distanceToTargetZ[shape][mentsu + jyantou * 5];
}

//计算14张手牌的向听数(0为一向听，-1为和牌)
int Algorithms::getDistanceStandard(const CompactSingles& handTile) {
	//计算标准型向听数
	int dp[5][5][2]; //a当前已经完成的花色数，b当前已经完成的面子数，c当前已经完成的雀头数,dp表示最小距离
	//int pre[5][5][2];
	for (auto& i : dp)
		for (auto& j : i)
			for (int& k : j) {
				k = 99; //此距离最大为18，表示8向听
				//pre[i][j][k] = -1;
			}
	dp[0][0][0] = 0; //设定边界值，0花色0面子0雀头距离为0
	for (auto a = 1; a < 5; ++a)
		for (auto b = 0; b < 5; ++b)
			for (auto k = 0; k < 5 && b - k >= 0; ++k) //枚举这一种花色要达成的面子数
			{
				dp[a][b][0] = std::min(dp[a][b][0], dp[a - 1][b - k][0] + getDistanceSingle(handTile.colors[a-1].raw(), k, 0, a == 4));
				dp[a][b][1] = std::min(dp[a][b][1], dp[a - 1][b - k][0] + getDistanceSingle(handTile.colors[a-1].raw(), k, 1, a == 4));
				dp[a][b][1] = std::min(dp[a][b][1], dp[a - 1][b - k][1] + getDistanceSingle(handTile.colors[a-1].raw(), k, 0, a == 4));
				/*
				if (dp[a][b][0] > dp[a - 1][b - k][0] + getDistanceSingle(shape[a], k, 0, a == 4))
				{
					dp[a][b][0] = dp[a - 1][b - k][0] + getDistanceSingle(shape[a], k, 0, a == 4);
					pre[a][b][0] = (b - k);
				}
				if (dp[a][b][1]> dp[a - 1][b - k][0] + getDistanceSingle(shape[a], k, 1, a == 4))
				{
					dp[a][b][1] = dp[a - 1][b - k][0] + getDistanceSingle(shape[a], k, 1, a == 4);
					pre[a][b][1] = (b - k);
				}
				if (dp[a][b][1]> dp[a - 1][b - k][1] + getDistanceSingle(shape[a], k, 0, a == 4))
				{
					dp[a][b][1]= dp[a - 1][b - k][1] + getDistanceSingle(shape[a], k, 0, a == 4);
					pre[a][b][1] = (b - k)+5;
				}*/
			}
	/*
	int a = 4, b = 4, c = 1;
	std::cout << a << " " << b << " " << c << " " << dp[a][b][c] << std::endl;
	while (1)
	{
		int pa = a, pb = b, pc = c;
		a--;
		b = pre[pa][pb][pc] % 5;
		c = pre[pa][pb][pc] / 5;
		if (a >= 0 && b >= 0 && c >= 0)
			std::cout << a << " " << b << " " << c << " " << dp[a][b][c] << std::endl;
		else break;
	}
	*/
	/*
	for (int k = 0; k < 2; ++k)
	{
		for (int i = 0; i < 5; ++i)
		{
			for (int j = 0; j < 5; ++j)
				std::cout << dp[i][j][k] << " ";
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}*/
	return dp[4][4][1] / 2 - 1;
}

//计算14张手牌的向听数(0为一向听，-1为和牌)
int Algorithms::getDistanceChiitoi(const CompactSingles& handTile) {
	//计算七对子向听数
	auto toitsuCount = 0;
	auto singleCount = 0;
	for (auto i = 0; i <= 3; ++i)
		for (auto j = 1; j <= (i == 4 ? 7 : 9); ++j)
			if (handTile.colors[i].get(j)>= 2)
				toitsuCount++;
			else if (handTile.colors[i].get(j) == 1)
				singleCount++;
	return 6 - toitsuCount + std::max(0, 7 - toitsuCount - singleCount);
}
//计算14张手牌的国士无双向听数(0为一向听，-1为和牌)
int Algorithms::getDistanceKokushi(const CompactSingles& handTile){
	auto yaochuuTypeCount=0;
	auto yaochuuCount = 0;
	for (int i = 0; i < 3;++i) {
		for (int j = 1; j <= 9; j+=8){
			yaochuuCount += handTile.colors[i].get(j);
			yaochuuTypeCount += handTile.colors[i].get(j) ? 1:0;
		}
	}
	for (int j = 1; j <= 7; j++) {
		yaochuuCount += handTile.colors[3].get(j);
		yaochuuTypeCount += handTile.colors[3].get(j) ? 1 : 0;
	}
	return 12 + static_cast<int>(yaochuuCount <= yaochuuTypeCount) - yaochuuTypeCount;
}
//计算14张手牌的向听数(0为一向听，-1为和牌)
int Algorithms::getDistance(const CompactSingles& handTile){
	return std::min(std::min(getDistanceKokushi(handTile), getDistanceChiitoi(handTile)), getDistanceStandard(handTile));
}