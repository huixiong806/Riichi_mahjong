#include"Player.h"

void Player::setInfo(int subround_, 
					 int score_, 
					 WindType selfWind_, 
					 const std::vector<Single>& handTile_, 
					 const std::vector<bool>& disabledHandTile_, 
					 const std::vector<Group>& groupTile_, 
					 const Single& nowTile_, 
					 const std::vector<Single>& discardedTile_, 
					 bool rinshan_, 
					 bool tsumogiri_, 
					 int riichi_, 
					 int riichiJunme_, 
					 bool ippatsu_) {
	subround = subround_; //巡数
	score = score_;
	selfWind = selfWind_; //门风
	handTile = handTile_; //手牌
	disabledHandTile = disabledHandTile_; //禁打手牌
	groupTile = groupTile_; //副露牌组
	nowTile = nowTile_; //自摸牌
	discardedTile = discardedTile_; //牌河
	rinshan = rinshan_; //玩家当前的自摸牌是岭上牌
	tsumogiri = tsumogiri_; //表示牌河里最后一张牌是否为摸切
	riichi = riichi_; //立直宣言牌是牌河中的第几张，-1为未立直
	riichiJunme = riichiJunme_; //立直的巡目,从0开始,-1为未立直,-2为w立
	ippatsu = ippatsu_; //当前是否为一发巡
}

bool Player::canPon(Single target, Group result, int relativePosition) const {
	//检查类型
	if (result.type != GroupType::Koutsu)
		return false;
	//检查颜色
	if (target.color() != result.color)
		return false;
	//检查数值
	if (target.value() != result.value)
		return false;
	//检查位置
	if (relativePosition != result.state)
		return false;
	//检查红宝牌是否正确
	if ((result.akadora & 1) != target.isAkadora())
		return false;
	const auto akadoraCount = (result.akadora & 2) + (result.akadora & 4);
	auto realAkadoraCount = 0;
	auto tileCount = 0;
	for (auto& item : handTile) {
		if (item.color() == result.color && item.value() == result.value) {
			tileCount++;
			if (item.isAkadora())
				realAkadoraCount++;
		}
	}
	//牌数不够
	if (tileCount < 2)return false;
	//红宝牌不够
	if (realAkadoraCount < akadoraCount)
		return false;
	//非红宝牌不够
	if ((tileCount - realAkadoraCount) < (2 - akadoraCount))
		return false;
	return true;
}

bool Player::canMinKan(Single target, Group result, int relativePosition) const {
	//检查类型
	if (result.type != GroupType::Kantsu)
		return false;
	//检查颜色
	if (target.color() != result.color)
		return false;
	//检查数值
	if (target.value() != result.value)
		return false;
	//检查位置
	if (relativePosition != result.state)
		return false;
	//检查红宝牌是否正确
	if ((result.akadora & 1) != target.isAkadora())
		return false;
	const auto akadoraCount = (result.akadora & 2) + (result.akadora & 4) + (result.akadora & 8);
	auto realAkadoraCount = 0;
	auto tileCount = 0;
	for (auto& item : handTile) {
		if (item.color() == result.color && item.value() == result.value) {
			tileCount++;
			if (item.isAkadora())
				realAkadoraCount++;
		}
	}
	//牌数不够
	if (tileCount < 3)return false;
	//红宝牌不够
	if (realAkadoraCount < akadoraCount)
		return false;
	//非红宝牌不够
	if ((tileCount - realAkadoraCount) < (3 - akadoraCount))
		return false;
	return true;
}

void Player::pon(Group result) {
	groupTile.push_back(result);
	const auto other0 = Single(result.value, result.color, result.akadora & 2);
	const auto other1 = Single(result.value, result.color, result.akadora & 4);
	auto ok0 = false, ok1 = false;
	for (auto& item : handTile) {
		if (!ok0 && other0 == item) {
			ok0 = true;
			item = Null;
		}
		else if (!ok1 && other1 == item) {
			ok1 = true;
			item = Null;
		}
	}
	sort(handTile.begin(), handTile.end());
	while (handTile.back() == Null)handTile.pop_back();
}

void Player::minkan(Group result) {
	groupTile.push_back(result);
	const auto other0 = Single(result.value, result.color, result.akadora & 2);
	const auto other1 = Single(result.value, result.color, result.akadora & 4);
	const auto other2 = Single(result.value, result.color, result.akadora & 8);
	auto ok0 = false, ok1 = false, ok2 = false;
	for (auto& item : handTile) {
		if (!ok0 && other0 == item) {
			ok0 = true;
			item = Null;
		}
		else if (!ok1 && other1 == item) {
			ok1 = true;
			item = Null;
		}
		else if (!ok2 && other2 == item) {
			ok2 = true;
			item = Null;
		}
	}
	sort(handTile.begin(), handTile.end());
	while (handTile.back() == Null)handTile.pop_back();
}

bool Player::canDiscardTile(Single target) const {
	if (target == Null)return false;
	if (nowTile == target)
		return true;
	//立直必须摸切
	if (riichiJunme != -1)return false;
	for (auto i = 0; i < handTile.size(); ++i)
		if (target == handTile[i])
			return true;
	return false;
}

//返回false时不应变动私有变量

void Player::discardTile(Single target) {//打牌
	if (target == Null)
		return throw("打牌失败-Null是能打的牌？");
	if (nowTile == target) {
		discardedTile.push_back(nowTile);
		tsumogiri = true;
		nowTile = Null;
		return;
	}
	for (auto i = 0; i < handTile.size(); ++i) {
		if (target == handTile[i]) {
			discardedTile.push_back(target);
			handTile[i] = nowTile;
			std::sort(handTile.begin(), handTile.end()); //从小到大排序
			if (nowTile == Null)
				handTile.pop_back();
			nowTile = Null;
			tsumogiri = false;
			return;
		}
	}
	throw("打牌失败-要打的牌不存在");
}

/*
state
正常=0
天地和,w立=1
河底/海底=2
*/
bool Player::canRiichi(BonusYakuState state, Single target) const {
	//要打的牌是否存在?
	if (target == Null)return false;
	auto myTile = handTile;
	myTile.push_back(nowTile);
	auto success = false;
	for (auto i = 0; i < myTile.size(); ++i) {
		if (target == myTile[i]) {
			myTile[i] = Null;
			std::sort(myTile.begin(), myTile.end()); //从小到大排序,Null一定被排到最大一个
			myTile.pop_back();
			success = true;
			break;
		}
	}
	if (!success)return false;
	//是否非暗杠副露？
	if (!groupTile.empty()) {
		for (auto& item : groupTile) {
			//暗杠必须满足type=Gang且state=0
			if (item.type != GroupType::Kantsu || item.state != 0)
				return false;
		}
	}
	//是否已经立直过了？
	if (riichiJunme != -1)
		return false;
	//点棒够不够?
	if (score < 1000)
		return false;
	//是否听牌?
	return !Algorithms::tenpai(myTile).empty();
}

void Player::doRiichi(BonusYakuState state, Single target) {
	//要打的牌是否存在?
	if (target == Null){
		throw("立直失败-Null是能打的牌？");
		return;
	}
	auto myTile = handTile;
	myTile.push_back(nowTile);
	auto success = false;
	for (auto i = 0; i < myTile.size(); ++i) {
		if (target == myTile[i]) {
			myTile[i] = Null;
			std::sort(myTile.begin(), myTile.end()); //从小到大排序,Null一定被排到最大一个
			myTile.pop_back();
			success = true;
			break;
		}
	}
	if (!success){
		throw("立直失败-要打的牌不存在");
		return;
	}
	//是否非暗杠副露？
	if (!groupTile.empty()) {
		for (auto& item : groupTile) {
			//暗杠必须满足type=Gang且state=0
			if (item.type != GroupType::Kantsu || item.state != 0){
				throw("立直失败-有非暗杠副露");
				return;
			}
		}
	}
	//是否已经立直过了？
	if (riichiJunme != -1)
		return throw("立直失败-已经立直过了");
	//点棒够不够?
	if (score < 1000)
		return throw("立直失败-点棒不够");
	//是否听牌?
	const auto backup = handTile;
	handTile = myTile;
	const auto isTenpai = tenpai();
	/*
	for (int i = 0; i < handTile.size(); ++i)
	std::cout << handTile[i].getDisplay() << " ";
	std::cout << isTingpai << std::endl;
	*/
	handTile = backup;
	if (isTenpai) {
		discardTile(target);
		riichiJunme = subround;
		ippatsu = true;
		riichi = discardedTile.size() - 1;
		if (state == 1)
			riichiJunme = -2;
		score -= 1000;
		return;
	}
	return throw("立直失败-没听牌");
}
