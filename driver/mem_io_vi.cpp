#include "mem_io_vi.h"
#include "pci_dev_vi.h"

ViSession vi_mem_io::g_ddr_session = 0;

vi_mem_io::vi_mem_io() :
    m_phy_addr(0),
    m_phy_addr_bfr_align(0)
{
    pci_dev_vi::open_default_rm();
    m_intr_entry = nullptr;
}

int32_t vi_mem_io::allocate(uint32_t size)
{
    if (m_phy_addr) {
		return VI_SUCCESS;
    }

    ViStatus status = VI_SUCCESS;

	if (!g_ddr_session) {
        if ((status = viOpen(pci_dev_vi::get_default_rm(),ViRsrc("PXI::MEMACC"),VI_NULL,VI_NULL,&g_ddr_session)) < VI_SUCCESS) {
			g_ddr_session = 0;
			return status;
		}
	}

    /*
     * 4k align.
     * For example,if we need 1M bytes memory but the very beginning bus address is 0x0000 0001,
     * 4K more space should be allocated.
     * The address space will be 0x0000 1000 --- 0x0010 1000.
     * In this case,m_phy_addr_bfr_align = 0x0000 0001,m_phy_addr = 0x0000 1000.
     */
    size += 4096;

    if ((status = viMemAlloc(g_ddr_session,size,&m_phy_addr)) < VI_SUCCESS) {
        g_ddr_session = 0;
        return status;
    }

    m_phy_addr_bfr_align = m_phy_addr;
    m_phy_addr = ViBusAddress(m_phy_addr / 4096 + 1) * 4096;

    if ((status = viSetAttribute(g_ddr_session,VI_ATTR_DEST_INCREMENT,1)) < VI_SUCCESS) {
        return status;
    }

//	if (m_intr_entry) {
//        if ((status = viInstallHandler(g_ddr_session,VI_EVENT_PXI_INTR,m_intr_entry,nullptr)) < VI_SUCCESS) {
//			return status;
//        }
//        if ((status = viEnableEvent(g_ddr_session,VI_EVENT_PXI_INTR,VI_HNDLR,VI_NULL)) < VI_SUCCESS) {
//			return status;
//        }
//	}

    return status;
}

int32_t vi_mem_io::release()
{
    ViStatus status = VI_SUCCESS;

    if (g_ddr_session) {
        if ((status = viMemFree(g_ddr_session,m_phy_addr_bfr_align)) < VI_SUCCESS) {
            return status;
        }

        if ((status = viClose(g_ddr_session)) < VI_SUCCESS) {
            return status;
        }
	}

    g_ddr_session = 0;
    return status;
}

void vi_mem_io::releases()
{
    if (g_ddr_session) {
        viMemFree(g_ddr_session, m_phy_addr_bfr_align);
        viClose(g_ddr_session);
    }
}

uint64_t vi_mem_io::phy_addr()
{
	return uint64_t(m_phy_addr);
}

int32_t vi_mem_io::w8(uint8_t *buf,uint32_t size)
{
    return viMoveOut8(g_ddr_session,VI_PXI_ALLOC_SPACE,m_phy_addr,size,buf);
}

int32_t vi_mem_io::w16(uint16_t *buf,uint32_t size)
{
    return viMoveOut16(g_ddr_session,VI_PXI_ALLOC_SPACE,m_phy_addr,size,buf);
}

int32_t vi_mem_io::w32(uint32_t *buf,uint32_t size)
{
    return viMoveOut32(g_ddr_session,VI_PXI_ALLOC_SPACE,m_phy_addr,size,ViAUInt32(buf));
}

int32_t vi_mem_io::r8(uint8_t *buf,uint32_t size)
{
    return viMoveIn8(g_ddr_session,VI_PXI_ALLOC_SPACE,m_phy_addr,size,buf);
}

int32_t vi_mem_io::r16(uint16_t *buf,uint32_t size)
{
    return viMoveIn16(g_ddr_session,VI_PXI_ALLOC_SPACE,m_phy_addr,size,buf);
}

int32_t vi_mem_io::r32(uint32_t *buf,uint32_t size)
{
    return viMoveIn32(g_ddr_session,VI_PXI_ALLOC_SPACE,m_phy_addr,size,ViAUInt32(buf));
}

void vi_mem_io::set_intr_entry(ViStatus(*entry)(ViSession session,ViEventType type,ViEvent context,ViAddr user_handle))
{
	m_intr_entry = entry;
}

ViStatus _VI_FUNCH pxi_isr(ViSession session,ViEventType type,ViEvent context,ViAddr user_handle)
{
	//	SetEvent(&g_Event);

	// 	viOut32(vi,VI_PXI_BAR0_SPACE, LEAGACYCLR, 0x00000100);
	// 	viOut32(vi,VI_PXI_BAR0_SPACE, LEAGACYCLR, 0x00000000);

	// 	viGetAttribute(context, VI_ATTR_PXI_RECV_INTR_SEQ, &g_isr_params.recv_seq);
	// 	viGetAttribute(context, VI_ATTR_PXI_RECV_INTR_DATA, &g_isr_params.recv_data);

	return VI_SUCCESS;
}
