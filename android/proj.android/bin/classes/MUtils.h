//
//  MUtils.h
//  legend
//
//  Created by Xu Jinyang on 13-3-11.
//
//

#ifndef __mlib__MUtils__
#define __mlib__MUtils__

#include <iostream>
#include "mlib_helpers.h"
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;

MLIB_NS_BEGIN


long long get_milliseconds_since_boot();


float get_content_scale_factor();
cocos2d::CCSize get_design_resolution_size();
void initialize_resolution();

CCSpriteFrame * get_sprite_frame(const std::string & fileName);
void change_sprite_frame(cocos2d::CCSprite *sprite, std::string frameName);
int change_s9sprite_frame(cocos2d::extension::CCScale9Sprite *sprite, const std::string & fileName,
                          const CCSize & size, const CCRect &inset);
void image_to_gray(cocos2d::CCImage *pImage);
int change_sprite_texture_to_gray(cocos2d::CCSprite *sprite, const std::string & fileName);
cocos2d::CCImage * get_ccimage(std::string file);
cocos2d::CCImage * get_special_ccimage(std::string file, std::string opacityFile);
cocos2d::CCTexture2D * create_gray_texture(cocos2d::CCImage *pImage);
cocos2d::CCTexture2D * create_mask_texture_by_opacity(const std::string & file, const std::string & opacity_file, bool isGray = false);
int change_sprite_texture_by_mask(cocos2d::CCSprite * sprite, const std::string & file, const std::string & opacity_file, bool isGray = false);

cocos2d::CCArray * create_ccarray_without_copy(cocos2d::CCArray * array);
cocos2d::CCSet * create_ccset_without_copy(cocos2d::CCSet * set);

typedef int CCArray_Comparator_t(const void *, const void *);
void sort_ccarray(cocos2d::CCArray * array, CCArray_Comparator_t comparator);

void set_lock_screen(bool isLock);

cocos2d::CCNode * get_root_parent_of_node(cocos2d::CCNode * node);
float get_real_scale(cocos2d::CCNode * node);

float get_angle(const CCPoint & startPosition, const CCPoint & endPosition);

CCSize get_content_size(CCNode * node);

MLIB_NS_END

#endif /* defined(__mlib__MUtils__) */
