/*
*   fs.cpp
*       by Reisyukaku
*   Copyright (c) 2015 All Rights Reserved
*/

#include <stdio.h>
#include <citrus/core.hpp>

void writeFile(const char *filename, void *buffer, u32 size){
    FILE *fp;
    fp = fopen(filename, "wb");
	fwrite(buffer, 1, size, fp);
    fclose(fp);
    
}