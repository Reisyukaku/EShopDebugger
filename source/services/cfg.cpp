#include "services/cfg.hpp"

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

string CFG::GetLanguageCodeA2(void){
    u8 lang = 0;
    string code;
    string langList[] = {"ja", "en", "fr", "it", "es", "zh", "ko", "nl", "pt", "ru", "zh"};
    CFGU_GetSystemLanguage(&lang);
    if(!(lang >= 0xC))
        code = langList[lang];
    string str(code);
    return str;
}