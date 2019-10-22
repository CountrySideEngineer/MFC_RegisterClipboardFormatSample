#pragma once
#include "CDataSample.h"

class COrgClipBoard
{
public:
	COrgClipBoard();


public:
	virtual void Copy(CArray<CDataSample*>& SrcData);
	virtual void Copy(CString& SrcData);
	virtual void Paste(CArray<CDataSample*>& SrcData);

protected:
	virtual UINT RegistClipboardFormat();
	virtual int MemorySize(const CArray<CDataSample*>& SrcData);
	virtual CString ConnectName(const CArray<CDataSample*>& SrcData);

	virtual HGLOBAL GetHGlobal(CArray<CDataSample*>& SrcData);
	virtual HGLOBAL GetHGlobal(CString& SrcData);

protected:
	UINT m_RegisteredId;


public:
	static const CString ORIGINAL_FORMAT_NAME;



};

