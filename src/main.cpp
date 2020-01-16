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
	//手牌
	res.handTile.emplace_back(1, 's', 0);
	res.handTile.emplace_back(1, 's', 0);
	res.handTile.emplace_back(1, 's', 0);
	res.handTile.emplace_back(1, 's', 0);
	res.handTile.emplace_back(2, 's', 0);
	res.handTile.emplace_back(2, 's', 0);
	res.handTile.emplace_back(2, 's', 0);
	res.handTile.emplace_back(2, 's', 0);
	res.handTile.emplace_back(3, 's', 0);
	res.handTile.emplace_back(3, 's', 0);
	res.handTile.emplace_back(3, 's', 0);
	res.handTile.emplace_back(3, 'z', 0);
	res.handTile.emplace_back(3, 'z', 0);
	sort(res.handTile.begin(), res.handTile.end());
	res.nowTile = Single(3, 's', false);
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
	//}
	auto re = r.result;
	cout << "Test " << (re.tsumo ? "自摸" : "荣和") << endl;
	cout << endl;
	for (auto&& yk : re.yaku) {
		cout << getYakuName(yk) << " ";
		cout << endl;
	}
	cout << "宝牌" << re.dora << "   红宝牌" << re.akadora << "   里宝牌" << re.uradora << endl;
	if (re.han < 0) {
		const string ykman[7] = {"", "一", "两", "三", "四", "五", "六"};
		if (re.han == -1)
			cout << "役满 " << re.scoreAdd << "点" << endl;
		else
			cout << ykman[-re.han] << "倍役满 " << re.scoreAdd << "点" << endl;
		cout << "庄家失点" << re.scoreDecOya << "  闲家失点" << re.scoreDecKodomo << endl;
	}
	else {
		cout << re.han << "番" << re.fu << "符   " << re.scoreAdd << "点" << endl;
		cout << "庄家失点" << re.scoreDecOya << "  闲家失点" << re.scoreDecKodomo << endl;
	}

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
	种子收集
	3 杠3z
	233 7对子自摸，留1m和3p

	已知bug:
	红宝牌碰和杠的时候会出问题
	*/
	const int seed = time(0);
	//cout << "输入种子:" << endl;
	//cin >> seed;
	srand(seed);
	player[0] = make_shared<Tester>("全自动放铳鸡 A");
	player[1] = make_shared<Tester>("全自动放铳鸡 B");
	player[2] = make_shared<Tester>("全自动放铳鸡 C");
	player[3] = make_shared<Human>("测试人");
	cout << "游戏开始！" << endl;
	auto first = true;
	do {
		if (first)
			game.startNewGame();
		else game.startNextRound();
		first = false;
		cout << "新的一局开始了" << endl;
		do {
			for (auto i = 0; i < 4; ++i) {
				const auto action = player[i]->generateAction(game.getGameInfo(i));
				const auto res = game.setPlayerAction(i, action);
				if (!res.success) {
					cout << "出现错误,输入任意内容退出,错误编码:" << static_cast<int>(res.type) << endl;
					assert(0);
					int a;
					cin >> a;
					return 0;
				}
			}
			assert(game.endThisTurn());
		} while (!game.roundOver());
		auto result = game.getRoundResult();
		cout << "本局结束" << endl;
		if (result.ryuukyoku) { cout << "流局" << endl; }
		else if (!result.agariResult.empty()) {
			for (auto& res : result.agariResult) {
				cout << player[res.agariID]->getName() << " " << (res.tsumo ? "自摸" : "荣和") << endl;
				cout << "役种:" << endl;
				for (auto&& yk : res.yaku) {
					cout << getYakuName(yk) << " ";
					cout << endl;
				}
				cout << "宝牌" << res.dora << "   红宝牌" << res.akadora << "   里宝牌" << res.uradora << endl;
				if (res.han < 0) {
					const string ykman[7] = { "", "一", "两", "三", "四", "五", "六" };
					if (res.han == -1)
						cout << "役满 " << res.scoreAdd << "点" << endl;
					else
						cout << ykman[-res.han] << "倍役满 " << res.scoreAdd << "点" << endl;
				}
				else { cout << res.han << "番" << res.fu << "符   " << res.scoreAdd << "点" << endl; }
			}
		}
		cout << endl;
	} while (!game.gameOver());
	cout << "游戏结束！" << endl;
	std::cin.get();
	return 0;
}

int main() {
	//push_console_locale_utf8("zh_CN");
	cout << "预处理中..." << endl;
	Algorithms::preprocessDistance();
	cout << "预处理结束" << endl;
	//test();
	//system("pause");
	//test2();
	main_loop();
	pop_console_locale();
}
