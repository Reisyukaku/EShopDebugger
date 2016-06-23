#include <3ds.h>
#include <string>
#include <cstdio>

using namespace std;

class ACT {
private:
    static bool isInit;
public:
    static bool GetInitState(void);
    static void SetInitState(bool);
    static u64 GetPrincipalId(void);
    static string GetNNID(void);
    static Result AquireIndepServiceToken(char *, const char *, u32, bool, bool, u32);
};