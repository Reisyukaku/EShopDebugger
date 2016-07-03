/*
*   main.cpp
*       by Reisyukaku
*   Copyright (c) 2015 All Rights Reserved
*/

#include <3ds.h>
#include <cstdio>
#include <stdlib.h>
#include <string>
#include <malloc.h>
#include <citrus/core.hpp>
#include <citrus/gpu.hpp>
#include <citrus/gput.hpp>
#include <citrus/hid.hpp>

#include "draw.hpp"
#include "fs.hpp"
#include "util.hpp"

#include "services/ac.hpp"
#include "services/act.hpp"
#include "services/am.hpp"
#include "services/os.hpp"
#include "services/cfg.hpp"
#include "services/frd.hpp"

using namespace ctr;

#define SDK(a,b,c,d) ((a<<24)|(b<<16)|(c<<8)|d)

int main(int argc, char **argv) {
    if(!ctr::core::initLight()) return 1;
    
    //Service Init
    srvInit();
    acInit(SDK(11,4,0,200));
    actInit(SDK(11,4,0,200), 0x20000);
    amInit();    
    cfgInit();
    httpcInit(0x9000);
    frdInit(SDK(11,4,0,200));
    
    //Setup framebuffer
    gpu::setViewport(gpu::SCREEN_TOP, 0, 0, gpu::TOP_WIDTH, gpu::TOP_HEIGHT);
    gput::setOrtho(0, gpu::TOP_WIDTH, 0, gpu::TOP_HEIGHT, -1, 1);
    consoleInit(GFX_BOTTOM, NULL);
    
    //Print debug info
    printf("Firmware Version: %s", OS::GetFirmVersion().c_str());
    printf("Debug Console: \n\n");
    
    //Init access point
    printf("Setting up access point...\n");
    Result ret = 0;
    u8 config[0x10];
    bool skipEula = true;
    
    if(AC::IsConnected()){
        printf("AC already init! Closing current connection...\n");
        AC::CloseAsync();
    }
    
    AC::CreateDefaultConfig(config);
    if(skipEula){
        ret = AC::ConnectWithoutEula(config);
    }else{
        AC::SetFromApplication(config, config, AP_TYPE_USER_SETTING_2);
        ret = AC::Connect(config);
    }
    if(R_FAILED(ret)){
        u32 errorCode = 0;
        AC::GetLastError(&errorCode);
        printf("Last error: 0x%X\n", errorCode);
        if ( ret == (int)0xC9209F84 ){
          printf("Error:\x1b[33m 0xC8A13865\x1b[0m\n");
        }else{
            if ( errorCode == 0x765B )
                printf("Error:\x1b[33m 0xC8A13812\x1b[0m\n");
            else
                printf("Error:\x1b[33m 0xC8A13813\x1b[0m\n");
        }
    }
    printf("%s\n", ret == 0 ? "Access point connected!" : "Failed to setup access point!");
    
    //Main program loop
    while(aptMainLoop()) {
        hid::poll();

        //Refresh display
        Draw::consoleClear(200);
        Draw::printCTR(16, 0xDD8E15, " ~EShop Debugger Tool~");
        Draw::printCTR(8, 0xFFFFFF, "NNID: %s", (char*)ACT::GetNNID().c_str());
        Draw::printCTR(8, 0xFFFFFF, "Device ID: %lu", AM::GetDeviceId());
        Draw::printCTR(8, 0xFFFFFF, "ECS Device ID: %llu", AM::GetSoapId());
        Draw::printCTR(8, 0xFFFFFF, "Region: %s", (char*)CFG::GetConsoleRegion().c_str());
        Draw::printCTR(8, 0xFFFFFF, "Device Model: %s", (char*)CFG::GetConsoleModel().c_str());
        Draw::printCTR(8, 0xFFFFFF, "Device Serial: %s", (char*)CFG::GetSerialNum().c_str());
        Draw::printCTR(8, 0xFFFFFF, "Principal ID: %llu", ACT::GetPrincipalId());
        Draw::printCTR(8, 0xFFFFFF, "Friend Code: %llu", FRD::PrincipalIdToFriendCode(FRD::GetMyFriendKey().principalId));
        Draw::printCTR(8, 0xFFFFFF, "Device cert: ");
        Draw::printCTR(8, 0xFFFFFF, "%s", (char*)AM::GetDeviceCert().c_str()); 
        Draw::flush();
        
        //Get HID
        switch(hid::anyPressed()){
            case hid::BUTTON_X:
            {
                printf("Obtaining Ec token...\n");
                Result ret = 0;
                char ecToken[0x100];
                const char *serviceToken = "71a6f5d6430ea0183e3917787d717c46";
                
                if(R_SUCCEEDED(ret = ACT::AquireIndepServiceToken(ecToken, serviceToken, 0, 0, 1))){
                    printf("\nEc Token:\n%s\n\n", ecToken);
                }
                break;
            }
            case hid::BUTTON_Y:
            {
                printf("Obtaining Olv token...\n");
                Result ret = 0;
                char olvToken[0x100];
                const char *serviceToken = "87cd32617f1985439ea608c2746e4610";

                if(R_SUCCEEDED(ret = ACT::AquireIndepServiceToken(olvToken, serviceToken, 0, 0, 1))){
                    printf("\nOlv Token:\n%s\n\n", olvToken);
                }
                break;
            }
            case hid::BUTTON_DRIGHT:
            {

                break;
            }
            default:
                break;
        }
    }
    
    srvExit();
    acExit();
    actExit();
    amExit();
    cfgExit();
    httpcExit();
    frdExit();
    ctr::core::exitLight();
    
    return 0;
}