#ifndef _CCB_CCBREADER_H_
#define _CCB_CCBREADER_H_

#include "cocos2d.h"
#include "ExtensionMacros.h"
#include <string>
#include <vector>
#include "CCBSequence.h"


#define CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(T, METHOD) static T * METHOD() { \
    T * ptr = new T(); \
    if(ptr != NULL) { \
        ptr->autorelease(); \
        return ptr; \
    } \
    CC_SAFE_DELETE(ptr); \
    return NULL; \
}

#define CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(T, METHOD) static T * METHOD() { \
    T * ptr = new T(); \
    if(ptr != NULL && ptr->init()) { \
        ptr->autorelease(); \
        return ptr; \
    } \
    CC_SAFE_DELETE(ptr); \
    return NULL; \
}

#define kCCBVersion 5

enum {
    kCCBPropTypePosition = 0,
    kCCBPropTypeSize,
    kCCBPropTypePoint,
    kCCBPropTypePointLock,
    kCCBPropTypeScaleLock,
    kCCBPropTypeDegrees,
    kCCBPropTypeInteger,
    kCCBPropTypeFloat,
    kCCBPropTypeFloatVar,
    kCCBPropTypeCheck,
    kCCBPropTypeSpriteFrame,
    kCCBPropTypeTexture,
    kCCBPropTypeByte,
    kCCBPropTypeColor3,
    kCCBPropTypeColor4FVar,
    kCCBPropTypeFlip,
    kCCBPropTypeBlendmode,
    kCCBPropTypeFntFile,
    kCCBPropTypeText,
    kCCBPropTypeFontTTF,
    kCCBPropTypeIntegerLabeled,
    kCCBPropTypeBlock,
	kCCBPropTypeAnimation,
    kCCBPropTypeCCBFile,
    kCCBPropTypeString,
    kCCBPropTypeBlockCCControl,
    kCCBPropTypeFloatScale,
    kCCBPropTypeFloatXY
};

enum {
    kCCBFloat0 = 0,
    kCCBFloat1,
    kCCBFloatMinus1,
    kCCBFloat05,
    kCCBFloatInteger,
    kCCBFloatFull
};

enum {
    kCCBPlatformAll = 0,
    kCCBPlatformIOS,
    kCCBPlatformMac
};

enum {
    kCCBTargetTypeNone = 0,
    kCCBTargetTypeDocumentRoot = 1,
    kCCBTargetTypeOwner = 2,
};

enum
{
    kCCBKeyframeEasingInstant,
    
    kCCBKeyframeEasingLinear,
    
    kCCBKeyframeEasingCubicIn,
    kCCBKeyframeEasingCubicOut,
    kCCBKeyframeEasingCubicInOut,
    
    kCCBKeyframeEasingElasticIn,
    kCCBKeyframeEasingElasticOut,
    kCCBKeyframeEasingElasticInOut,
    
    kCCBKeyframeEasingBounceIn,
    kCCBKeyframeEasingBounceOut,
    kCCBKeyframeEasingBounceInOut,
    
    kCCBKeyframeEasingBackIn,
    kCCBKeyframeEasingBackOut,
    kCCBKeyframeEasingBackInOut,
};

enum
{
    kCCBPositionTypeRelativeBottomLeft,
    kCCBPositionTypeRelativeTopLeft,
    kCCBPositionTypeRelativeTopRight,
    kCCBPositionTypeRelativeBottomRight,
    kCCBPositionTypePercent,
    kCCBPositionTypeMultiplyResolution,
};

enum
{
    kCCBSizeTypeAbsolute,
    kCCBSizeTypePercent,
    kCCBSizeTypeRelativeContainer,
    kCCBSizeTypeHorizontalPercent,
    kCCBSizeTypeVerticalPercent,
    kCCBSizeTypeMultiplyResolution,
};

enum
{
    kCCBScaleTypeAbsolute,
    kCCBScaleTypeMultiplyResolution
};


NS_CC_EXT_BEGIN

/**
 * @addtogroup cocosbuilder
 * @{
 */

class CCBFile : public CCNode
{
private:
    CCNode *mCCBFileNode;
    
public:
    CCBFile();
    
    static CCBFile* create();
    
    CCNode* getCCBFileNode();
    void setCCBFileNode(CCNode *pNode); // retain
};

/* Forward declaration. */
class CCNodeLoader;
class CCNodeLoaderLibrary;
class CCNodeLoaderListener;
class CCBMemberVariableAssigner;
class CCBSelectorResolver;
class CCBAnimationManager;
class CCData;
class CCBKeyframe;

/**
 * @brief Parse CCBI file which is generated by CocosBuilder
 */
class CCBReader : public CCObject 
{
private:
    
    CCData *mData;
    unsigned char *mBytes;
    int mCurrentByte;
    int mCurrentBit;
    
    std::vector<std::string> mStringCache;
    std::set<std::string> mLoadedSpriteSheets;
    
    CCObject *mOwner;
    
    CCBAnimationManager *mActionManager; //retain
    CCDictionary* mActionManagers;

    std::set<std::string> *mAnimatedProps;

    CCNodeLoaderLibrary *mCCNodeLoaderLibrary;
    CCNodeLoaderListener *mCCNodeLoaderListener;
    CCBMemberVariableAssigner *mCCBMemberVariableAssigner;
    CCBSelectorResolver *mCCBSelectorResolver;
    
    std::vector<std::string> mOwnerOutletNames;
    CCArray* mOwnerOutletNodes;
    CCArray* mNodesWithAnimationManagers;
    CCArray* mAnimationManagersForNodes;
    
    std::vector<std::string> mOwnerCallbackNames;
    CCArray* mOwnerCallbackNodes;
    std::string mCCBRootPath;
    bool hasScriptingOwner;    
    bool init();
public:
    
    bool jsControlled;
    CCBReader(CCNodeLoaderLibrary *pCCNodeLoaderLibrary, CCBMemberVariableAssigner *pCCBMemberVariableAssigner = NULL, CCBSelectorResolver *pCCBSelectorResolver = NULL, CCNodeLoaderListener *pCCNodeLoaderListener = NULL);
    CCBReader(CCBReader *pCCBReader);
    virtual ~CCBReader();
    CCBReader();
   
    void setCCBRootPath(const char* pCCBRootPath);
    const std::string& getCCBRootPath() const;

    CCNode* readNodeGraphFromFile(const char *pCCBFileName);
    CCNode* readNodeGraphFromFile(const char *pCCBFileName, CCObject *pOwner);
    CCNode* readNodeGraphFromFile(const char *pCCBFileName, CCObject *pOwner, const CCSize &parentSize);
    
    CCNode* readNodeGraphFromData(CCData *pData, CCObject *pOwner, const CCSize &parentSize);
   
    CCScene* createSceneWithNodeGraphFromFile(const char *pCCBFileName);
    CCScene* createSceneWithNodeGraphFromFile(const char *pCCBFileName, CCObject *pOwner);
    CCScene* createSceneWithNodeGraphFromFile(const char *pCCBFileName, CCObject *pOwner, const CCSize &parentSize);

    CCBMemberVariableAssigner* getCCBMemberVariableAssigner();
    CCBSelectorResolver* getCCBSelectorResolver();
    
    CCBAnimationManager* getAnimationManager();
    void setAnimationManager(CCBAnimationManager *pAnimationManager);
    
    // Used in CCNodeLoader::parseProperties()
    std::set<std::string>* getAnimatedProperties();
    std::set<std::string>& getLoadedSpriteSheet();
    CCObject* getOwner();

    /* Utility methods. */
    static std::string lastPathComponent(const char* pString);
    static std::string deletePathExtension(const char* pString);
    static std::string toLowerCase(const char* pCCString);
    static bool endsWith(const char* pString, const char* pEnding);

    /* Parse methods. */
    int readInt(bool pSigned);
    unsigned char readByte();
    bool readBool();
    std::string readUTF8();
    float readFloat();
    std::string readCachedString();
    bool isJSControlled();
    
    bool readCallbackKeyframesForSeq(CCBSequence* seq);
    bool readSoundKeyframesForSeq(CCBSequence* seq);
    
    CCArray* getOwnerCallbackNames();
    CCArray* getOwnerCallbackNodes();
    CCArray* getOwnerOutletNames();
    CCArray* getOwnerOutletNodes();
    CCArray* getNodesWithAnimationManagers();
    CCArray* getAnimationManagersForNodes();

    CCDictionary* getAnimationManagers();
    void setAnimationManagers(CCDictionary* x);  // weak reference
    
    void addOwnerCallbackName(std::string name);
    void addOwnerCallbackNode(CCNode *node);
    
    void addDocumentCallbackName(std::string name);
    void addDocumentCallbackNode(CCNode *node);
    
    static float getResolutionScale();
    static void setResolutionScale(float scale);
    
    CCNode* readFileWithCleanUp(bool bCleanUp, CCDictionary* am);
    
    void addOwnerOutletName(std::string name);
    void addOwnerOutletNode(CCNode *node);
    CCNode * preLoadNode(CCNodeLoader *loader);

private:
    void cleanUpNodeGraph(CCNode *pNode);
    bool readSequences();
    CCBKeyframe* readKeyframe(int type);
    
    bool readHeader();
    bool readStringCache();
    //void readStringCacheEntry();
    CCNode* readNodeGraph();
    CCNode* readNodeGraph(CCNode * pParent);

    bool getBit();
    void alignBits();

    friend class CCNodeLoader;
};

// end of effects group
/// @}

NS_CC_EXT_END

#endif