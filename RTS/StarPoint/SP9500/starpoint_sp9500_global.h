#ifndef STARPOINT_SP9500_GLOBAL_H
#define STARPOINT_SP9500_GLOBAL_H

#include "../device/StarPoint/SP9500/rd_sp9500.h"
#include "../device/StarPoint/SP9500/sp1401.h"
#include "../device/StarPoint/SP9500/sp1401_r1a.h"
#include "../device/StarPoint/SP9500/sp1401_r1b.h"
#include "../device/StarPoint/SP9500/sp1401_r1c.h"
#include "../device/StarPoint/SP9500/sp1401_r1d.h"
#include "../device/StarPoint/SP9500/sp1401_r1e.h"
#include "../device/StarPoint/SP9500/sp1401_r1f.h"
#include "../device/StarPoint/SP9500/sp2401_r1a.h"
#include "../device/StarPoint/SP9500/sp3501.h"
#include "../device/StarPoint/SP9500/sp3301.h"
#include "../device/StarPoint/SP9500/cal_file.h"

#include "thread_widget.h"
#include "ui_adf5355.h"
#include "ui_starpoint_sp9500_sp1401_r1a.h"
#include "ui_starpoint_sp9500_sp1401_r1c.h"
#include "ui_starpoint_sp9500_sp1401_r1f.h"
#include "ui_starpoint_sp9500_sp1401_r1a_adv.h"
#include "ui_starpoint_sp9500_sp1401_r1c_adv.h"
#include "ui_starpoint_sp9500_sp1401_r1f_adv.h"
#include "ui_starpoint_sp9500_sp2401_r1a.h"
#include "ui_starpoint_sp9500_fpga.h"
#include "ui_starpoint_sp9500_cal_tx_lo_leak.h"

using namespace ns_sp9500;
using namespace ns_sp1401;
using namespace ns_sp2401;

extern sp3301 *SP3301;

extern sp1401 *SP1401;
extern sp1401_r1a *SP1401_R1A;
extern sp1401_r1b *SP1401_R1B;
extern sp1401_r1c *SP1401_R1C;
extern sp1401_r1d *SP1401_R1D;
extern sp1401_r1e *SP1401_R1E;
extern sp1401_r1f *SP1401_R1F;
extern sp2401_r1a *SP2401;

#endif
