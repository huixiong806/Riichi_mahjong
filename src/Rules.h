#pragma once
enum WindType {
	EAST,
	SOUTH,
	WEST,
	NORTH
};

struct Rule {
	int doraCount = 3; //宝牌数量
	int Hanshibari = 1; //番缚
	int startPoint = 25000; //起始点数
	int endPoint = 30000; //一位必要点数
	WindType gameType = SOUTH; //游戏类型，东风局/半庄局/全庄局
};

extern const Rule DefualtRule;
