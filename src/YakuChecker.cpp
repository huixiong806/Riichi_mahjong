#include"YakuChecker.h"
#include<iostream>
namespace {
	class S_Init : public YakuChecker {
	public:
		S_Init() noexcept {
			Set<Yaku::Chinitsu>(&YakuChecker::chinitsu);
			Set<Yaku::Chuurenpoutou>(&YakuChecker::chuurenpoutou);
			Set<Yaku::Chuurenkyuumenmachi>(&YakuChecker::chuurenkyuumenmachi);
		}

	private:
		template <Yaku Y>
		static void Set(const InternalInvoke invoke) noexcept {
			static constexpr auto Idx = yakuToIndex(Y);
			yakuCheckFunc[Idx] = invoke;
		}
	} __UU_Init__;

	template <Yaku Y>
	void addYaku(AgariResult& result) noexcept {
		result.yaku.add<Y>();
		if constexpr (static_cast<int>(Y) >> 8 >= 0xf) { result.han -= 2; }
		else if constexpr (static_cast<int>(Y) >> 8 >= 0xd) { result.han -= 1; }
		else { result.han += static_cast<int>(Y) >> 8; }
	}
}

YakuChecker::YakuChecker(
	const AgariParameters& i_par,
	const std::vector<Group>& i_menchinGroup
) noexcept
	: menchinCount(i_menchinGroup.size()), //非副露的group数(雀头和面子),不包括暗杠
	par(i_par) {
		menchin = true;
		for (auto&& group : i_par.groupTile)
			if (!group.isAnkan())
				menchin = false;
		mentsu = i_menchinGroup;
		mentsu.insert(mentsu.end(), i_par.groupTile.begin(), i_par.groupTile.end());
	}
//判断天和
bool YakuChecker::tenhou() const noexcept { return par.state == BonusYakuState::FirstTurn && 
												   par.type == AgariWays::Tsumo && 
												   par.selfWind == EAST; }
//判断地和
bool YakuChecker::chihou() const noexcept { return par.state == BonusYakuState::FirstTurn && 
												   par.type == AgariWays::Tsumo &&
												   par.selfWind != EAST; }
//判断字一色
bool YakuChecker::tsuuiisou() const noexcept {
	for (auto& group : mentsu) {
		if (group.color != 'z')
			return false;
	}
	return true;
}
//判断四杠子
bool YakuChecker::suukantsu() const noexcept {
	auto kantsuCount = 0;
	for (auto& group : mentsu) {
		if (group.type == GroupType::Kantsu)
			kantsuCount++;
	}
	return kantsuCount == 4;
}
//判断清一色
bool YakuChecker::chinitsu() const noexcept {
	return UnSyncCacheInvoke<Yaku::Chinitsu>(
		[this]() noexcept {
			auto chinitsu = true;
			auto color = '0';
			for (auto& group : mentsu) {
				if (group.color == 'z') { chinitsu = false; }
				else {
					if (color == '0')
						color = group.color;
					else if (color != group.color)
						chinitsu = false;
				}
			}
			return chinitsu && menchin;
		});
}
//判断清一色（副露版）
bool YakuChecker::chinitsuF() const noexcept {
	auto chinitsu = true;
	auto color = '0';
	for (auto& group : mentsu) {
		if (group.color == 'z') { chinitsu = false; }
		else {
			if (color == '0')
				color = group.color;
			else if (color != group.color)
				chinitsu = false;
		}
	}
	return chinitsu && !menchin;
}
//判断清老头
bool YakuChecker::chinroutou() const noexcept {
	for (auto& group : mentsu) {
		if (!group.isRoutouAndNotZ())
			return false;
	}
	return true;
}
//判断绿一色
bool YakuChecker::ryuuiisou() const noexcept {
	for (auto& group : mentsu) {
		if (!group.isGreen())
			return false;
	}
	return true;
}

//判断小四喜
bool YakuChecker::shousuushi() const noexcept {

	auto fuuCount = 0; //风面子/雀头计数
	auto fuuJyantou = false;
	for (auto& group : mentsu) {
		if (group.color == 'z') {
			if (group.value <= 4) {
				fuuCount++;
				if (group.type == GroupType::Toitsu)
					fuuJyantou = true;
			}
		}
	}
	return fuuCount == 4 && fuuJyantou;
}

//大四喜
bool YakuChecker::daisuushi() const noexcept {
	auto fuuCount = 0; //风面子/雀头计数
	for (auto& group : mentsu) {
		if (group.color == 'z') {
			if (group.value <= 4) {
				fuuCount++;
				if (group.type == GroupType::Toitsu)
					return false;
			}
		}
	}
	return fuuCount == 4;
}

//大三元
bool YakuChecker::daisangen() const noexcept {
	auto sangenCount = 0;
	for (auto& group : mentsu) {
		if (group.color == 'z') {
			if (group.value > 4) {
				sangenCount++;
				if (group.type == GroupType::Toitsu)
					return false;
			}
		}
	}
	return sangenCount == 3;
}
//判断四暗刻
bool YakuChecker::suuankou() const noexcept {
	if (!menchin)return false;
	auto koutsuCount = 0;
	Single jyantou;
	for (auto& group : mentsu) {
		if (group.type == GroupType::Koutsu || group.isAnkan())
			koutsuCount++;
		if (group.type == GroupType::Toitsu)
			jyantou = Single(group.value, group.color, false);
	}
	return koutsuCount == 4 && par.type == AgariWays::Tsumo && !par.target.valueEqual(jyantou) && !tenhou();
}
//判断四暗刻单骑
bool YakuChecker::suuankoutanki() const noexcept {
	if (!menchin)return false;
	auto koutsuCount = 0;
	Single jyantou;
	for (auto& group : mentsu) {
		if (group.type == GroupType::Koutsu || group.isAnkan())
			koutsuCount++;
		if (group.type == GroupType::Toitsu)
			jyantou = Single(group.value, group.color, false);
	}
	return koutsuCount == 4 && (par.target.valueEqual(jyantou) || tenhou());
}
//判断是否为九莲宝灯的牌型
bool YakuChecker::chuurenpoutouShape()const noexcept{
	if (!chinitsu())return false;
	int ct[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	for (auto& item : par.handTile)
		ct[item.value()]++;
	ct[par.target.value()]++;
	auto subflag = false;
	for (auto i = 1; i <= 9; ++i) {
		if (!subflag && (ct[i] > 0 && ct[i] % 2 == 0)) {
			ct[i]--;
			subflag = true;
		}
	}
	if (!subflag) return false;
	for (auto i = 1; i <= 9; ++i) {
		if ((i == 1 || i == 9) && ct[i] != 3) return false;
		if (i > 1 && i < 9 && ct[i] != 1) return false;
	}
	return true;
}
//判断是否为纯九莲的牌型
bool YakuChecker::chuurenkyuumenmachiShape()const noexcept {
	if (!chinitsu())return false;
	int ct[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	for (auto& item : par.handTile)
		ct[item.value()]++;
	for (auto i = 1; i <= 9; ++i) {
		if ((i == 1 || i == 9) && ct[i] != 3) return false;
		if (i > 1 && i < 9 && ct[i] != 1) return false;
	}
	return true;
}
//判断九莲宝灯
bool YakuChecker::chuurenpoutou() const noexcept {
	return UnSyncCacheInvoke<Yaku::Chuurenpoutou>(
		[this]() noexcept {
			if (tenhou()|| chuurenkyuumenmachiShape()) return false;
			return chuurenpoutouShape();
		});
}
//判断纯九莲
bool YakuChecker::chuurenkyuumenmachi() const noexcept {
	return UnSyncCacheInvoke<Yaku::Chuurenkyuumenmachi>(
		[this]() noexcept {
			return chuurenkyuumenmachiShape() || (chuurenpoutouShape() && tenhou());
		});
}
//判断混一色
bool YakuChecker::honitsu() const noexcept{
	auto color = '0';
	bool honitsu = true;
	for (auto& group : mentsu) {
		if (group.color != 'z') {
			if (color == '0')
				color = group.color;
			else if (color != group.color) { honitsu = false; }
		}
	}
	if (chinitsu())honitsu = false;
	return honitsu && menchin;
}
//判断混一色(副露版)
bool YakuChecker::honitsuF() const noexcept {
	auto color = '0';
	bool honitsuF = true;
	for (auto& group : mentsu) {
		if (group.color != 'z') {
			if (color == '0')
				color = group.color;
			else if (color != group.color) { honitsuF = false; }
		}
	}
	if (chinitsuF())honitsuF = false;
	return honitsuF && !menchin;
}
//判断断幺九
bool YakuChecker::tanyao() const noexcept
{
	for (auto& group : mentsu)
		if (group.is19Z())
			return false;
	return true;
}
bool YakuChecker::honroutou() const noexcept
{
	//枚举面子
	for (auto& group : mentsu)
		if (!group.isRoutou())
			return false;
	return true;
}
bool YakuChecker::honchantaiyaochuu() const noexcept
{
	//枚举面子
	auto honchan=true;
	for (auto& group : mentsu)
		if (!group.is19Z())
			honchan=false;
	return honchan && menchin;
}
bool YakuChecker::honchantaiyaochuuF() const noexcept
{
	//枚举面子
	auto honchan = true;
	for (auto& group : mentsu)
		if (!group.is19Z())
			honchan = false;
	return honchan && !menchin;
}
bool YakuChecker::junchantaiyaochuu() const noexcept
{
	auto junchan = true;
	for (auto& group : mentsu)
		if (!group.is19AndNotZ())
			junchan = false;
	return junchan && menchin;
}
bool YakuChecker::junchantaiyaochuuF() const noexcept
{
	auto junchan = true;
	for (auto& group : mentsu)
		if (!group.is19AndNotZ())
			junchan = false;
	return junchan && !menchin;
}
bool YakuChecker::sanshokudoujun() const noexcept
{
	bool sanshoku = false;
	bool pool[7][4] = { false };
	for (auto& group : mentsu)
		if (group.type==GroupType::Shuntsu)
			pool[group.value - 1][group.color == 'm' ? 0 : (group.color == 'p' ? 1 : (group.color == 's' ? 2 : 3))]=true;
	for (auto i = 0; i < 7; ++i)
		if (pool[i][0] && pool[i][1] && pool[i][2])
			sanshoku = true;
	return sanshoku&&menchin;
}
bool YakuChecker::sanshokudoujunF() const noexcept
{
	bool sanshoku = false;
	bool pool[7][4] = { false };
	for (auto& group : mentsu)
		if (group.type == GroupType::Shuntsu)
			pool[group.value - 1][group.color == 'm' ? 0 : (group.color == 'p' ? 1 : (group.color == 's' ? 2 : 3))] = true;
	for (auto i = 0; i < 7; ++i)
		if (pool[i][0] && pool[i][1] && pool[i][2])
			sanshoku = true;
	return sanshoku && !menchin;
}
//三色同刻
bool YakuChecker::sanshokudoukou() const noexcept
{
	bool sanshoku = false;
	bool pool[9][4] = { false };
	for (auto& group : mentsu)
		if (group.type == GroupType::Koutsu|| group.type == GroupType::Kantsu)
			pool[group.value - 1][group.color == 'm' ? 0 : (group.color == 'p' ? 1 : (group.color == 's' ? 2 : 3))] = true;
	for (auto i = 0; i < 9; ++i)
		if (pool[i][0] && pool[i][1] && pool[i][2])
			sanshoku = true;
	return sanshoku;
}
//一气通贯
bool YakuChecker::ikkitsuukan() const noexcept
{
	bool ittsuu = false;
	bool pool[7][4] = { false };
	for (auto& group : mentsu)
		if (group.type == GroupType::Shuntsu)
			pool[group.value - 1][group.color == 'm' ? 0 : (group.color == 'p' ? 1 : (group.color == 's' ? 2 : 3))] = true;
	for (auto i = 0; i < 3; ++i)
		if (pool[0][i] && pool[3][i] && pool[6][i])
			ittsuu = true;
	return ittsuu && menchin;
}
//一气通贯副露版
bool YakuChecker::ikkitsuukanF() const noexcept
{
	auto ittsuu = false;
	bool pool[7][4] = { false };
	for (auto& group : mentsu)
		if (group.type == GroupType::Shuntsu)
			pool[group.value - 1][group.color == 'm' ? 0 : (group.color == 'p' ? 1 : (group.color == 's' ? 2 : 3))] = true;
	for (auto i = 0; i < 3; ++i)
		if (pool[0][i] && pool[3][i] && pool[6][i])
			ittsuu = true;
	return ittsuu && !menchin;
}
//小三元
bool YakuChecker::shousangen() const noexcept
{
	auto sangenCount = 0;
	auto jyantou = false;
	for (auto& group : mentsu) {
		if (group.color == 'z') {
			if (group.value > 4) {
				sangenCount++;
				if (group.type == GroupType::Toitsu)
					jyantou = true;
			}
		}
	}
	return sangenCount == 3 && jyantou;
}
bool YakuChecker::sankantsu() const noexcept
{
	auto gangziCount = 0;
	for (auto& group : mentsu) {
		if (group.type == GroupType::Kantsu)
			gangziCount++;
	}
	return gangziCount == 3;
}
bool YakuChecker::toitoihou() const noexcept
{
	auto toitoiCount = 0;
	for (auto& group : mentsu)
		if (group.type == GroupType::Koutsu|| group.type == GroupType::Kantsu)
			toitoiCount++;
	return toitoiCount==4;
}
bool YakuChecker::iipeikou() const noexcept
{
	auto peikouCount = 0;
	int pool[7][4] = { 0 };
	for (auto& group : mentsu)
		if (group.type == GroupType::Shuntsu)
			pool[group.value - 1][group.color == 'm' ? 0 : (group.color == 'p' ? 1 : (group.color == 's' ? 2 : 3))]++;
	for (auto i = 0; i < 7; ++i)
		for (auto j = 0; j < 3; ++j)
			if (pool[i][j]==2|| pool[i][j] == 3)
				peikouCount++;
			else if(pool[i][j]==4)
				peikouCount+=2;
	return peikouCount==1 && menchin;
}
bool YakuChecker::ryanpeikou() const noexcept
{
	auto peikouCount = 0;
	int pool[7][4] = { 0 };
	for (auto& group : mentsu)
		if (group.type == GroupType::Shuntsu)
			pool[group.value - 1][group.color == 'm' ? 0 : (group.color == 'p' ? 1 : (group.color == 's' ? 2 : 3))]++;
	for (auto i = 0; i < 7; ++i)
		for (auto j = 0; j < 3; ++j)
			if (pool[i][j] == 2)
				peikouCount++;
			else if (pool[i][j] == 4)
				peikouCount += 2;
	return peikouCount == 2 && menchin;
}
//自风牌
bool YakuChecker::yakuhaiJikaze() const noexcept
{
	for (auto& group : mentsu)
		if (group.type != GroupType::Toitsu&&group.color == 'z'&&group.value - 1 == par.selfWind)
			return true;
	return false;
}
//场风牌
bool YakuChecker::yakuhaiBakaze() const noexcept
{
	for (auto& group : mentsu)
		if (group.type != GroupType::Toitsu&&group.color == 'z'&&group.value - 1 == par.prevailingWind)
			return true;
	return false;
}
//白
bool YakuChecker::yakuhaiHaku() const noexcept
{
	for (auto& group : mentsu)
		if (group.type != GroupType::Toitsu&&group.color == 'z'&&group.value==5)
			return true;
	return false;
}
//发
bool YakuChecker::yakuhaiHatsu() const noexcept
{
	for (auto& group : mentsu)
		if (group.type !=GroupType::Toitsu&&group.color == 'z'&&group.value == 6)
			return true;
	return false;
}
//中
bool YakuChecker::yakuhaiChun() const noexcept
{
	for (auto& group : mentsu)
		if (group.type != GroupType::Toitsu&&group.color == 'z'&&group.value == 7)
			return true;
	return false;
}
//立直
bool YakuChecker::riichi() const noexcept
{
	
	return par.riichiJunme != -1 && par.riichiJunme != -2;
}
//两立直
bool YakuChecker::doubleRiichi() const noexcept
{
	return par.riichiJunme == -2;
}
//一发
bool YakuChecker::ippatsu() const noexcept
{
	return par.ippatsu;
}
//抢杠
bool YakuChecker::chankan() const noexcept
{
	return par.type==AgariWays::Chankan;
}
//岭上开花
bool YakuChecker::rinshan() const noexcept
{
	return par.rinshan&&par.type == AgariWays::Tsumo;
}
//河底捞鱼
bool YakuChecker::houtei() const noexcept
{
	return par.state == BonusYakuState::LastTurn&&par.type == AgariWays::Ron;
}
//海底
bool YakuChecker::haitei() const noexcept
{
	return par.state == BonusYakuState::LastTurn&&par.type == AgariWays::Tsumo;
}
//门清自摸
bool YakuChecker::menchintsumo() const noexcept
{
	return par.type == AgariWays::Tsumo && menchin;
}
bool YakuChecker::sanankou() const noexcept
{
	auto fuurou = 0;
	for (auto&& group : par.groupTile)
		if (!group.isAnkan())
			fuurou++;
	if (fuurou >= 2)return false;
	auto ankouCount = 0;
	for (auto i=0;i<mentsu.size();++i)
		if ((mentsu[i].type == GroupType::Koutsu&&i<menchinCount)|| mentsu[i].isAnkan())
			ankouCount++;
	if (fuurou == 0){
		if (ankouCount == 4)return true;
		else if (ankouCount == 3) {
			//自摸则三暗刻
			if (par.type == AgariWays::Tsumo)
				return true;
			//不自摸就需要荣牌不成暗刻的一部分
			auto id = 0;
			//找到唯一的顺子
			for (auto i = 0; i < mentsu.size(); ++i)
				if (mentsu[i].type == GroupType::Shuntsu) {
					id = i;
					break;
				}
			const Single tar[4] = {
				Single(mentsu[id].value,mentsu[id].color,false),
				Single(mentsu[id].value + 1, mentsu[id].color, false),
				Single(mentsu[id].value + 2,mentsu[id].color,false),
				Single(mentsu[0].value, mentsu[0].color, false)
			};
			//荣牌可以不是暗刻的一部分
			for (auto i = 0; i < 4; ++i){
				if (tar[i].valueEqual(par.target))
					return true;
			}
			return false;
		}else return false;
	}
	else/*fuurou==1*/{
		if (ankouCount == 3)return par.type==AgariWays::Tsumo||par.target.valueEqual(Single(mentsu[0].value,mentsu[0].color,false));
		else return false;
	}
}
//平和和符数计算
std::pair<bool, int> YakuChecker::pinghuAndFuCount() const noexcept{
	auto meetTheCounditions = false;
	auto fu = 20;
	auto pinhu = true;
	//首先判断雀头是否为役牌
	if (mentsu[0].color == 'z') {
		if (mentsu[0].value >= 5) {
			pinhu = false;
			fu += 2;
		}
		if (mentsu[0].value == par.prevailingWind + 1) {
			fu += 2;
			pinhu = false;
		}
		if (mentsu[0].value == par.selfWind + 1) {
			fu += 2;
			pinhu = false;
		}
	}
	//判断是否有刻子
	for (auto i = 1; i <= 4; ++i) {
		if (mentsu[i].type != GroupType::Shuntsu) {
			pinhu = false;
			if (mentsu[i].type == GroupType::Koutsu)
				fu += 2 * (mentsu[i].is19Z() ? 2 : 1) * (i < menchinCount ? 2 : 1);
			if (mentsu[i].type == GroupType::Kantsu)
				//暗杠的下标>=menchinCount
				fu += 8 * (mentsu[i].is19Z() ? 2 : 1) * (i < menchinCount ? 2 : 1)* (mentsu[i].isAnkan() ? 4 : 1);
		}
	}
	//和牌方式是否为两面
	auto ryanmenOnly = true; //是否只能被看成两面
	auto ryanmen = false; //是否可以被看成两面
	for (auto i = 0; i < menchinCount; ++i) {
		if (mentsu[i].type == GroupType::Shuntsu) {
			if (par.target.valueEqual(Single(mentsu[i].value, mentsu[i].color, false)))
				ryanmen = true;
			if (par.target.valueEqual(Single(mentsu[i].value + 2, mentsu[i].color, false)))
				ryanmen = true;
		}
		if (mentsu[i].type == GroupType::Toitsu) {
			if (par.target.valueEqual(Single(mentsu[i].value, mentsu[i].color, false)))
				ryanmenOnly = false;
		}
		if (mentsu[i].type == GroupType::Koutsu || mentsu[i].type == GroupType::Kantsu) {
			if (par.target.valueEqual(Single(mentsu[i].value, mentsu[i].color, false)))
				ryanmenOnly = false;
		}
	}
	if (ryanmen == false)
		pinhu = false;
	//为平和型
	if (pinhu) {
		//门清，附加平和一役
		if (menchin) {
			if (par.type != AgariWays::Tsumo)fu += 10;
			meetTheCounditions = true;
			//assert(res.result.fu == 20);
		}
		else //非门清,30符固定
			fu = 30;
	}
	else {
		if (ryanmenOnly == false)
			fu += 2;
		//门清荣和+10符,自摸+2符
		if (menchin) {
			if (par.type != AgariWays::Tsumo)
				fu += 10;
			else fu += 2;
		}
		else //副露自摸+2符
		{
			if (par.type == AgariWays::Tsumo)
				fu += 2;
		}
	}
	if (fu % 10 != 0)fu = fu - fu % 10 + 10;
	return { meetTheCounditions ,fu };
}
TryToAgariResult YakuChecker::getResult() {
	auto result = AgariResult();
	if (mentsu.size() != 5)return TryToAgariResult(AgariFaildReason::ShapeWrong);
	result.tsumo = par.type == AgariWays::Tsumo;
	//役满型 
	//天地和
	if (tenhou())addYaku<Yaku::Tenhou>(result);
	if (chihou())addYaku<Yaku::Chihou>(result);
	//字一色、四杠子
	if (tsuuiisou())addYaku<Yaku::Tsuuiisou>(result);
	if (suukantsu())addYaku<Yaku::Suukantsu>(result);
	//清老头，绿一色，小四喜，大四喜，大三元
	if (chinroutou())addYaku<Yaku::Chinroutou>(result);
	if (ryuuiisou())addYaku<Yaku::Ryuuiisou>(result);
	if (shousuushi())addYaku<Yaku::Shousuushi>(result);
	if (daisuushi())addYaku<Yaku::Daisuushi>(result);
	if (daisangen())addYaku<Yaku::Daisangen>(result);
	if (suuankou())addYaku<Yaku::Suuankou>(result);
	if (suuankoutanki())addYaku<Yaku::Suuankoutanki>(result);
	//九莲宝灯，纯正九莲宝灯
	if (chuurenpoutou())addYaku<Yaku::Chuurenpoutou>(result);
	if (chuurenkyuumenmachi())addYaku<Yaku::Chuurenkyuumenmachi>(result);
	//满足役满型
	if (result.han < 0) {
		return AgariResult(Algorithms::getScore(par.selfWind, result));
	}
	auto myHandTile = par.handTile;
	myHandTile.push_back(par.target);
	//检查dora，akadora和uradora,并在最后有役的情况下增加翻数。
	for (auto& doraneko : par.dora)
		for (auto& item : myHandTile)
			if (doraneko.valueEqual(item))
				result.dora++;
	for (auto& doraneko : par.ura)
		for (auto& item : myHandTile)
			if (doraneko.valueEqual(item))
				result.uradora++;
	for (auto& item : myHandTile)
		if (item.isAkadora())
			result.akadora++;
	//平和
	auto pinhuResult = pinghuAndFuCount();
	if (pinhuResult.first){
		addYaku<Yaku::Pinhu>(result);
	}
	result.fu = pinhuResult.second;
	//清一色，混一色
	if (chinitsu())addYaku<Yaku::Chinitsu>(result);
	else if (chinitsuF())addYaku<Yaku::ChinitsuF>(result);
	else if (honitsu())addYaku<Yaku::Honitsu>(result);
	else if (honitsuF())addYaku<Yaku::HonitsuF>(result);
	//断幺九，混老头，纯全带幺九，混全带幺九
	if (tanyao())addYaku<Yaku::Tanyao>(result);
	if (honroutou())addYaku<Yaku::Honroutou>(result);
	if (junchantaiyaochuu())addYaku<Yaku::Junchantaiyaochuu>(result);
	else if(junchantaiyaochuuF())addYaku<Yaku::JunchantaiyaochuuF>(result);
	if (honchantaiyaochuu())addYaku<Yaku::Honchantaiyaochuu>(result);
	else if (honchantaiyaochuuF())addYaku<Yaku::HonchantaiyaochuuF>(result);
	//三色同顺，三色同刻
	if (sanshokudoujun())addYaku<Yaku::Sanshokudoujun>(result);
	else if (sanshokudoujunF())addYaku<Yaku::SanshokudoujunF>(result);
	if (sanshokudoukou())addYaku<Yaku::Sanshokudoukou>(result);
	//一气通贯
	if (ikkitsuukan())addYaku<Yaku::Ikkitsuukan>(result);
	else if (ikkitsuukanF())addYaku<Yaku::IkkitsuukanF>(result);
	//小三元
	if (shousangen())addYaku<Yaku::Shousangen>(result);
	//三杠子,三暗刻
	if (sankantsu())addYaku<Yaku::Sankantsu>(result);
	if (sanankou())addYaku<Yaku::Sanankou>(result);
	//对对和
	if (toitoihou())addYaku<Yaku::Toitoihou>(result);
	//一杯口，二杯口
	if (iipeikou())addYaku<Yaku::Iipeikou>(result);
	else if (ryanpeikou())addYaku<Yaku::Ryanpeikou>(result);
	//自风场风白发中
	if (yakuhaiJikaze())addYaku<Yaku::YakuhaiJikaze>(result);
	if (yakuhaiBakaze())addYaku<Yaku::YakuhaiBakaze>(result);
	if (yakuhaiHaku())addYaku<Yaku::YakuhaiHaku>(result);
	if (yakuhaiHatsu())addYaku<Yaku::YakuhaiHatsu>(result);
	if (yakuhaiChun())addYaku<Yaku::YakuhaiChun>(result);
	//立直,两立直和一发
	if(riichi())addYaku<Yaku::Riichi>(result);
	else if(doubleRiichi())addYaku<Yaku::doubleRiichi>(result);
	if (ippatsu())addYaku<Yaku::Ippatsu>(result);
	if (menchintsumo())addYaku<Yaku::Menzenchintsumo>(result);
	//抢杠，岭上，海底，河底
	if (chankan())addYaku<Yaku::Ippatsu>(result);
	else if (menchintsumo())addYaku<Yaku::Menzenchintsumo>(result);
	else if (haitei())addYaku<Yaku::Haiteiraoyue>(result);
	else if (houtei())addYaku<Yaku::Houteiraoyui>(result);
	//std::cout << "*" << std::endl;
	if (result.han == 0) { return TryToAgariResult(AgariFaildReason::NoYaku); }
	result.han += result.dora + result.akadora + result.uradora;
	result = Algorithms::getScore(par.selfWind, result);
	/*//输出调试信息-不同拆牌方式的牌型和役种
	for (auto& item : mentsu)
	{
		std::cout << item.getString() << " ";
	}
	std::cout << std::endl;
	for (auto&& yk : result.yaku)
	{
		std::cout << getYakuName(yk) << " ";
		std::cout << std::endl;
	}
	std::cout << std::endl;*/
	return TryToAgariResult(result);
}
//menchinGroup 下标为0的元素描述雀头，下标为1~3的元素描述门前清的面子(可选)
TryToAgariResult CheckYakuForStandardType(const AgariParameters& par, const std::vector<Group>& menchinGroup) {
	return YakuChecker(par, menchinGroup).getResult();
}
