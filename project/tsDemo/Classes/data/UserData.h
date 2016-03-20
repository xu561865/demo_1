#ifndef __client1__UserData__
#define __client1__UserData__

#include <iostream>
using namespace std;

class UserData
{
    int _characterId;
    int _level;
    int _exp;
    int _gold;
    int _coin;
    int _tili;
    int _huoli;
    int _vipLevel;
    int _power;  //攻击力
    string _name;

public:
    UserData() : _characterId(0), _level(1), _exp(0), _gold(0), _coin(0), _tili(0), _huoli(0), _vipLevel(0), _power(0), _name("")
    {
    }
    
    int getLevel()
    {
        return _level;
    }
    
    int getExp()
    {
        return _exp;
    }
    
    int getGold()
    {
        return _gold;
    }
    
    int getCoin()
    {
        return _coin;
    }
    
    int getTili()
    {
        return _tili;
    }
    
    int getHuoli()
    {
        return _huoli;
    }
    
    int getVip()
    {
        return _vipLevel;
    }
    
    int getPower()
    {
        return _power;
    }
    
    string getName()
    {
        return _name;
    }
    
    void setCharacterId(int chId)
    {
        _characterId = chId;
    }
    
    void setLevel(int level)
    {
        _level = level;
    }
    
    void setExp(int exp)
    {
        _exp = exp;
    }
    
    void setGold(int gold)
    {
        _gold = gold;
    }
    
    void setCoin(int coin)
    {
        _coin = coin;
    }
    
    void setPower(int tili)
    {
        _tili = tili;
    }
    
    void setTili(int huoli)
    {
        _huoli = huoli;
    }
    
    void setName(string name)
    {
        _name = name;
    }
    
    int calPower();
};

#endif /* defined(__client1__UserData__) */
