#include "pch.h"
#include "CDataSample.h"

/**
 *	�f�t�H���g�R���X�g���N�^
 */
CDataSample::CDataSample()
	: m_SampleNumber(0)
	, m_SampleName(_T(""))
{
	this->m_SubDataSample.RemoveAll();
}

/**
 *	�R���X�g���N�^
 *
 *	@param	SampleNumber	�ݒ肷��SampleNumber�̒l
 *	@param	SampleName		�ݒ肷��SampleName�̒l
 */
CDataSample::CDataSample(INT_PTR SampleNumber, CString SampleName)
	: m_SampleNumber(SampleNumber)
	, m_SampleName(SampleName)
{
	this->m_SubDataSample.RemoveAll();
}

/**
 *	�f�X�g���N�^
 */
CDataSample::~CDataSample()
{
	for (INT_PTR Index = 0; Index < this->m_SubDataSample.GetCount(); Index++) {
		CDataSample* Item = this->m_SubDataSample.GetAt(Index);
		delete Item;
		Item = NULL;
	}
}

/**
 *	�R�s�[�R���X�g���N�^
 *
 *	@param	ItemSrc	�R�s�[���f�[�^�̎���
 */
CDataSample::CDataSample(const CDataSample* ItemSrc)
{
	this->m_SampleNumber = ItemSrc->GetSampleNumber();
	this->m_SampleName = ItemSrc->GetSampleName();

	for (INT_PTR Index = 0; Index < ItemSrc->m_SubDataSample.GetCount(); Index++) {
		CDataSample* SubItemSrc = ItemSrc->m_SubDataSample.GetAt(Index);
		this->m_SubDataSample.Add(SubItemSrc->CreateCopy());
	}
}

/**
 *	���g�̃R�s�[�𐶐�����B
 *	�T�u�f�[�^�̓��e�́A�V���ɗ̈���m�ۂ��A�����ɓ����l���R�s�[�����B
 *	(�f�B�[�v�R�s�[)
 *
 *	@return	�R�s�[���ꂽ�̈�ւ̃|�C���^
 */
CDataSample* CDataSample::CreateCopy()
{
	CDataSample* NewItem = new CDataSample(this->m_SampleNumber, this->m_SampleName);

	for (INT_PTR Index = 0; Index < this->m_SubDataSample.GetCount(); Index++) {
		CDataSample* SubItem = this->m_SubDataSample.GetAt(Index);
		NewItem->AddSubData(SubItem);
	}
	return NewItem;
}

/**
 *	�T�u�f�[�^��ǉ�����B
 *
 *	@param	NewItemBase	�ǉ�����T�u�f�[�^
 *						�������A���ۂɒǉ������͈̂����Ŏw�肳�ꂽ�l�̃R�s�[�B
 */
void CDataSample::AddSubData(CDataSample* NewItemBase)
{
	this->m_SubDataSample.Add(NewItemBase->CreateCopy());

}

/**
 *	�T�u�f�[�^���܂ށA�f�[�^�̍��v���擾����B
 */
INT_PTR CDataSample::GetSize()
{
	INT_PTR DataSize = 0;
	DataSize += sizeof(*this);

	for (INT_PTR Index = 0; Index < this->m_SubDataSample.GetCount(); Index++) {
		CDataSample* SubDataItem = this->m_SubDataSample.GetAt(Index);
		DataSize += SubDataItem->GetSize();
	}

	return DataSize;
}

/**
 *	�f�[�^�̓��e��\������B
 */
void CDataSample::Trace()
{
	TRACE(_T("SampleNumber = %5d\n"), this->m_SampleNumber);
	TRACE(_T("SampleName = %s\n"), this->m_SampleName);
	TRACE(_T("Sub data number = %d\n"), this->m_SubDataSample.GetCount());

	for (INT_PTR Index = 0; Index < this->m_SubDataSample.GetCount(); Index++) {
		TRACE(_T("Sub data index = %d\n"), Index);

		CDataSample* Item = this->m_SubDataSample.GetAt(Index);
		Item->Trace();
	}
}