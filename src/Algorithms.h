#pragma once
#include "Single.hpp"
#include "Group.hpp"
#include<set>
#include<map>
#include<cassert>
class Algorithms
{
private:

	//判断是否为国士无双和牌型
	static bool guoshiwushuang(const Single& target,const std::vector<Single>&handTile)
	{
		bool res = false;
		int guoshi = 0;
		if (handTile.size()==13)
		{
			std::set<Single>all;
			for (auto& item : handTile)
				if (item.value() == 1 || item.value() == 9 || item.color() == 'z')
					all.insert(item);
			//国士
			if (all.size() == 12)
			{
				if (target.value() == 1 || target.value() == 9 || target.color() == 'z')
				{
					all.insert(target);
					if (all.size() == 13)
						guoshi = 1;
				}
			}
			//国士十三面
			else if (all.size() == 13)
			{
				if (target.value() == 1 || target.value() == 9 || target.color() == 'z')
					guoshi = 2;
			}
		}
		return guoshi > 0;
	}
	static bool qidui(const Single& target, const std::vector<Single>&handTile)
	{
		bool res=false;
		std::map<int, int>all;
		for (auto& item : handTile)
			all[item.color() * 100 + item.value()]++;
		all[target.color() * 100 + target.value()]++;
		if (all.size() != 7)return false;
		for (auto& item : all)
		{
			if (item.second != 2)
			{
				return false;
			}
		}
		return true;
	}
	//判断标准型的形式和牌
	static bool agariSearch(std::vector<int> pool, int depth, int remainTiles)
	{
		bool res=false;
		if (remainTiles==0)
			return true;
		if (depth == 0)
		{
			//判断是否虚和(一种牌拿5张)
			for (int i = 0; i <= 33; ++i)
				if (pool[i] >= 5)
					return false;
			//判断顺子和刻子
			for (int i = 0; i <= 33; ++i)
			{
				if (pool[i]>=2)
				{
					pool[i] -= 2;
					res = std::max(res, agariSearch(pool, depth + 1, remainTiles-2));
					if (res)return true;
				}
			}
			return false;
		}
		//判断顺子和刻子
		for (int i = 0; i <= 33; ++i)
		{
			if (pool[i] >= 3)
			{
				pool[i] -= 3;
				remainTiles -= 3;
				res = std::max(res, agariSearch(pool,depth + 1, remainTiles-3));
				return res;
			}
			//顺子
			if (i != 7 && i != 8 && i != 16 && i != 17 && i <= 24)
			{
				if (pool[i] > 0 && pool[i + 1] > 0 && pool[i + 2] > 0)
				{
					pool[i]--;
					pool[i + 1]--;
					pool[i + 2]--;
					res = std::max(res, agariSearch(pool, depth + 1, remainTiles - 3));
					return res;
				}
			}
			//存在浮牌
			if (pool[i] > 0)
				return false;
		}
		assert(0);
		return false;
	}
	//判断是否形式和牌,不考虑役
	static bool agari (const Single& target, const std::vector<Single>&handTile)
	{
		//国士无双型的判定
		bool guoshi = guoshiwushuang(target,handTile);
		if (guoshi)return true;
		//七对型的判定(两杯口将被拆解为七对型)
		bool qiduizi = qidui(target, handTile);
		if (qidui)return true;
		//一般型的判定
		std::vector<Single>allTiles = handTile;
		allTiles.push_back(target);
		sort(allTiles.begin(), allTiles.end());
		auto pool = getPool(allTiles);
		bool normal = agariSearch(pool, 0, allTiles.size());
		return std::max(qiduizi, normal);
	}
public:
	//0~8 万字,9~17 饼子,18~26 索子,27~33 东南西北白发中
	static std::vector<int> getPool(const std::vector<Single>& tiles)
	{
		std::vector<int> res;
		res.resize(34);
		for (auto& item : tiles)
		{
			if (item.color() == 'm')
			{
				res[item.value() - 1]++;
			}
			else if (item.color() == 'p')
			{
				res[item.value() + 8]++;
			}
			else if (item.color() == 's')
			{
				res[item.value() + 17]++;
			}
			else if (item.color() == 'z')
			{
				res[item.value() + 26]++;
			}
			else
			{
				assert(0);
			}
		}
		return res;
	}
	//快速判断是否形式听牌。不允许虚听，允许空听。返回听牌种类
	static std::vector<Single> tingpai(std::vector<Single> handTile)
	{
		std::vector<Single> res;
		for (int i = 1; i <= 9; ++i)
		{
			if (agari(Single(i, 'm', 0), handTile))
				res.push_back(Single(i, 'm', 0));
			if (agari(Single(i, 'p', 0), handTile))
				res.push_back(Single(i, 'p', 0));
			if (agari(Single(i, 's', 0), handTile))
				res.push_back(Single(i, 's', 0));
			if (i <= 7 && agari(Single(i, 'z', 0), handTile))
				res.push_back(Single(i, 'z', 0));
		}
		return res;
		
	}
};
