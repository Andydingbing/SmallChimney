#ifndef RTS_GLOBAL_H
#define RTS_GLOBAL_H

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

extern Project project;
extern Mode mode;

#endif
