#include "pch.h"
#include "CDataSampleExtend.h"

/**
 *	デフォルトコンストラクタ
 */
CDataSampleExtend::CDataSampleExtend()
	: m_ExtendedNumber(0)
	, m_ExtendedName(_T(""))
	, CDataSample()
{}

/**
 *	引数ありのコンストラクタ
 *
 *	@param	ExtendedNumber	拡張番号
 *	@param	ExtendedName	拡張名前
 *	@param	SampleNumber	試作番号
 *	@param	SampleName		試作番号
 */
CDataSampleExtend::CDataSampleExtend(INT_PTR ExtendedNumber, CString ExtendedName, INT_PTR SampleNumber, CString SampleName)
	: m_ExtendedNumber(0)
	, m_ExtendedName(ExtendedName)
	, CDataSample(SampleNumber, SampleName)
{}

/**
 *	コピーコンストラクタ
 *
 *	@param[in]	SrcItem	コピー元データへのポインタ
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
 *	自身のコピーを生成する。
 *	サブデータの内容は、新たに領域を確保し、そこに同じ値がコピーされる。
 *	(ディープコピー)
 *
 *	@return	コピーされた領域へのポインタ
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
 *	データの内容を表示する。
 */
void CDataSampleExtend::Trace()
{
	TRACE(_T("ExtendedNumber = %5d\n"), this->m_ExtendedNumber);
	TRACE(_T("ExtendedName = %s\n"), this->m_ExtendedName);

	CDataSample::Trace();
}