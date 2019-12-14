#include"YakuChecker.h"

namespace {
	class S_Init : public YakuChecker {
	public:
		S_Init() noexcept {
			Set<Yaku::Chinitsu>(&YakuChecker::qingyise);
			Set<Yaku::ChinitsuF>(&YakuChecker::qingyiseF);
			Set<Yaku::Chuurenpoutou>(&YakuChecker::jiulianbaodeng);
			Set<Yaku::Chuurenkyuumenmachi>(&YakuChecker::chunzhengjiulianbaodeng);
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
	const std::vector<Group>& i_menqingMianzi
) noexcept
	: menchinCount(i_menqingMianzi.size() - 1), //非副露的面子数
	  menchin(menchinCount == 4), //是否门清
	  par(i_par), mentsu(i_menqingMianzi) {}

bool YakuChecker::tenhu() const noexcept { return par.state == 1 && par.type == 0 && par.selfWind == EAST; }

bool YakuChecker::chihu() const noexcept { return par.state == 1 && par.type == 0 && par.selfWind != EAST; }

bool YakuChecker::ziyise() const noexcept {
	for (auto& group : mentsu) {
		if (group.color != 'z')
			return false;
	}
	return true;
}

bool YakuChecker::sigangzi() const noexcept {
	auto gangziCount = 0;
	for (auto& group : mentsu) {
		if (group.type == GroupType::Kantsu)
			gangziCount++;
	}
	return gangziCount == 4;
}

bool YakuChecker::qingyise() const noexcept {
	return UnSyncCacheInvoke<Yaku::Chinitsu>(
		[this]() noexcept {
			auto qingyise = true;
			auto color = '0';
			for (auto& group : mentsu) {
				if (group.color == 'z') { qingyise = false; }
				else {
					if (color == '0')
						color = group.color;
					else if (color != group.color)
						qingyise = false;
				}
			}
			return qingyise && menchin;
		});
}

bool YakuChecker::qingyiseF() const noexcept {
	return UnSyncCacheInvoke<Yaku::ChinitsuF>(
		[this]() noexcept {
			auto qingyise = true;
			auto color = '0';
			for (auto& group : mentsu) {
				if (group.color == 'z') { qingyise = false; }
				else {
					if (color == '0')
						color = group.color;
					else if (color != group.color)
						qingyise = false;
				}
			}
			return qingyise && !menchin;
		});
}

bool YakuChecker::qinglaotou() const noexcept {
	for (auto& group : mentsu) {
		if (!group.islaotou())
			return false;
	}
	return true;
}

bool YakuChecker::lvyise() const noexcept {
	for (auto& group : mentsu) {
		if (!group.isgreen())
			return false;
	}
	return true;
}

//小四喜
bool YakuChecker::xiaosixi() const noexcept {

	auto fengCount = 0; //风面子/雀头计数
	auto fengQuetou = false;
	for (auto& group : mentsu) {
		if (group.color == 'z') {
			if (group.value <= 4) {
				fengCount++;
				if (group.type == GroupType::Toitsu)
					fengQuetou = true;
			}
		}
	}
	return fengCount == 4 && fengQuetou;
}

//大四喜
bool YakuChecker::dasixi() const noexcept {
	auto fengCount = 0; //风面子/雀头计数
	for (auto& group : mentsu) {
		if (group.color == 'z') {
			if (group.value <= 4) {
				fengCount++;
				if (group.type == GroupType::Toitsu)
					return false;
			}
		}
	}
	return fengCount == 4;
}

//大三元
bool YakuChecker::dasanyuan() const noexcept {
	auto sanyuanCount = 0;
	for (auto& group : mentsu) {
		if (group.color == 'z') {
			if (group.value > 4) {
				sanyuanCount++;
				if (group.type == GroupType::Toitsu)
					return false;
			}
		}
	}
	return sanyuanCount == 3;
}

bool YakuChecker::sianke() const noexcept {
	if (!menchin)return false;
	auto keziCount = 0;
	Single quetou;
	for (auto& group : mentsu) {
		if (group.type == GroupType::Koutsu)
			keziCount++;
		if (group.type == GroupType::Toitsu)
			quetou = Single(group.value, group.color, false);
	}
	return keziCount == 4 && par.type == 0 && !par.target.valueEqual(quetou) && !tenhu();
}

bool YakuChecker::siankedanqi() const noexcept {
	if (!menchin)return false;
	auto keziCount = 0;
	Single quetou;
	for (auto& group : mentsu) {
		if (group.type == GroupType::Koutsu)
			keziCount++;
		if (group.type == GroupType::Toitsu)
			quetou = Single(group.value, group.color, false);
	}
	return keziCount == 4 && (par.target.valueEqual(quetou) || tenhu());
}
bool YakuChecker::jiulianbaodengShape()const noexcept{
	if (!qingyise())return false;
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
bool YakuChecker::chunzhengjiulianbaodengShape()const noexcept {
	if (!qingyise())return false;
	int ct[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	for (auto& item : par.handTile)
		ct[item.value()]++;
	for (auto i = 1; i <= 9; ++i) {
		if ((i == 1 || i == 9) && ct[i] != 3) return false;
		if (i > 1 && i < 9 && ct[i] != 1) return false;
	}
	return true;
}
bool YakuChecker::jiulianbaodeng() const noexcept {
	return UnSyncCacheInvoke<Yaku::Chuurenpoutou>(
		[this]() noexcept {
			if (tenhu()|| chunzhengjiulianbaodengShape()) return false;
			return jiulianbaodengShape();
		});
}

bool YakuChecker::chunzhengjiulianbaodeng() const noexcept {
	return UnSyncCacheInvoke<Yaku::Chuurenkyuumenmachi>(
		[this]() noexcept {
			return chunzhengjiulianbaodengShape() || (jiulianbaodengShape() && tenhu());
		});
}
bool YakuChecker::hunyise() const noexcept{
	auto color = '0';
	bool hunyise = true;
	//副露牌
	for (auto& group : mentsu) {
		if (group.color != 'z') {
			if (color == '0')
				color = group.color;
			else if (color != group.color) { hunyise = false; }
		}
	}
	if (qingyise())hunyise = false;
	return hunyise && menchin;
}
bool YakuChecker::hunyiseF() const noexcept {
	auto color = '0';
	bool hunyiseF = true;
	//副露牌
	for (auto& group : mentsu) {
		if (group.color != 'z') {
			if (color == '0')
				color = group.color;
			else if (color != group.color) { hunyiseF = false; }
		}
	}
	if (qingyiseF())hunyiseF = false;
	return hunyiseF && !menchin;
}
//平和和符数计算
std::pair<bool, int> YakuChecker::pinghuAndFuCount() const noexcept{
	auto meetTheCounditions = false;
	auto fu = 20;
	auto pinghu = true;
	//首先判断雀头是否为役牌
	if (mentsu[0].color == 'z') {
		if (mentsu[0].value >= 5) {
			pinghu = false;
			fu += 2;
		}
		if (mentsu[0].value == par.prevailingWind + 1) {
			fu += 2;
			pinghu = false;
		}
		if (mentsu[0].value == par.selfWind + 1) {
			fu += 2;
			pinghu = false;
		}
	}
	//判断是否有刻子
	for (auto i = 1; i <= 4; ++i) {
		if (mentsu[i].type != GroupType::Shuntsu) {
			pinghu = false;
			if (mentsu[i].type == GroupType::Koutsu)
				fu += 2 * (mentsu[i].isyaojiu() ? 2 : 1) * (i <= menchinCount ? 2 : 1);
			if (mentsu[i].type == GroupType::Kantsu)
				fu += 8 * (mentsu[i].isyaojiu() ? 2 : 1) * (i <= menchinCount ? 2 : 1);
		}
	}
	//和牌方式是否为两面
	auto liangmianOnly = true; //是否只能被看成两面
	auto liangmian = false; //是否可以被看成两面
	for (auto i = 1; i <= menchinCount; ++i) {
		if (mentsu[i].type == GroupType::Shuntsu) {
			if (par.target.valueEqual(Single(mentsu[i].value, mentsu[i].color, false)))
				liangmian = true;
			if (par.target.valueEqual(Single(mentsu[i].value + 2, mentsu[i].color, false)))
				liangmian = true;
		}
		if (mentsu[i].type == GroupType::Toitsu) {
			if (par.target.valueEqual(Single(mentsu[i].value, mentsu[i].color, false)))
				liangmianOnly = false;
		}
		if (mentsu[i].type == GroupType::Koutsu || mentsu[i].type == GroupType::Kantsu) {
			if (par.target.valueEqual(Single(mentsu[i].value, mentsu[i].color, false)))
				liangmianOnly = false;
		}
	}
	if (liangmian == false)
		pinghu = false;
	//为平和型
	if (pinghu) {
		//门清，附加平和一役
		if (menchin) {
			if (par.type == 1)fu += 10;
			meetTheCounditions = true;
			//assert(res.result.fu == 20);
		}
		else //非门清,30符固定
			fu = 30;
	}
	else {
		if (liangmianOnly == false)
			fu += 2;
		//门清荣和+10符,自摸+2符
		if (menchin) {
			if (par.type >= 1)
				fu += 10;
			else fu += 2;
		}
		else //副露自摸+2符
		{
			if (par.type == 0)
				fu += 2;
		}
	}
	if (fu % 10 != 0)fu = fu - fu % 10 + 10;
	return { meetTheCounditions ,fu };
}
TryToAgariResult YakuChecker::getResult() {
	auto result = AgariResult();
	if (mentsu.size() != 5)return TryToAgariResult(AgariFaildReason::ShapeWrong);
	result.tsumo = par.type == 0;
	//役满型 
	//天地和
	if (tenhu())
		addYaku<Yaku::Tenhou>(result);
	if (chihu())
		addYaku<Yaku::Chihou>(result);
	//字一色、四杠子
	if (ziyise())
		addYaku<Yaku::Tsuuiisou>(result);
	if (sigangzi())
		addYaku<Yaku::Suukantsu>(result);
	//清老头，绿一色，小四喜，大四喜，大三元
	if (qinglaotou())
		addYaku<Yaku::Chinroutou>(result);
	if (lvyise())
		addYaku<Yaku::Ryuuiisou>(result);
	if (xiaosixi())
		addYaku<Yaku::Shousuushi>(result);
	if (dasixi())
		addYaku<Yaku::Daisuushi>(result);
	if (dasanyuan())
		addYaku<Yaku::Daisangen>(result);
	if (sianke())
		addYaku<Yaku::Suuankou>(result);
	if (siankedanqi())
		addYaku<Yaku::Suuankoutanki>(result);
	//九莲宝灯，纯正九莲宝灯
	if (jiulianbaodeng())
		addYaku<Yaku::Chuurenpoutou>(result);
	if (chunzhengjiulianbaodeng())
		addYaku<Yaku::Chuurenkyuumenmachi>(result);
	//满足役满型
	if (result.han < 0) { return AgariResult(Algorithms::getScore(par.selfWind, result)); }
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
	auto pinghuResult = pinghuAndFuCount();
	if (pinghuResult.first){
		addYaku<Yaku::Pinhu>(result);
	}
	result.fu = pinghuResult.second;
	if (qingyise())
		addYaku<Yaku::Chinitsu>(result);
	else if (qingyiseF())
		addYaku<Yaku::ChinitsuF>(result);
	else if (hunyise())
		addYaku<Yaku::Honitsu>(result);
	else if (hunyiseF())
		addYaku<Yaku::HonitsuF>(result);
	//清一色，混一色，断幺九，混老头，纯全带幺九，混全带幺九的判断
	auto duanyao = true, hunlaotou = true, chunquan = true, hunquan = true;
	auto color = '0';
	//枚举面子
	for (auto& group : mentsu) {
		if (!group.isyaojiu()) {
			hunquan = false;
			chunquan = false;
		}
		else { chunquan = false; }
		if (group.type == GroupType::Shuntsu) {
			hunlaotou = false;
			if (group.value == 7 || group.value == 1)
				duanyao = false;
		}
		else //刻子、杠子或者雀头
		{
			if (group.isyaojiu())
				duanyao = false;
			else hunlaotou = false;
		}
	}
	if (duanyao) {
		result.han += 1;
		result.yaku.add<Yaku::Tanyao>();
	}
	if (hunlaotou) {
		result.han += 2;
		result.yaku.add<Yaku::Honroutou>();
	}
	if (chunquan) {
		if (menchin) {
			result.han += 3;
			result.yaku.add<Yaku::Junchantaiyaochuu>();
		}
		else {
			result.han += 2;
			result.yaku.add<Yaku::JunchantaiyaochuuF>();
		}
	}
	else if (hunquan) {
		if (menchin) {
			result.han += 2;
			result.yaku.add<Yaku::Honchantaiyaochuu>();
		}
		else {
			result.han += 1;
			result.yaku.add<Yaku::HonchantaiyaochuuF>();
		}
	}
	/*
	//三色同顺，三色同刻的判断
	int sansetongshun_bukkit[9];
	int sansetongke_bukkit[9];
	memset(sansetongshun_bukkit, 0, sizeof(sansetongshun_bukkit));
	memset(sansetongke_bukkit, 0, sizeof(sansetongke_bukkit));
	for (auto& group : mianzi)
	{
		if (group.type == GroupType::Kezi)
			sansetongke_bukkit[group.value]++;
		if (group.type == GroupType::Shunzi)
			sansetongshun_bukkit[group.value]++;
	}
	for (int i = 1; i <= 9; ++i)
	{
		int colorCount = 0;
		if (sansetongshun_bukkit[i] >= 3)
		{
			for (auto& group : mianzi)
			{
			}
			break;
		}
	}*/
	//立直,两立直和一发
	if (par.riichiJunme != -1) {
		//w立
		if (par.riichiJunme == -2) {
			result.han += 2;
			result.yaku.add<Yaku::doubleRiichi>();
		}
		else {
			result.han += 1;
			result.yaku.add<Yaku::Riichi>();
		}
		if (par.ippatsu == true) {
			result.han += 1;
			result.yaku.add<Yaku::Ippatsu>();
		}
	}
	//检查抢杠
	if (par.type == 2) {
		result.han += 1;
		result.yaku.add<Yaku::Chankan>();
	}
	//检查门清自摸
	if (par.type == 0 && menchinCount == 4) {
		result.han += 1;
		result.yaku.add<Yaku::Menzenchintsumo>();
	}
	//检查河底/海底
	if (par.state == 2) {
		if (par.type == 0)
			result.yaku.add<Yaku::Haiteiraoyue>();
		else if (par.type == 1)result.yaku.add<Yaku::Houteiraoyui>();
	}
	//std::cout << "*" << std::endl;
	if (result.han == 0) { return TryToAgariResult(AgariFaildReason::NoYaku); }
	result.han += result.dora + result.akadora + result.uradora;
	result = Algorithms::getScore(par.selfWind, result);
	/*输出调试信息-不同拆牌方式的牌型和役种
	for (auto& item : mianzi)
	{
		std::cout << item.getString() << " ";
	}
	std::cout << std::endl;
	for (auto& yk : result.Yaku)
	{
		std::cout << getYakuName((int)yk) << " ";
		std::cout << std::endl;
	}
	std::cout << std::endl;
	*/
	return TryToAgariResult(result);
}

TryToAgariResult CheckYakuForStandardType(const AgariParameters& par, const std::vector<Group>& mentsu) {
	return YakuChecker(par, mentsu).getResult();
}
