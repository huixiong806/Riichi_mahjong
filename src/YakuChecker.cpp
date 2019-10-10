#include"YakuChecker.h"
YakuChecker::YakuChecker() {}
bool YakuChecker::tianhu(const AgariParameters& par)
{
	return par.state == 1 && par.type == 0 && par.selfWind == WindType::EAST;
}
bool YakuChecker::dihu(const AgariParameters& par)
{
	return par.state == 1 && par.type == 0 && par.selfWind != WindType::EAST;
}
void YakuChecker::addYaku(AgariResult& result, Yaku yaku)
{
	result.yaku.push_back(yaku);
	if((int)yaku >> 8 >= 0xf)
	{
		result.fan -= 2;
		return;
	}
	if ((int)yaku >> 8 >= 0xd)
	{
		result.fan -= 1;
		return;
	}
	result.fan += (int)yaku >> 8;
}
bool YakuChecker::ziyise(const AgariParameters& par, const std::vector<Group>& mianzi)
{
	for (auto& group : mianzi)
	{
		if (group.color != 'z')
			return false;
	}
	return true;
}
bool YakuChecker::sigangzi(const AgariParameters& par, const std::vector<Group>& mianzi)
{
	int gangziCount = 0;
	for (auto& group : mianzi)
	{
		if (group.type == GroupType::Gang)
			gangziCount++;
	}
	return gangziCount == 4;
}
TryToAgariResult YakuChecker::getResult(const AgariParameters& par, const std::vector<Group>& menqingMianzi)
{
	AgariResult result = AgariResult();
	menqingCount = menqingMianzi.size() - 1;//非副露的面子数
	menqing = menqingCount == 4;//是否门清
	std::vector<Group> mianzi = menqingMianzi;
	mianzi.insert(mianzi.end(), par.groupTile.begin(), par.groupTile.end());
	if (mianzi.size() != 5)return TryToAgariResult(AgariFaildReason::ShapeWrong);
	result.zimo = par.type == 0;
	//役满型 
	//天地和
	if (tianhu(par))
		addYaku(result, Yaku::Tianhu);
	if (dihu(par))
		addYaku(result, Yaku::Dihu);
	//字一色、四杠子
	if (ziyise(par,mianzi))
		addYaku(result, Yaku::Ziyise);
	if (sigangzi(par, mianzi))
		addYaku(result, Yaku::Sigangzi);
	//小四喜、大四喜和大三元、清老头、绿一色、清一色(为了判断九莲宝灯)
	int fengCount = 0;//风面子/雀头计数
	int sanyuanCount = 0;
	bool fengQuetou = false, sanyuanQuetou = false;
	char color = '0';
	qingyise = true;
	bool lvyise = true;
	bool qinglaotou = true;
	for (auto& group : mianzi)
	{
		if (!group.islaotou())qinglaotou = false;
		if (!group.isgreen())lvyise = false;
		if (group.color == 'z')
		{
			qingyise = false;
			if (group.value <= 4)
				fengCount++;
			else sanyuanCount++;
			if (group.type == GroupType::Quetou)
			{
				if (group.value <= 4)
					fengQuetou = true;
				else sanyuanQuetou = true;
			}
		}
		else
		{
			if (color == '0')
				color = group.color;
			else if (color != group.color)
				qingyise = false;
		}
	}
	if (qinglaotou)
	{
		result.yaku.push_back(Yaku::Qinglaotou);
		result.fan -= 1;
	}
	if (lvyise)
	{
		result.yaku.push_back(Yaku::Lvyise);
		result.fan -= 1;
	}
	if (fengCount == 4)
	{
		if (fengQuetou)
		{
			result.yaku.push_back(Yaku::Xiaosixi);
			result.fan -= 1;
		}
		else
		{
			result.yaku.push_back(Yaku::Dasixi);
			result.fan -= 2;
		}
	}
	if (sanyuanCount == 4)
	{
		if (!sanyuanQuetou)
		{
			result.yaku.push_back(Yaku::Dasanyuan);
			result.fan -= 1;
		}
	}
	//四暗刻和四暗刻单骑
	if (menqing == true)
	{
		int keziCount = 0;
		Single quetou;
		for (auto& group : mianzi)
		{
			if (group.type == GroupType::Kezi)
				keziCount++;
			if (group.type == GroupType::Quetou)
				quetou = Single(group.value, group.color, 0);
		}
		if (keziCount == 4)
		{
			//判断四暗刻单骑
			if (par.target.valueEqual(quetou) || tianhu(par))
			{
				result.yaku.push_back(Yaku::Siankedanqi);
				result.fan -= 2;
			}
			else if (par.type == 0)
			{
				//判断四暗刻
				result.yaku.push_back(Yaku::Sianke);
				result.fan -= 1;
			}

		}
	}
	//九莲宝灯，纯正九莲宝灯
	if (qingyise)
	{
		int ct[10] = { 0,0,0,0,0,0,0,0,0,0 };
		for (auto& item : par.handTile)
			ct[item.value()]++;
		bool chunzhengjiulianbaodeng = true;
		for (int i = 1; i <= 9; ++i)
		{
			if ((i == 1 || i == 9) && ct[i] != 3)chunzhengjiulianbaodeng = false;
			if (i > 1 && i < 9 && ct[i] != 1)chunzhengjiulianbaodeng = false;
		}
		ct[par.target.value()]++;
		bool subflag = false;
		for (int i = 1; i <= 9; ++i)
		{
			if (!subflag && (ct[i] > 0 && ct[i] % 2 == 0))
			{
				ct[i]--;
				subflag = true;
			}
		}
		bool jiulianbaodeng = true;
		if (!subflag)jiulianbaodeng = false;
		for (int i = 1; i <= 9; ++i)
		{
			if ((i == 1 || i == 9) && ct[i] != 3)jiulianbaodeng = false;
			if (i > 1 && i < 9 && ct[i] != 1)jiulianbaodeng = false;
		}
		if (chunzhengjiulianbaodeng || (jiulianbaodeng && tianhu(par)))
		{
			result.yaku.push_back(Yaku::Chunzhengjiulianbaodeng);
			result.fan -= 2;
		}
		else if (jiulianbaodeng)
		{
			result.yaku.push_back(Yaku::Jiulianbaodeng);
			result.fan -= 1;
		}
	}
	//满足役满型
	if (result.fan < 0)
	{
		return AgariResult(Algorithms::getScore(par.selfWind, result));
	}
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
	bool pinghu = true;
	//首先判断雀头是否为役牌
	if (mianzi[0].color == 'z')
	{
		if (mianzi[0].value >= 5)
		{
			pinghu = false;
			result.fu += 2;
		}
		if (mianzi[0].value == par.prevailingWind + 1)
		{
			result.fu += 2;
			pinghu = false;
		}
		if (mianzi[0].value == par.selfWind + 1)
		{
			result.fu += 2;
			pinghu = false;
		}
	}
	//判断是否有刻子
	for (int i = 1; i <= 4; ++i)
	{
		if (mianzi[i].type != GroupType::Shunzi)
		{
			pinghu = false;
			if (mianzi[i].type == GroupType::Kezi)
				result.fu += 2 * (mianzi[i].isyaojiu() ? 2 : 1) * (i <= menqingCount ? 2 : 1);
			if (mianzi[i].type == GroupType::Gang)
				result.fu += 8 * (mianzi[i].isyaojiu() ? 2 : 1) * (i <= menqingCount ? 2 : 1);
		}
	}
	//和牌方式是否为两面
	bool liangmianOnly = true;//是否只能被看成两面
	bool liangmian = false;//是否可以被看成两面
	for (int i = 1; i <= menqingCount; ++i)
	{
		if (mianzi[i].type == GroupType::Shunzi)
		{
			if (par.target.valueEqual(Single(mianzi[i].value, mianzi[i].color, false)))
				liangmian = true;
			if (par.target.valueEqual(Single(mianzi[i].value + 2, mianzi[i].color, false)))
				liangmian = true;
		}
		if (mianzi[i].type == GroupType::Quetou)
		{
			if (par.target.valueEqual(Single(mianzi[i].value, mianzi[i].color, false)))
				liangmianOnly = false;
		}
		if (mianzi[i].type == GroupType::Kezi || mianzi[i].type == GroupType::Gang)
		{
			if (par.target.valueEqual(Single(mianzi[i].value, mianzi[i].color, false)))
				liangmianOnly = false;
		}
	}
	if (liangmian == false)
		pinghu = false;
	//为平和型
	if (pinghu)
	{
		//门清，附加平和一役
		if (menqing)
		{
			result.fan++;
			result.yaku.push_back(Yaku::Pinghu);
			if (par.type == 1)result.fu += 10;
			//assert(res.result.fu == 20);
		}
		else //非门清,30符固定
			result.fu = 30;
	}
	else
	{
		if (liangmianOnly == false)
			result.fu += 2;
		//门清荣和+10符,自摸+2符
		if (menqing)
		{
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
	bool hunyise = true, duanyao = true, hunlaotou = true, chunquan = true, hunquan = true;
	color = '0';
	//副露牌
	for (auto& group : mianzi)
	{
		if (!group.isyaojiu())
		{
			hunquan = false;
			chunquan = false;
		}
		if (group.color != 'z')
		{
			if (color == '0')
				color = group.color;
			else if (color != group.color)
			{

				hunyise = false;
			}
		}
		else
		{
			chunquan = false;
		}
		if (group.type == GroupType::Shunzi)
		{
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
	if (qingyise)hunyise = false;
	if (qingyise)
	{
		if (menqing)
		{
			result.fan += 6;
			result.yaku.push_back(Yaku::Qingyise);
		}
		else
		{
			result.fan += 5;
			result.yaku.push_back(Yaku::QingyiseF);
		}
	}
	else if (hunyise)
	{
		if (menqing)
		{
			result.fan += 3;
			result.yaku.push_back(Yaku::Hunyise);
		}
		else
		{
			result.fan += 2;
			result.yaku.push_back(Yaku::HunyiseF);
		}
	}
	if (duanyao)
	{
		result.fan += 1;
		result.yaku.push_back(Yaku::Duanyaojiu);
	}
	if (hunlaotou)
	{
		result.fan += 2;
		result.yaku.push_back(Yaku::Hunlaotou);
	}
	if (chunquan)
	{
		if (menqing)
		{
			result.fan += 3;
			result.yaku.push_back(Yaku::Chunquantaiyaojiu);
		}
		else
		{
			result.fan += 2;
			result.yaku.push_back(Yaku::ChunquantaiyaojiuF);
		}
	}
	else if (hunquan)
	{
		if (menqing)
		{
			result.fan += 2;
			result.yaku.push_back(Yaku::Hunquandaiyaojiu);
		}
		else
		{
			result.fan += 1;
			result.yaku.push_back(Yaku::HunquandaiyaojiuF);
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
	if (par.lizhiXunmu != -1)
	{
		//w立
		if (par.lizhiXunmu == -2)
		{
			result.fan += 2;
			result.yaku.push_back(Yaku::Lianglizhi);
		}
		else
		{
			result.fan += 1;
			result.yaku.push_back(Yaku::Lizhi);
		}
		if (par.yifa == true)
		{
			result.fan += 1;
			result.yaku.push_back(Yaku::Yifa);
		}
	}
	//检查抢杠
	if (par.type == 2)
	{
		result.fan += 1;
		result.yaku.push_back(Yaku::Qianggang);
	}
	//检查门清自摸
	if (par.type == 0 && menqingCount == 4)
	{
		result.fan += 1;
		result.yaku.push_back(Yaku::Menqianqingzimo);
	}
	//检查河底/海底
	if (par.state == 2)
	{
		if (par.type == 0)
			result.yaku.push_back(Yaku::Haidilaoyue);
		else if (par.type == 1)result.yaku.push_back(Yaku::Hedilaoyu);
	}
	result = Algorithms::getScore(par.selfWind, result);
	//std::cout << "*" << std::endl;
	if (result.fan == 0)
	{
		return TryToAgariResult(AgariFaildReason::NoYaku);
	}
	/*输出调试信息-不同拆牌方式的牌型和役种
	for (auto& item : mianzi)
	{
		std::cout << item.getString() << " ";
	}
	std::cout << std::endl;
	for (auto& yk : result.yaku)
	{
		std::cout << getYakuName((int)yk) << " ";
		std::cout << std::endl;
	}
	std::cout << std::endl;
	*/
	return TryToAgariResult(result);
}