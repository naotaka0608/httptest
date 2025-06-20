﻿
// httptestDlg.h : ヘッダー ファイル
//

#pragma once


// ChttptestDlg ダイアログ
class ChttptestDlg : public CDialogEx
{

private :
	BOOL RequestHTTP_GET();
	BOOL RequestHTTP_POST();
	UINT RequestHTTP_POST2();
	UINT RequestHTTP_POST3();

// コンストラクション
public:
	ChttptestDlg(CWnd* pParent = nullptr);	// 標準コンストラクター

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HTTPTEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSendhttp();
};
