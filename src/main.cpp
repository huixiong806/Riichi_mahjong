#include<iostream>
#include<map>
#include<ctime>
#include<unordered_map>
#include"Game.hpp"
#include"Human.hpp"
#include"AutoFurikomu.hpp"
using namespace std;
shared_ptr<ActionGenerator> player[4];
Game game;
unordered_map<int,string> yaku;
void init()
{
	yaku[0x110]="立直";
	yaku[0x111] = "一发";
	yaku[0x112] = "门前清自摸和";
	yaku[0x100] = "断幺九";
	yaku[0x101] = "役牌：自风牌";
	yaku[0x102] = "役牌：场风牌";
	yaku[0x103] = "役牌：白";
	yaku[0x104] = "役牌：发";
	yaku[0x105] = "役牌：中";
	yaku[0x113] = "平和";
	yaku[0x114] = "一杯口";
	yaku[0x106] = "枪杠";
	yaku[0x107] = "岭上开花";
	yaku[0x108] = "海底捞月";
	yaku[0x109] = "河底捞鱼";
	yaku[0x210] = "两立直";
	yaku[0x200] = "三色同刻";
	yaku[0x201] = "三杠子";
	yaku[0x202] = "对对和";
	yaku[0x203] = "三暗刻";
	yaku[0x204] = "小三元";
	yaku[0x205] = "混老头";
	yaku[0x211] = "七对子";
	yaku[0x206] = "混全带幺九";
	yaku[0x226] = "混全带幺九";
	yaku[0x207] = "一气通贯";
	yaku[0x227] = "一气通贯";
	yaku[0x208] = "三色同顺";
	yaku[0x228] = "三色同顺";
	yaku[0x310] = "二杯口";
	yaku[0x300] = "纯全带幺九";
	yaku[0x320] = "纯全带幺九";
	yaku[0x301] = "混一色";
	yaku[0x321] = "混一色";
	yaku[0x500] = "流局满贯";
	yaku[0x600] = "清一色";
	yaku[0x620] = "清一色";
	yaku[0xd00] = "天和";
	yaku[0xd01] = "地和";
	yaku[0xd02] = "大三元";
	yaku[0xd10] = "四暗刻";
	yaku[0xd03] = "字一色";
	yaku[0xd04] = "绿一色";
	yaku[0xd05] = "清老头";
	yaku[0xd11] = "国士无双";
	yaku[0xd06] = "小四喜";
	yaku[0xd07] = "四杠子";
	yaku[0xd12] = "九莲宝灯";
	yaku[0xf10] = "四暗刻单骑";
	yaku[0xf11] = "国士无双十三面";
	yaku[0xf12] = "纯正九莲宝灯";
	yaku[0xf00] = "大四喜";
}
int main()
{
	/*
	种子收集
	3 杠3z
	233 7对子自摸，留1m和3p
	*/
	srand(233);
	init();
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
				cout<<player[res.first]->getName()<<" "<<(res.second.zimo?"自摸":"荣和")<<endl;
				cout << "役种:" << endl;
				for (auto& yk : res.second.yaku)
				{
					cout << yaku[(int)yk] << " ";
					cout << endl;
				}
				cout << "宝牌" << res.second.dora << "   红宝牌" << res.second.akadora << "   里宝牌" << res.second.uradora << endl;
				cout << res.second.fan << "番" << res.second.fu << "符   "<<res.second.scoreAdd<<"点" << endl;

			}
		}
	} while (!game.gameOver());
	cout << "游戏结束！" << endl;
	while (1);
	return 0;
}