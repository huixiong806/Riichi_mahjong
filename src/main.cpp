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
	player[0] = make_shared<AutoFurikomu>("�����A");
	player[1] = make_shared<AutoFurikomu>("�����B");
	player[2] = make_shared<AutoFurikomu>("�����C");
	player[3] = make_shared<Human>("������");
	cout << "��Ϸ��ʼ��" << endl;
	bool first = true;
	do
	{
		if(first)
		game.startNewGame();
		else game.startNextRound();
		first = false;
		cout << "�µ�һ�ֿ�ʼ��" << endl;
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
	cout << "��Ϸ������" << endl;
	while (1);
	return 0;
}