#include "am.hpp"

using namespace std;

bool AM::isInit = false;

bool AM::GetInitState(void){
    return AM::isInit;
}

void AM::SetInitState(bool b){
    AM::isInit = b;
}

u32 AM::GetDeviceId(void){
    u32 tmp = 0;
    AM_GetDeviceId(&tmp);
    return tmp;
}

u64 AM::GetSoapId(void){
    u32 tmp = 0;
    AM_GetDeviceId(&tmp);
    return tmp | (((u64) 4) << 32);
}

string AM::GetDeviceCert(void){
    u8 cert[0x180];
    AMNet_GetDeviceCert(cert);
    return Util::base64Encode(cert, 0x180);
}