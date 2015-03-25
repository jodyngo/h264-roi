#ifndef ZRANDOM_H
#define ZRANDOM_H

#include "ztypes.h"
#include "zbinary.h"

#if PLATFORM == WINDOWS
    #include <windows.h>
#else
    #include "zfile.h"
#endif

namespace LibChaos {

class ZRandom {
public:
    ZRandom();
    ~ZRandom();

    zu64 genzu(zu64 min = 0, zu64 max = ZU64_MAX);
    ZBinary generate(zu64 size);

private:
#if PLATFORM == WINDOWS
    HCRYPTPROV _cryptprov;
#else
    FILE *_devrandom;
#endif
};

}

#endif // ZRANDOM_H