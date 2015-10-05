/*******************************************************************************
**                                  LibChaos                                  **
**                                  zjpeg.cpp                                 **
**                          See COPYRIGHT and LICENSE                         **
*******************************************************************************/
#include "zjpeg.h"
#include "zfile.h"
#include "zexception.h"

namespace LibChaos {

bool ZJPEG::decode(ZBinary &jpegdata_in, ReadOptions *options){

    return false;
}

bool ZJPEG::encode(ZBinary &jepgdata_out, WriteOptions *options){

    return false;
}

bool ZJPEG::read(ZPath path){
    ZBinary data;
    ZFile::readBinary(path, data);
    if(!data.size())
        throw ZException("JPEG Read: empty read file", JPEGError::badreadfile, false);

    return decode(data);
}

bool ZJPEG::write(ZPath path, JPEGWrite::jpegoptions options){
    ZBinary data;
    if(!encode(data))
        return false;

    if(!ZFile::writeBinary(path, data))
        throw ZException("JPEG Read: cannot write file", JPEGError::badwritefile, false);

    return true;
}

}