#include "Yaku.h"

static constexpr const char* YakuName[] =
{
	"立直",
	"一发",
	"门前清自摸和",
	"断幺九",
	"役牌：自风牌",
	"役牌：场风牌",
	"役牌：白",
	"役牌：发",
	"役牌：中",
	"平和",
	"一杯口",
	"岭上开花",
	"海底捞月",
	"河底捞鱼",
	"两立直",
	"三色同刻",
	"三杠子",
	"对对和",
	"三暗刻",
	"小三元",
	"混老头",
	"七对子",
	"混全带幺九",
	"混全带幺九",
	"一气通贯",
	"一气通贯",
	"三色同顺",
	"三色同顺",
	"二杯口",
	"纯全带幺九",
	"纯全带幺九",
	"混一色",
	"混一色",
	"流局满贯",
	"清一色",
	"清一色",
	"天和",
	"地和",
	"大三元",
	"四暗刻",
	"字一色",
	"绿一色",
	"清老头",
	"国士无双",
	"小四喜",
	"四杠子",
	"四暗刻单骑",
	"国士无双十三面",
	"纯正九莲宝灯",
	"大四喜"
};

std::string_view getYakuName(const Yaku index) noexcept {
	const auto idx = yakuToIndex(index);
	return idx != 0xFFFFFFFF ? YakuName[idx] : std::string_view();
}
