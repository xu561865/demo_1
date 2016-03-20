#include "MIAPTask.h"

MLIB_NS_BEGIN

MIAPTask * MIAPTask::createWithProductId(const std::string &pid)
{
    return nullptr;
}

std::vector<MIAPTransaction> MIAPTask::getFinishedTransactions()
{
	std::vector<MIAPTransaction> ret;
	return ret;
}

MLIB_NS_END
