/*
*   main.cpp
*       by Reisyukaku
*   Copyright (c) 2015 All Rights Reserved
*/

#include <3ds.h>
#include <cstdio>
#include <stdlib.h>
#include <string>
#include <citrus/core.hpp>
#include <citrus/gpu.hpp>
#include <citrus/gput.hpp>
#include <citrus/hid.hpp>

#include "draw.hpp"
#include "app.hpp"
#include "fs.hpp"
#include "util.hpp"

#include "services/ac.hpp"
#include "services/act.hpp"
#include "services/am.hpp"
#include "services/os.hpp"
#include "services/cfg.hpp"

using namespace ctr;

#define SDK(a,b,c,d) ((a<<24)|(b<<16)|(c<<8)|d)

//Settings
u32 font = 8;

int main(int argc, char **argv) {
    if(!ctr::core::init(argc)) return 0;
    
    //Service Init
    amInit();
    cfgInit();
    acInit();
    actInit(SDK(11,2,0,200), 0x20000);
    
    //Setup framebuffer
    gpu::setViewport(gpu::SCREEN_TOP, 0, 0, gpu::TOP_WIDTH, gpu::TOP_HEIGHT);
    gput::setOrtho(0, gpu::TOP_WIDTH, 0, gpu::TOP_HEIGHT, -1, 1);

    consoleInit(GFX_BOTTOM, NULL);
    printf("Firmware Version: %s", OS::GetFirmVersion().c_str());
    printf("Debug Console: \n\n");
    
    
    while(ctr::core::running()) {
        hid::poll();
        if(core::launcher() && hid::pressed(hid::BUTTON_START)) break;

        //Refresh display
        Draw::consoleClear(200);
        Draw::printCTR(16, 0xDD8E15, " ~EShop Debugger Tool~");
        Draw::printCTR(font, 0xFFFFFF, "NNID: %s", (char*)ACT::GetNNID().c_str());
        Draw::printCTR(font, 0xFFFFFF, "Device ID: %lu", AM::GetDeviceId());
        Draw::printCTR(font, 0xFFFFFF, "ECS Device ID: %llu", AM::GetSoapId());
        Draw::printCTR(font, 0xFFFFFF, "Region: %s", (char*)CFG::GetConsoleRegion().c_str());
        Draw::printCTR(font, 0xFFFFFF, "Device Model: %s", (char*)CFG::GetConsoleModel().c_str());
        Draw::printCTR(font, 0xFFFFFF, "Device Serial: %s", (char*)CFG::GetSerialNum().c_str());
        Draw::printCTR(font, 0xFFFFFF, "Principal ID: %llu", ACT::GetPrincipalId());
        Draw::printCTR(font, 0xFFFFFF, "Device cert: ");
        Draw::printCTR(font, 0xFFFFFF, "%s", (char*)AM::GetDeviceCert().c_str()); 
        Draw::flush();
        
        //Get HID
        switch(hid::anyPressed()){
            case hid::BUTTON_A:
            {
                printf("Launching EShop...\n");
                App::launch(0, 0x0004001000021900LL);
                break;
            }
            case hid::BUTTON_Y:
            {
                printf("Setting up access point...\n");
                Result ret = 0;
                u8 config[0x10];
                bool skipEula = true;
                
                AC_CreateDefaultConfig(config);
                if(skipEula){
                    ret = AC::ConnectWithoutEula(config);
                }else{
                    AC_SetFromApplication(config, config, AP_TYPE_USER_SETTING_2);
                    ret = AC::Connect(config);
                }
                
                if(R_SUCCEEDED(ret))
                    AC::SetInitState(true);
                printf("%s\n", ret == 0 ? "Access point connected!" : "Failed to setup access point!");
                if(R_FAILED(ret)) break;
                
                printf("Obtaining Ec token...\n");
                char token[0x100];
                u32 procId = 0;
                
                if(!AC::GetInitState()){
                    printf("AC isn't init. Aborting.\n");
                    break;
                }
                
                svcGetProcessId(&procId, 0xFFFF8001);
                if(R_SUCCEEDED(ACT::AquireIndepServiceToken(token, "71a6f5d6430ea0183e3917787d717c46", 0, 0, 1, procId))){
                    printf("\nEc Token:\n%s\n\n", token);
                }
                
                //TODO
                
                printf("Done!");
                break;
            }
            case hid::BUTTON_X:
            {
                
                break;
            }
            default:
                break;
        }
    }
    amExit();
    cfgExit();
    acExit();
    actExit();
    ctr::core::exit();
    return 0;
}