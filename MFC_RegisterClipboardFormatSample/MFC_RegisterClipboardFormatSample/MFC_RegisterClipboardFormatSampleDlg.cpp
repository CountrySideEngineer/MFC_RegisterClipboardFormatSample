
// MFC_RegisterClipboardFormatSampleDlg.cpp : 実装ファイル
//

#include "pch.h"
#include "framework.h"
#include "MFC_RegisterClipboardFormatSample.h"
#include "MFC_RegisterClipboardFormatSampleDlg.h"
#include "afxdialogex.h"
#include "CDataSample.h"
#include "CDataSampleExtend.h"
#include "COrgClipBoard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCRegisterClipboardFormatSampleDlg ダイアログ



CMFCRegisterClipboardFormatSampleDlg::CMFCRegisterClipboardFormatSampleDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFC_REGISTERCLIPBOARDFORMATSAMPLE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCRegisterClipboardFormatSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCRegisterClipboardFormatSampleDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCRegisterClipboardFormatSampleDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMFCRegisterClipboardFormatSampleDlg メッセージ ハンドラー

BOOL CMFCRegisterClipboardFormatSampleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CMFCRegisterClipboardFormatSampleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CMFCRegisterClipboardFormatSampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCRegisterClipboardFormatSampleDlg::OnBnClickedButton1()
{
	this->Copy();
	this->Paste();
}

void CMFCRegisterClipboardFormatSampleDlg::Copy()
{
	CDataSample DataSample1(10, _T("SampleData1"));
	CDataSample DataSample1_2(12, _T("SampleData1_2"));
	CDataSample DataSample2(20, _T("SampleData2"));
	CDataSample DataSample2_1(21, _T("SampleData2_1"));
	CDataSample DataSample3(30, _T("SampleData3"));
	CDataSample DataSample3_1(31, _T("SampleData3_1"));
	CDataSample DataSample3_2(32, _T("SampleData3_2"));
	CDataSample DataSample4(40, _T("SampleData4"));
	CDataSampleExtend DataSample4_1(41, _T("ExtendedData4_1"), 411, _T("SampleData4_1"));
	CDataSampleExtend DataSample4_2(42, _T("ExtendedData4_2"), 421, _T("SampleData4_2"));
	CDataSampleExtend DataSample4_3(43, _T("ExtendedData4_2"), 421, _T("SampleData4_2"));

	DataSample1.AddSubData(&DataSample1_2);
	DataSample2.AddSubData(&DataSample2_1);
	DataSample3.AddSubData(&DataSample3_1);
	DataSample3.AddSubData(&DataSample3_2);
	DataSample4.AddSubData(&DataSample4_1);
	DataSample4.AddSubData(&DataSample4_2);
	DataSample4.AddSubData(&DataSample4_3);

	CArray<CDataSample*> SrcData1;
	SrcData1.Add(&DataSample1);
	SrcData1.Add(&DataSample2);
	SrcData1.Add(&DataSample3);
	SrcData1.Add(&DataSample4);

	COrgClipBoard clipBoard;
	clipBoard.Copy(SrcData1);

	for (INT_PTR Index = 0; Index < SrcData1.GetCount(); Index++) {
		CDataSample* Item = SrcData1.GetAt(Index);
		Item->Trace();
	}

}

void CMFCRegisterClipboardFormatSampleDlg::Paste()
{
	CArray<CDataSample*> DstData;
	DstData.RemoveAll();

	COrgClipBoard clipBoard;
	clipBoard.Paste(DstData);
	for (INT_PTR Index = 0; Index < DstData.GetCount(); Index++) {
		CDataSample* Item = DstData.GetAt(Index);
		Item->Trace();

		delete Item;
		Item = NULL;
	}
	DstData.RemoveAll();
}