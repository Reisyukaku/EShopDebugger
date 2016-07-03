#include <3ds.h>
#include <string>
#include <cstdio>

using namespace std;

class FRD {
public:
    static u64 PrincipalIdToFriendCode(u64);
    static FriendKey GetMyFriendKey(void);
};