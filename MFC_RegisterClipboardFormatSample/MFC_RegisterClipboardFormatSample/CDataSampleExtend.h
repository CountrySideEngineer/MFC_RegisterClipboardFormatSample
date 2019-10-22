#pragma once
#include "CDataSample.h"

class CDataSampleExtend : public CDataSample
{
public:
	CDataSampleExtend();
	CDataSampleExtend(INT_PTR ExtendedNumber, CString ExtendedName, INT_PTR SampleNumber, CString SampleName);
	CDataSampleExtend(const CDataSampleExtend* ItemSrc);
	virtual ~CDataSampleExtend() {}

	virtual INT_PTR	 GetExtendedNumber() const { return this->m_ExtendedNumber; }
	virtual void SetExtendedNumber(INT_PTR ExtendedNumber) { this->m_ExtendedNumber = ExtendedNumber; }
	virtual CString GetExtendedName() const { return this->m_ExtendedName; }
	virtual void SetExtendedName(CString ExtendedName) { this->m_ExtendedName = ExtendedName; }

	virtual CDataSampleExtend* CreateCopy();

	virtual void Trace();

protected:
	INT_PTR m_ExtendedNumber;
	CString m_ExtendedName;
};

