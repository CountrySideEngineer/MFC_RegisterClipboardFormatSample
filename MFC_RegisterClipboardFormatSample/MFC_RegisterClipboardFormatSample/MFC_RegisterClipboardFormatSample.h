
// MFC_RegisterClipboardFormatSample.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'pch.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CMFCRegisterClipboardFormatSampleApp:
// このクラスの実装については、MFC_RegisterClipboardFormatSample.cpp を参照してください
//

class CMFCRegisterClipboardFormatSampleApp : public CWinApp
{
public:
	CMFCRegisterClipboardFormatSampleApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CMFCRegisterClipboardFormatSampleApp theApp;
