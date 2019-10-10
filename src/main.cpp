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
	auto human = make_shared<Human>("������");
	GameInfo res;
	res.rule = Rule();//����
					 //ÿ����ҵ��ƺӣ��Ƿ���ֱ���Լ���¶����
	for (int i = 0;i<=3;++i)
	{
		PlayerInfo info;
		info.lizhi = false;
		info.lizhiXunmu = -1;
		info.score = 99999;
		res.playerInfo.push_back(info);
	}
	//����
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
	res.prevailingWind = WindType::EAST;   //����
	res.selfWind =WindType::EAST;  //�ŷ�
	res.nowWind = WindType::EAST;
	res.mingpai = false;
	res.w = false;
	human->generateAction(res);
	Player p;
	p.setInfo(0, 1000, res.selfWind, res.handTile, {}, {}, res.nowTile, {}, false, false, -1, -1, 0);
	auto r=p.zimo(res.prevailingWind,0, {}, {});
	auto re = r.result;
	cout << "Test " << (re.zimo ? "����" : "�ٺ�") << endl;
	cout << endl;
	for (auto& yk : re.yaku)
	{
		cout << getYakuName((int)yk) << " ";
		cout << endl;
	}
	cout << "����" << re.dora << "   �챦��" << re.akadora << "   �ﱦ��" << re.uradora << endl;
	if (re.fan < 0)
	{
		
		const string ykman[7] = { "","һ","��","��","��","��","��" };
		if (re.fan == -1)
			cout << "���� " << re.scoreAdd << "��" << endl;
		else 
			cout << ykman[-re.fan] << "������ "<<re.scoreAdd << "��" << endl;
		cout << "ׯ��ʧ��" << re.scoreDecZhuang << "  �м�ʧ��" << re.scoreDecXian << endl;
	}
	else
	{
		cout << re.fan << "��" << re.fu << "��   " << re.scoreAdd << "��" << endl;
		cout << "ׯ��ʧ��" << re.scoreDecZhuang << "  �м�ʧ��" << re.scoreDecXian << endl;
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
	cout << "Ԥ������..." << endl;
	Algorithms::preprocessDistance();
	cout << "Ԥ�������" << endl;
	test();
	//test2();
	/*
	�����ռ�
	3 ��3z
	233 7������������1m��3p
	*/
	int seed;
	cout << "��������:" << endl;
	cin >> seed;
	srand(seed);
	player[0] = make_shared<AutoFurikomu>("�����A");                                 
	player[1] = make_shared<AutoFurikomu>("�����B");
	player[2] = make_shared<AutoFurikomu>("�����C");
	player[3] = make_shared<Human>("ʵս������");
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
				if (res.success == false)
				{
					cout<<"���ִ���,�������������˳�"<<endl;
					int a;
					cin >> a;
					return 0;
				}
				if (game.roundOver())break;
				//cout << (int)(res.type)<<endl;
			}
		} while (!game.roundOver());
		RoundResult result=game.getRoundResult();
		cout << "���ֽ���" << endl;
		if (result.liuju)
		{
			cout << "����" << endl;
		}
		else if(!result.agariResult.empty())
		{
			for (auto& res : result.agariResult)
			{
				cout<<player[res.hupaiID]->getName()<<" "<<(res.zimo?"����":"�ٺ�")<<endl;
				cout << "����:" << endl;
				for (auto& yk : res.yaku)
				{
					cout << getYakuName((int)yk) << " ";
					cout << endl;
				}
				cout << "����" << res.dora << "   �챦��" << res.akadora << "   �ﱦ��" << res.uradora << endl;
				if (res.fan < 0)
				{
					const string ykman[7] = { "","һ","��","��","��","��","��" };
					if (res.fan == -1)
						cout << "���� " << res.scoreAdd << "��" << endl;
					else
						cout << ykman[-res.fan] << "������ " << res.scoreAdd << "��" << endl;
				}
				else
				{
					cout << res.fan << "��" << res.fu << "��   " << res.scoreAdd << "��" << endl;
				}
			}
		}
	} while (!game.gameOver());
	cout << "��Ϸ������" << endl;
	while (1);
	return 0;
}
