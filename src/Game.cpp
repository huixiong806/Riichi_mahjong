#include "Game.h"
//新游戏
void Game::startNewGame(Rule rule_) {
	player.resize(4);
	rule = rule_; //设置规则
	prevailingWind = EAST; //东场
	round = 1; //1局
	east = rand() % 4; //随机庄家
	honba = 0; //0本场
	riichibouValue = 0;//场上立直棒清空
	doraIndicatorCount = 0;
	gameIsOver = false;
	for (auto i = 0; i < 4; ++i)
		player[i].score = rule.startPoint;
	startNewRound();
}

//开始下一局
void Game::startNextRound() {
	//连庄，本场数+1，局数不变
	if (result.renchan) { honba++; }
	else //不连庄，进入下一局
	{
		east = (east + 1) % 4; //东家换成其下家
		if (round == 4) //4局的话换场风,局数重置
		{
			prevailingWind = static_cast<WindType>((prevailingWind + 1) % 4);
			round = 1;
		}
		else { round++; }
	}
	startNewRound();
}

//开始新局
void Game::startNewRound() {
	w = true;
	gameIsOver = false;
	roundIsOver = false;
	turn = east; //东家起手
	mountain.reset(rule); //牌山重置
	for (auto i = east, j = 0; j < 4; i = (i + 1) % 4, j++) {
		player[i].groupTile.clear();
		player[i].discardedTile.clear();
		//mPlayer[i].score = mRule.startPoint;
		player[i].handTile = mountain.hand[j];
		std::sort(player[i].handTile.begin(), player[i].handTile.end());
		player[i].selfWind = static_cast<WindType>(j);
		player[i].riichi = -1;
		player[i].riichiJunme = -1;
		player[i].setNowTile(Null, false);
		player[i].subround = -1;
		player[i].riichiFuriten = false;
		player[i].doujunFuriten = false;
		player[i].sutehaiFuriten = false;
	}
	for (auto& item : isReady)
		item = false;
	//mLastTurn = -1;
	doraIndicatorCount = 1;
	gameState = GameState::OneAct;
	newTurn(east, false);
}
void Game::newTurn(int who, bool lingShang) {
	turn = who;
	for (auto i = 0; i < 4; ++i) { player[i].setNowTile(Null, false); }
	//mLastTurn = -1;
	if (mountain.tsumohai != Null) {
		player[who].setNowTile(mountain.tsumohai, false); //开局给东家设置自摸牌
		mountain.tsumohai = Null;
	}
	else {
		//正常摸牌
		if (lingShang) { player[who].setNowTile(mountain.nextRinshan(), true); }
		else { player[who].setNowTile(mountain.nextHand(), false); }
	}
	//std::cout << "***********" << std::endl;
	//巡数+1
	player[who].subround++;
	if (player[who].subround > 0)
		w = false;
	//解除同巡振听
	player[who].resetDoujunFuriten();
}

//鸣牌(除了杠)后打牌

void Game::newTurnAfterNari(int who) {
	turn = who;
	for (auto i = 0; i < 4; ++i) { player[i].setNowTile(Null, false); }
	//巡数+1
	player[who].subround++;
	//解除同巡振听
	player[who].resetDoujunFuriten();
}
bool Game::endThisTurn()
{
	for (int i = 0; i < 4; ++i)
		if (!isReady[i])
			return false;
	if (gameState == GameState::WaitingForNaki){
		processNari();
		gameState = GameState::OneAct;
	}
	else{
		Action action = playerAction[turn];
		if (action.type == ActionType::Riichi) {
			player[turn].doRiichi(static_cast<BonusYakuState>(w), action.target);
			riichibouValue += 1000;
			if (player[turn].rinshan == true) {
				doraIndicatorCount++;
				player[turn].rinshan = false;
				//todo:判断四杠散了
			}
		}
		else if (action.type == ActionType::DiscardTile){
			player[turn].discardTile(action.target);
			player[turn].ippatsu = false;
			if (player[turn].rinshan == true) {
				doraIndicatorCount++;
				player[turn].rinshan = false;
				//todo:判断四杠散了
			}
		}
		else if (action.type == ActionType::Tsumo){
			std::vector<Single> allDora = mountain.getDoras(doraIndicatorCount);
			std::vector<Single> allUra = mountain.getUras(doraIndicatorCount);
			auto result= player[turn].tsumo(prevailingWind, static_cast<BonusYakuState>(w), allDora, allUra);
			if (result.success) {
				result.result.furikomiID = -1;
				result.result.agariID = turn;
				endThisRound(std::vector<AgariResult>({ result.result }), false);
			}
			else
			{
				throw("不具备自摸条件");
			}
		}
		gameState = GameState::WaitingForNaki;
	}
	for (auto i = 0; i < 4; ++i) {
		isReady[i] = false;
		playerAction[i] = Action();
		player[i].updateSutehaiFuriten();
	}
	return true;
}
SetActionResult Game::setPlayerAction(int playerID, Action action) {
	SetActionResult setActionResult = { true ,ErrorType::Success };
	if (gameState == GameState::WaitingForNaki) {
		Single target = player[turn].discardedTile.rbegin()->tile;
		if (playerID == turn) {
			isReady[playerID] = true;
			playerAction[playerID] = Action();
		}
		if (isReady[playerID] == false) {
			switch (action.type) {
			case ActionType::Null:
				setActionResult.success = false;
				setActionResult.type = ErrorType::CannotChooseNull;
				break;
			case ActionType::Skip:
				isReady[playerID] = true;
				playerAction[playerID] = action;
				break;
			case ActionType::Ron: {
				std::vector<Single> allDora = mountain.getDoras(doraIndicatorCount);
				std::vector<Single> allUra = mountain.getUras(doraIndicatorCount);
				TryToAgariResult result;
				if (!player[playerID].canRon(target, prevailingWind, AgariWays::Ron,
											  static_cast<BonusYakuState>(w), allDora, allUra)) {
					setActionResult.success = false;
					setActionResult.type = ErrorType::ActionRejected;
					return setActionResult;
				}
				isReady[playerID] = true;
				playerAction[playerID] = action;
				break;
			}
			case ActionType::Chii:
				setActionResult.type = player[playerID].canChii(target, action.group,
					(static_cast<int>(player[turn].selfWind) + 4 - player[playerID].selfWind) % 4);
				setActionResult.success = setActionResult.type == ErrorType::Success;
				if (!setActionResult.success)
					return setActionResult;
				//立直后不能鸣牌
				if (player[playerID].riichiJunme != -1) {
					setActionResult.success = false;
					setActionResult.type = ErrorType::Naki_AlreadyRiichi;
					return setActionResult;
				}
				isReady[playerID] = true;
				playerAction[playerID] = action;
				break;
			case ActionType::Pon:
				if (!player[playerID].canPon(target, action.group,
				                           (static_cast<int>(player[turn].selfWind) + 4 - static_cast<int>(player[playerID].selfWind)) % 4)
				) {
					//std::cout<<"*********"<< std::endl;
					setActionResult.success = false;
					setActionResult.type = ErrorType::ActionRejected;
					return setActionResult;
				}
				//立直后不能鸣牌
				if (player[playerID].riichiJunme != -1) {
					setActionResult.success = false;
					setActionResult.type = ErrorType::ActionRejected;
					return setActionResult;
				}
				isReady[playerID] = true;
				playerAction[playerID] = action;
				break;
			case ActionType::Kan:
				if (!player[playerID].canMinKan(target, action.group,
				                           (static_cast<int>(player[turn].selfWind) + 4 - player[playerID].selfWind) % 4)
				) {
					setActionResult.success = false;
					setActionResult.type = ErrorType::ActionRejected;
					return setActionResult;
				}
				//立直后不能鸣牌
				if (player[playerID].riichiJunme != -1) {
					setActionResult.success = false;
					setActionResult.type = ErrorType::ActionRejected;
					return setActionResult;
				}
				isReady[playerID] = true;
				playerAction[playerID] = action;
				break;
			default:
				setActionResult.success = false;
				setActionResult.type = ErrorType::ActionRejected;
				return setActionResult;
			}
		}
	}
	else {
		if (action.type == ActionType::Null){
			//重构提示:这里可以多加一个ActionType，表示我不是决策者，但是我成功收到了局面信息。
			if (playerID != turn)
				isReady[playerID] = true;
			return setActionResult;
		}
		if (playerID != turn) {
			setActionResult.success = false;
			setActionResult.type = ErrorType::NotYourTurn;
			return setActionResult;
		}
		switch (action.type) {
		case ActionType::DiscardTile:
			if (!player[playerID].canDiscardTile(action.target)){
				setActionResult.success = false;
				setActionResult.type = ErrorType::ActionRejected;
				return setActionResult;
			}
			playerAction[playerID] = action;
			break;
		case ActionType::Riichi: {
			if (mountain.remainCount() < 4) {
				setActionResult.success = false;
				setActionResult.type = ErrorType::Riichi_lessTilesLeft;
				return setActionResult;
			}
			if (!player[playerID].canRiichi(static_cast<BonusYakuState>(w),action.target)){
				setActionResult.success = false;
				setActionResult.type = ErrorType::Riichi_Other;
				return setActionResult;
			}
			playerAction[playerID] = action;
			break;
		}
		case ActionType::Tsumo: {
			std::vector<Single> allDora=mountain.getDoras(doraIndicatorCount);
			std::vector<Single> allUra = mountain.getUras(doraIndicatorCount);
			if (!player[playerID].canTsumo(prevailingWind, static_cast<BonusYakuState>(w), allDora, allUra)){
				setActionResult.success = false;
				setActionResult.type = ErrorType::ActionRejected;
				return setActionResult;
			}
			playerAction[playerID] = action;
			break;
		}
		default:
			setActionResult.success = false;
			setActionResult.type = ErrorType::ActionRejected;
			return setActionResult;
		}
		isReady[playerID] = true;
		//暂时不考虑鸣牌
	}
	return setActionResult;
}

//途中流局连庄
void Game::endThisRound(std::vector<AgariResult> res, bool tuzhongLiuju) {
	result = RoundResult();
	roundIsOver = true;
	gameIsOver = false;
	auto top = 0;
	for (auto i = 1; i < 4; ++i) {
		if (player[top].score < player[i].score || (player[top].score == player[i].score && ((player[top].selfWind +
			4 - 1) % 4 >(player[i].selfWind + 4 - 1) % 4)))
			top = i;
	}
	//有人和牌
	if (!res.empty()) {
		result.ryuukyoku = false;
		result.renchan = false;
		result.agariResult = res;
		//立直棒和本场费计算，按照头跳规则
		int targetPlayer; //获取点棒的人 
		if (res.size() == 1)
			targetPlayer = res[0].agariID;
		else {
			targetPlayer = res[0].furikomiID;
			auto minSpace = 4; //最小间距先调整为4
			const int a = player[res[0].furikomiID].selfWind;
			for (auto& item : res) {
				int b = player[item.agariID].selfWind;
				if (b < a)b += 4;
				const auto space = b - a;
				if (space < minSpace) {
					minSpace = space;
					targetPlayer = item.agariID;
				}
			}
		}
		player[targetPlayer].score += riichibouValue;
		player[targetPlayer].score += honba * 300;
		//荣和的本场费计算 
		if (!res[0].tsumo)
			player[res[0].furikomiID].score -= honba * 300;
		else {//自摸的本场费计算 
			for (auto i = 0; i < 4; ++i)
				if (targetPlayer != i)
					player[i].score -= honba * 100;
		}
		//分数计算 
		for (auto& item : res) {
			//和牌得失点 
			player[item.agariID].score += item.scoreAdd;
			if (player[item.agariID].selfWind == EAST)
				result.renchan = true;
			if (item.tsumo) {
				for (auto i = 0; i < 4; ++i) {
					if (item.agariID != i) {
						if (player[i].selfWind == EAST)
							player[i].score -= item.scoreDecOya;
						else
							player[i].score -= item.scoreDecKodomo;
					}
				}
			}
			else player[item.furikomiID].score -= item.scoreDecFurikomi;
		}
		if (prevailingWind == rule.gameType && round == 4) {
			//庄家没和牌或者和牌后是1位,游戏结束
			auto oyaAgari = false;
			for (auto& item : res)
				if (player[item.agariID].selfWind == EAST)
					oyaAgari = true;
			if (!oyaAgari || (oyaAgari && top == east))
				gameIsOver = true;
			//但如果top的点数不足就要继续
			if (player[top].score < rule.endPoint)
				gameIsOver = false;
		}
		else if (prevailingWind == rule.gameType + 1) //北4局不考虑东入
		{
			//4局强制结束，分数超过返点也强制结束
			if (round == 4)gameIsOver = true;
			if (player[top].score >= rule.endPoint)
				gameIsOver = true;
		}
		return;
	}
	//*****以下为流局****** 
	//TODO：流局满贯结算 
	result.ryuukyoku = true;
	if (tuzhongLiuju) {
		result.renchan = true;
		return;
	}
	result.renchan = player[east].tenpai();
	auto bonus = 0, deBonus = 0;
	auto tenpaiCount = 0;
	for (auto i = 0; i < 4; ++i)
		if (player[i].tenpai())
			tenpaiCount++;
	if (tenpaiCount == 1)bonus = 3000, deBonus = -1000;
	else if (tenpaiCount == 2)bonus = 1500, deBonus = -1500;
	else if (tenpaiCount == 3)bonus = 1000, deBonus = -3000;
	for (auto i = 0; i < 4; ++i) {
		if (player[i].tenpai())player[i].score += bonus;
		else player[i].score += deBonus;
	}
	//游戏结束判断
	if (prevailingWind == rule.gameType && round == 4) {
		//庄家没听牌或者收罚符后是1位,游戏结束
		const auto oyaTenpai = result.renchan;
		if (!oyaTenpai || (oyaTenpai && top == east))
			gameIsOver = true;
		//但如果top的点数不足就要继续
		if (player[top].score < rule.endPoint)
			gameIsOver = false;
	}
	else if (prevailingWind == rule.gameType + 1) //北4局不考虑东入
	{
		//4局强制结束，分数超过返点也强制结束
		if (round == 4)gameIsOver = true;
		if (player[top].score >= rule.endPoint)
			gameIsOver = true;
	}
	if (gameIsOver)
		player[top].score += riichibouValue;
}

//消去w立,一发,地和

void Game::resetBounusYaku() {
	w = false;
	for (auto i = 0; i < 4; ++i)
		player[i].ippatsu = false;
}

//处理鸣牌
void Game::processNari() {
	std::vector<AgariResult> result;
	auto overflag = false;
	//荣
	for (auto i = 0; i < 4; ++i) {
		if (playerAction[i].type == ActionType::Ron) {
			assert(player[turn].discardedTile.rbegin()->exist);
			std::vector<Single> allDora;
			std::vector<Single> allUra;
			for (auto i = 0; i < doraIndicatorCount; ++i) {
				allDora.push_back(mountain.getDora(i));
				allUra.push_back(mountain.getUra(i));
			}
			auto tmp = player[i].ron(player[turn].discardedTile.rbegin()->tile, prevailingWind, AgariWays::Ron,
									  static_cast<BonusYakuState>(w), allDora, allUra);
			tmp.agariID = i;
			tmp.furikomiID = turn;
			result.push_back(tmp);
			overflag = true;
			//std::cout << "*" << std::endl;
		}
	}
	if (overflag) {
		endThisRound(result, false);
		return;
	}
	//杠和碰
	for (auto i = 0; i < 4; ++i) {
		if (playerAction[i].type == ActionType::Kan) {
			assert(player[turn].discardedTile.rbegin()->exist);
			player[i].minkan(playerAction[i].group);
			player[turn].discardedTile.rbegin()->exist = false;
			resetBounusYaku();
			newTurn(i, true);
			return;
		}
		if (playerAction[i].type == ActionType::Pon) {
			assert(player[turn].discardedTile.rbegin()->exist);
			player[i].pon(playerAction[i].group);
			player[turn].discardedTile.rbegin()->exist = false;
			resetBounusYaku();
			newTurnAfterNari(i);
			return;
		}
	}
	//吃
	for (auto i = (turn + 1) % 4, j = 0; j < 3; ++j, i = (i + 1) % 4) {
		if (playerAction[i].type == ActionType::Chii) {
			assert(player[turn].discardedTile.rbegin()->exist);
			player[i].chii(playerAction[i].group);
			player[turn].discardedTile.rbegin()->exist=false;
			resetBounusYaku();
			newTurnAfterNari(i);
			return;
		}
	}
	//全部放弃鸣牌或者没法鸣牌
	//更新立直振听和同巡振听
	for (auto i = 0; i < 4; ++i) {
		if (i == turn)continue;
		assert(player[turn].discardedTile.rbegin()->exist);
		auto target = player[turn].discardedTile.rbegin()->tile;
		auto machihai = Algorithms::tenpai(player[i].handTile);
		if (!machihai.empty()){
			for (auto& item : machihai) {
				if (item.valueEqual(target)){
					if (player[i].riichiJunme != -1) {
						player[i].riichiFuriten = true;
					}
					player[i].doujunFuriten = true;
					break;
				}
			}
		}
	}
	if (mountain.remainCount() == 0) { endThisRound(std::vector<AgariResult>(), false); }
	else { this->newTurn((turn + 1) % 4, false); }
}


//玩家获取游戏信息
GameInfo Game::getGameInfo(int index)const {
	GameInfo res;
	res.rule = rule; //规则
	//每个玩家的牌河，是否立直，以及副露牌组
	for (auto i = 0, j = east; i <= 3; ++i, j = (j + 1) % 4) {
		PlayerInfo info;
		info.discardTile = player[j].discardedTile;
		info.groupTile = player[j].groupTile;
		info.riichi = player[j].riichi;
		info.riichiJunme = player[j].riichiJunme;
		info.ippatsu = player[j].ippatsu;
		info.score = player[j].score;
		res.playerInfo.push_back(info);
	}
	//手牌
	res.handTile = player[index].handTile;
	//剩余牌数
	res.remainTiles = mountain.remainCount();
	//宝牌指示牌
	res.doraIndicator = mountain.getDoraIndicators(doraIndicatorCount);
	res.prevailingWind = prevailingWind; //场风
	res.selfWind = player[index].selfWind; //门风
	if (gameState == GameState::WaitingForNaki) {
		res.nowWind = player[turn].selfWind;
		res.nowTile = player[turn].discardedTile.rbegin()->tile;
		assert(player[turn].discardedTile.rbegin()->exist);
	}
	else {
		res.nowWind = player[turn].selfWind;
		if (index == turn)
			res.nowTile = player[turn].nowTile;
		else
			res.nowTile = Null;
	}
	//振听状态
	res.doujunFuriten = player[index].doujunFuriten;
	res.sutehaiFuriten = player[index].sutehaiFuriten;
	res.riichiFuriten = player[index].riichiFuriten;
	res.remainTiles = mountain.remainCount();
	res.riichibouCount = riichibouValue; //额外立直棒数量
	res.honba = honba; //本场
	res.gameState = gameState;
	res.round = round;
	res.w = w;
	return res;
}