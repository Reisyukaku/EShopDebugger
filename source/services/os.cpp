#include "os.hpp"

using namespace std;

string OS::GetFirmVersion(void){
    char tmp[0x20];
    osGetSystemVersionDataString(NULL, NULL, tmp, 0x20);
    string str = tmp;
    return str;
}