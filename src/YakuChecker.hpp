#include"ResultInspector.hpp"
#include "Algorithms.h"
class YakuChecker
{
private:
	bool qingyise;
	bool menqing;
	int menqingCount;
public:
	YakuChecker();
	TryToAgariResult getResult(const AgariParameters& par, const std::vector<Group>& menqingMianzi);
};