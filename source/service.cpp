#include "service.hpp"

using namespace std;

string Service::getNNID(void){
    char tmp[0x11];
    ACT_GetAccountInfo(tmp, 0x11, 0x8);
    string str(tmp);
    return str;
}

u32 Service::getDeviceId(void){
    u32 tmp = 0;
    AM_GetDeviceId(&tmp);
    return tmp;
}

u64 Service::getSoapId(void){
    u32 tmp = 0;
    AM_GetDeviceId(&tmp);
    return tmp | (((u64) 4) << 32);
}

string Service::getFirmVersion(void){
    char tmp[0x20];
    osGetSystemVersionDataString(NULL, NULL, tmp, 0x20);
    string str = tmp;
    return str;
}

/*u8 *Service::getCTCert(void){
    u8 *tmp;//[0x180];
    //AMNet_GetDeviceCert(tmp);
    return tmp;
}*/

string Service::getSerialNum(void){
    char tmp[0x10];
    CFGS_SecureInfoGetSerialNo(tmp);
    string str = tmp;
    return str;
}

string Service::getConsoleRegion(void){
    u8 tmp;
    const string regions[] = {"JPN", "USA", "EUR", "AUS", "CHN", "KOR", "TWN"};
    CFGU_SecureInfoGetRegion(&tmp);
    return regions[tmp];
}

string Service::getConsoleModel(void){
    u8 tmp;
    const string models[] = {"CTR", "SPR", "KTR", "FTR", "RED"};
    CFGU_GetSystemModel(&tmp);
    return models[tmp];
}

u64 Service::getPrincipalId(void){
    u64 tmp = 0;
    ACT_GetAccountInfo(&tmp, 0x4, 0xC);
    return tmp;
}

u32 Service::getAge(void){
    u32 tmp = 0;
    ACT_GetAccountInfo(&tmp, 0x2, 0x2C);
    return tmp;
}