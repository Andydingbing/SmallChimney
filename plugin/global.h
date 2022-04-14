#ifndef PLUGIN_GLOBAL_H
#define PLUGIN_GLOBAL_H

#include "global_header.h"

#ifdef PLUGIN_DLL_EXPORT
    #define PLUGIN_EXPORT SYM_EXPORT
#else
    #define PLUGIN_EXPORT SYM_IMPORT
#endif


#if defined (__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

PLUGIN_EXPORT int32_t plugin_hash_generator();

#ifdef __cplusplus
}
#endif


#endif
