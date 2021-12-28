#include "pm_rs_nrp.h"
#include <cmath>
#include "rsnrpz.h"
#include "sleep_common.h"

using namespace std;

#ifdef NRP_DEBUG
#define NRP_CHECK(func) \
    do { \
        ViStatus ret; \
        char errMsg[256]; \
        ZeroMemory(errMsg, 256); \
        if ((ret = (func)) != 0) { \
            FILE *fp; \
            fopen_s(&fp, "C:\\nrp.log", "a"); \
            rsnrp_error_message(m_hVisa, ret, errMsg); \
            fprintf(fp, "F : %s\n\t%s\n", #func, errMsg); \
        } else { \
            printf("T : %s\n", #func); \
        } \
	} while (0);
#else
#define NRP_CHECK(func) func;
#endif

bool rs_nrp::init(const string &dev)
{
    ViSession session = 0;
    NRP_CHECK(rsnrpz_init(ViRsrc(dev.c_str()),false,false,&session));
    sleep_ms(100);
    m_session = session;
	return true;
}

bool rs_nrp::reset()
{
    NRP_CHECK(rsnrpz_status_preset(m_session));
    NRP_CHECK(rsnrpz_avg_setAutoEnabled(m_session,1,VI_FALSE));
    NRP_CHECK(rsnrpz_avg_setCount(m_session,1,3));
    NRP_CHECK(rsnrpz_trigger_setSource(m_session,1,RSNRPZ_TRIGGER_SOURCE_IMMEDIATE));
    NRP_CHECK(rsnrpz_reset(m_session));
    NRP_CHECK(rsnrpz_chans_initiate(m_session));
    sleep_ms(100);
	return true;
}

bool rs_nrp::set_freq(double freq)
{
    NRP_CHECK(rsnrpz_chan_setCorrectionFrequency(m_session,1,freq));
	return true;
}

bool rs_nrp::get_pwr(double &pwr)
{
    double meas = 0.0;
    ViBoolean meas_complete = VI_FALSE;

    NRP_CHECK(rsnrpz_chan_initiate(m_session, 1));
	do {
        sleep_ms(50);
        NRP_CHECK(rsnrpz_chan_isMeasurementComplete(m_session,1,&meas_complete));
    } while (meas_complete == VI_FALSE);

    NRP_CHECK(rsnrpz_meass_readMeasurement(m_session,1,10000,&meas));
    pwr = 10.0 * log10(fabs(meas * 1000.0));
	return true;	
}

bool rs_nrp::get_pwr(double freq,double &pwr)
{
    BOOL_CHECK(set_freq(freq));
    sleep_ms(10);
    BOOL_CHECK(get_pwr(pwr));
	return true;
}

bool rs_nrp::close()
{
    if (m_session) {
        rsnrpz_status_preset(m_session);
        rsnrpz_CloseSensor(m_session, 1);
        rsnrpz_close(m_session);
        m_session = 0;
	}
	return true;
}
