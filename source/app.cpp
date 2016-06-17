/*
*   app.cpp
*       by Reisyukaku
*   Copyright (c) 2015 All Rights Reserved
*/

#include "app.hpp"

void App::launch(u8 flags, u64 tid){
    u8 buf0[0x300];
    u8 buf1[0x20];
    memset(buf0, 0, 0x300);
    memset(buf1, 0, 0x20);
    
    aptOpenSession();
    APT_PrepareToDoAppJump(flags, tid, 0);
    APT_DoAppJump(0x300, 0x20, buf0, buf1);
    aptCloseSession();
}