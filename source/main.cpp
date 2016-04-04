/*
*   main.cpp
*       by Reisyukaku
*   Copyright (c) 2015 All Rights Reserved
*/

#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <malloc.h>
#include <citrus/core.hpp>
#include <citrus/gpu.hpp>
#include <citrus/gput.hpp>
#include <citrus/hid.hpp>

#include "draw.hpp"
#include "app.hpp"
#include "fs.hpp"
#include "misc.hpp"

using namespace ctr;

//Console info
u8  model,
    region;
u32 deviceid;
u64 soapid;

u8 *ctcert;
u8 *serial;
char *firmVersion;

const char *regions[] = {
    "JPN", "USA", "EUR", "AUS", "CHN", "KOR", "TWN"
};

const char *models[] = {
    "CTR", "SPR", "KTR", "FTR", "RED"
};

//Settings
u32 font = 8;

void fetchData(){
    //Device Cert
    ctcert = (u8*)malloc(0x200);
    AM_GetCTCert(ctcert);
    
    //Device serial
    serial = (u8*)malloc(0xF);
    CFGS_SecureInfoGetSerialNo(&serial);
    
    //Device ID
    AM_GetDeviceId(&deviceid);
    
    //SOAP ID
    soapid = deviceid | (((u64) 4) << 32);
    
    //Device region
    CFGU_SecureInfoGetRegion(&region);
    
    //Device model
    CFGU_GetSystemModel(&model);

    //Firm version
    firmVersion = (char*)malloc(0x20);
    osGetSystemVersionDataString(NULL, NULL, firmVersion, 0x20);
}

void flushData(){
    free(ctcert);
    free(serial);
    free(firmVersion);
}

void debugInit(void){
    consoleInit(GFX_BOTTOM, NULL);
    printf("Firmware Version: %s", firmVersion);
    printf("Debug Console: \n\n");
}

int main(int argc, char **argv) {
    if(!ctr::core::init(argc)) return 0;
    
    //Service Init
    amInit();
    frdInit();
    cfguInit();
    actInit();

    fetchData();
    
    //Setup framebuffer
    gpu::setViewport(gpu::SCREEN_TOP, 0, 0, gpu::TOP_WIDTH, gpu::TOP_HEIGHT);
    gput::setOrtho(0, gpu::TOP_WIDTH, 0, gpu::TOP_HEIGHT, -1, 1);

    debugInit();

    while(ctr::core::running()) {
        hid::poll();
        if(core::launcher() && hid::pressed(hid::BUTTON_START)) break;
       
        //Refresh display
        consoleClear(200);
        printCTR(16, 0xDD8E15, " ~EShop Debugger Tool~");
        //printCTR(font, 0xFFFFFF, "Name: %ls", screenName);
        printCTR(font, 0xFFFFFF, "Device ID: %lu", deviceid);
        printCTR(font, 0xFFFFFF, "ECS Device ID: %llu", soapid);
        printCTR(font, 0xFFFFFF, "Region: %s", regions[region]);
        printCTR(font, 0xFFFFFF, "Device Model: %s", models[model]);
        //printCTR(font, 0xFFFFFF, "Device Serial: %s", serial);
        printCTR(font, 0xFFFFFF, "Device Cert: ");
        printCTR(font, 0xFFFFFF, "%s", base64Encode(ctcert, 0x200));
        flush();
        
        //Get HID
        switch(hid::anyPressed()){
            case hid::BUTTON_A:
                printf("Launching EShop...\n");
                launchApp(0, 0x0004001000021900LL);
                break;
            case hid::BUTTON_B:
                printf("Dumping CTCert...\n");
                writeFile("/CTCert.bin", ctcert, 0x180);
                printf("Done!\n");
                break;
            default:
                break;
        }
    }
    flushData();
    amExit();
    frdExit();
    cfguExit();
    actExit();
    ctr::core::exit();
    return 0;
}