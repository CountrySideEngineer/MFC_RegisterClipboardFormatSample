#include "pch.h"
#include "CDataSample.h"

/**
 *	デフォルトコンストラクタ
 */
CDataSample::CDataSample()
	: m_SampleNumber(0)
	, m_SampleName(_T(""))
{
	this->m_SubDataSample.RemoveAll();
}

/**
 *	コンストラクタ
 *
 *	@param	SampleNumber	設定するSampleNumberの値
 *	@param	SampleName		設定するSampleNameの値
 */
CDataSample::CDataSample(INT_PTR SampleNumber, CString SampleName)
	: m_SampleNumber(SampleNumber)
	, m_SampleName(SampleName)
{
	this->m_SubDataSample.RemoveAll();
}

/**
 *	デストラクタ
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
 *	コピーコンストラクタ
 *
 *	@param	ItemSrc	コピー元データの実体
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
 *	自身のコピーを生成する。
 *	サブデータの内容は、新たに領域を確保し、そこに同じ値がコピーされる。
 *	(ディープコピー)
 *
 *	@return	コピーされた領域へのポインタ
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
 *	サブデータを追加する。
 *
 *	@param	NewItemBase	追加するサブデータ
 *						ただし、実際に追加されるのは引数で指定された値のコピー。
 */
void CDataSample::AddSubData(CDataSample* NewItemBase)
{
	this->m_SubDataSample.Add(NewItemBase->CreateCopy());

}

/**
 *	サブデータを含む、データの合計を取得する。
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
 *	データの内容を表示する。
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