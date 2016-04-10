//
//  Utils.cpp
//  tsDemo
//
//  Created by xuyi on 4/7/16.
//
//

#include <stdio.h>

namespace utils
{
    bool checkUserName(const char* userName)
    {
        bool ret = true;
        
        int i = 0, uIllegal = 0;
        while((userName[i]!='\0'))
        {
            if((userName[i]>='0'&&userName[i]<='9')||(userName[i]>='a'&&userName[i]<='z')||(userName[i]>='A'&&userName[i]<='Z'))
            {
                i++;
                continue;
            }
            else
            {
                i++;
                uIllegal++;
            }
        }
        
        
        if(uIllegal > 0)
        {
            ret = false;
        }
        
        return ret;
    }
    
    bool checkPwd(const char* password)
    {
        bool ret = true;
        
        int j = 0, pIllegal = 0;
        while(password[j]!='\0')
        {
            if((password[j]>='0'&&password[j]<='9')||(password[j]>='a'&&password[j]<='z')||(password[j]>='A'&&password[j]<='Z'))
            {
                j++;
                continue;
            }
            else
            {
                j++;
                pIllegal++;
            }
        }
        
        if(pIllegal > 0)
        {
            ret = false;
        }
        
        return ret;
    }
    
}