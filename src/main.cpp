#include<iostream>
#include<map>
#include<ctime>
#include<vector>
#include<unordered_map>
#include"Game.h"
#include"Human.hpp"
#include"AutoFurikomu.hpp"
using namespace std;
shared_ptr<ActionGenerator> player[4];
Game game;
void test()
{
	auto human = make_shared<Human>("测试人");
	GameInfo res;
	res.rule = Rule();//规则
					 //每个玩家的牌河，是否立直，以及副露牌组
	for (int i = 0;i<=3;++i)
	{
		PlayerInfo info;
		info.lizhi = false;
		info.lizhiXunmu = -1;
		info.score = 99999;
		res.playerInfo.push_back(info);
	}
	//手牌
	res.handTile.push_back(Single(1, 's', 0));
	res.handTile.push_back(Single(1, 's', 0));
	res.handTile.push_back(Single(1, 's', 0));
	res.handTile.push_back(Single(1, 's', 0));
	res.handTile.push_back(Single(2, 's', 0));
	res.handTile.push_back(Single(2, 's', 0));
	res.handTile.push_back(Single(2, 's', 0));
	res.handTile.push_back(Single(2, 's', 0));
	res.handTile.push_back(Single(3, 's', 0));
	res.handTile.push_back(Single(3, 's', 0));
	res.handTile.push_back(Single(3, 's', 0));
	res.handTile.push_back(Single(3, 's', 0));
	res.handTile.push_back(Single(7, 'z', 0));
	sort(res.handTile.begin(), res.handTile.end());
	res.nowTile = Single(7, 'z', 0);
	res.prevailingWind = WindType::EAST;   //场风
	res.selfWind =WindType::EAST;  //门风
	res.nowWind = WindType::EAST;
	res.mingpai = false;
	res.w = false;
	human->generateAction(res);
	Player p;
	p.setInfo(0, 1000, res.selfWind, res.handTile, {}, {}, res.nowTile, {}, false, false, -1, -1, 0);
	auto r=p.zimo(res.prevailingWind,0, {}, {});
	auto re = r.result;
	cout << "Test " << (re.zimo ? "自摸" : "荣和") << endl;
	cout << endl;
	for (auto& yk : re.yaku)
	{
		cout << getYakuName((int)yk) << " ";
		cout << endl;
	}
	cout << "宝牌" << re.dora << "   红宝牌" << re.akadora << "   里宝牌" << re.uradora << endl;
	if (re.fan < 0)
	{
		
		const string ykman[7] = { "","一","两","三","四","五","六" };
		if (re.fan == -1)
			cout << "役满 " << re.scoreAdd << "点" << endl;
		else 
			cout << ykman[-re.fan] << "倍役满 "<<re.scoreAdd << "点" << endl;
		cout << "庄家失点" << re.scoreDecZhuang << "  闲家失点" << re.scoreDecXian << endl;
	}
	else
	{
		cout << re.fan << "番" << re.fu << "符   " << re.scoreAdd << "点" << endl;
		cout << "庄家失点" << re.scoreDecZhuang << "  闲家失点" << re.scoreDecXian << endl;
	}
	
}
void test2()
{
	while (1)
	{
		int a[7];
		int mianzi=0, quetou=0;
		cin >> a[0]>> a[1] >> a[2]>>a[3]>>a[4]>>a[5]>>a[6]>>mianzi>>quetou;
		int shape = a[0]+(a[1]*5)+(a[2]*25)+(a[3]*125)+(a[4]*625)+a[5]*3125+a[6]*15625;
		//while (pai)
		//{
		//	Algorithms::addNumberCount(shape, pai % 10, 1);
		//	pai = pai / 10;
		//}
		//int target = quetou * 5 + mianzi;
		cout << Algorithms::getDistanceSingle(shape,mianzi,quetou,1)<<endl;
	}
}
int main()
{
	cout << "预处理中..." << endl;
	Algorithms::preprocessDistance();
	cout << "预处理结束" << endl;
	test();
	//test2();
	/*
	种子收集
	3 杠3z
	233 7对子自摸，留1m和3p
	*/
	int seed;
	cout << "输入种子:" << endl;
	cin >> seed;
	srand(seed);
	player[0] = make_shared<AutoFurikomu>("放铳人A");                                 
	player[1] = make_shared<AutoFurikomu>("放铳人B");
	player[2] = make_shared<AutoFurikomu>("放铳人C");
	player[3] = make_shared<Human>("实战测试人");
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
				if (res.success == false)
				{
					cout<<"出现错误,输入任意内容退出"<<endl;
					int a;
					cin >> a;
					return 0;
				}
				if (game.roundOver())break;
				//cout << (int)(res.type)<<endl;
			}
		} while (!game.roundOver());
		RoundResult result=game.getRoundResult();
		cout << "本局结束" << endl;
		if (result.liuju)
		{
			cout << "流局" << endl;
		}
		else if(!result.agariResult.empty())
		{
			for (auto& res : result.agariResult)
			{
				cout<<player[res.hupaiID]->getName()<<" "<<(res.zimo?"自摸":"荣和")<<endl;
				cout << "役种:" << endl;
				for (auto& yk : res.yaku)
				{
					cout << getYakuName((int)yk) << " ";
					cout << endl;
				}
				cout << "宝牌" << res.dora << "   红宝牌" << res.akadora << "   里宝牌" << res.uradora << endl;
				if (res.fan < 0)
				{
					const string ykman[7] = { "","一","两","三","四","五","六" };
					if (res.fan == -1)
						cout << "役满 " << res.scoreAdd << "点" << endl;
					else
						cout << ykman[-res.fan] << "倍役满 " << res.scoreAdd << "点" << endl;
				}
				else
				{
					cout << res.fan << "番" << res.fu << "符   " << res.scoreAdd << "点" << endl;
				}
			}
		}
	} while (!game.gameOver());
	cout << "游戏结束！" << endl;
	while (1);
	return 0;
}
