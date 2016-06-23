#include <3ds.h>
#include <string>
#include <cstdio>

using namespace std;

class CFG {
private:
    static bool isInit;
public:
    static bool GetInitState(void);
    static void SetInitState(bool);
    static string GetSerialNum(void);
    static string GetConsoleRegion(void);
    static string GetConsoleModel(void);
};