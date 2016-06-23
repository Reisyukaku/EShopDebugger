#include <3ds.h>
#include <string>
#include <cstdio>

using namespace std;

class OS {
public:
    static bool GetInitState(void);
    static void SetInitState(bool);
    static string GetFirmVersion(void);
};