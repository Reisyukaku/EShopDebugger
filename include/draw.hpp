/*
*   draw.hpp
*       by Reisyukaku
*   Copyright (c) 2015 All Rights Reserved
*/ 
#pragma once

#include <cstdio>
#include <string>
#include <cstdarg>
#include <vector>
#include <cstddef>
#include <citrus/core.hpp>
#include <citrus/gpu.hpp>
#include <citrus/gput.hpp>

class Draw {
private:
    static u32 ypos, xpos;
public:
    static void consoleClear(u32);
    static void flush(void);
    static void printCTR(u32, u32, std::string, ...);
};