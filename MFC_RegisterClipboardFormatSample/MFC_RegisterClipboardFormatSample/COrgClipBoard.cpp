#include "pch.h"
#include "COrgClipBoard.h"

const CString COrgClipBoard::ORIGINAL_FORMAT_NAME = _T("RegisterClipboardFormatSample");

/**
 *	デフォルトコンストラクタ
 */
COrgClipBoard::COrgClipBoard()
{
	this->m_RegisteredId = this->RegistClipboardFormat();
}

/**
 *	引数で指定されたデータを、シリアライズ化してクリップボードにセットする。
 *
 *	@param[in]	SrcData	コピー元データ
 */
void COrgClipBoard::Copy(CArray<CDataSample*>& SrcData)
{
	HGLOBAL hData = this->GetHGlobal(SrcData);

	CString ConnectedName = this->ConnectName(SrcData);
	HGLOBAL hConnNameData = this->GetHGlobal(ConnectedName);

	if (::OpenClipboard(NULL)) {
		::EmptyClipboard();
		if (NULL == ::SetClipboardData(this->m_RegisteredId, hData)) {
			TRACE(_T("SetClipboardData() failed\r\n"));
			::GlobalFree(hData);
		}
		if (NULL == ::SetClipboardData(CF_TEXT, hConnNameData)) {
			TRACE(_T("SetClipboardData(CF_TEXT) failed\r\n"));
			::GlobalFree(hConnNameData);
		}
		::CloseClipboard();
	}
}

HGLOBAL COrgClipBoard::GetHGlobal(CArray<CDataSample*>& SrcData)
{
	CSharedFile SharedFile;
	{
		CArchive Archive(&SharedFile, CArchive::store);
		if (Archive.IsStoring()) {
			Archive << SrcData.GetCount();
			SrcData.Serialize(Archive);
		}
	}
	HGLOBAL hData = SharedFile.Detach();
	
	return hData;
}

HGLOBAL COrgClipBoard::GetHGlobal(CString& SrcData)
{
	int SrcDataLen = SrcData.GetLength() + 1;	//末尾の'\0'のために、+1する。
	HGLOBAL hData = (HGLOBAL)::GlobalAlloc(GHND, SrcDataLen * sizeof(TCHAR));
	if (0 == hData) {
		TRACE(_T("GlobalAlloc() failed\r\n"));
		return hData;
	}
	TCHAR* CharacterData = (TCHAR*)::GlobalLock(hData);
	if (NULL == CharacterData) {
		TRACE(_T("GlobalLock() failed\r\n"));
		::GlobalFree(hData);

		return 0;
	}
	ZeroMemory(CharacterData, SrcDataLen * sizeof(TCHAR));	//「0」で初期化
	if (0 != _tcscpy_s(CharacterData, SrcDataLen, SrcData.GetString())) {//第2引数の単位に注意！
		TRACE(_T("_tcscpy_s() failed\r\n"));
		::GlobalFree(hData);

		return 0;
	}
	::GlobalUnlock(hData);

	return hData;
}


/**
 *	引数で指定されたデータを、シリアライズ化してクリップボードにセットする。
 *
 *	@param[in]	SrcData	コピー元データ
 */
void COrgClipBoard::Copy(CString& SrcData)
{
	INT_PTR SrcDataLen = SrcData.GetLength() * sizeof(TCHAR) + sizeof(TCHAR);
	HGLOBAL hData = (HGLOBAL)::GlobalAlloc(GHND, SrcDataLen);
	if (NULL == hData) {
		TRACE(_T("::GlobalAlloc() failed\n"));

		return;
	}
	TCHAR* Data = (TCHAR*)::GlobalLock(hData);
	if (NULL == Data) {
		TRACE(_T("::GlobalLock() failed\n"));

		return;
	}
	_tcscpy_s(Data, SrcDataLen, SrcData);
	::GlobalUnlock(hData);

	if (::OpenClipboard(NULL)) {
		::SetClipboardData(CF_TEXT, hData);
		::SetClipboardData(CF_TEXT, hData);
		::SetClipboardData(CF_TEXT, hData);
	}
	::CloseClipboard();
}

/**
 *	クリップボードからデータを抽出する。
 *
 *	@param[out]	DstData	クリップボードから取得したデータの格納先
 */
void COrgClipBoard::Paste(CArray<CDataSample*>& DstData)
{
	if (::OpenClipboard(NULL)) {
		HANDLE BoardDataHandle = ::GetClipboardData(this->m_RegisteredId);
		if (NULL != BoardDataHandle) {
			CSharedFile SharedFile;
			SharedFile.SetHandle(BoardDataHandle);
			{
				CArchive Archive(&SharedFile, CArchive::load);
				INT_PTR Count = 0;
				Archive >> Count;
				DstData.Serialize(Archive);
			}
			SharedFile.Detach();
		}
		::CloseClipboard();
	}
}


/**
 *	フォーマットを登録する。
 */
UINT COrgClipBoard::RegistClipboardFormat()
{
	int RegisteredId = ::RegisterClipboardFormat(COrgClipBoard::ORIGINAL_FORMAT_NAME);

	if (0 == RegisteredId) {
		TRACE(_T("Format registeration failed.\r\n"));
	}
	else {
		TRACE(_T("Format registered as 0x%08x.\r\n"), RegisteredId);
	}
	return RegisteredId;
}

int COrgClipBoard::MemorySize(const CArray<CDataSample*>& SrcData)
{
	int DataSize = 0;

	for (INT_PTR Index = 0; Index < SrcData.GetCount(); Index++) {
		CDataSample SubSrcData = SrcData.GetAt(Index);
		DataSize += SubSrcData.GetSize();
	}
	return DataSize;
}

CString COrgClipBoard::ConnectName(const CArray<CDataSample*>& SrcData)
{
	CString ConnectedName = _T("");
	for (INT_PTR Index = 0; Index < SrcData.GetCount(); Index++) {
		CDataSample* SrcItem = SrcData.GetAt(Index);
		ConnectedName += SrcItem->GetSampleName();
		ConnectedName += _T("\t");
	}

	return ConnectedName;
}