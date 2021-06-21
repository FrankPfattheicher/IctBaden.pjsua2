
#include "library.h"

#include <stdlib.h>
#include <string.h>
#include <cstring>
#include <pjsua.h>
#include "../config_site.h"

extern "C"
{

    PJ_EXPORT_SPECIFIER char* PjGetVersion()
    {
        auto version = pj_get_version();
        auto ptrVersion = (char*)malloc(strlen(version) + 2);
        strcpy(ptrVersion, version);
        return ptrVersion;
    }

}
