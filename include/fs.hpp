/*
*   fs.hpp
*       by Reisyukaku
*   Copyright (c) 2015 All Rights Reserved
*/

#pragma once

#include <cstdio>
#include <3ds.h>

class FS {
public:
    static void WriteFile(const char *, void *, u32);
};