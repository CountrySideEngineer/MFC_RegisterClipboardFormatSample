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
	CSharedFile SharedFile;
	{
		CArchive Archive(&SharedFile, CArchive::store);
		if (Archive.IsStoring()) {
			Archive << SrcData.GetCount();
			SrcData.Serialize(Archive);
		}
	}
	HGLOBAL hData = SharedFile.Detach();

	if (::OpenClipboard(NULL)) {
		::EmptyClipboard();
		if (NULL == ::SetClipboardData(this->m_RegisteredId, hData)) {
			TRACE(_T("SetClipboardData() failed\r\n"));
			::GlobalFree(hData);
		}
		::CloseClipboard();
	}
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