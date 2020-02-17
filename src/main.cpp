// TODO: Check Your Variable Initializations
// TODO: Check Your Virtual Destructors
// TODO: Mark Non-throw Subroutines "nothrow"
// TODO: Mark Non-State-Modifying Getters "const"
// TODO: Check TODO markings in other files

#include <iostream>
#include <vector>
#include <ctime>
#include <unordered_map>
#include <memory>
#include "Game.h"
#include "ai/Human.h"
#include "ai/AutoFurikomu.hpp"
#include "InformationStream.hpp"
#include "utils/console.h"
#include "ai/Tester.h"

using namespace std;
shared_ptr<ActionGenerator> player[4];
Game game;
void test() {
	auto human = make_shared<Human>("测试人");
	GameInfo res;
	res.rule = Rule(); //规则
	//每个玩家的牌河，是否立直，以及副露牌组
	for (auto i = 0; i <= 3; ++i) {
		PlayerInfo info;
		info.riichi = false;
		info.riichiJunme = -1;
		info.score = 99999;
		res.playerInfo.push_back(info);
	}
	res.remainTiles = 10;
	//手牌
	res.handTile.emplace_back(1, 's', 0);
	res.handTile.emplace_back(1, 's', 0);
	res.handTile.emplace_back(2, 's', 0);
	res.handTile.emplace_back(2, 's', 0);
	res.handTile.emplace_back(3, 's', 0);
	res.handTile.emplace_back(3, 's', 0);
	res.handTile.emplace_back(4, 's', 0);
	res.handTile.emplace_back(4, 's', 0);
	res.handTile.emplace_back(5, 'm', 0);
	res.handTile.emplace_back(5, 'm', 0);
	res.handTile.emplace_back(6, 'm', 0);
	res.handTile.emplace_back(6, 'm', 0);
	res.handTile.emplace_back(7, 's', 0);
	sort(res.handTile.begin(), res.handTile.end());
	res.nowTile = Single(7,'s',false);
	res.prevailingWind = EAST; //场风
	res.selfWind = EAST; //门风
	res.nowWind = EAST;
	res.gameState = GameState::OneAct;
	res.w = false;
	human->generateAction(res);
	Player p;
	p.setInfo(0, 1000, res.selfWind, res.handTile, {}, {}, res.nowTile, {}, false, false, -1, -1, false);
	TryToAgariResult r{};
	//for (int i = 1000000; i > 0; --i) {
	r = p.tsumo(res.prevailingWind, static_cast<BonusYakuState>(res.w), {}, {});
	//r = p.ron(res.nowTile,res.prevailingWind,AgariWays::Ron, static_cast<BonusYakuState>(res.w), {}, {});
	//}
	auto re = r.result;
	cout << BookManager::lang.mj_yaku << ":" << endl;
	for (auto&& yk : re.yaku) {
		cout << getYakuName(yk) << " ";
		cout << endl;
	}
	cout << BookManager::lang.mj_dora << re.dora << "   "
		<< BookManager::lang.mj_aka << re.akadora << "   "
		<< BookManager::lang.mj_ura << re.uradora << endl;
	if (re.han < 0) {
		cout << BookManager::lang.mj_yakuman[-re.han] << " " << re.scoreAdd << BookManager::lang.mj_den << endl;
	}
	else { cout << re.han << BookManager::lang.mj_han << re.fu << BookManager::lang.mj_fu << "   " << re.scoreAdd << BookManager::lang.mj_den << endl; }

}

void test2() {
	while (true) {
		vector<Single>handTile;
		handTile.emplace_back(1, 's', 0);
		handTile.emplace_back(4, 's', 0);
		handTile.emplace_back(1, 'm', 0);
		handTile.emplace_back(4, 'm', 0);
		handTile.emplace_back(7, 'm', 0);
		cout << Algorithms::getDistanceStandard(CompactSingles(handTile))<<endl;
		break;
	}
	while (true);
}

int main_loop() {

	/*
	已知bug:
	暂无
	*/
	const int seed = time(0);
	//cout << "输入种子:" << endl;
	//cin >> seed;
	srand(time(0));
	player[0] = make_shared<Tester>("A");
	player[1] = make_shared<Tester>("B");
	player[2] = make_shared<Tester>("C");
	player[3] = make_shared<Human>("Tester");
	game.startNewGame();
	InfoPrinter::printControlInfoLine(BookManager::lang.ct_gameStarted);
	auto first = true;
	do {
		if(!first)game.startNextRound();
		first = false;
		InfoPrinter::printControlInfoLine(BookManager::lang.ct_roundStarted);
		do {
			for (auto i = 0; i < 4; ++i) {
				const auto action = player[i]->generateAction(game.getGameInfo(i));
				const auto res = game.setPlayerAction(i, action);
				if (!res.success) {
					cout << "error happend!,error code:" << static_cast<int>(res.type) << endl;
					int a;
					cin >> a;
					return 0;
				}
			}
			assert(game.endThisTurn());
		} while (!game.roundOver());
		auto result = game.getRoundResult();
		InfoPrinter::printControlInfoLine(BookManager::lang.ct_roundOver);
		if (result.ryuukyoku) {InfoPrinter::printControlInfoLine(BookManager::lang.mj_ryuukyoku);}
		else if (!result.agariResult.empty()) {
			for (auto& res : result.agariResult) {
				cout << player[res.agariID]->getName() << " " << (res.tsumo ? BookManager::lang.mj_tsumo : BookManager::lang.mj_ron) << endl;
				cout << BookManager::lang.mj_yaku<<":" << endl;
				for (auto&& yk : res.yaku) {
					cout << getYakuName(yk) << " ";
					cout << endl;
				}
				cout << BookManager::lang.mj_dora << res.dora << "   "
					<< BookManager::lang.mj_aka << res.akadora << "   "
					<< BookManager::lang.mj_ura << res.uradora << endl;
				if (res.han < 0) {
					cout << BookManager::lang.mj_yakuman[-res.han]<<" " << res.scoreAdd << BookManager::lang.mj_den << endl;
				}
				else { cout << res.han << BookManager::lang.mj_han << res.fu<< BookManager::lang.mj_fu << "   " << res.scoreAdd << BookManager::lang.mj_den << endl; }
			}
		}
		cout << endl;
	}while (!game.gameOver());
	InfoPrinter::printControlInfoLine(BookManager::lang.ct_gameOver);
	std::cin.get();
	return 0;
}

int main() {
	//push_console_locale_utf8("zh_CN");
	system("chcp 936");
	BookManager::init();
	//test();
	//system("pause");
	//test2();
	
	main_loop();
	//pop_console_locale();
}
