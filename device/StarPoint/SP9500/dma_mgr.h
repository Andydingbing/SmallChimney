#ifndef SP9500_DMA_MGR_H
#define SP9500_DMA_MGR_H

#include "global_header.h"
#include <boost/smart_ptr.hpp>

#define T_BLOCK_SAMPLES uint32_t(1048576) // fpga dma read, physical memory one block size(1M samples = 4M bytes)
#define R_BLOCK_SAMPLES uint32_t(1048576) // fpga dma write,physi......
#define R_BLOCKS        uint32_t(48)      // fpga dma write physical memory total blocks

class pci_dev_vi;
class vi_mem_io;

namespace ns_sp9500 {

class API dma_mgr
{
public:
    typedef boost::shared_ptr<dma_mgr> sptr;
    dma_mgr(pci_dev_vi *k7);
    ~dma_mgr();
public:
    typedef struct w_package_t {
        iq_data_t data[2];
    }w_package_t;
public:
    uint32_t *usr_spc() const;
    int32_t init();
public:
    int32_t fpga_r(uint32_t length);
    int32_t fpga_w();
    int32_t fpga_w_start();
    int32_t fpga_w_trans();
    int32_t fpga_w_abort();
    int32_t set_fpga_w_samples(uint32_t samples);
    int32_t get_fpga_w_samples(uint32_t &samples);
public:
    int32_t dump2buf(int32_t *data,uint32_t samples);
    int32_t dump2buf_ch(uint32_t rf_idx,int32_t *data,uint32_t samples,int32_t *data_2 = nullptr);
    int32_t dump2file_ch(uint32_t rf_idx,char *path,uint32_t samples,char *path_2 = nullptr);
    int32_t iq2buf(uint32_t rf_idx,int16_t *I,int16_t *Q,int16_t *I_2 = nullptr,int16_t *Q_2 = nullptr);
    int32_t iq2buf(uint32_t rf_idx,int16_t *I,int16_t *Q,uint32_t samples,int16_t *I_2 = nullptr,int16_t *Q_2 = nullptr);
    int32_t iq2file(uint32_t rf_idx,char *path,uint32_t samples,char *path_2 = nullptr);
private:
    int32_t r_alloc();
    int32_t w_alloc();
    int32_t r_addr2fpga();
    int32_t w_addr2fpga();
private:
    int32_t reset();
private:
    pci_dev_vi *m_k7;
    vi_mem_io *m_dmar;
    vi_mem_io *m_dmaw[R_BLOCKS];
    uint32_t *m_usr_spc;
};

} // namespace ns_sp9500

#endif
