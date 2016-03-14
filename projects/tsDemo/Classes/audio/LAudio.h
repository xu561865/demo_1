//
//  LAudio.h
//  legend
//
//  Created by adminstrator on 13-6-19.
//
//

#ifndef __legend__LAudio__
#define __legend__LAudio__

#include <iostream>
#include "SimpleAudioEngine.h"
#include "LConstants.h"

class LAudio
{
public:
    static AudioMusic_t _curMusicType;
public:
    static void resumeAll();
    
    static void pauseAll();
    
    static void end();
    
    static void pauseMusic();
       
    static void pauseEffect();
    
    static void resumeMusic();
    
    static void resumeEffect();
    
    static void playMusic( AudioMusic_t eType );
    
    static unsigned int playEffect(AudioEffect_t eType, bool bLoop = false);
    static void stopEffect(unsigned int nSoundID);
    
    static void stopAllMusic();
    
    static void stopAllEffect();
    
    static void playSkillEffect(std::string name);
    
    static bool isCanPlay();
};



#endif /* defined(__legend__LAudio__) */
