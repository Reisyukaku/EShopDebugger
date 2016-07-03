#include <3ds.h>
#include <string>
#include <cstdio>

using namespace std;

class ACT {
public:
    static u64 GetPrincipalId(void);
    static string GetNNID(void);
    static Result AquireIndepServiceToken(char *, const char *, u32, bool, bool);
};