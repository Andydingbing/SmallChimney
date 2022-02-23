#include "sgpmm.h"
#include <stdio.h>
#include <io.h>
#include <share.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/locking.h>
#include <errno.h>
#include "libLog.h"
#include "algorithm.h"
#include "CalTable.h"

#define SGPMMap map<PointKey,float>

#define LOW_LEVEL_IO_ASSERT(handle,func,error)											\
	if (error == func) {																\
        Log.SetLastError("%s:%s:%d(errno:%d)",__FILE__,__FUNCTION__,__LINE__,errno);	\
		_close(handle);																	\
		return errno;																	\
	}

bool CSGPMMFile::PointKey::operator < (const PointKey &Key) const
{
	if (this->m_uiFreq < Key.m_uiFreq)
		return true;
	else if (this->m_uiFreq == Key.m_uiFreq && this->m_fPow > Key.m_fPow)
		return true;
	return false;
}

CSGPMMFile::PointKey & CSGPMMFile::PointKey::operator = (const PointKey &Key)
{
	this->m_uiFreq = Key.m_uiFreq;
	this->m_fPow   = Key.m_fPow;
	return *this;
}

CSGPMMFile::CSGPMMFile(char *pPath)
{
	memset(m_szPath,0,ARRAY_SIZE(m_szPath));
	strcpy(m_szPath,pPath);
	if (_access(m_szPath,0)) {
		CSGPMMFile::Point Pt;
		Pt.m_Key.m_uiFreq = 2400000000;
		Pt.m_Key.m_fPow   = 0.0f;
		Pt.m_fPow		  = Pt.m_Key.m_fPow + float(PM_IL);
		Create(Pt);
	}
	Map2buf();
}

CSGPMMFile::~CSGPMMFile()
{
}

int32_t CSGPMMFile::Create(Point Pt)
{
	int iFh;
	LOW_LEVEL_IO_ASSERT(iFh,_sopen_s(&iFh,m_szPath,_O_CREAT | _O_BINARY,_SH_DENYNO,_S_IREAD | _S_IWRITE),-1);
	LOW_LEVEL_IO_ASSERT(iFh,_write(iFh,&Pt,sizeof(Point)),-1);
	LOW_LEVEL_IO_ASSERT(iFh,_close(iFh),-1);
	return 0;
}

int32_t CSGPMMFile::Insert(Point Pt)
{
	PointKey Key = Pt.m_Key;
	SGPMMap::iterator iter = m_Map.find(Key);
	if (iter != m_Map.end())
		iter->second = Pt.m_fPow;
	else
		m_Map.insert(SGPMMap::value_type(Pt.m_Key,Pt.m_fPow));
	return 0;
}

int32_t CSGPMMFile::Get(uint64_t uiFreq,float fPowPM,float &fPowSG)
{
	PointKey Key;
	Key.m_uiFreq = uiFreq;
	Key.m_fPow	 = fPowPM;
	SGPMMap::iterator iter = m_Map.find(Key);
	if (iter != m_Map.end()) {
		fPowSG = iter->second;
		return 0;
	}
	else
		return -1;
}

int32_t CSGPMMFile::Fuse(vector<CSGPMMFile::Point> &Pts)
{
	vector<CSGPMMFile::Point>::iterator iter;
	for (iter = Pts.begin();iter != Pts.end();iter ++)
		Insert((*iter));
	return 0;
}

int32_t CSGPMMFile::Commit()
{
	if (m_Map.size()) {
		int iFh;
		SGPMMap::iterator iter = m_Map.begin();

		LOW_LEVEL_IO_ASSERT(iFh,_sopen_s(&iFh,m_szPath,_O_RDWR | _O_BINARY,_SH_DENYNO,_S_IREAD | _S_IWRITE),-1);
		for (iter = m_Map.begin();iter != m_Map.end();iter ++)
			LOW_LEVEL_IO_ASSERT(iFh,_write(iFh,&(*iter),sizeof(CSGPMMFile::Point)),-1);
		LOW_LEVEL_IO_ASSERT(iFh,_commit(iFh),-1);
		LOW_LEVEL_IO_ASSERT(iFh,_close(iFh),-1);
		return 0;
	}
	return 0;
}

int32_t CSGPMMFile::To2DArray(vector<FreqSeg> &Segs,vector<uint64_t> *Freqs,vector<float> *MaxPtPows)
{
	Segs.clear();
	bool bMapFreq = Freqs ? true : false;
	bool bMapPow  = MaxPtPows ? true : false;
	int32_t iMaxPtPows = -100;

	FreqSeg Seg;
	FreqSeg SegMaxPt;
	CSGPMMFile::Point Pt;
	SGPMMap::iterator iter = m_Map.end();
	uint64_t uiFreqStar = 0;

	if (bMapFreq)
		Freqs->clear();
	if (bMapPow)
		MaxPtPows->clear();
	for (iter = m_Map.begin();iter != m_Map.end();/*iter ++*/) {
		uiFreqStar = (*iter).first.m_uiFreq;
		if (bMapFreq)
			Freqs->push_back(uiFreqStar);
		Seg.clear();
		int32_t iSegPt = 0;
		while ((*iter).first.m_uiFreq == uiFreqStar) {
			Pt.m_Key.m_uiFreq = uiFreqStar;
			Pt.m_Key.m_fPow	  = (*iter).first.m_fPow;
			Pt.m_fPow		  = (*iter).second;
			Seg.push_back(Pt);
			iSegPt ++;
			if (iter != m_Map.end())
				iter ++;
			if (iter == m_Map.end())
				break;
		}
		if (bMapPow && iSegPt > iMaxPtPows) {
			iMaxPtPows = iSegPt;
			SegMaxPt = Seg;
		}
		Segs.push_back(Seg);
	}
	if (bMapPow)
		for (FreqSeg::iterator iter = SegMaxPt.begin();iter != SegMaxPt.end();iter ++)
			MaxPtPows->push_back((*iter).m_Key.m_fPow);
	return 0;
}

int32_t CSGPMMFile::Map2buf()
{
	int iFh;
	long lFileSize = 0;
	CSGPMMFile::Point Pt;
	LOW_LEVEL_IO_ASSERT(iFh,_sopen_s(&iFh,m_szPath,_O_RDWR | _O_BINARY,_SH_DENYNO,_S_IREAD | _S_IWRITE),-1);
	LOW_LEVEL_IO_ASSERT(iFh,_lseek(iFh,0,SEEK_SET),-1L);
	LOW_LEVEL_IO_ASSERT(iFh,_lseek(iFh,0,SEEK_END),-1L);

	LOW_LEVEL_IO_ASSERT(iFh,(lFileSize = _tell(iFh)),-1);
	int32_t iPt = lFileSize / sizeof(CSGPMMFile::Point);
	LOW_LEVEL_IO_ASSERT(iFh,_lseek(iFh,0,SEEK_SET),-1L);

	m_Map.clear();

	LOW_LEVEL_IO_ASSERT(iFh,_locking(iFh,_LK_LOCK,lFileSize),-1);
	for (int32_t i = 0;i < iPt;i ++) {
		LOW_LEVEL_IO_ASSERT(iFh,_read(iFh,&Pt,sizeof(CSGPMMFile::Point)),-1);
		m_Map.insert(SGPMMap::value_type(Pt.m_Key,Pt.m_fPow));
	}
	LOW_LEVEL_IO_ASSERT(iFh,_lseek(iFh,0,SEEK_SET),-1L);
	LOW_LEVEL_IO_ASSERT(iFh,_locking(iFh,_LK_UNLCK,lFileSize),-1);
	LOW_LEVEL_IO_ASSERT(iFh,_lseek(iFh,0,SEEK_SET),-1L);
	LOW_LEVEL_IO_ASSERT(iFh,_close(iFh),-1);
	return 0;
}

int32_t CSGPMMFile::Clear()
{
	m_Map.clear();
	return 0;
}
