#include "services/nim.hpp"
#include "services/am.hpp"
#include <cstdio>

using namespace std;

string NIM::GetInfraDeviceId(void){
    u64 nimDeviceId = 0;
    char buffer[40];
    Result ret = NIMS_GetInfraDeviceId(AM::GetDeviceId(), &nimDeviceId);
    printf("Result of infra id: %X\n", (int)ret);
    snprintf(buffer, 40, "%llu", nimDeviceId);
    string str(buffer);
    return str;
}