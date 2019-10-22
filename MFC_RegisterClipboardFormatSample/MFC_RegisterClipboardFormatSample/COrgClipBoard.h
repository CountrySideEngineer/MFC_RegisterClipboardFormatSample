#pragma once
#include "CDataSample.h"

class COrgClipBoard
{
public:
	COrgClipBoard();


public:
	virtual void Copy(CArray<CDataSample*>& SrcData);
	virtual void Paste(CArray<CDataSample*>& SrcData);

protected:
	virtual UINT RegistClipboardFormat();
	virtual int MemorySize(const CArray<CDataSample*>& SrcData);
	virtual void CopySrcToGlobal() {}

protected:
	UINT m_RegisteredId;


public:
	static const CString ORIGINAL_FORMAT_NAME;



};

