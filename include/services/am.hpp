#include <3ds.h>
#include <string>
#include <cstdio>
#include "../util.hpp"

using namespace std;

class AM {
private:
    static bool isInit;
public:
    static bool GetInitState(void);
    static void SetInitState(bool);
    static u32 GetDeviceId(void);
    static u64 GetSoapId(void);
    static string GetDeviceCert(void);
};