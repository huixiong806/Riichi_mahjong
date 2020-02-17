#pragma once
#include <CGE.h>
#include <Console.h>
#include <string>
#include <iostream>
class InfoPrinter {
public:
	static void printColorStringLine(const std::string& info,Color foreColor, Color backColor=Color(0,0,0)) {
		Console::setColor(foreColor, backColor);
		std::cout << info << std::endl;
		Console::setColor(Color(192, 192, 192), Color(0, 0, 0));
	}
	static void printColorString(const std::string& info, Color foreColor, Color backColor = Color(0, 0, 0)) {
		Console::setColor(foreColor, backColor);
		std::cout << info;
		Console::setColor(Color(192, 192, 192), Color(0, 0, 0));
	}
	static void printControlInfoLine(const std::string& info) {
		printColorStringLine(info, Color(255, 0, 255));
	}
	static void printControlInfo(const std::string& info) {
		printColorString(info, Color(255, 0, 255));
	}
	static void printInfoLine(const std::string& info) {
		printColorStringLine(info, Color(255, 0, 0));
	}
	static void printInfo(const std::string& info) {
		printColorString(info, Color(255, 0, 0));
	}
};