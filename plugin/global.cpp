#include "global.h"

#ifdef COMPILER_MSC
#pragma data_seg("plugin_seg")
    static int32_t g_hash = 0;
#pragma data_seg()
#pragma comment(linker,"/section:plugin_seg,s")
#elif defined COMPILER_GCC
    static int32_t g_hash __attribute__((section("plugin_seg"))) = 0;
#endif

int32_t plugin_hash_generator()
{
    return ++ g_hash;
}
