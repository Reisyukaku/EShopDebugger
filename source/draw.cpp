/*
*   draw.cpp
*       by Reisyukaku
*   Copyright (c) 2015 All Rights Reserved
*/

#include "draw.hpp"

using namespace ctr;

u32 Draw::ypos = 0, Draw::xpos = 10;

//Clear screen
void Draw::consoleClear(u32 startOff){
    gpu::clear();
    ypos = startOff;
}

void Draw::flush(void){
    gpu::flushCommands();
    gpu::flushBuffer();
    gpu::swapBuffers(true);
}

//Printf with color and font size~!
void Draw::printCTR(u32 font_pt, u32 color, std::string message, ...){
    auto temp = std::vector<char> {};
    auto len = std::size_t {63};
    std::va_list vl;
    while(temp.size() <= len){
        temp.resize(len+1);
        va_start(vl, message);
        const auto status = vsnprintf(temp.data(), temp.size(), message.c_str(), vl);
        va_end(vl);
        if(status < 0) printf("Error: string format error.\n");
        len = static_cast<std::size_t>(status);
    }
    std::string buffer = std::string {temp.data(), len};

    //Draw string
    int splitLen = (400/font_pt) - 1;
    int strAmount = buffer.length() / splitLen;
    std::vector<std::string> ret;
    for (auto i = 0; i < strAmount; i++) ret.push_back(buffer.substr(i * splitLen, splitLen));
    if (buffer.length() % splitLen != 0) ret.push_back(buffer.substr(splitLen * strAmount));

    for(auto retStr : ret) {
        if(ypos >= 10){
            gput::drawString(retStr.c_str(), xpos, ypos, font_pt, font_pt, (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF);
            ypos -= (10 + (gput::getStringHeight(retStr.c_str(), font_pt)/2));
        }
    }
}