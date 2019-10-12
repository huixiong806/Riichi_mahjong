#include"YakuChecker.h"

namespace {
	class S_Init : public YakuChecker {
	public:
		S_Init() noexcept {
			Set<Yaku::Qingyise>(&YakuChecker::qingyise);
			Set<Yaku::QingyiseF>(&YakuChecker::qingyiseF);
			Set<Yaku::Jiulianbaodeng>(&YakuChecker::jiulianbaodeng);
			Set<Yaku::Chunzhengjiulianbaodeng>(&YakuChecker::chunzhengjiulianbaodeng);
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
		if constexpr (static_cast<int>(Y) >> 8 >= 0xf) { result.fan -= 2; }
		else if constexpr (static_cast<int>(Y) >> 8 >= 0xd) { result.fan -= 1; }
		else { result.fan += static_cast<int>(Y) >> 8; }
	}
}

YakuChecker::YakuChecker(
	const AgariParameters& i_par,
	const std::vector<Group>& i_menqingMianzi
) noexcept
	: menqingCount(i_menqingMianzi.size() - 1), //非副露的面子数
	  menqing(menqingCount == 4), //是否门清
	  par(i_par), mianzi(i_menqingMianzi) {}

bool YakuChecker::tianhu() const noexcept { return par.state == 1 && par.type == 0 && par.selfWind == EAST; }

bool YakuChecker::dihu() const noexcept { return par.state == 1 && par.type == 0 && par.selfWind != EAST; }

bool YakuChecker::ziyise() const noexcept {
	for (auto& group : mianzi) {
		if (group.color != 'z')
			return false;
	}
	return true;
}

bool YakuChecker::sigangzi() const noexcept {
	auto gangziCount = 0;
	for (auto& group : mianzi) {
		if (group.type == GroupType::Gang)
			gangziCount++;
	}
	return gangziCount == 4;
}

bool YakuChecker::qingyise() const noexcept {
	return UnSyncCacheInvoke<Yaku::Qingyise>(
		[this]() noexcept {
			auto qingyise = true;
			auto color = '0';
			for (auto& group : mianzi) {
				if (group.color == 'z') { qingyise = false; }
				else {
					if (color == '0')
						color = group.color;
					else if (color != group.color)
						qingyise = false;
				}
			}
			return qingyise && menqing;
		});
}

bool YakuChecker::qingyiseF() const noexcept {
	return UnSyncCacheInvoke<Yaku::QingyiseF>(
		[this]() noexcept {
			auto qingyise = true;
			auto color = '0';
			for (auto& group : mianzi) {
				if (group.color == 'z') { qingyise = false; }
				else {
					if (color == '0')
						color = group.color;
					else if (color != group.color)
						qingyise = false;
				}
			}
			return qingyise && !menqing;
		});
}

bool YakuChecker::qinglaotou() const noexcept {
	for (auto& group : mianzi) {
		if (!group.islaotou())
			return false;
	}
	return true;
}

bool YakuChecker::lvyise() const noexcept {
	for (auto& group : mianzi) {
		if (!group.isgreen())
			return false;
	}
	return true;
}

//小四喜
bool YakuChecker::xiaosixi() const noexcept {

	auto fengCount = 0; //风面子/雀头计数
	auto fengQuetou = false;
	for (auto& group : mianzi) {
		if (group.color == 'z') {
			if (group.value <= 4) {
				fengCount++;
				if (group.type == GroupType::Quetou)
					fengQuetou = true;
			}
		}
	}
	return fengCount == 4 && fengQuetou;
}

//大四喜
bool YakuChecker::dasixi() const noexcept {
	auto fengCount = 0; //风面子/雀头计数
	for (auto& group : mianzi) {
		if (group.color == 'z') {
			if (group.value <= 4) {
				fengCount++;
				if (group.type == GroupType::Quetou)
					return false;
			}
		}
	}
	return fengCount == 4;
}

//大三元
bool YakuChecker::dasanyuan() const noexcept {
	auto sanyuanCount = 0;
	for (auto& group : mianzi) {
		if (group.color == 'z') {
			if (group.value > 4) {
				sanyuanCount++;
				if (group.type == GroupType::Quetou)
					return false;
			}
		}
	}
	return sanyuanCount == 3;
}

bool YakuChecker::sianke() const noexcept {
	if (!menqing)return false;
	auto keziCount = 0;
	Single quetou;
	for (auto& group : mianzi) {
		if (group.type == GroupType::Kezi)
			keziCount++;
		if (group.type == GroupType::Quetou)
			quetou = Single(group.value, group.color, false);
	}
	return par.type == 0 && !par.target.valueEqual(quetou) && !tianhu();
}

bool YakuChecker::siankedanqi() const noexcept {
	if (!menqing)return false;
	auto keziCount = 0;
	Single quetou;
	for (auto& group : mianzi) {
		if (group.type == GroupType::Kezi)
			keziCount++;
		if (group.type == GroupType::Quetou)
			quetou = Single(group.value, group.color, false);
	}
	return keziCount == 4 && (par.target.valueEqual(quetou) || tianhu());
}

bool YakuChecker::jiulianbaodeng() const noexcept {
	return UnSyncCacheInvoke<Yaku::Jiulianbaodeng>(
		[this]() noexcept {
			if (qingyise() || chunzhengjiulianbaodeng() || tianhu()) return false;
			int ct[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
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
		});
}

bool YakuChecker::chunzhengjiulianbaodeng() const noexcept {
	return UnSyncCacheInvoke<Yaku::Chunzhengjiulianbaodeng>(
		[this]() noexcept {
			if (qingyise()) return false;
			if (tianhu()) return true;
			int ct[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			for (auto& item : par.handTile)
				ct[item.value()]++;
			for (auto i = 1; i <= 9; ++i) {
				if ((i == 1 || i == 9) && ct[i] != 3) return false;
				if (i > 1 && i < 9 && ct[i] != 1) return false;
			}
			return true;
		});
}

TryToAgariResult YakuChecker::getResult() {
	auto result = AgariResult();
	if (mianzi.size() != 5)return TryToAgariResult(AgariFaildReason::ShapeWrong);
	result.zimo = par.type == 0;
	//役满型 
	//天地和
	if (tianhu())
		addYaku<Yaku::Tianhu>(result);
	if (dihu())
		addYaku<Yaku::Dihu>(result);
	//字一色、四杠子
	if (ziyise())
		addYaku<Yaku::Ziyise>(result);
	if (sigangzi())
		addYaku<Yaku::Sigangzi>(result);
	//清老头，绿一色，小四喜，大四喜，大三元
	if (qinglaotou())
		addYaku<Yaku::Qinglaotou>(result);
	if (lvyise())
		addYaku<Yaku::Lvyise>(result);
	if (xiaosixi())
		addYaku<Yaku::Xiaosixi>(result);
	if (dasixi())
		addYaku<Yaku::Dasixi>(result);
	if (dasanyuan())
		addYaku<Yaku::Dasanyuan>(result);
	if (sianke())
		addYaku<Yaku::Sianke>(result);
	if (siankedanqi())
		addYaku<Yaku::Siankedanqi>(result);
	//九莲宝灯，纯正九莲宝灯
	if (jiulianbaodeng())
		addYaku<Yaku::Jiulianbaodeng>(result);
	if (chunzhengjiulianbaodeng())
		addYaku<Yaku::Chunzhengjiulianbaodeng>(result);
	//满足役满型
	if (result.fan < 0) { return AgariResult(Algorithms::getScore(par.selfWind, result)); }
	auto myHandTile = par.handTile;
	myHandTile.push_back(par.target);
	//检查dora，akadora和uradora
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
	result.fan += result.dora + result.akadora + result.uradora;
	//平和和符数计算
	result.fu = 20;
	auto pinghu = true;
	//首先判断雀头是否为役牌
	if (mianzi[0].color == 'z') {
		if (mianzi[0].value >= 5) {
			pinghu = false;
			result.fu += 2;
		}
		if (mianzi[0].value == par.prevailingWind + 1) {
			result.fu += 2;
			pinghu = false;
		}
		if (mianzi[0].value == par.selfWind + 1) {
			result.fu += 2;
			pinghu = false;
		}
	}
	//判断是否有刻子
	for (auto i = 1; i <= 4; ++i) {
		if (mianzi[i].type != GroupType::Shunzi) {
			pinghu = false;
			if (mianzi[i].type == GroupType::Kezi)
				result.fu += 2 * (mianzi[i].isyaojiu() ? 2 : 1) * (i <= menqingCount ? 2 : 1);
			if (mianzi[i].type == GroupType::Gang)
				result.fu += 8 * (mianzi[i].isyaojiu() ? 2 : 1) * (i <= menqingCount ? 2 : 1);
		}
	}
	//和牌方式是否为两面
	auto liangmianOnly = true; //是否只能被看成两面
	auto liangmian = false; //是否可以被看成两面
	for (auto i = 1; i <= menqingCount; ++i) {
		if (mianzi[i].type == GroupType::Shunzi) {
			if (par.target.valueEqual(Single(mianzi[i].value, mianzi[i].color, false)))
				liangmian = true;
			if (par.target.valueEqual(Single(mianzi[i].value + 2, mianzi[i].color, false)))
				liangmian = true;
		}
		if (mianzi[i].type == GroupType::Quetou) {
			if (par.target.valueEqual(Single(mianzi[i].value, mianzi[i].color, false)))
				liangmianOnly = false;
		}
		if (mianzi[i].type == GroupType::Kezi || mianzi[i].type == GroupType::Gang) {
			if (par.target.valueEqual(Single(mianzi[i].value, mianzi[i].color, false)))
				liangmianOnly = false;
		}
	}
	if (liangmian == false)
		pinghu = false;
	//为平和型
	if (pinghu) {
		//门清，附加平和一役
		if (menqing) {
			result.fan++;
			result.yaku.add<Yaku::Pinghu>();
			if (par.type == 1)result.fu += 10;
			//assert(res.result.fu == 20);
		}
		else //非门清,30符固定
			result.fu = 30;
	}
	else {
		if (liangmianOnly == false)
			result.fu += 2;
		//门清荣和+10符,自摸+2符
		if (menqing) {
			if (par.type >= 1)
				result.fu += 10;
			else result.fu += 2;
		}
		else //副露自摸+2符
		{
			if (par.type == 0)
				result.fu += 2;
		}
	}
	if (result.fu % 10 != 0)result.fu = result.fu - result.fu % 10 + 10;
	//清一色，混一色，断幺九，混老头，纯全带幺九，混全带幺九的判断
	auto hunyise = true, duanyao = true, hunlaotou = true, chunquan = true, hunquan = true;
	auto color = '0';
	//副露牌
	for (auto& group : mianzi) {
		if (!group.isyaojiu()) {
			hunquan = false;
			chunquan = false;
		}
		if (group.color != 'z') {
			if (color == '0')
				color = group.color;
			else if (color != group.color) { hunyise = false; }
		}
		else { chunquan = false; }
		if (group.type == GroupType::Shunzi) {
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
	if (qingyise())hunyise = false;
	if (qingyise()) {
		if (menqing) {
			result.fan += 6;
			result.yaku.add<Yaku::Qingyise>();
		}
		else {
			result.fan += 5;
			result.yaku.add<Yaku::QingyiseF>();
		}
	}
	else if (hunyise) {
		if (menqing) {
			result.fan += 3;
			result.yaku.add<Yaku::Hunyise>();
		}
		else {
			result.fan += 2;
			result.yaku.add<Yaku::HunyiseF>();
		}
	}
	if (duanyao) {
		result.fan += 1;
		result.yaku.add<Yaku::Duanyaojiu>();
	}
	if (hunlaotou) {
		result.fan += 2;
		result.yaku.add<Yaku::Hunlaotou>();
	}
	if (chunquan) {
		if (menqing) {
			result.fan += 3;
			result.yaku.add<Yaku::Chunquantaiyaojiu>();
		}
		else {
			result.fan += 2;
			result.yaku.add<Yaku::ChunquantaiyaojiuF>();
		}
	}
	else if (hunquan) {
		if (menqing) {
			result.fan += 2;
			result.yaku.add<Yaku::Hunquandaiyaojiu>();
		}
		else {
			result.fan += 1;
			result.yaku.add<Yaku::HunquandaiyaojiuF>();
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
	if (par.lizhiXunmu != -1) {
		//w立
		if (par.lizhiXunmu == -2) {
			result.fan += 2;
			result.yaku.add<Yaku::Lianglizhi>();
		}
		else {
			result.fan += 1;
			result.yaku.add<Yaku::Lizhi>();
		}
		if (par.yifa == true) {
			result.fan += 1;
			result.yaku.add<Yaku::Yifa>();
		}
	}
	//检查抢杠
	if (par.type == 2) {
		result.fan += 1;
		result.yaku.add<Yaku::Qianggang>();
	}
	//检查门清自摸
	if (par.type == 0 && menqingCount == 4) {
		result.fan += 1;
		result.yaku.add<Yaku::Menqianqingzimo>();
	}
	//检查河底/海底
	if (par.state == 2) {
		if (par.type == 0)
			result.yaku.add<Yaku::Haidilaoyue>();
		else if (par.type == 1)result.yaku.add<Yaku::Hedilaoyu>();
	}
	result = Algorithms::getScore(par.selfWind, result);
	//std::cout << "*" << std::endl;
	if (result.fan == 0) { return TryToAgariResult(AgariFaildReason::NoYaku); }
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

TryToAgariResult CheckYakuForStandardType(const AgariParameters& par, const std::vector<Group>& mianzi) {
	return YakuChecker(par, mianzi).getResult();
}
