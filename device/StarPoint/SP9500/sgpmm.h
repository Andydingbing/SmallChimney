#ifndef __SG_PM_MAP__
#define __SG_PM_MAP__

#define MAX_PATH 260

#include <vector>
#include <map>
#include <stddef.h>
#include "stdint_common.h"

using namespace std;

class CSGPMMFile
{
public:
	typedef struct PointKey {
		uint64_t m_uiFreq;
		float	 m_fPow;
	public:
		bool operator < (const PointKey &Key) const;
		PointKey & operator = (const PointKey &Key);
	}PointKey;

	typedef struct Point {
		PointKey m_Key;
		float m_fPow;
	}Point;

	typedef vector<CSGPMMFile::Point> FreqSeg;
public:
	CSGPMMFile(char *pPath);
	virtual ~CSGPMMFile();
public:
	int32_t Get(uint64_t uiFreq,float fPowPM,float &fPowSG);
	int32_t Fuse(FreqSeg &Pts);
	int32_t Commit();
	int32_t To2DArray(vector<FreqSeg> &Segs,vector<uint64_t> *Freqs = NULL,vector<float> *MaxPtPows = NULL);
	int32_t Clear();
private:
	int32_t Create(Point Pt);
	int32_t Map2buf();
	int32_t Insert(Point Pt);
private:
	char m_szPath[MAX_PATH];
	map<PointKey,float> m_Map;
};
#endif
