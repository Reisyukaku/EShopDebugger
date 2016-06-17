#include <3ds.h>
#include <string>

using namespace std;

class Service {
public:
    static string getNNID(void);
    static u32 getDeviceId(void);
    static u64 getSoapId(void);
    static string getFirmVersion(void);
    //static u8 *getCTCert(void);
    static string getSerialNum(void);
    static string getConsoleRegion(void);
    static string getConsoleModel(void);
    static u64 getPrincipalId(void);
    static u32 getAge(void);
};