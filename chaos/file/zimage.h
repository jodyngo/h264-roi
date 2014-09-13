#ifndef ZIMAGE_H
#define ZIMAGE_H

#include "ztypes.h"
#include <cstring>
//#include "zbitmap.h"
//#include "zpath.h"
//#include "zerror.h"
#include "zarray.h"

namespace LibChaos {

// Notes on Usage:
// Guarantee: buffer will never be allocated to a nonzero size different than size(). buffer may be null when size() is nonzero.
// size() returns the size of the image in bytes, but this does NOT guarantee the buffer() is allocated
// isLoaded() indicates size() is nonzero and buffer() is allocated, check this before using raw buffer() access
// setDimensions() sets the virtual size of the image, may delete buffer, but does not allocate buffer
//      check the actual dimensions after setting dimensions
//      partial dimensions are allowed
//      no image may be loaded unless all dimenions are valid
//      (channels * depth) % 8 must be zero. dimensions are invalid otherwise
// copyData() copies raw data of size() into buffer, buffer is allocated if necessary
// takeData() takes ownership of raw data. CAUTION: do not delete memory passed to takeData()

class ZImage {
public:
    typedef unsigned char byte;

public:
    ZImage() : _width(0), _height(0), _channels(0), _depth(0), _buffer(nullptr){

    }
    ZImage(zu64 width, zu64 height, zu8 channels = 3, zu8 depth = 8) : ZImage(){
        setDimensions(width, height, channels, depth);
    }
    ZImage(const byte *data, zu64 width, zu64 height, zu8 channels = 3, zu8 depth = 8) : ZImage(){
        setDimensions(width, height, channels, depth);
        copyData(data);
    }
    ZImage(const ZImage &other) : ZImage(other._buffer, other._width, other._height, other._channels, other._depth){

    }

    ~ZImage(){
        destroy();
    }
    void destroy(){
        _width = 0;
        _height = 0;
        _channels = 0;
        _depth = 0;
        delete[] _buffer;
        _buffer = nullptr;
    }

    ZImage &operator=(const ZImage &other){
        setDimensions(other._width, other._height, other._channels, other._depth);
        copyData(other._buffer);
        return *this;
    }

    byte &operator[](zu64 i){
        return _buffer[i];
    }
    const byte &operator[](zu64 i) const {
        return _buffer[i];
    }

    void setDimensions(zu64 width, zu64 height, zu8 channels, zu8 depth){
        // Basically, if new dimensions are invalid or result in a different *size()* changing, buffer is destroyed
        if(!width || !height || !channels || !depth || ((zu8)(channels * depth) % (zu8)8) != 0 || (width * height * ((zu8)(channels * depth) / (zu8)8)) != size())
            destroy();
        _width = width;
        _height = height;
        _channels = channels;
        _depth = depth;
    }

    void newData(){
        if(validDimensions()){
            if(!_buffer)
                _buffer = new byte[size()];
        }
    }
    void zeroData(){
        if(validDimensions()){
            if(!_buffer)
                _buffer = new byte[size()];
            memset(_buffer, 0, size());
        }
    }
    void copyData(const byte *data){
        if(validDimensions()){
            if(!_buffer)
                _buffer = new byte[size()];
            memcpy(_buffer, data, size());
        }
    }
    void takeData(byte *data){
        if(validDimensions()){
            // We totally trust the user here. Could go reeeaaalllyyy bad.
            _buffer = data;
        }
    }

    // Example: reformat({'R','G','B'}, {'R','G','B','A'});
    // each arbitrary char corresponds to a channel component of a pixel before and after the transform, in the order they are packed in buffer
    // the same char may not be used twice in <before>
    // channels corresponding to a char in <before> and <after> are reordered according to <after> in every pixel
    // channels corresponding to a char in <before> but not <after> are lost in every pixel
    // channels corresponding to a char in <after> but not <before> are zero in every pixel
    // after may be kept in the zimage as a hint about the image content
    void reformat(ZArray<char> before, ZArray<char> after){
        // we'll get to sub-byte channels later
        if(_depth % 8 == 0){

        }
    }

    void setChannels(zu8 channels, int fill = 0){
        if(channels != _channels){
            if(validDimensions()){
                ZImage temp(_width, _height, channels, _depth);
                if(temp.validDimensions()){
                    temp.zeroData();
                    if(_channels > channels){
                        zu8 cycle = 0;
                        for(zu64 i = 0, j = 0; i < temp.realSize() && j < realSize(); ++i, ++j, ++cycle){
                            if(cycle >= _channels){
                                j += _channels - channels;
                            }
                            temp[i] = _buffer[j];
                        }
                    }
                }
            }
            _channels = channels;
        }
    }

    bool validDimensions() const {
        return _width && _height && _channels && _depth && ((_channels * _depth) % 8) == 0 && size();
    }

    bool isLoaded() const {
        return validDimensions() && _buffer != nullptr;
    }

    zu64 width() const {
        return _width;
    }

    zu64 height() const {
        return _height;
    }

    zu8 channels() const {
        return _channels;
    }

    zu8 depth() const {
        return _depth;
    }

    zu8 pixelSize() const {
        return (_channels * _depth) / 8;
    }
    zu64 rowSize() const {
        return _width * pixelSize();
    }
    zu64 size() const {
        return _width * _height * pixelSize();
    }
    zu64 realSize(){
        return !_buffer ? 0 : size();
    }

    byte *buffer() const {
        return _buffer;
    }

public:
    zu64 _width, _height;
    zu8 _channels, _depth;
    byte *_buffer;
};

}

#endif // ZIMAGE_H
