#include <3ds.h>
#include <string>
#include <cstdio>

using namespace std;

class AC {
private:
    static bool isNdmInit;
public:
    static Result CreateDefaultConfig(u8 *);
    static Result GetLastError(u32 *);
    static Result SetFromApplication(const u8 *, u8 *, u32);
    static Result ConnectAsyncWithoutEula(u8 *, Handle *);
    static Result ConnectWithoutEula(u8 *);
    static Result Connect(u8 *);
    static bool IsConnected(void);
    static Result CloseAsync(void);
    static Result GetStatus(u32 *);
};