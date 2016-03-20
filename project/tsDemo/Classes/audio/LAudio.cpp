//
//  LAudio.cpp
//  legend
//
//  Created by adminstrator on 13-6-19.
//
//

#include "LAudio.h"
//#include "LMemory.h"
using namespace CocosDenshion;

bool g_skip_play =
#ifdef DEBUG
//true
false
#else
false
#endif
;

bool g_effect_paused = false;


AudioMusic_t LAudio::_curMusicType =kMusicOther;

void LAudio::resumeAll()
{
    if (g_skip_play) return;
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->resumeAllEffects();
    
    g_effect_paused = false;
}

void LAudio::pauseAll()
{
    if (g_skip_play) return;
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
    
    g_effect_paused = true;
}

void LAudio::pauseMusic()
{
    if (g_skip_play) return;
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

void LAudio::pauseEffect()
{
   if (g_skip_play) return;
   SimpleAudioEngine::sharedEngine()->pauseAllEffects();
    
    g_effect_paused = true;
}

void LAudio::resumeMusic()
{
    if (g_skip_play) return;
    
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

void LAudio::resumeEffect()
{
    if (g_skip_play) return;
    SimpleAudioEngine::sharedEngine()->resumeAllEffects();
    
    g_effect_paused = false;
}

void LAudio::end()
{
    if (g_skip_play) return;
    SimpleAudioEngine::sharedEngine()->end();
}

void LAudio::playMusic( AudioMusic_t eType )
{
    if (g_skip_play) return;
    
    if( _curMusicType==eType )
        return;
    
    //if( SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying() )
    //    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    
    _curMusicType = eType;
    
    if( eType == kMusicMain )
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic("bgm_main.mp3", true);
    else if( eType == kMusicBattle )
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic("bgm_battle.mp3", true);
    else if( eType == kMusicCampaign )
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic("bgm_campaign.mp3", true);
    else if( eType == kMusic5Card )
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic("bgm_5_card.mp3", true);
    else if( eType == kMusicFairyland )
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic("bgm_fairyland.mp3", true);
    else if( eType == kMusicLegendGod )
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic("bgm_legend_god.mp3", true);
    else if( eType == kMusicLegendCall )
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic("bgm_legend_call.mp3", true);
    else
        ;
}

void LAudio::stopAllMusic( )
{
    if (g_skip_play) return;
    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
}

void LAudio::stopAllEffect( )
{
    if (g_skip_play) return;
    SimpleAudioEngine::sharedEngine()->stopAllEffects();
}

unsigned int LAudio::playEffect(AudioEffect_t eType, bool bLoop)
{
    if (isCanPlay() == false)
    {
        return -1;
    }
    
    std::string snd_file = "";
    
    switch (eType)
    {
        case kEffectSpiritPsychicSurgery:
        {
            snd_file = "eft_spirit_psychic_surgery.mp3";
        }break;
            
        case kEffectSpiritEnergyFly:
        {
            snd_file = "eft_spirit_energy_fly.mp3";
        }break;
            
        case kEffectSpiritEnergyGot:
        {
            snd_file = "eft_spirit_energy_got.mp3";
        }break;
            
        case kEffectBattleWin:
        {
            snd_file = "battle_win.mp3";
        }break;
            
        case kEffectBattleDropDownWin:
        {
            snd_file = "battle_drop_down_win.mp3";
        }break;
            
        case kEffectBattleLose:
        {
            snd_file = "battle_lose.mp3";
        }break;
            
        case kEffectButtonDown:
        {
            snd_file = "button_down.mp3";
        }break;
            
        case kEffectBattleNinjaForward:
        {
            snd_file = "eft_battle_ninja_forward.mp3";
        }break;
            
        case kEffectBattleMoveNormal:
        {
            snd_file = "eft_battle_move_normal.mp3";
        }break;
            
        case kEffectBattleCardDie:
        {
            snd_file = "campaign_die.mp3";
        }break;
            
        case kEffectBattleResurrection:
        {
            snd_file = "campaign_revive.mp3";
        }break;
            
        case kEffectEnhanceMaterialRotating:
        {
            snd_file = "eft_enhance_material_ratating.mp3";
        }break;
            
        case kEffectEnhanceMaterialBullet:
        {
            snd_file = "eft_enhance_material_bullet.mp3";
        }break;
            
        case kEffectEnhanceTarget:
        {
            snd_file = "eft_enhance_target.mp3";
        }break;
            
        case kEffectEnhanceQuick:
        {
            snd_file = "enhance5.mp3";
        }break;
            
        case kEffectEvolveToTarget:
        {
            snd_file = "eft_evolve_to_target.mp3";
        }break;
            
        case kEffectEvolveMaterialDisappear:
        {
            snd_file = "eft_evolve_material_disappear.mp3";
        }break;
            
        case kEffectLegendCallBullet:
        {
            snd_file = "eft_legend_call_bullet.mp3";
        }break;
            
        case kEffectLegendCallCenter:
        {
            snd_file = "eft_legend_call_center.mp3";
        }break;
            
        case kEffectGemFuseStart:
        {
            snd_file = "eft_gem_fuse_start.mp3";
        }break;
            
        case kEffectGemFuseFlow:
        {
            snd_file = "eft_gem_fuse_flow.mp3";
        }break;
            
        case kEffectGemFuseEnd:
        {
            snd_file = "eft_gem_fuse_end.mp3";
        }break;
            
        case kEffectGemDrill:
        {
            snd_file = "eft_gem_drill.mp3";
        }break;
            
        case kEffectGemMount:
        {
            snd_file = "eft_gem_mount.mp3";
        }break;
            
        case kEffectGemUnmount:
        {
            snd_file = "eft_gem_mount.mp3";
        }break;
            
        case kEffectBuildingUnlock:
        {
            snd_file = "building_unlock.mp3";
        }break;
            
        case kEffectCountDown:
        {
            snd_file = "pack_countdown.mp3";
        }break;
            
        case kEffectShuffle1:
        {
            snd_file = "pack_shuffle_1.mp3";
        }break;
            
        case kEffectShuffle2:
        {
            snd_file = "pack_shuffle_2.mp3";
        }break;
            
        case kEffectTurnover:
        {
            snd_file = "pack_turnover.mp3";
        }break;
            
        case kEffectReward:
        {
            snd_file = "pack_reward.mp3";
        }break;
            
        case kEffectBanner:
        {
            snd_file = "buttonDown.mp3";
        }break;
            
        case kEffectOther:
        {
            snd_file = "buttonDown.mp3";
        }break;
            
        case kEffectActivityFirstOpen:
        {
            snd_file = "activityAndEliteOpen.mp3";
        }break;
            
        case kEffectEliteFirstOpen:
        {
            snd_file = "activityAndEliteOpen.mp3";
        }break;
            
        case kEffectCampaignNewStart:
        {
            snd_file = "campaignNewStart.mp3";
        }break;
            
        case kEffectCampaignNewEnd:
        {
            snd_file = "campaignNewEnd.mp3";
        }break;
            
        case kEffectCardDetailPage:
        {
            snd_file = "cardDetailPage.mp3";
        }break;
            
        case kEffectCardDroopDrag:
        {
            snd_file = "cardDroopDrag.mp3";
        }break;
            
        case kEffectFragmentFuse:
        {
            snd_file = "eft_fragmentFuse.mp3";
        }break;
            
        case kEffectCardBreakDownExploding:
        {
            snd_file = "cardBreakDownExploding.mp3";
        }break;
            
        case kEffectCardBreakDownFly:
        {
            snd_file = "eft_cardBreakDownFly.mp3";
        }break;
            
        case kEffectCardBreakDownCollect:
        {
            snd_file = "eft_cardBreakDownCollect.mp3";
        }break;
            
        case kEffectAlertTips:
        {
            snd_file = "alertTips.mp3";
        }break;
            
        case kEffectReceiveAward:
        {
            snd_file = "receiveAward.mp3";
        }break;
            
        case kEffectProtectTina1:
        {
            snd_file = "protect_tina_1.mp3";
        }break;
            
        case kEffectProtectTina2:
        {
            snd_file = "protect_tina_2.mp3";
        }break;
            
        case kEffectProtectTina3:
        {
            snd_file = "protect_tina_3.mp3";
        }break;
            
        case kEffectProtectTina4:
        {
            snd_file = "eft_protect_daming.mp3";
        }break;
            
        case kEffectProtectTinaActionStop:
        {
            snd_file = "protect_tina_view_action_stop.mp3";
        }break;
            
        case kEffectAngelBless1:
        {
            snd_file = "eft_yilelamian.mp3";
        }break;
            
        case kEffectAngelBless2:
        {
            snd_file = "bless_2.mp3";
        }break;
            
        case kEffectGuildCreate:
        {
            snd_file = "guildCreate.mp3";
        }break;
            
        case kEffectGuildDonate:
        {
            snd_file = "guildDonate.mp3";
        }break;
            
        case kEffectFuseSpirit:
        {
            snd_file = "eft_fuse_spirit.mp3";
        }break;
            
        case kEffectGuildUpgradeCash:
        {
            snd_file = "guildUpgradeCash.mp3";
        }break;
            
        case kEffectGuildUpgradeTech:
        {
            snd_file = "guildUpgradeTech.mp3";
        }break;
            
        case kEffectFragmentExplore:
        {
            snd_file = "eft_spirit_explore.mp3";
        }break;
            
        case kEffectFragmentObtain:
        {
            snd_file = "eft_spirit_obtain.mp3";
        }break;
            
        case kEffectDailyLoginFlip:
        {
            snd_file = "dailyLoginFlip.mp3";
        }break;
            
        case kEffectGuildUpgradeFail:
        {
            snd_file = "guildUpgradeFail.mp3";
        }break;
            
        case kEffectUnlock:
        {
            snd_file = "navUnlock.mp3";
        }break;
            
        case kEffectShowBattleCry:
        {
            snd_file = "show_battle_cry.mp3";
        }break;
            
        case kEffectRefreshBattleCry:
        {
            snd_file = "eft_battle_cry_refresh.mp3";
        }break;
            
        case kEffectAbility:
        {
            snd_file = "ability.mp3";
        }break;
            
        case kEffectLevyCoin:
        {
            snd_file = "eft_levy_coin.mp3";
        }break;
            
        case kEffectLevyFireworks:
        {
            snd_file = "eft_levy_fireworks.mp3";
        }break;
            
        case kEffectPartner:
        {
            snd_file = "partner.mp3";
        }break;
            
        case kEffectObtainNewCard:
        {
            snd_file = "obtain_new_card.mp3";
        }break;
            
        case kEffectRefreshInfluence:
        {
            snd_file = "refresh_influence.mp3";
        }break;
            
        case kEffectStudyInfluence:
        {
            snd_file = "study_influence.mp3";
        }break;
            
        case kEffectLuckyBoxOpen:
        {
            snd_file = "eft_lucky_box_open.mp3";
        }break;
            
        case kEffectLuckyBoxCardFly:
        {
            snd_file = "eft_lucky_box_card_fly.mp3";
        }break;
            
        case kEffectLuckyBoxSoulCard:
        {
            snd_file = "eft_lucky_box_soul_card.mp3";
        }break;
        case kEffectEnhanceEquip:
        {
            snd_file = "eft_equip_enhance.mp3";
        }break;
        case kEffectFuseEquipMaterial:
        {
            snd_file = "eft_fuse_equip_material.mp3";
        }break;
            
        default: break;
    } // end switch
    
    if (snd_file.empty())
    {
        return -1;
    }
    
    return SimpleAudioEngine::sharedEngine()->playEffect(snd_file.c_str(), bLoop);
}


void LAudio::stopEffect(unsigned int nSoundID)
{
    SimpleAudioEngine::sharedEngine()->stopEffect(nSoundID);
}


void LAudio::playSkillEffect(std::string name)
{
    if (isCanPlay() == false)
    {
        return ;
    }
    
    if (name.compare("NONE") == 0)
    {
        return;
    }
    
    std::string src = "skill_" + name + ".mp3";
    SimpleAudioEngine::sharedEngine()->playEffect(src.c_str());
}

bool LAudio::isCanPlay()
{
    // 避免android下返回游戏activity前回调出错的问题
    if (g_effect_paused)
    {
        return false;
    }
    
    if (g_skip_play)
    {
        return false;
    }
    
    return true;
}

