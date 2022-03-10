#include "../sp9500/dma_mgr.h"
#include "../chip/reg_def_sp9500.h"
#include "sleep_common.h"
#include "pci_dev_vi.h"
#include "mem_io_vi.h"
#include <boost/timer/timer.hpp>

using namespace ns_starpoint;
using namespace ns_sp9500;

dma_mgr::dma_mgr(pci_dev_vi *k7) : _k7(k7)
{
	m_dmar = new vi_mem_io;
    for (uint32_t i = 0; i < R_BLOCKS; i++) {
		m_dmaw[i] = new vi_mem_io;
    }

	uint32_t max_block_spls = T_BLOCK_SAMPLES >= R_BLOCK_SAMPLES ? T_BLOCK_SAMPLES : R_BLOCK_SAMPLES;
    m_usr_spc = new uint32_t[max_block_spls];
}

dma_mgr::~dma_mgr()
{
    SAFE_DEL(m_usr_spc);
}

uint32_t *dma_mgr::usr_spc() const
{
	return m_usr_spc;
}

int32_t dma_mgr::init()
{
    RFU_K7_REG_DECLARE(0x10cf);

    RFU_K7_REG(0x10cf).cap_tlp_count = 4096; // ((368.64 * 1e3) /32 )/ 2
    RFU_K7_W(0x10cf);

	INT_CHECK(r_alloc());
	INT_CHECK(w_alloc());
	INT_CHECK(r_addr2fpga());
	INT_CHECK(w_addr2fpga());

    INT_CHECK(fpga_w_abort());
    INT_CHECK(set_fpga_w_samples(245760));
	return 0;
}

int32_t dma_mgr::fpga_r(uint32_t length)
{
    RFU_K7_REG_DECLARE(0x0001);
    RFU_K7_REG_DECLARE(0x0008);
    RFU_K7_REG_DECLARE(0x0009);
    RFU_K7_REG_DECLARE(0x0021);

    bool is_complete = false;

    INT_CHECK(m_dmar->w32(m_usr_spc, length));

//	Log.stdprintf("fpga prepare read samples:%d\n",uiLength);

    RFU_K7_REG(0x0008).tlp_size = 16;
    RFU_K7_W(0x0008);

    RFU_K7_REG(0x0009).tlp_count = length / 16;
    RFU_K7_W(0x0009);

    INT_CHECK(reset());
    RFU_K7OP(0x0001, fpga_rd_op);

	/* wait interrupt
	for (int i = 0; i < 3; i++) {
		ret = WaitForSingleObject(g_Event, 1000);
		Log.stdprintf("WaitForSingleObject:%d\n",ret);

		CSE_K7_R(0x0021);
		Log.stdprintf("REG(0x0021) = %d\n",REG(0x0021).dma_rd_counter);
		if (ret != WAIT_TIMEOUT)
		break;
	}*/

    unsigned bytes_cnt[5] = {0};
	while(1) {
		for (int i = 0;i < 5;i ++) {
            RFU_K7_R(0x0021);
            bytes_cnt[i] = RFU_K7_REG(0x0021).dma_rd_counter;
            if (bytes_cnt[i] == length) {
                is_complete = true;
				break;
			}
		}
        if (is_complete) {
			break;
        } else if (bytes_cnt[4] == bytes_cnt[3]) {
            Log.set_last_err("stop@sample:%d",bytes_cnt[4]);
            Log.stdprintf("\n%s\n",Log.last_err());
			return -1;
		}
	}

//	Log.stdprintf("timer = %lu\n", REG(0x0020).dma_rd_timer);
	return 0;
}

int32_t dma_mgr::fpga_w()
{
    INT_CHECK(fpga_w_start());
    int32_t res = fpga_w_trans();
    INT_CHECK(fpga_w_abort());
    return res;
}

int32_t dma_mgr::fpga_w_start()
{
    RFU_K7_REG_DECLARE(0x0001);
    RFU_K7_REG_DECLARE(0x0025);
    RFU_K7_REG_DECLARE(0x10c4);
    RFU_K7_REG_DECLARE(0x10c5);

    RFU_K7_REG(0x10c5).abort = 0;
    RFU_K7_W(0x10c5);

    RFU_K7_OP(0x10c4);
    RFU_K7OP(0x0001,fpga_wr_op);

    RFU_K7_REG(0x0025).op_0 = 1;
    RFU_K7_W(0x0025);
    return 0;
}

int32_t dma_mgr::fpga_w_trans()
{
    RFU_K7_REG_DECLARE(0x0014);
    RFU_K7_REG_DECLARE(0x0025);
    RFU_K7_REG_DECLARE(0x0030);

    RFU_K7_WAIT_IDLE(0x0030,1,INT32_MAX);

    RFU_K7_REG(0x0025).op_1 = 1;
    RFU_K7_W(0x0025);

    uint32_t sample_current[5] = { 0 };
    uint32_t sample_total = 0;
    uint32_t time_us = 0;
    bool is_timeout = false;
    boost::timer::cpu_timer time;

    get_fpga_w_samples(sample_total);
    sample_total = sample_total << 1;

    while (1) {
        time_us = uint32_t(time.elapsed().wall / 1000);
        is_timeout = time_us > 6000000;
//		INT_CHECK(is_iq_cap_timeout(timeout));

        for (uint32_t i = 0; i < 5; i++) {
            RFU_K7_R(0x0014);
            sample_current[i] = RFU_K7_REG(0x0014).samples;
        }
        if (!is_timeout) {
            if (sample_current[4] == sample_total)	//success
                return 0;
            if (sample_current[4] == sample_current[3] && sample_current[4] < sample_total) {
                Log.set_last_err("stop at samples:%d(need samples:%d)",sample_current[4],sample_total);
                return -1;
            }
            if (sample_current[4] == sample_current[3] && sample_current[4] > sample_total) {
                Log.set_last_err("exceed,samples:%d(need samples%d)",sample_current[4],sample_total);
                return -1;
            }
        } else {
            if (sample_current[0] < sample_current[1])
                continue;
            if (sample_current[4] == sample_total)	//also success
                return 0;
            if (sample_current[4] == sample_current[3] && sample_current[4] < sample_total) {
                Log.set_last_err("stop at samples:%d(need samples:%d)", sample_current[4], sample_total);
                return -1;
            }
            if (sample_current[4] == sample_current[3] && sample_current[4] > (sample_total)) {
                Log.set_last_err("exceed,samples:%d(need samples%d)", sample_current[4], sample_total);
                return -1;
            }
        }
    }
}

int32_t dma_mgr::fpga_w_abort()
{
    RFU_K7_REG_DECLARE(0x0025);
    RFU_K7_REG_DECLARE(0x10c5);

    RFU_K7_REG(0x10c5).abort = 1;
    RFU_K7_W(0x10c5);
    RFU_K7_W(0x0025);
    INT_CHECK(reset());
    return 0;
}

int32_t dma_mgr::set_fpga_w_samples(uint32_t samples)
{
    RFU_K7_REG_DECLARE(0x10c9);
    RFU_K7_REG_DECLARE(0x10c6);
    RFU_K7_REG_DECLARE(0x10cb);

    RFU_K7_REG(0x10c9).data = 1;
    RFU_K7_W(0x10c9);
    RFU_K7_REG(0x10c6).samples = samples << 1;
    RFU_K7_W(0x10c6);
    RFU_K7_REG(0x10cb).data = samples;
    RFU_K7_W(0x10cb);
	return 0;
}

int32_t dma_mgr::get_fpga_w_samples(uint32_t &samples)
{
	RFU_K7_REG_DECLARE(0x10c6);
	RFU_K7_R(0x10c6);
    samples = RFU_K7_REG_DATA(0x10c6);
	samples /= 2;
	return 0;
}

int32_t dma_mgr::dump2buf(int32_t *data, uint32_t samples)
{
    uint32_t data_idx = 0;
    uint32_t total_blocks = samples / R_BLOCK_SAMPLES;
    uint32_t samples_left = samples;
    uint32_t samples_reading = 0;
    uint32_t samples_last_block = samples % R_BLOCK_SAMPLES;

    if (samples_last_block != 0) {
        total_blocks ++;
    }

    for (uint32_t i = 0;i < total_blocks;i ++) {
        samples_reading = (samples_left > int(R_BLOCK_SAMPLES) ? int(R_BLOCK_SAMPLES) : samples_left);
        INT_CHECK(m_dmaw[i]->r32(m_usr_spc,samples_reading));

        for (uint32_t j = 0;j < samples_reading;j ++) {
            data_idx = i * R_BLOCK_SAMPLES + j;
            data[data_idx] = m_usr_spc[j];
		}
        samples_left -= samples_reading;
	}
	return 0;
}

int32_t dma_mgr::dump2buf_ch(uint32_t rf_idx,int32_t *data,uint32_t samples,int32_t *data_2)
{
    samples *= 2;
    uint32_t data_idx = 0;
    uint32_t package_idx = (rf_idx + 1) % 2;
    uint32_t package_idx_inter_ch = (0 == package_idx ? 1 : 0);
    uint32_t total_blocks = samples / R_BLOCK_SAMPLES;
    uint32_t samples_left = samples;
    uint32_t samples_reading = 0;
    uint32_t samples_last_block = samples % R_BLOCK_SAMPLES;

    if (samples_last_block != 0) {
        total_blocks ++;
    }

    iq_data_t iq_data_this_ch;
    iq_data_t iq_data_2;

    for (uint32_t i = 0;i < total_blocks;i ++) {
        samples_reading = (samples_left > int(R_BLOCK_SAMPLES) ? int(R_BLOCK_SAMPLES) : samples_left);
        INT_CHECK(m_dmaw[i]->r32(m_usr_spc,samples_reading));

        for (uint32_t j = 0;j < samples_reading;j += 2) {
            data_idx = (i * R_BLOCK_SAMPLES + j) / 2;

            iq_data_this_ch = ((w_package_t *)(&m_usr_spc[j]))->data[package_idx];
            data[data_idx] = *(int32_t *)(&iq_data_this_ch);

            if (data_2) {
                iq_data_2 = ((w_package_t *)(&m_usr_spc[j]))->data[package_idx_inter_ch];
                data_2[data_idx] = *(int32_t *)(&iq_data_2);
			}
		}
        samples_left -= samples_reading;
	}
	return 0;
}

int32_t dma_mgr::dump2file_ch(uint32_t rf_idx,char *path,uint32_t samples,char *path_2)
{
    samples *= 2;
    uint32_t package_idx = (rf_idx + 1) % 2;
    uint32_t package_idx_inter_ch = (0 == package_idx ? 1 : 0);
    uint32_t total_blocks = samples / R_BLOCK_SAMPLES;
    uint32_t samples_left = samples;
    uint32_t samples_reading = 0;
    uint32_t samples_last_block = samples % R_BLOCK_SAMPLES;

    if (samples_last_block != 0) {
        total_blocks ++;
    }

    FILE *fp = fopen(path,"w");
    if (nullptr == fp) {
        Log.set_last_err("can't open %s",path);
		return -1;
	}
    FILE *fp_interch = nullptr;
    if (path_2) {
        fp_interch = fopen(path_2,"w");
        if (nullptr == fp_interch) {
            Log.set_last_err("can't open %s",path_2);
			return -1;
		}
	}

    iq_data_t iq_data_this_ch;
    iq_data_t iq_data_2;
    int32_t i_data_this_ch;
    int32_t i_data_2;

    for (uint32_t i = 0;i < total_blocks;i ++) {
        samples_reading = (samples_left > int(R_BLOCK_SAMPLES) ? int(R_BLOCK_SAMPLES) : samples_left);
        INT_CHECK(m_dmaw[i]->r32(m_usr_spc,samples_reading));

        for (uint32_t j = 0;j < samples_reading;j += 2) {
            iq_data_this_ch = ((w_package_t *)(&m_usr_spc[j]))->data[package_idx];
            i_data_this_ch = *(int32_t *)&iq_data_this_ch;
            fprintf(fp,"%d\n",i_data_this_ch);

            if (path_2) {
                iq_data_2 = ((w_package_t *)(&m_usr_spc[j]))->data[package_idx_inter_ch];
                i_data_2 = *(int32_t *)&iq_data_2;
                fprintf(fp_interch,"%d\n",i_data_2);
			}
		}
        samples_left -= samples_reading;
	}
	fclose(fp);
    if (fp_interch) {
		fclose(fp_interch);
    }

	return 0;
}

int32_t dma_mgr::iq2buf(uint32_t rf_idx,int16_t *I,int16_t *Q,int16_t *I_2,int16_t *Q_2)
{
    uint32_t samples = 0;
    INT_CHECK(get_fpga_w_samples(samples));
    return iq2buf(rf_idx,I,Q,samples,I_2,Q_2);
}

int32_t dma_mgr::iq2buf(uint32_t rf_idx,int16_t *I,int16_t *Q,uint32_t samples,int16_t *I_2,int16_t *Q_2)
{
    samples *= 2;

    if (samples == 0 || I == nullptr || Q == nullptr) {
        return 0;
    }

    uint32_t data_idx = 0;
    uint32_t package_idx = (rf_idx + 1) % 2;
    uint32_t package_idx_inter_ch = (0 == package_idx ? 1 : 0);
    uint32_t total_blocks = samples / R_BLOCK_SAMPLES;
    uint32_t samples_left = samples;
    uint32_t samples_reading = 0;
    uint32_t samples_last_block = samples % R_BLOCK_SAMPLES;

    if (samples_last_block != 0) {
        total_blocks ++;
    }

    iq_data_t iq_data_this_ch;
    iq_data_t iq_data_2;

    for (uint32_t i = 0;i < total_blocks;i ++) {
        samples_reading = (samples_left > int(R_BLOCK_SAMPLES) ? int(R_BLOCK_SAMPLES) : samples_left);
        INT_CHECK(m_dmaw[i]->r32(m_usr_spc,samples_reading));

        for (uint32_t j = 0;j < samples_reading;j += 2) {
            data_idx = (i * R_BLOCK_SAMPLES + j) / 2;

            iq_data_this_ch = ((w_package_t *)(&m_usr_spc[j]))->data[package_idx];
            I[data_idx] = iq_data_this_ch.i;
            Q[data_idx] = iq_data_this_ch.q;

            iq_data_2 = ((w_package_t *)(&m_usr_spc[j]))->data[package_idx_inter_ch];
            if (I_2) { I_2[data_idx] = iq_data_2.i; }
            if (Q_2) { Q_2[data_idx] = iq_data_2.q; }
		}
        samples_left -= samples_reading;
	}
	return 0;
}

int32_t	dma_mgr::iq2file(uint32_t uiRfIdx,char *pPath,uint32_t uiSamples,char *path_2)
{
    uiSamples *= 2;
    uint32_t package_idx = (uiRfIdx + 1) % 2;
    uint32_t package_idx_inter_ch = (0 == package_idx ? 1 : 0);
    uint32_t total_blocks = uiSamples / R_BLOCK_SAMPLES;
    uint32_t samples_left = uiSamples;
    uint32_t samples_reading = 0;
    uint32_t samples_last_block = uiSamples % R_BLOCK_SAMPLES;

    if (samples_last_block != 0) {
        total_blocks ++;
    }

	FILE *fp = fopen(pPath,"w");
    if (nullptr == fp) {
        Log.set_last_err("can't open %s",pPath);
		return -1;
	}
    FILE *fp_interch = nullptr;
    if (path_2) {
        fp_interch = fopen(path_2,"w");
        if (nullptr == fp_interch) {
            Log.set_last_err("can't open %s",path_2);
			return -1;
		}
	}

    iq_data_t iq_data_this_ch;
    iq_data_t iq_data_2;

    for (uint32_t i = 0;i < total_blocks;i ++) {
        samples_reading = (samples_left > int(R_BLOCK_SAMPLES) ? int(R_BLOCK_SAMPLES) : samples_left);
        INT_CHECK(m_dmaw[i]->r32(m_usr_spc,samples_reading));

        for (uint32_t j = 0;j < samples_reading;j += 2) {
            iq_data_this_ch = ((w_package_t *)(&m_usr_spc[j]))->data[package_idx];
            fprintf(fp,"%6d,%6d\n",iq_data_this_ch.i,iq_data_this_ch.q);

            if (path_2) {
                iq_data_2 = ((w_package_t *)(&m_usr_spc[j]))->data[package_idx_inter_ch];
                fprintf(fp_interch,"%6d,%6d\n",iq_data_2.i,iq_data_2.q);
			}
		}
        samples_left -= samples_reading;
	}
	fclose(fp);
    if (fp_interch) {
		fclose(fp_interch);
    }

	return 0;
}

int32_t dma_mgr::r_alloc()
{
    for (uint32_t i = 0; i < T_BLOCK_SAMPLES; i++) {
		m_usr_spc[i] = 0xffffffff;
    }

    if (m_dmar->allocate(T_BLOCK_SAMPLES * 4)) {
		Log.set_last_err("dma read memory alloc 4M error");
	}
	Log.add_msg("dmar physical address:%#x", m_dmar->phy_addr());

	if (m_dmar->w32(m_usr_spc, T_BLOCK_SAMPLES)) {
		Log.set_last_err("dma read memory write default value error");
		return -1;
	}
	return 0;
}

int32_t dma_mgr::w_alloc()
{
    for (uint32_t i = 0; i < R_BLOCK_SAMPLES; i++) {
		m_usr_spc[i] = 0xffffffff;
    }

	for (uint32_t i = 0; i < R_BLOCKS; i++) {
        if (m_dmaw[i]->allocate(R_BLOCK_SAMPLES * 4) < 0) {
			Log.set_last_err("dma write memory alloc @block%d error", i);
			return -1;
		}
		if (m_dmaw[i]->w32(m_usr_spc, R_BLOCK_SAMPLES)) {
			Log.set_last_err("dma write memory write default valued @block%d error", i);
			return -1;
		}
	}
	return 0;
}

int32_t dma_mgr::r_addr2fpga()
{
	RFU_K7_REG_DECLARE(0x0007);

	RFU_K7_REG(0x0007).adr = (m_dmar->phy_addr() & 0x000000ffffffffff) >> 8;
	RFU_K7_W(0x0007);
	return 0;
}

int32_t dma_mgr::w_addr2fpga()
{
	RFU_K7_REG_DECLARE(0x0016);
	RFU_K7_REG_DECLARE(0x0017);
	uint64_t addr_fpga[8] = { 0 }; //8 divide

	for (uint32_t i = 0; i < R_BLOCKS; i++) {
        for (uint32_t j = 0; j < 8; j++) {
			addr_fpga[j] = m_dmaw[i]->phy_addr() + 128 * 1024 * 4 * j;
			addr_fpga[j] = (addr_fpga[j] & 0x000000ffffffffff) >> 8;
            RFU_K7_REG(0x0017).adr = uint32_t(addr_fpga[j]);
			RFU_K7_W(0x0017);
			RFU_K7_REG(0x0016).data = 0;
			RFU_K7_W(0x0016);
			RFU_K7_REG(0x0016).data = 0x80000000 | (i * 8 + j);
			RFU_K7_W(0x0016);
		}
	}
	return 0;
}

int32_t dma_mgr::reset()
{
    RFU_K7_REG_DECLARE(0x0000);
    RFU_K7_OP(0x0000);
	return 0;
}
