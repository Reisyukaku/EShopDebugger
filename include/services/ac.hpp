#include <3ds.h>
#include <string>
#include <cstdio>

using namespace std;

class AC {
private:
    static bool isInit;
public:
    static bool GetInitState(void);
    static void SetInitState(bool);
    static Result ConnectAsyncWithoutEula(u8 *, Handle *);
    static Result ConnectWithoutEula(u8 *);
    static Result Connect(u8 *);
};