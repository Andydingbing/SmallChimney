#ifndef RD_TYPES_H
#define RD_TYPES_H

/*
 * RD(RF Driver) Types.
 *
 * 1, All types begin with the prefix "RD_".
 * 2, All enum variables begin with the prefix "RD_".
 * 3, Project related types or variables begin with "RD_##Project_".
 */

enum RD_Log_Switch {
    RD_Log_All_Off      = 0x00000000,
    RD_Log_Prompt       = 0x00000001,
    RD_Log_Message      = 0x00000004,
    RD_Log_Message_File = 0x00000008,
    RD_Log_Reg          = 0x00000010,
    RD_Log_Reg_File     = 0x00000020,
    RD_Log_All_On       = 0x7FFFFFFF
};

enum RD_IO_Mode {
    RD_IO,
    RD_Output,
    RD_Loop,
    RD_Input,
    RD_Close
};

enum RD_TX_Source {
    RD_Phy,
    RD_ARB,
    RD_CW
};

#endif
