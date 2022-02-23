#include "StdAfx.h"
#include "ad9361_spi.h"
#include "log.h"

int32_t ad9361_spi_write(uint32_t Addr, uint32_t WriteData)
{
	uint32_t wAddr;
	uint32_t wData;
	uint32_t wBar1Data = 0;
	bool status;
	wAddr = Addr;
	wData = WriteData;
	wBar1Data = wBar1Data | 0x02000000;
	wData = wData & 0x000000ff;
	wAddr = wAddr << 8;
	wAddr = wAddr & 0x0003ff00;
	wBar1Data = wBar1Data | wAddr | wData;

	if ((status = wait_spi_busy()) != 0)
		return status;

//	if ((status = PCIE_Core.WriteWordBar1(0x88, wBar1Data)) == -1) {			//д9361REG
//		Log.Write_Message_List((E_K7REG_W | 0x88 | E_AD9361_SPI_W),"д9361_SPIʧ��! Addr:0x%x  Data:0x%x",Addr,wBar1Data);
//		return (E_K7REG_W | 0x88 | E_AD9361_SPI_W);
//	}

	if ((status = wait_spi_busy()) != 0)
		return status;

	Log.Write_SPI_Log(Addr,wBar1Data,-1);
	return 0;
}

/*
ad9361 spi bit write
@param mask The bits mask (00001000)
@param val  The bits value
*/
//int32_t __ad9361_spi_writef(uint32_t Addr,uint32_t mask, uint32_t offset,uint32_t val)
int32_t ad9361_spi_writef(uint32_t Addr,uint32_t mask, uint32_t val)
{
	uint32_t buf;
	int32_t ret;
	uint32_t offset;
//	offset = __ffs(mask);

	if(!mask)
		return -EINVAL; //invalid argument
	
	ret=ad9361_spi_read(Addr,&buf);

	if(ret<0)
		return ret; 
		
	buf &= ~mask;
	buf |= ((val<<offset) & mask);

	return ad9361_spi_write(Addr, buf);
}

/**
 * SPI multiple bytes register write.
 * @param spi
 * @param reg The register address.
 * @param tbuf The data buffer.
 * @param num The number of bytes to read.
 * @return 0 in case of success, negative error code otherwise.
 */
int32_t ad9361_spi_writem(uint32_t reg, uint32_t *tbuf, uint32_t num)
{
	//�Ӹ�λ��ʼд��
	uint32_t i=0;
	for (i=0;i<num;i++) {
		ad9361_spi_write(reg-i,*(tbuf+i));
	}
	return 0;
}

/**
 * SPI register read.
 * @param Addr The register address.
 * @param ReadData the register value read from ad9361
 * @return 0 in case of success or negative error code in case of failure.
 */
int32_t ad9361_spi_read(uint32_t Addr, uint32_t *ReadData)
{
	uint32_t rAddr;
	uint32_t wBar1Data = 0;
	uint32_t rBar1Data = 0;
	uint32_t temp=0;
	CString rData;
	BOOL status;
	rAddr = Addr;

	wBar1Data = wBar1Data | 0x01000000;
	rAddr = rAddr << 8;
	rAddr = rAddr & 0x0003ff00;
	wBar1Data = wBar1Data | rAddr;

	if((status = wait_spi_busy()) != 0)
		return status;

//	if((status = PCIE_Core.WriteWordBar1(0x88, wBar1Data)) == -1) {		//0x88����׼��д									
//		Log.Write_Message_List((E_K7REG_W | 0x88 | E_AD9361_SPI_R),"д9361_SPIʧ��! Addr:0x%88  Data:0x%x",wBar1Data);
//		return (E_K7REG_W | 0x88 | E_AD9361_SPI_R);
//	}

	if((status = wait_spi_busy()) != 0)
		return status;

	int spi_read_data_count = 0;
	while ((rBar1Data & 0x01000000) == 0) {
//		if((status = PCIE_Core.ReadWordBar1(0x8C, &rBar1Data)) == -1) {	//���Զ�9361REG
//			Log.Write_Message_List((E_K7REG_R | 0x8C | E_AD9361_SPI_R),"��9361_SPIʧ��!");
//			return (E_K7REG_R | 0x8C | E_AD9361_SPI_R); 
//		}
		spi_read_data_count ++;
		if(spi_read_data_count > 50) {
			Log.Write_Message_List(E_AD9361_SPI_R_OverTime,"��9361_SPI��ʱ");
			return E_AD9361_SPI_R_OverTime;
		}
	}
	Log.Write_SPI_Log(Addr,-1,rBar1Data);
	rBar1Data = rBar1Data & 0x000000ff;
	*ReadData = rBar1Data;

	return 0;
}

/**
 * SPI register bits read.
 * @param reg The register address.
 * @param mask The bits mask.
 * @param offset The mask offset.
 * @return 0 in case of success negative error code in case of failure.
 */
int32_t ad9361_spi_readf(uint32_t reg,uint32_t mask, uint32_t *buf)
{
	int32_t ret;
	uint32_t offset;
//	offset = __ffs(mask);

	if (!mask)
		return -EINVAL;

	ret=ad9361_spi_read(reg, buf);
	if (ret < 0)
		return ret;

	*buf &= mask;
	*buf >>= offset;

	return ret;
}

/**
 * SPI multiple bytes register read.
 * @param reg The register address.
 * @param rbuf The data buffer.
 * @param num The number of bytes to read.
 * @return 0 in case of success, negative error code otherwise.
 */
int32_t ad9361_spi_readm(uint32_t reg, uint32_t *rbuf, uint32_t num)
{
	//��һ������Ϊ�ߵ�ַ��numΪ������ȡ��ַ��
	uint32_t i=0;

	for (i=0;i<num;i++) {
		ad9361_spi_read(reg-i,rbuf+i);
	}
	return 0;
}

int32_t ioexte_spi_write(uint32_t Addr,uint32_t WriteData)
{
	uint32_t wAddr;
	uint32_t wData;
	uint32_t wBar1Data=0;
	uint32_t rBar1Data=0;
	BOOL status;
	wAddr = Addr;
	wData = WriteData;
	wBar1Data = wBar1Data | 0x00400000;
	wData = wData & 0x000000ff;
	wAddr = wAddr << 8;
	wAddr = wAddr & 0x0000ff00;
	wBar1Data = wBar1Data | wAddr | wData;

	if((status = wait_spi_busy()) != 0)
		return status;

//	if((status = PCIE_Core.WriteWordBar1(0x98, wBar1Data)) == -1) {
//		Log.Write_Message_List((E_K7REG_W | 0x98 | E_IOEXT_SPI_W),"дIO��չ��SPIʧ��! Addr:0x%x  Data:0x%x",Addr,wBar1Data);
//		return (E_K7REG_W | 0x98 | E_IOEXT_SPI_W);
//	}

	if((status = wait_spi_busy()) != 0)
		return status;
	return 0;
}

//�ݲ�֧��
int32_t ioexte_spi_read(uint32_t Addr, uint32_t *ReadData)
{
	uint32_t wAddr;
	uint32_t wData=0;
	uint32_t wBar1Data=0;
	uint32_t rBar1Data=0;
	BOOL status;
	wAddr = Addr;
	wBar1Data = wBar1Data | 0x00410000;					//W/Rλд1
	wAddr = wAddr << 8;
	wAddr = wAddr & 0x0000ff00;
	wBar1Data = wBar1Data | wAddr | wData;
//	status = PCIE_Core.WriteWordBar1(0x98, wBar1Data);
	if(status == -1) {
		Log.Write_Message_List(E_IOEXT_SPI_W,"дioexte_SPIʧ��! Addr:0x98  Data:0x%x",wBar1Data);
		return -EIO;
	}
	Sleep(1);

//	status = PCIE_Core.ReadWordBar1(0x9C, &rBar1Data);
	if(status == -1) {
		Log.Write_Message_List(E_IOEXT_SPI_R,"��ioexte_SPIʧ��!");
		return -EIO; 
	}
	Sleep(1);
	if((rBar1Data & 0x00410000)==0)
	{
		Log.Write_Message_List(E_IOEXT_SPI_D,"ioexte_SPI������Ч! Data:0x%x",rBar1Data);
		return -EIO;
	}
	rBar1Data = rBar1Data & 0x000000ff;
	*ReadData = rBar1Data;

	return 0;
}

int32_t wait_spi_busy()
{
	uint32_t rBar1Data = 0;
	int spi_write_enable_count = 0;	
	bool status;
	while ((rBar1Data & 0x00000001) == 0) {
// 		if ((status = PCIE_Core.ReadWordBar1(0xC0,&rBar1Data)) == -1) {	
//			Log.Write_Message_List((E_K7REG_R | 0xC0 | E_AD9361_SPI_W_Enable),"��9361_SPIʧ��! Addr:0xC0  Data:0x%x",rBar1Data);
// 			return (E_K7REG_R | 0xC0 | E_AD9361_SPI_W_Enable);
//		}
 		spi_write_enable_count ++;
 		if (spi_write_enable_count > 100) {										//ʹ�ܳ�ʱ
 			Log.Write_Message_List(E_AD9361_SPI_W_OverTime,"д9361SPI��ʱ");
 			return E_AD9361_SPI_W_OverTime;
 		}
 	}
	return 0;
}