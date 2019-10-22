#include "pch.h"
#include "CDataSampleExtend.h"

/**
 *	�f�t�H���g�R���X�g���N�^
 */
CDataSampleExtend::CDataSampleExtend()
	: m_ExtendedNumber(0)
	, m_ExtendedName(_T(""))
	, CDataSample()
{}

/**
 *	��������̃R���X�g���N�^
 *
 *	@param	ExtendedNumber	�g���ԍ�
 *	@param	ExtendedName	�g�����O
 *	@param	SampleNumber	����ԍ�
 *	@param	SampleName		����ԍ�
 */
CDataSampleExtend::CDataSampleExtend(INT_PTR ExtendedNumber, CString ExtendedName, INT_PTR SampleNumber, CString SampleName)
	: m_ExtendedNumber(0)
	, m_ExtendedName(ExtendedName)
	, CDataSample(SampleNumber, SampleName)
{}

/**
 *	�R�s�[�R���X�g���N�^
 *
 *	@param[in]	SrcItem	�R�s�[���f�[�^�ւ̃|�C���^
 */
CDataSampleExtend::CDataSampleExtend(const CDataSampleExtend* SrcItem)
{
	this->m_ExtendedName = SrcItem->GetExtendedName();
	this->m_ExtendedNumber = SrcItem->GetExtendedNumber();
	this->m_SampleName = SrcItem->GetSampleName();
	this->m_SampleNumber = SrcItem->GetSampleNumber();

	for (INT_PTR Index = 0; Index < SrcItem->GetSubData()->GetCount(); Index++) {
		CDataSample* Item = SrcItem->GetSubData()->GetAt(Index);
		this->m_SubDataSample.Add(Item->CreateCopy());
	}
}

/**
 *	���g�̃R�s�[�𐶐�����B
 *	�T�u�f�[�^�̓��e�́A�V���ɗ̈���m�ۂ��A�����ɓ����l���R�s�[�����B
 *	(�f�B�[�v�R�s�[)
 *
 *	@return	�R�s�[���ꂽ�̈�ւ̃|�C���^
 */
CDataSampleExtend* CDataSampleExtend::CreateCopy()
{
	CDataSampleExtend* NewItem = new CDataSampleExtend(
		this->m_ExtendedNumber, this->m_ExtendedName, 
		this->m_SampleNumber, this->m_SampleName);

	for (INT_PTR Index = 0; Index < this->m_SubDataSample.GetCount(); Index++) {
		CDataSample* SubItem = this->m_SubDataSample.GetAt(Index);
		NewItem->AddSubData(SubItem);
	}
	return NewItem;
}


/**
 *	�f�[�^�̓��e��\������B
 */
void CDataSampleExtend::Trace()
{
	TRACE(_T("ExtendedNumber = %5d\n"), this->m_ExtendedNumber);
	TRACE(_T("ExtendedName = %s\n"), this->m_ExtendedName);

	CDataSample::Trace();
}