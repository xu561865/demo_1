//
//  MAnalysisManager.cpp
//  legend
//
//  Created by Minhao Zhang on 8/16/13.
//
//

#include "MAnalysisManager.h"

MAnalysisManager * MAnalysisManager::sharedInstance()
{
    static MAnalysisManager * inst = nullptr;
    if (inst == nullptr)
    {
        inst = new MAnalysisManager;
    }
    return inst;
}
