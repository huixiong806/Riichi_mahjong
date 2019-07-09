#include<iostream>
#include<map>
#include<ctime>
#include"Game.hpp"
#include"Human.hpp"
#include"AutoFurikomu.hpp"
using namespace std;
shared_ptr<ActionGenerator> player[4];
Game game;
int main()
{
	srand(time(0));
	player[0] = make_shared<AutoFurikomu>("放铳人A");
	player[1] = make_shared<AutoFurikomu>("放铳人B");
	player[2] = make_shared<AutoFurikomu>("放铳人C");
	player[3] = make_shared<Human>("测试人");
	cout << "游戏开始！" << endl;
	bool first = true;
	do
	{
		if(first)
		game.startNewGame();
		else game.startNextRound();
		first = false;
		cout << "新的一局开始了" << endl;
		do
		{
			for (int i = 0; i < 4; ++i)
			{
				auto action = player[i]->generateAction(game.getGameInfo(i));
				ActionResult res=game.doAction(i, action);
				//cout << (int)(res.type)<<endl;
			}
		} while (!game.roundOver());

	} while (!game.gameOver());
	cout << "游戏结束！" << endl;
	while (1);
	return 0;
}