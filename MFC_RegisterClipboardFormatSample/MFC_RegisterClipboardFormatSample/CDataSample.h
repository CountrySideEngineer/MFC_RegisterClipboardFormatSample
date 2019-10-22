#pragma once
class CDataSample
{
public:
	CDataSample();
	CDataSample(INT_PTR SampleNumber, CString SampleName);
	CDataSample(const CDataSample* ItemSrc);

	virtual ~CDataSample();

	virtual	INT_PTR	GetSampleNumber() const { return this->m_SampleNumber; }
	virtual VOID	SetSampleNumber(INT_PTR SampleNumber) { this->m_SampleNumber = SampleNumber; }
	virtual CString	GetSampleName() const { return this->m_SampleName; }
	virtual VOID	SetSampleName(CString SampleName) { this->m_SampleName = SampleName; }
	virtual const CArray<CDataSample*>* GetSubData() const { return &(this->m_SubDataSample); }

	virtual void	AddSubData(CDataSample* NewItemBase);
	virtual INT_PTR	GetSize();

	virtual CDataSample* CreateCopy();

	virtual void Trace();

protected:
	INT_PTR	m_SampleNumber;
	CString	m_SampleName;
	CArray<CDataSample*>	m_SubDataSample;
};

