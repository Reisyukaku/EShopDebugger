#include "services/ac.hpp"

using namespace std;

bool AC::isNdmInit = false;

/*
*   Service call wrappers.
*/
Result AC::CreateDefaultConfig(u8 *config){
    Result ret = 0;
    if(config)
        ret = AC_CreateDefaultConfig(config);
    else
        ret = 0xE0409FFF;
    return ret;
}

Result AC::GetLastError(u32 *out){
    Result ret = 0;
    if(out)
        ret = AC_GetLastErrorCode(out);
    else
        ret = 0xE0409FFF;
    return ret;
}

Result AC::SetFromApplication(const u8 *configIn, u8 *configOut, u32 app){
    Result ret = 0;
    if(configIn)
        ret = AC_SetFromApplication(configIn, configOut, app);
    else
        ret = 0xE0409FFF;
    return ret;
}

Result AC::GetStatus(u32 *out){
    Result ret = 0;
    if(R_SUCCEEDED(ret = AC_GetStatus(out)))
        ret = *out;
    return ret;
}

/*
*   Misc wrappers.
*/
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
        else
            AC::isNdmInit = true;
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
        printf("Calling AC_ConnectAsync(): %s\n", ret == 0 ? "\x1b[32m SUCCESS!\x1b[0m" : "\x1b[31m FAIlED!\x1b[0m");
        ret = svcWaitSynchronization(event, U64_MAX);
    printf("Sync %s\n", ret == 0 ? "\x1b[32m SUCCESS!\x1b[0m" : "\x1b[31m FAIlED!\x1b[0m");
    
    //Dispose of event
    if(event){
        svcCloseHandle(event);
        event = 0;
    }
    
    if(AC::isNdmInit){
        ndmuLeaveExclusiveState();
        AC::isNdmInit = false;
        ndmuExit();
    }
    
    if(R_SUCCEEDED(ret)){
        ret = AC_GetConnectResult();
    }
    
    return ret;
}

Result AC::Connect(u8 *config){
    Result ret = 0;
    u8 eulaAgreed = 0;
    printf("Getting Eula Agreement...\n");
    CFG_GetConfig(4, 0xD0000, &eulaAgreed);
    if(eulaAgreed)
        ret = AC::ConnectWithoutEula(config);
    else
        ret = 0xC9209F84;
    return ret;
}

bool AC::IsConnected(void){
    u32 isCon = 0;
    AC_IsConnected(&isCon);
    return isCon == 0 ? false : true;
}

Result AC::CloseAsync(void){
    Result ret = 0;
    Handle event = 0;
    
    //Create event
    if(R_FAILED(osInitializeEvent(&event, 0))){
        ret = 0xF9609FF3;
        if(event)
            svcCloseHandle(event);
        return ret;
    }
    
    //Close
    if(event){
        ret = AC_CloseAsync(event);
    }else{
        ret = 0xE0409FFF;
    }
    printf("Calling AC_CloseAsync(): %s\n", ret == 0 ? "\x1b[32m SUCCESS!\x1b[0m" : "\x1b[31m FAIlED!\x1b[0m");
    
    if(R_SUCCEEDED(ret))
        ret = svcWaitSynchronization(event, U64_MAX);
    printf("Sync %s\n", ret == 0 ? "\x1b[32m SUCCESS!\x1b[0m" : "\x1b[31m FAIlED!\x1b[0m");
    
    //Dispose of event
    if(event){
        svcCloseHandle(event);
        event = 0;
    }
    
    if(R_SUCCEEDED(ret))
        ret = AC_GetCloseResult();
    
    return ret;
}