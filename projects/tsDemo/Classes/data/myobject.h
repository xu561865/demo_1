#ifndef client1_myobject_h
#define client1_myobject_h

#include <iostream>
#include"cocos2d.h"
using namespace cocos2d;
using namespace std;

class MapItem:public CCObject
{
public:
    MapItem()
    {
        nickname = "";
        desc = "";
        bid = 0;
        mapId = 0;
        itemId = 0;
        enemy = "";
        enemydesc = "";
    }
    
    CCPoint pos;
    string nickname;
    string desc;
    string zj;
    string zjdesc;
    string enemy;
    string enemydesc;
    
    int bid;
    int mapId;
    int img;
    int exp;
    int coin;
    int itemId;
    
};


#endif
