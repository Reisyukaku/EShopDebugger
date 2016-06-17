#include <3ds.h>
#include <string>
#include "util.hpp"

using namespace std;

class Service {
public:
    static string getNNID(void);
    static u32 getDeviceId(void);
    static u64 getSoapId(void);
    static string getFirmVersion(void);
    static string getDeviceCert(void);
    static string getSerialNum(void);
    static string getConsoleRegion(void);
    static string getConsoleModel(void);
    static u64 getPrincipalId(void);
};