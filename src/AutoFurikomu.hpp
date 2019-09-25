#pragma once
#include"MovementGenerator.hpp"
#include<string>
#include<iostream>
class AutoFurikomu:public ActionGenerator
{
private:

public:
	AutoFurikomu() { name = "∑≈Ô•»À"; }
	AutoFurikomu(std::string name_)
	{
		name = name_;
	}
	virtual Action generateAction(const GameInfo& info)
	{
		Action res;
		res.type = ActionType::Null;
		if (info.nowWind != info.selfWind&&info.mingpai==false)return res;
		if (info.nowWind == info.selfWind&&info.mingpai == true)return res;
		if (info.mingpai)
		{
			res.type = ActionType::Skip;
		}
		else
		{
			res.type = ActionType::Dapai;
			res.target = info.nowTile;
		}
		
		return res;
	}
};
