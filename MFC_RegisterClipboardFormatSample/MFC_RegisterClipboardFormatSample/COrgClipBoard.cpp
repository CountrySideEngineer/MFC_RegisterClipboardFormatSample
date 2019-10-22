#include "pch.h"
#include "COrgClipBoard.h"

const CString COrgClipBoard::ORIGINAL_FORMAT_NAME = _T("RegisterClipboardFormatSample");

/**
 *	�f�t�H���g�R���X�g���N�^
 */
COrgClipBoard::COrgClipBoard()
{
	this->m_RegisteredId = this->RegistClipboardFormat();
}

/**
 *	�����Ŏw�肳�ꂽ�f�[�^���A�V���A���C�Y�����ăN���b�v�{�[�h�ɃZ�b�g����B
 *
 *	@param[in]	SrcData	�R�s�[���f�[�^
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
 *	�z��̃R�s�[�p�n���h���𐶐�����B
 *
 *	@param[in]	SrcData	�R�s�[���f�[�^
 *	@return	�R�s�[���f�[�^���N���b�v�{�[�h�ɐݒ肷�邽�߂̃n���h��
 */
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

/**
 *	�z��̒��̕�����R�s�[�p�n���h���𐶐�����B
 *
 *	@param[in]	SrcData	�R�s�[���f�[�^
 *	@return	�R�s�[���f�[�^���N���b�v�{�[�h�ɐݒ肷�邽�߂̃n���h��
 */
HGLOBAL COrgClipBoard::GetHGlobal(CString& SrcData)
{
	int SrcDataLen = SrcData.GetLength() + 1;	//������'\0'�̂��߂ɁA+1����B
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
	ZeroMemory(CharacterData, SrcDataLen * sizeof(TCHAR));	//�u0�v�ŏ�����
	if (0 != _tcscpy_s(CharacterData, SrcDataLen, SrcData.GetString())) {//��2�����̒P�ʂɒ��ӁI
		TRACE(_T("_tcscpy_s() failed\r\n"));
		::GlobalFree(hData);

		return 0;
	}
	::GlobalUnlock(hData);

	return hData;
}

/**
 *	�N���b�v�{�[�h����f�[�^�𒊏o����B
 *
 *	@param[out]	DstData	�N���b�v�{�[�h����擾�����f�[�^�̊i�[��
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
 *	�t�H�[�}�b�g��o�^����B
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
 *	���O�̃v���p�e�B�̓��e�����������������Ԃ��B
 *
 *	@param[in]	SrcData	���������������܂ރf�[�^�̔z��(�̎Q��)
 *	@return	��������������
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