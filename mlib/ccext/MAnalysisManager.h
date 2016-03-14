//
//  MAnalysisManager.h
//  legend
//
//  Created by Minhao Zhang on 8/16/13.
//
//

#ifndef __legend__MAnalysisManager__
#define __legend__MAnalysisManager__

#include <string>

class MAnalysisManager
{
public:
    static MAnalysisManager * sharedInstance();
    
    void logEvent(const std::string& eventName);
    void logError(const std::string& eventName, const std::string& eventMessage);
};

#define ANALYSIS_MANAGER (MAnalysisManager::sharedInstance())

#endif /* defined(__legend__MAnalysisManager__) */
