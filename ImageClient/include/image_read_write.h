#ifndef _IMAGE_READ_WRITE_H
#define _IMAGE_READ_WRITE_H

#include "globle_def.h"
#include <string>

class ImageRW
{
private:
    /* data */
    
public:
    ImageRW(/* args */);
    ~ImageRW();

    STATUS ReadImage(std::string dir, int num);

};




#endif