#pragma once
#include "ResultInspector.hpp"
#include <queue>

class Algorithms {
private:
	/*
	已淘汰函数
	//判断是否为国士无双和牌型
	//static bool guoshiWithoutYaku(SparseSingles handTile);
	//判断是否为七对和牌型
	//static bool qiduiWithoutYaku(SparseSingles handTile);
	//判断是否为标准和牌型,*************有bug，勿用！！！请用getdistance返回向听数是否为-1*****************
	//static bool agariSearchWithoutYaku(std::vector<int> pool);
	*/
	//判断是否形式和牌,不考虑役
	static bool agariWithoutYaku(const Single& target, const std::vector<Single>& handTile);
	//试图和牌，并计算国士无双型和牌点数
	static TryToAgariResult kokushi(const AgariParameters& par);
	//试图和牌，并计算标准型和牌点数，返回(点数最大的)结果,depth=0表示枚举雀头，dep=4为最深层
	static TryToAgariResult agariSearch(const AgariParameters& par, int depth, const std::vector<Single>& remainTiles,
	                                    std::vector<Group> mentsu);
	//试图和牌，并计算七对型和牌点数
	static TryToAgariResult chiitoi(const AgariParameters& par);
	//构造给定数量的雀头和面子的牌型，存到queue中,target表示目标雀头和面子数的压缩型
	static void constructTarget(int quetou, int mentsu, std::queue<SparseSinglesOfColor>& q, SparseSinglesOfColor shape, int target, bool z);
	//5的幂
	static const int pow5[10];
	//到给定目标的距离，参数分别为原形状、目标雀头数*5+目标面子数(雀头最多只能有一个) 原形状为9位5进制数，从低到高表示手牌中数字1~9有几个
	static uint8_t distanceToTarget[1953125][10];
	//到给定目标的距离(字牌)，参数分别为原形状、目标雀头数*5+目标面子数(雀头最多只能有一个) 原形状为9位5进制数，从低到高表示手牌中东南西北白发中有几个
	static uint8_t distanceToTargetZ[78125][10];
	//标准型役种判断
	static TryToAgariResult YakuCheckForStandard(const AgariParameters& par, std::vector<Group>& mentsu);
public:

	//试图和牌，返回点数最大的结果
	static TryToAgariResult agari(const AgariParameters& par);
	/*type
	自摸=0
	荣和=1
	抢杠=2
	抢暗杠=3
	*/
	//根据和牌结果计算打点
	static AgariResult getScore(WindType selfWind, AgariResult inp);
	//0~8 万字,9~17 饼子,18~26 索子,27~33 东南西北白发中
	static int getTileIndex(Single& tile);
	static std::vector<int> getPool(const std::vector<Single>& tiles);
	//快速判断是否形式听牌。不允许虚听，允许空听。返回听牌种类
	static std::vector<Single> tenpai(const std::vector<Single>& handTile);
	//获取所有类型的牌
	static std::vector<Single> allKindsOfTiles();
	//shape为压缩后的牌型参数，value为要获取个数的数字。函数对单花色牌的压缩型进行操作。函数已废除
	//static int getNumberCount(int shape, int value);
	//static void setNumberCount(int& shape, int value, int count);
	//static void addNumberCount(int& shape, int value, int count);
	//预处理，计算单花色离目标的距离(仅允许插入和删除两个操作)
	static void preprocessDistance();
	//计算2,5,8,11,14张手牌的标准型向听数(0为一向听，-1为和牌)
	static int getDistanceStandard(const CompactSingles& handTile);
	//计算14张手牌的七对型向听数(0为一向听，-1为和牌)
	static int getDistanceChiitoi(const CompactSingles& handTile);
	//计算14张手牌的国士无双向听数(0为一向听，-1为和牌)
	static int getDistanceKokushi(const CompactSingles& handTile);
	//计算14张手牌的向听数(0为一向听，-1为和牌)
	static int getDistance(const CompactSingles& handTile);
	//获得单花色向听数
	static int getDistanceSingle(int shape, int mentsu, int quetou, bool z);
};
