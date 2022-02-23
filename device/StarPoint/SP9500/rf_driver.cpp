#include "rf_driver.h"
#include "gen_ini_file.inline.hpp"
#include "algo_math.inline.hpp"
#include "algo_chip_pll.inl.hpp"
#include "memory_physical.inline.hpp"
#include "memory_allocator.inline.hpp"

static const char *ver = "1.1.15330";

int32_t RF_DriverVersion(char **Version)
{
    *Version = const_cast<char *>(ver);
    return 0;
}
