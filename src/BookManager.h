#pragma once
#include"Single.h"
#include<queue>
struct Language{
	std::string mj_wind[4];			//东 南 西 北
	std::string mj_sangen[3];		//白 发 中
	std::string mj_han;				//番
	std::string mj_fu;				//符
	std::string mj_den;				//点
	std::string mj_yaku;			//役种
	std::string mj_yakuman[6];		//(x倍)役满
	std::string mj_get;				//摸
	std::string mj_discard;			//切
	std::string mj_tsumogiri;		//摸切
	std::string mj_tsumo;
	std::string mj_ron;
	std::string mj_furikomu;
	std::string mj_discardTile;
	std::string mj_riichi;
	std::string mj_ryuukyoku;
	std::string mj_chii;
	std::string mj_pon;
	std::string mj_kan;
	std::string mj_nuku;
	std::string mj_standardType;
	std::string mj_chiitoiType;
	std::string mj_kokushiType;
	std::string mj_syanten;
	std::string mj_dora;
	std::string mj_aka;
	std::string mj_ura;
	std::string mj_doraIndicator;
	std::string mj_uraIndicator;
	std::string mj_subround;
	std::string mj_round;
	std::string mj_honba;
	std::string mj_remain;
	std::string mj_tileMeasureWord;
	std::string mj_jikaze;
	std::string mj_bakaze;
	std::string mj_direction;
	std::string mj_furiten;
	std::string mj_doujunFuriten;
	std::string mj_riichiFuriten;
	std::string mj_sutehaiFuriten;
	std::string ct_makeChoice;
	std::string ct_hasDiscard;
	std::string ct_gameStarted;
	std::string ct_roundStarted;
	std::string ct_gameOver;
	std::string ct_roundOver;
	std::string ct_action[11];
	std::string i_tableNotFound;
	std::string i_tableBroken;
	std::string i_tableFound;
	std::string i_tableLoaded;
	std::string i_calculationStarted;
	std::string i_calculationFinished;
	std::string i_saveSuccess;
	std::string i_saveUnsuccess;
	std::string y_yakuName[52];
	std::string p_pai[37];
};
class BookManager {
private:
	//构造给定数量的雀头和面子的牌型，存到queue中,target表示目标雀头和面子数的压缩型
	static void constructTarget(int quetou, int mentsu, std::queue<SparseSinglesOfColor>& q, SparseSinglesOfColor shape, int target, bool z);
	//5的幂
	static const int pow5[10];
	//预处理，计算单花色离目标的距离(仅允许插入和删除两个操作)
	static void preprocessDistance();
	//加载语言
	static void initLanguage();
public:
	//到给定目标的距离，参数分别为原形状、目标雀头数*5+目标面子数(雀头最多只能有一个) 原形状为9位5进制数，从低到高表示手牌中数字1~9有几个
	static uint8_t distanceToTarget[1953125][10];
	//到给定目标的距离(字牌)，参数分别为原形状、目标雀头数*5+目标面子数(雀头最多只能有一个) 原形状为9位5进制数，从低到高表示手牌中东南西北白发中有几个
	static uint8_t distanceToTargetZ[78125][10];
	//语言实例
	static Language lang;
	static void init();
};