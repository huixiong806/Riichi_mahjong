#include"yaku.h"
const std::unordered_map<int, const std::string> YakuName =
{
	std::unordered_map<int,const std::string>::value_type(0x110,"立直"),
	std::unordered_map<int,const std::string>::value_type(0x111,"一发"),
	std::unordered_map<int,const std::string>::value_type(0x112,"门前清自摸和"),
	std::unordered_map<int,const std::string>::value_type(0x100,"断幺九"),
	std::unordered_map<int,const std::string>::value_type(0x101,"役牌：自风牌"),
	std::unordered_map<int,const std::string>::value_type(0x102,"役牌：场风牌"),
	std::unordered_map<int,const std::string>::value_type(0x103,"役牌：白"),
	std::unordered_map<int,const std::string>::value_type(0x104,"役牌：发"),
	std::unordered_map<int,const std::string>::value_type(0x105,"役牌：中"),
	std::unordered_map<int,const std::string>::value_type(0x113,"平和"),
	std::unordered_map<int,const std::string>::value_type(0x114,"一杯口"),
	std::unordered_map<int,const std::string>::value_type(0x107,"岭上开花"),
	std::unordered_map<int,const std::string>::value_type(0x108,"海底捞月"),
	std::unordered_map<int,const std::string>::value_type(0x109,"河底捞鱼"),
	std::unordered_map<int,const std::string>::value_type(0x210,"两立直"),
	std::unordered_map<int,const std::string>::value_type(0x200,"三色同刻"),
	std::unordered_map<int,const std::string>::value_type(0x201,"三杠子"),
	std::unordered_map<int,const std::string>::value_type(0x202,"对对和"),
	std::unordered_map<int,const std::string>::value_type(0x203,"三暗刻"),
	std::unordered_map<int,const std::string>::value_type(0x204,"小三元"),
	std::unordered_map<int,const std::string>::value_type(0x205,"混老头"),
	std::unordered_map<int,const std::string>::value_type(0x211,"七对子"),
	std::unordered_map<int,const std::string>::value_type(0x206,"混全带幺九"),
	std::unordered_map<int,const std::string>::value_type(0x126,"混全带幺九"),
	std::unordered_map<int,const std::string>::value_type(0x207,"一气通贯"),
	std::unordered_map<int,const std::string>::value_type(0x127,"一气通贯"),
	std::unordered_map<int,const std::string>::value_type(0x208,"三色同顺"),
	std::unordered_map<int,const std::string>::value_type(0x128,"三色同顺"),
	std::unordered_map<int,const std::string>::value_type(0x310,"二杯口"),
	std::unordered_map<int,const std::string>::value_type(0x300,"纯全带幺九"),
	std::unordered_map<int,const std::string>::value_type(0x220,"纯全带幺九"),
	std::unordered_map<int,const std::string>::value_type(0x301,"混一色"),
	std::unordered_map<int,const std::string>::value_type(0x221,"混一色"),
	std::unordered_map<int,const std::string>::value_type(0x500,"流局满贯"),
	std::unordered_map<int,const std::string>::value_type(0x600,"清一色"),
	std::unordered_map<int,const std::string>::value_type(0x520,"清一色"),
	std::unordered_map<int,const std::string>::value_type(0xd00,"天和"),
	std::unordered_map<int,const std::string>::value_type(0xd01,"地和"),
	std::unordered_map<int,const std::string>::value_type(0xd02,"大三元"),
	std::unordered_map<int,const std::string>::value_type(0xd10,"四暗刻"),
	std::unordered_map<int,const std::string>::value_type(0xd03,"字一色"),
	std::unordered_map<int,const std::string>::value_type(0xd04,"绿一色"),
	std::unordered_map<int,const std::string>::value_type(0xd05,"清老头"),
	std::unordered_map<int,const std::string>::value_type(0xd11,"国士无双"),
	std::unordered_map<int,const std::string>::value_type(0xd06,"小四喜"),
	std::unordered_map<int,const std::string>::value_type(0xd12,"四杠子"),
	std::unordered_map<int,const std::string>::value_type(0xf10,"四暗刻单骑"),
	std::unordered_map<int,const std::string>::value_type(0xf11,"国士无双十三面"),
	std::unordered_map<int,const std::string>::value_type(0xf12,"纯正九莲宝灯"),
	std::unordered_map<int,const std::string>::value_type(0xf00,"大四喜"),
};
const std::string& getYakuName(int index)
{
	const std::unordered_map<int, const std::string>::const_iterator iter = YakuName.find(index);
	if (iter != YakuName.end())
	{
		return iter->second;
	}
	return "";
}