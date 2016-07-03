#include "services/act.hpp"
#include "services/ac.hpp"

using namespace std;

u64 ACT::GetPrincipalId(void){
    u64 tmp = 0;
    ACT_GetAccountInfo(&tmp, 0x4, 0xC);
    return tmp;
}

string ACT::GetNNID(void){
    char tmp[0x11];
    ACT_GetAccountInfo(tmp, 0x11, 0x8);
    string str(tmp);
    return str;
}

Result ACT::AquireIndepServiceToken(char *tokenHandle, const char *clientId, u32 reusableRangeInSeconds, bool b1, bool b2){
    Result ret = (int)0xE0A14CC9;
    Handle event = 0;
    u32 status = 0;
    u32 procId = 0;
    
    svcGetProcessId(&procId, 0xFFFF8001);
    
    if(!AC::IsConnected())
        return 0xE0A14C67;
    
    ret = AC::GetStatus(&status);
    bool stat = ret == 2;
    if(ret != 2)
        stat = ret == 3;
    if(!stat)
        return 0xE0A14C67;
    
    osInitializeEvent(&event, 0);

    printf("Aquiring service token...");
    if(R_SUCCEEDED(ret = ACT_AcquireIndependentServiceToken(event, clientId, reusableRangeInSeconds, !b1, b2, procId))){
        svcWaitSynchronization(event, U64_MAX);
        if(R_SUCCEEDED(ACT_GetLastResponseResult()))
            ret = ACT_GetIndependentServiceToken(tokenHandle, false);
    }
    printf("%s\n", ret == 0 ? "\x1b[32m SUCCESS!\x1b[0m" : "\x1b[31m FAIlED!\x1b[0m");
    
    svcCloseHandle(event);
    event = 0;
    return ret;
}