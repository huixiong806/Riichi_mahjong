#include<iostream>
#include<map>
#include<ctime>
#include<vector>
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
	yaku[0x110] = "��ֱ";
	yaku[0x111] = "һ��";
	yaku[0x112] = "��ǰ��������";
	yaku[0x100] = "���۾�";
	yaku[0x101] = "���ƣ��Է���";
	yaku[0x102] = "���ƣ�������";
	yaku[0x103] = "���ƣ���";
	yaku[0x104] = "���ƣ���";
	yaku[0x105] = "���ƣ���";
	yaku[0x113] = "ƽ��";
	yaku[0x114] = "һ����";
	yaku[0x106] = "ǹ��";
	yaku[0x107] = "���Ͽ���";
	yaku[0x108] = "��������";
	yaku[0x109] = "�ӵ�����";
	yaku[0x210] = "����ֱ";
	yaku[0x200] = "��ɫͬ��";
	yaku[0x201] = "������";
	yaku[0x202] = "�ԶԺ�";
	yaku[0x203] = "������";
	yaku[0x204] = "С��Ԫ";
	yaku[0x205] = "����ͷ";
	yaku[0x211] = "�߶���";
	yaku[0x206] = "��ȫ���۾�";
	yaku[0x226] = "��ȫ���۾�";
	yaku[0x207] = "һ��ͨ��";
	yaku[0x227] = "һ��ͨ��";
	yaku[0x208] = "��ɫͬ˳";
	yaku[0x228] = "��ɫͬ˳";
	yaku[0x310] = "������";
	yaku[0x300] = "��ȫ���۾�";
	yaku[0x320] = "��ȫ���۾�";
	yaku[0x301] = "��һɫ";
	yaku[0x321] = "��һɫ";
	yaku[0x500] = "��������";
	yaku[0x600] = "��һɫ";
	yaku[0x620] = "��һɫ";
	yaku[0xd00] = "���";
	yaku[0xd01] = "�غ�";
	yaku[0xd02] = "����Ԫ";
	yaku[0xd10] = "�İ���";
	yaku[0xd03] = "��һɫ";
	yaku[0xd04] = "��һɫ";
	yaku[0xd05] = "����ͷ";
	yaku[0xd11] = "��ʿ��˫";
	yaku[0xd06] = "С��ϲ";
	yaku[0xd07] = "�ĸ���";
	yaku[0xd12] = "��������";
	yaku[0xf10] = "�İ��̵���";
	yaku[0xf11] = "��ʿ��˫ʮ����";
	yaku[0xf12] = "������������";
	yaku[0xf00] = "����ϲ";
}
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
	res.handTile.push_back(Single(1,'m',0));
	res.handTile.push_back(Single(9, 'm', 0));
	res.handTile.push_back(Single(1, 'p', 0));
	res.handTile.push_back(Single(9, 'p', 0));
	res.handTile.push_back(Single(1, 's', 0));
	res.handTile.push_back(Single(9, 's', 0));
	res.handTile.push_back(Single(1, 'z', 0));
	res.handTile.push_back(Single(2, 'z', 0));
	res.handTile.push_back(Single(3, 'z', 0));
	res.handTile.push_back(Single(4, 'z', 0));
	res.handTile.push_back(Single(5, 'z', 0));
	res.handTile.push_back(Single(6, 'z', 0));
	res.handTile.push_back(Single(7, 'z', 0));
	res.nowTile = Single(6, 'z', 0);
	res.prevailingWind = WindType::EAST;   //����
	res.selfWind =WindType::WEST;  //�ŷ�
	res.nowWind = WindType::WEST;
	res.mingpai = false;
	res.w = false;
	human->generateAction(res);
	Player p;
	p.setInfo(0, 1000, res.selfWind, res.handTile, {}, {}, res.nowTile, {}, false, false, -1, -1, 0);
	auto r=p.zimo(res.prevailingWind,1, {}, {});
	auto re = r.result;
	cout << "Test " << (re.zimo ? "����" : "�ٺ�") << endl;
	cout << endl;
	for (auto& yk : re.yaku)
	{
		cout << yaku[(int)yk] << " ";
		cout << endl;
	}
	cout << "����" << re.dora << "   �챦��" << re.akadora << "   �ﱦ��" << re.uradora << endl;
	if (re.fan < 0)
	{
		if (re.fan == -1)
			cout << "���� " << re.scoreAdd << "��" << endl;
		const string ykman[7] = { "","һ","��","��","��","��","��" };
		cout << ykman[-re.fan] << "������ "<<re.scoreAdd << "��" << endl;
		cout << "ׯ��ʧ��" << re.scoreDecZhuang << "  �м�ʧ��" << re.scoreDecXian << endl;
	}
	else
	{
		cout << re.fan << "��" << re.fu << "��   " << re.scoreAdd << "��" << endl;
		cout << "ׯ��ʧ��" << re.scoreDecZhuang << "  �м�ʧ��" << re.scoreDecXian << endl;
	}
	
}
int main()
{
	init();
	test();
	/*
	�����ռ�
	3 ��3z
	233 7������������1m��3p
	*/
	int seed;
	cout << "��������:" << endl;
	cin >> seed;
	srand(seed);
	init();
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
					cout << yaku[(int)yk] << " ";
					cout << endl;
				}
				cout << "����" << res.dora << "   �챦��" << res.akadora << "   �ﱦ��" << res.uradora << endl;
				if (res.fan < 0)
				{
					if (res.fan == -1)
						cout << "���� " << res.scoreAdd << "��" << endl;
					const string ykman[7] = { "","һ","��","��","��","��","��" };
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
