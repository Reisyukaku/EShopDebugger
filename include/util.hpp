/*
*   misc.hpp
*       by Reisyukaku
*   Copyright (c) 2015 All Rights Reserved
*/

#pragma once

#include <string>
#include <cstdint>
#include <3ds.h>

using namespace std;

class Util {
public:
    static string base64Encode(u8 const* bytesToEnc, size_t bufLen);
};