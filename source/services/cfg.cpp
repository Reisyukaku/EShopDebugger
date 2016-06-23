#include "cfg.hpp"

using namespace std;

bool CFG::isInit = false;

bool CFG::GetInitState(void){
    return CFG::isInit;
}

void CFG::SetInitState(bool b){
    CFG::isInit = b;
}

string CFG::GetSerialNum(void){
    char tmp[0x10];
    CFGS_SecureInfoGetSerialNo(tmp);
    string str = tmp;
    return str;
}

string CFG::GetConsoleRegion(void){
    u8 tmp;
    const string regions[] = {"JPN", "USA", "EUR", "AUS", "CHN", "KOR", "TWN"};
    CFGU_SecureInfoGetRegion(&tmp);
    return regions[tmp];
}

string CFG::GetConsoleModel(void){
    u8 tmp;
    const string models[] = {"CTR", "SPR", "KTR", "FTR", "RED"};
    CFGU_GetSystemModel(&tmp);
    return models[tmp];
}