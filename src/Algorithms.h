#pragma once
#include "ResultInspector.hpp"
#include <queue>
#include <BookManager.h>
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
	static bool agariWithoutYaku(const Single& target, const Tiles& handTile);
	//试图和牌，并计算国士无双型和牌点数
	static TryToAgariResult kokushi(const AgariParameters& par);
	//试图和牌，并计算标准型和牌点数，返回(点数最大的)结果,depth=0表示枚举雀头，dep=4为最深层
	static TryToAgariResult agariSearch(const AgariParameters& par, int depth, const Tiles& remainTiles,
	                                    std::vector<Group> mentsu);
	//试图和牌，并计算七对型和牌点数
	static TryToAgariResult chiitoi(const AgariParameters& par);
	//标准型役种判断
	static TryToAgariResult YakuCheckForStandard(const AgariParameters& par, std::vector<Group>& mentsu);
public:

	//试图和牌，返回点数最大的结果
	static TryToAgariResult agari(const AgariParameters& par);
	//根据和牌结果计算打点
	static AgariResult getScore(WindType selfWind, AgariResult input);
	//0~8 万字,9~17 饼子,18~26 索子,27~33 东南西北白发中
	static int getTileIndex(const Single& tile);
	//获得牌池，编码同getTileIndex函数
	static std::vector<int> getPool(const Tiles& tiles);
	//快速判断是否形式听牌。不允许虚听，允许空听。返回听牌种类(返回值中Akadora属性无效)
	static std::vector<Single> tenpai(const Tiles& handTile);
	//获取所有类型的牌
	static std::vector<Single> allKindsOfTiles();
	
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
