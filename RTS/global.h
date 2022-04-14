#ifndef RTS_GLOBAL_H
#define RTS_GLOBAL_H

#include "plugin.h"

enum Mode {
    Sequence,
    Manual
};

void searchPlugin(const char *path,std::list<std::string> &plugin);

extern Mode mode;

#endif
