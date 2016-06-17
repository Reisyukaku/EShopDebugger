/*
*   misc.cpp
*       by Reisyukaku
*   Copyright (c) 2015 All Rights Reserved
*/

#include "util.hpp"

using namespace std;

static const string base64_chars = 
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

string Util::base64Encode(u8 const* bytesToEnc, u32 bufLen) {
  string ret;
  int i = 0, j = 0;
  u8 temp[3];
  u8 str[4];

  while (bufLen--) {
    temp[i++] = *(bytesToEnc++);
    if (i == 3) {
      str[0] = (temp[0] & 0xfc) >> 2;
      str[1] = ((temp[0] & 0x03) << 4) + ((temp[1] & 0xf0) >> 4);
      str[2] = ((temp[1] & 0x0f) << 2) + ((temp[2] & 0xc0) >> 6);
      str[3] = temp[2] & 0x3f;

      for(i = 0; (i <4) ; i++) ret += base64_chars[str[i]];
      i = 0;
    }
  }

  if (i){
    for(j = i; j < 3; j++) temp[j] = '\0';

    str[0] = (temp[0] & 0xfc) >> 2;
    str[1] = ((temp[0] & 0x03) << 4) + ((temp[1] & 0xf0) >> 4);
    str[2] = ((temp[1] & 0x0f) << 2) + ((temp[2] & 0xc0) >> 6);
    str[3] = temp[2] & 0x3f;

    for (j = 0; (j < i + 1); j++) ret += base64_chars[str[j]];

    while((i++ < 3)) ret += '=';
  }

  return ret;
}