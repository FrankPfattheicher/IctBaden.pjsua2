
#include <stdlib.h>
#include <cstring>
#include "../config_site.h"

using namespace std;
//using namespace pj;

extern "C"
{
	int main()
	{
		return 0;
	}
	
    extern void* CoTaskMemAlloc(long cb);

    PJ_EXPORT_SPECIFIER char* PjGetVersion()
    {
        // auto version = pj_get_version();
        // long ulSize = (long)(strlen(version) + sizeof(char));
        // auto pjVersion = (char*)CoTaskMemAlloc(ulSize);
        // strcpy(pjVersion, version);
        return (char*)"5665";	// pjVersion;
    }

}
