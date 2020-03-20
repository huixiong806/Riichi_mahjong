#include "BookManager.h"
#include<fstream>
#include<iostream>
#include"InformationStream.hpp"
using namespace std;
uint8_t BookManager::distanceToTarget[1953125][10];
uint8_t BookManager::distanceToTargetZ[78125][10];
Language BookManager::lang;
const int BookManager::pow5[10] = { 1, 5, 25, 125, 625, 3125, 15625, 78125, 390625, 1953125 };
void BookManager::constructTarget(int quetou, int mentsu, queue<SparseSinglesOfColor>& q, SparseSinglesOfColor shape, int target, bool z) {
	SparseSinglesOfColor nextShape;
	if (quetou > 0) {
		for (auto value = 1; value <= (z ? 7 : 9); ++value) {
			nextShape = shape;
			const auto count = shape.get(value);
			if (count <= 2) {
				nextShape.add(value, 2);
				constructTarget(quetou - 1, mentsu, q, nextShape, target, z);
			}
		}
	}
	if (mentsu > 0) {
		//刻子
		for (auto value = 1; value <= (z ? 7 : 9); ++value) {
			nextShape = shape;
			if (nextShape.get(value) <= 1) {
				nextShape.add(value, 3);
				constructTarget(quetou, mentsu - 1, q, nextShape, target, z);
			}
		}
		//顺子
		if (!z) {
			for (auto value = 1; value <= 7; ++value) {
				nextShape = shape;
				const auto count0 = shape.get(value);
				const auto count1 = shape.get(value + 1);
				const auto count2 = shape.get(value + 2);
				if (count0 <= 3 && count1 <= 3 && count2 <= 3) {
					nextShape.add(value, 1);
					nextShape.add(value + 1, 1);
					nextShape.add(value + 2, 1);
					constructTarget(quetou, mentsu - 1, q, nextShape, target, z);
				}
			}
		}
	}
	if (mentsu > 0 || quetou > 0)return;
	q.push(shape);
	if (!z)distanceToTarget[CompactSinglesOfColor(shape).raw()][target] = 0;
	else distanceToTargetZ[CompactSinglesOfColor(shape).raw()][target] = 0;
}
//预处理，计算单花色离目标的距离(仅允许插入和删除两个操作)
void BookManager::preprocessDistance() {
	InfoPrinter::printInfoLine(lang.i_calculationStarted);
	for (auto i = 0; i < pow5[9]; ++i)
		for (auto j = 0; j < 10; ++j)
			distanceToTarget[i][j] = 255;
	for (auto i = 0; i < pow5[7]; ++i)
		for (auto j = 0; j < 10; ++j)
			distanceToTargetZ[i][j] = 255;
	//枚举目标
	for (auto target = 0; target < 10; ++target) {
		const auto jyantou = target / 5;
		const auto mentsu = target % 5;
		//构造目标(同时也是BFS的起点)
		std::queue<SparseSinglesOfColor> q;
		constructTarget(jyantou, mentsu, q, SparseSinglesOfColor(), target, false);
		//BFS
		while (!q.empty()) {
			const auto now = q.front();
			q.pop();
			for (auto value = 1; value <= 9; ++value) {
				SparseSinglesOfColor next;
				const auto count = now.get(value);
				if (count > 0) {
					next = now;
					next.add(value, -1);
					if (distanceToTarget[CompactSinglesOfColor(next).raw()][target] == 255) {
						distanceToTarget[CompactSinglesOfColor(next).raw()][target] = distanceToTarget[CompactSinglesOfColor(now).raw()][target] + 1;
						q.push(next);
					}
				}
				if (count < 4) {
					next = now;
					next.add(value, +1);
					if (distanceToTarget[CompactSinglesOfColor(next).raw()][target] == 255) {
						distanceToTarget[CompactSinglesOfColor(next).raw()][target] = distanceToTarget[CompactSinglesOfColor(now).raw()][target] + 1;
						q.push(next);
					}
				}
			}
		}
		//字牌
		constructTarget(jyantou, mentsu, q, SparseSinglesOfColor(), target, true);
		while (!q.empty()) {
			const auto now = q.front();
			q.pop();
			for (auto value = 1; value <= 7; ++value) {
				SparseSinglesOfColor next;
				const auto count = now.get(value);
				if (count > 0) {
					next = now;
					next.add(value, -1);
					if (distanceToTargetZ[CompactSinglesOfColor(next).raw()][target] == 255) {
						distanceToTargetZ[CompactSinglesOfColor(next).raw()][target] = distanceToTargetZ[CompactSinglesOfColor(now).raw()][target] + 1;
						q.push(next);
					}
				}
				if (count < 4) {
					next = now;
					next.add(value, +1);
					if (distanceToTargetZ[CompactSinglesOfColor(next).raw()][target] == 255) {
						distanceToTargetZ[CompactSinglesOfColor(next).raw()][target] = distanceToTargetZ[CompactSinglesOfColor(now).raw()][target] + 1;
						q.push(next);
					}
				}
			}
		}
	}
	InfoPrinter::printInfoLine(lang.i_calculationFinished);
	ofstream bookStream;
	bookStream.open("syanten.book", ios::out | ios::binary);
	if (!bookStream)InfoPrinter::printInfoLine(lang.i_saveUnsuccess);
	else {
		//存储内容:先存储魔数'B'，然后按顺序先存储普通花色的数据，再存储字牌数据
		char magicNumber = 'B';
		bookStream.write(&magicNumber, sizeof(char));
		for (auto i = 0; i < pow5[9]; ++i)
			for (auto j = 0; j < 10; ++j)
				bookStream.write((char*)(&(distanceToTarget[i][j])), sizeof(uint8_t));
		for (auto i = 0; i < pow5[7]; ++i)
			for (auto j = 0; j < 10; ++j)
				bookStream.write((char*)(&(distanceToTargetZ[i][j])), sizeof(uint8_t));
		bookStream.close();
		InfoPrinter::printInfoLine(lang.i_saveSuccess);
	}
}

void BookManager::initLanguage(){
	ifstream langStream;
	langStream.open("language/zh_ANSI/mahjong.lang", ios::in);
	if (!langStream) {
		cout << "Can't find language file:mahjong.lang" << endl;
	}
	else {
		for(auto i=0;i<4;++i)
			langStream >> lang.mj_wind[i];
		for (auto i = 0; i<3; ++i)
			langStream >> lang.mj_sangen[i];
		langStream >> lang.mj_han;
		langStream >> lang.mj_fu;
		langStream >> lang.mj_den;
		langStream >> lang.mj_yaku;
		for (auto i = 0; i<6; ++i)
			langStream >> lang.mj_yakuman[i];
		langStream >> lang.mj_get;
		langStream >> lang.mj_discard;
		langStream >> lang.mj_tsumogiri;
		langStream >> lang.mj_tsumo;
		langStream >> lang.mj_ron;
		langStream >> lang.mj_furikomu;
		langStream >> lang.mj_discardTile;
		langStream >> lang.mj_riichi;
		langStream >> lang.mj_ryuukyoku;
		langStream >> lang.mj_chii;
		langStream >> lang.mj_pon;
		langStream >> lang.mj_kan;
		langStream >> lang.mj_nuku;
		langStream >> lang.mj_standardType;
		langStream >> lang.mj_chiitoiType;
		langStream >> lang.mj_kokushiType;
		langStream >> lang.mj_syanten;
		langStream >> lang.mj_dora;
		langStream >> lang.mj_aka;
		langStream >> lang.mj_ura;
		langStream >> lang.mj_doraIndicator;
		langStream >> lang.mj_uraIndicator;
		langStream >> lang.mj_subround;
		langStream >> lang.mj_round;
		langStream >> lang.mj_remain;
		langStream >> lang.mj_tileMeasureWord;
		langStream >> lang.mj_honba;
		langStream >> lang.mj_jikaze;
		langStream >> lang.mj_bakaze;
		langStream >> lang.mj_direction;
		langStream >> lang.mj_furiten;
		langStream >> lang.mj_doujunFuriten;
		langStream >> lang.mj_riichiFuriten;
		langStream >> lang.mj_sutehaiFuriten;
		langStream.close();
	}
	langStream.open("language/zh_ANSI/control.lang", ios::in);
	if (!langStream) {
		cout << "Can't find language file:control.lang" << endl;
	}
	else {
		langStream >> lang.ct_gameStarted;
		langStream >> lang.ct_roundStarted;
		langStream >> lang.ct_gameOver;
		langStream >> lang.ct_roundOver;
		langStream >> lang.ct_makeChoice;
		langStream >> lang.ct_hasDiscard;
		for (auto i = 0; i<11; ++i)
			langStream >> lang.ct_action[i];
		langStream.close();
	}
	langStream.open("language/zh_ANSI/information.lang", ios::in);
	if (!langStream) {
		cout << "Can't find language file:information.lang" << endl;
	}
	else {
		langStream >> lang.i_tableNotFound;
		langStream >> lang.i_tableBroken;
		langStream >> lang.i_tableFound;
		langStream >> lang.i_tableLoaded;
		langStream >> lang.i_calculationStarted;
		langStream >> lang.i_calculationFinished;
		langStream >> lang.i_saveSuccess;
		langStream >> lang.i_saveUnsuccess;
		langStream.close();
	}
	langStream.open("language/zh_ANSI/yaku.lang", ios::in);
	if (!langStream) {
		cout << "Can't find language file:yaku.lang" << endl;
	}
	else {
		for (auto i = 0; i<52; ++i)
			langStream >> lang.y_yakuName[i];
		langStream.close();
	}
	langStream.open("language/zh_ANSI/tile.lang", ios::in);
	if (!langStream) {
		cout << "Can't find language file:tile.lang" << endl;
	}
	else {
		for (auto i = 0; i < 37; ++i)
			langStream >> lang.p_pai[i];
		langStream.close();
	}
}

void BookManager::init(){
	initLanguage();
	ifstream bookStream;
	bookStream.open("syanten.book", ios::in | ios::binary);
	if (!bookStream) {
		InfoPrinter::printInfoLine(lang.i_tableNotFound);
		preprocessDistance();
	}
	else {
		char magicNumber;
		bookStream.read(&magicNumber, sizeof(char));
		if (magicNumber!='B') {
			InfoPrinter::printInfoLine(lang.i_tableBroken);
			bookStream.close();
			preprocessDistance();
		}
		else {
			InfoPrinter::printInfoLine(lang.i_tableFound);
			for (auto i = 0; i < pow5[9]; ++i)
				for (auto j = 0; j < 10; ++j)
					bookStream.read((char*)(&(distanceToTarget[i][j])), sizeof(uint8_t));
			for (auto i = 0; i < pow5[7]; ++i)
				for (auto j = 0; j < 10; ++j)
					bookStream.read((char*)(&(distanceToTargetZ[i][j])), sizeof(uint8_t));
			InfoPrinter::printInfoLine(lang.i_tableLoaded);
			bookStream.close();
		}
	}
}
