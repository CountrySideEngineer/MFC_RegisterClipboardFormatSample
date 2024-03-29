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

/**
 *	配列のコピー用ハンドルを生成する。
 *
 *	@param[in]	SrcData	コピー元データ
 *	@return	コピー元データをクリップボードに設定するためのハンドル
 */
HGLOBAL COrgClipBoard::GetHGlobal(CArray<CDataSample*>& SrcData)
{
	/*
	 *	要素がポインタの配列をSerializeすると、「ポインタの示すアドレス」が対象となる。
	 *	ここで、Serializeしたデータを再度参照する際に「ポインタの示すアドレス」が解放されて
	 *	いた場合、エラーが発生する。
	 *	これを回避するために、Serializeするデータは、元になるデータをコピー(ディープコピー)し、
	 *	それをクリップボードに設定する。
	 */
	CArray<CDataSample*> CopiedSrcData;
	for (INT_PTR Index = 0; Index < SrcData.GetCount(); Index++) {
		CDataSample* SrcItem = SrcData.GetAt(Index);
		CopiedSrcData.Add(SrcItem->CreateCopy());
	}

	CSharedFile SharedFile;
	{
		CArchive Archive(&SharedFile, CArchive::store);
		if (Archive.IsStoring()) {
			Archive << 10;
			for (INT_PTR Index = 0; Index < 10; Index++) {
				Archive << Index;
				Archive << CopiedSrcData.GetCount();
				CopiedSrcData.Serialize(Archive);
			}
		}
	}
	HGLOBAL hData = SharedFile.Detach();
	
	return hData;
}

/**
 *	配列の中の文字列コピー用ハンドルを生成する。
 *
 *	@param[in]	SrcData	コピー元データ
 *	@return	コピー元データをクリップボードに設定するためのハンドル
 */
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
				INT_PTR DataNum = 0;
				Archive >> DataNum;
				for (INT_PTR Index = 0; Index < DataNum; Index++) {
					INT_PTR ReadIndex = 0;
					INT_PTR ReadCount = 0;
					Archive >> ReadIndex;
					Archive >> ReadCount;
					DstData.Serialize(Archive);
				}
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

/**
 *	名前のプロパティの内容を結合した文字列を返す。
 *
 *	@param[in]	SrcData	結合したい情報を含むデータの配列(の参照)
 *	@return	結合した文字列
 */
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