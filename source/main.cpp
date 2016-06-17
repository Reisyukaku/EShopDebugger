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
#include "service.hpp"

using namespace ctr;

#define SDK(a,b,c,d) ((a<<24)|(b<<16)|(c<<8)|d)

//Settings
u32 font = 8;

int main(int argc, char **argv) {
    if(!ctr::core::init(argc)) return 0;
    
    //Service Init
    amInit();
    cfguInit();
    actInit(SDK(11,2,0,200), 0x20000);
    
    //Setup framebuffer
    gpu::setViewport(gpu::SCREEN_TOP, 0, 0, gpu::TOP_WIDTH, gpu::TOP_HEIGHT);
    gput::setOrtho(0, gpu::TOP_WIDTH, 0, gpu::TOP_HEIGHT, -1, 1);

    consoleInit(GFX_BOTTOM, NULL);
    printf("Firmware Version: %s", Service::getFirmVersion().c_str());
    printf("Debug Console: \n\n");

    while(ctr::core::running()) {
        hid::poll();
        if(core::launcher() && hid::pressed(hid::BUTTON_START)) break;

        //Refresh display
        Draw::consoleClear(200);
        Draw::printCTR(16, 0xDD8E15, " ~EShop Debugger Tool~");
        Draw::printCTR(font, 0xFFFFFF, "NNID: %s", (char*)Service::getNNID().c_str());
        Draw::printCTR(font, 0xFFFFFF, "Device ID: %lu", Service::getDeviceId());
        Draw::printCTR(font, 0xFFFFFF, "ECS Device ID: %llu", Service::getSoapId());
        Draw::printCTR(font, 0xFFFFFF, "Region: %s", (char*)Service::getConsoleRegion().c_str());
        Draw::printCTR(font, 0xFFFFFF, "Device Model: %s", (char*)Service::getConsoleModel().c_str());
        Draw::printCTR(font, 0xFFFFFF, "Device Serial: %s", (char*)Service::getSerialNum().c_str());
        Draw::printCTR(font, 0xFFFFFF, "Principal ID: %llu", Service::getPrincipalId());
        Draw::printCTR(font, 0xFFFFFF, "Device cert: ");
        Draw::printCTR(font, 0xFFFFFF, "%s", (char*)Service::getDeviceCert().c_str()); 
        Draw::flush();
        
        //Get HID
        switch(hid::anyPressed()){
            case hid::BUTTON_A:
                printf("Launching EShop...\n");
                App::launch(0, 0x0004001000021900LL);
                break;
            default:
                break;
        }
    }
    amExit();
    cfguExit();
    actExit();
    ctr::core::exit();
    return 0;
}