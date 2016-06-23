#include "ac.hpp"

using namespace std;

bool AC::isInit = false;

bool AC::GetInitState(void){
    return AC::isInit;
}

void AC::SetInitState(bool b){
    AC::isInit = b;
}

Result AC::ConnectAsyncWithoutEula(u8 *config, Handle *event){
    Result ret = -1;
    u32 priority = 0;
    
    printf("Connecting asynchronously without Eula...\n");
    if(R_FAILED(ret = AC_GetInfraPriority(config, &priority)))
        return ret;
    
    if(priority){
        AC_AddDenyApType(config, config, AP_TYPE_WIFI_STATION);
        return AC_ConnectAsync(config, *event);
    }
    if(R_SUCCEEDED(ndmuInit())){
        ret = ndmuEnterExclusiveState(EXCLUSIVE_STATE_INFRASTRUCTURE);
        if(R_FAILED(ret))
            ndmuExit();
    }
    if(R_SUCCEEDED(ret)){
        AC_SetRequestEulaVersion(config, config, 0, 0);
        return AC_ConnectAsync(config, *event);
    }
    return ret;
}

Result AC::ConnectWithoutEula(u8 *config){
    Result ret = 0;
    Handle event = 0;
    
    printf("Connecting without Eula...\n");
    
    //Create event
    if(R_FAILED(osInitializeEvent(&event, 0))){
        ret = 0xF9609FF3;
        if(event)
            svcCloseHandle(event);
            return ret;
    }
    
    //Connect asynchronously
    if(R_SUCCEEDED(AC::ConnectAsyncWithoutEula(config, &event)))
        ret = svcWaitSynchronization(event, U64_MAX);
    printf("Sync %s\n", ret == 0 ? "SUCCESSFUL!" : "FAILED!");
    
    //Dispose of event
    if(event){
        svcCloseHandle(event);
        event = 0;
    }
    
    //Return results
    ret = AC_GetConnectResult();
    return ret;
}

Result AC::Connect(u8 *config){
    Result ret = 0xC9209F84;
    u8 eulaAgreed = 0;
    printf("Getting Eula Agreement...\n");
    CFG_GetConfig(4, 0xD0000, &eulaAgreed);
    if(eulaAgreed)
        ret = AC::ConnectWithoutEula(config);
    return ret;
}