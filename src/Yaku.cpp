#include "Yaku.h"
#include "BookManager.h"
std::string_view getYakuName(const Yaku index) noexcept {
	const auto idx = yakuToIndex(index);
	return idx != 0xFFFFFFFF ? BookManager::lang.y_yakuName[idx] : std::string_view();
}
