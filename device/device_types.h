enum radio_system_t {
    GSM,
    CDMA,
    LTE,
    NR
};

enum radio_bw_t {
    _5M,
    _10M,
    _15M,
    _20M
};

enum radio_io_mode_t {
    RD_IO,
    RD_Output,
    RD_Loop,
    RD_Input,
    RD_Close
};

enum radio_tx_source_t {
    RD_Phy,
    RD_ARB,
    RD_CW
};
