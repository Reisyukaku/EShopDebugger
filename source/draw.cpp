/*
*   draw.cpp
*       by Reisyukaku
*   Copyright (c) 2015 All Rights Reserved
*/

#include <stdio.h>
#include <stdarg.h>
#include <malloc.h>
#include <citrus/core.hpp>
#include <citrus/gpu.hpp>
#include <citrus/gput.hpp>

//Settings
u32 ypos = 0,
    xpos = 10;

using namespace ctr;

//Clear screen
void consoleClear(u32 startOff){
    gpu::clear();
    ypos = startOff;
}

void flush(void){
    gpu::flushCommands();
    gpu::flushBuffer();
    gpu::swapBuffers(true);
}

//Printf with color and font size~!
void printCTR(u32 font_pt, u32 color, std::string message, ...){
    va_list vl;
    va_start(vl, message);
    char *buffer = (char*)malloc(0x100); //Need to think of something better.
    vsnprintf (buffer, 0xFF, message.c_str(), vl);
    gput::drawString(buffer, xpos, ypos, font_pt, font_pt, (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF);
    ypos -= (10 + (gput::getStringHeight(buffer, font_pt)/2));
    va_end(vl);
}