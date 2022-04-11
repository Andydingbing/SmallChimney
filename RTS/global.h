#ifndef RTS_GLOBAL_H
#define RTS_GLOBAL_H

#include "plugin.h"

enum Project {
    Ericsson_Radio_4415_B3,
    Ericsson_Radio_6449_B42,
    Ericsson_Air_3268_B47,
    StarPoint_SP9500,
    PROJECTS
};

enum Mode {
    Sequence,
    Manual
};

void searchPlugin(const char *path,std::list<std::string> &plugin);

extern Project project;
extern Mode mode;

#endif
