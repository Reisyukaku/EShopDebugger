#include "act.hpp"

using namespace std;

bool ACT::isInit = false;

bool ACT::GetInitState(void){
    return ACT::isInit;
}

void ACT::SetInitState(bool b){
    ACT::isInit = b;
}

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

Result ACT::AquireIndepServiceToken(char *tokenHandle, const char *clientId, u32 reusableRangeInSeconds, bool b1, bool b2, u32 procId){
    Result ret = 0;
    Handle event = 0;
    u32 acConn = 0;
    
    ret = osInitializeEvent(&event, 0);
    printf("osInitializeEvent: %s\n", ret == 0 ? "SUCCESS!" : "FAILURE!");
    ret = AC_IsConnected(&acConn);
    printf("AC connected: %s. It returned %d\n", ret == 0 ? "SUCCESS!" : "FAILURE!", (int)acConn);
    if(!acConn) 
        return 0xE0A14C67;

    if(R_SUCCEEDED(ret = ACT_AcquireIndependentServiceToken(event, clientId, reusableRangeInSeconds, !b1, b2, procId))){
        printf("Aquire indep token: %s\n", ret == 0 ? "SUCCESS!" : "FAILURE!");
        ret = svcWaitSynchronization(event, U64_MAX);
        printf("svcWaitSync %s\n", ret == 0 ? "SUCCESSFUL!" : "FAILURE!");
        if(R_SUCCEEDED(ret = ACT_GetLastResponseResult())){
            printf("Get last response result: %s\n", ret == 0 ? "SUCCESSFUL!" : "FAILURE!");
            ret = ACT_GetIndependentServiceToken(tokenHandle, false);
            printf("Getting token: %s\n", ret == 0 ? "SUCCESS!" : "FAILURE!");
        }
    }
    
    svcCloseHandle(event);    
    return ret;
}