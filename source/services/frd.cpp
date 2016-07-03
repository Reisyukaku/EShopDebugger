#include "services/frd.hpp"

using namespace std;

u64 FRD::PrincipalIdToFriendCode(u64 pid){
    u64 fc = 0;
    FRD_PrincipalIdToFriendCode(&fc, pid);
    return fc;
}

FriendKey FRD::GetMyFriendKey(void){
    FriendKey fk;
    FRD_GetMyFriendKey(&fk);
    return fk;
}