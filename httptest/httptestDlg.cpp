
// httptestDlg.cpp : 実装ファイル
//

#include "pch.h"
#include "framework.h"
#include "httptest.h"
#include "httptestDlg.h"
#include "afxdialogex.h"

#include <afx.h>
#include <afxwin.h>
#include <afxinet.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// ChttptestDlg ダイアログ



ChttptestDlg::ChttptestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HTTPTEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

void ChttptestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(ChttptestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SendHttp, &ChttptestDlg::OnBnClickedButtonSendhttp)
END_MESSAGE_MAP()


// ChttptestDlg メッセージ ハンドラー

BOOL ChttptestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "バージョン情報..." メニューをシステム メニューに追加します。

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void ChttptestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void ChttptestDlg::OnPaint()
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
HCURSOR ChttptestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void ChttptestDlg::OnBnClickedButtonSendhttp()
{
	// RequestHTTP_GET();
	RequestHTTP_POST();
}

BOOL ChttptestDlg::RequestHTTP_GET()
{
	BOOL bResult = FALSE;
	CInternetSession session;

	CHttpConnection* pConnection = NULL;

	CString strResponse = _T("");

	CString uri = _T("https://httpbin.org/get");
	// CString uri = _T("google.com");
	CString strServer, strObj, strHeader;
	INTERNET_PORT nPort;
	DWORD dwServiceType;

	try
	{
		::AfxParseURL(uri, dwServiceType, strServer, strObj, nPort);
		pConnection = (CHttpConnection*)session.GetHttpConnection(strServer, INTERNET_FLAG_SECURE, nPort, NULL, NULL);
		// pConnection = (CHttpConnection*)session.GetHttpConnection(uri, (INTERNET_PORT)INTERNET_DEFAULT_HTTP_PORT);
		// pConnection = (CHttpConnection*)session.GetHttpConnection(_T("google.com"), (INTERNET_PORT)INTERNET_DEFAULT_HTTP_PORT);

		if (pConnection)
		{
			//strHeader.Append(_T("Content-Type: application/json ; charset=\"utf-8\"\n"));
			strHeader.Append(_T("Content-Type: application/json ; charset=\"utf-8\"\n"));


			// 要求
			DWORD httpsFlags = INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID;
			CHttpFile* pHTTPFile = NULL;
			pHTTPFile = pConnection->OpenRequest(CHttpConnection::HTTP_VERB_GET, strObj, NULL, 1, NULL, _T("HTTP/1.1"), httpsFlags);
			// pHTTPFile = pConnection->OpenRequest(CHttpConnection::HTTP_VERB_POST, _T("/"));

			if (pHTTPFile)
			{
				// 自己証明書の許可
				DWORD dwSecFlags;
				pHTTPFile->QueryOption(INTERNET_OPTION_SECURITY_FLAGS, dwSecFlags);
				dwSecFlags |= SECURITY_FLAG_IGNORE_UNKNOWN_CA;
				pHTTPFile->SetOption(INTERNET_OPTION_SECURITY_FLAGS, dwSecFlags);

				// bResult = pHTTPFile->SendRequest();
				bResult = pHTTPFile->SendRequest(strHeader, NULL, 0);
				if (bResult)
				{
					char* pszBuff = new char[1025];
					int nRead = 0;
					while ((nRead = pHTTPFile->Read(pszBuff, 1024)) > 0)
					{
						pszBuff[nRead] = 0;
						strResponse += pszBuff;

					};

					delete[] pszBuff;

					bResult = TRUE;
				}

				pHTTPFile->Close();

				delete pHTTPFile;
			}

			pConnection->Close();
			delete pConnection;
		}
	}
	catch (CInternetException* pEx)
	{
		pConnection = NULL;
		pEx->Delete();
	}

	session.Close();

	return bResult;
}

BOOL ChttptestDlg::RequestHTTP_POST()
{
	BOOL bResult = FALSE;
	CInternetSession session;

	CHttpConnection* pConnection = NULL;

	CString strResponse = _T("");

	// CString uri = _T("https://httpbin.org/post");
	CString uri = _T("https://httpbin.org/response-headers");
	// CString uri = _T("google.com");
	CString strServer, strObj, strHeader;
	INTERNET_PORT nPort;
	DWORD dwServiceType;

	try
	{
		::AfxParseURL(uri, dwServiceType, strServer, strObj, nPort);
		pConnection = (CHttpConnection*)session.GetHttpConnection(strServer, INTERNET_FLAG_SECURE, nPort, NULL, NULL);
		// pConnection = (CHttpConnection*)session.GetHttpConnection(uri, (INTERNET_PORT)INTERNET_DEFAULT_HTTP_PORT);
		// pConnection = (CHttpConnection*)session.GetHttpConnection(_T("google.com"), (INTERNET_PORT)INTERNET_DEFAULT_HTTP_PORT);

		if (pConnection)
		{
			//strHeader.Append(_T("Content-Type: application/json ; charset=\"utf-8\"\n"));
			strHeader.Append(_T("Content-Type: application/json ; charset=\"utf-8\"\n"));
			// strHeaders = _T("Content-Type: application/x-www-form-urlencoded");


			// 要求
			DWORD httpsFlags = INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID;
			CHttpFile* pHTTPFile = NULL;
			pHTTPFile = pConnection->OpenRequest(CHttpConnection::HTTP_VERB_POST, strObj, NULL, 1, NULL, _T("HTTP/1.1"), httpsFlags);
			// pHTTPFile = pConnection->OpenRequest(CHttpConnection::HTTP_VERB_POST, _T("/"));

			// TCHAR	headers[512];
			// _stprintf_s(headers, 512, _T("Accept: text/*\r\nUser-Agent: %s \r\nContent-Type: text/plain; charset=Shift_JIS\r\n"), _T("ASCII Program Name"));
			// pHTTPFile->AddRequestHeaders(headers);
			// pHTTPFile->AddRequestHeaders("Accept: text/*\r\nUser-Agent: HttpPost\r\nContent-Type: application/octet-stream\r\n");

			if (pHTTPFile)
			{
				// 自己証明書の許可
				DWORD dwSecFlags;
				pHTTPFile->QueryOption(INTERNET_OPTION_SECURITY_FLAGS, dwSecFlags);
				dwSecFlags |= SECURITY_FLAG_IGNORE_UNKNOWN_CA;
				pHTTPFile->SetOption(INTERNET_OPTION_SECURITY_FLAGS, dwSecFlags);

				// bResult = pHTTPFile->SendRequest();
				// bResult = pHTTPFile->SendRequest(strHeader, NULL, 0);
				
				// CString strFormData = _T("name=John+Doe&userid=hithere&other=P%26Q");
				CString strFormData = _T("freeform=aaaa");
				bResult = pHTTPFile->SendRequest(strHeader, (LPVOID)(LPCTSTR)strFormData, strFormData.GetLength());

				/*
				DWORD status = 0;
				pHTTPFile->QueryInfoStatusCode(status);
				if ((status >= 200) && (status <= 299)) {
					{
						char bb[4096];
						int readlen = pHTTPFile->Read(bb, sizeof(bb));
						while (readlen > 0) {
							ReplyString += CString(bb, readlen);
							readlen = pHTTPFile->Read(bb, sizeof(bb));
						}
					}
				}
				else {
					switch (status) {
					case	404:
						break;
					default:
					}
				}
				*/


				if (bResult)
				{
					char* pszBuff = new char[1025];
					int nRead = 0;
					while ((nRead = pHTTPFile->Read(pszBuff, 1024)) > 0)
					{
						pszBuff[nRead] = 0;
						strResponse += pszBuff;

					};

					delete[] pszBuff;

					bResult = TRUE;
				}

				pHTTPFile->Close();

				delete pHTTPFile;
			}

			pConnection->Close();
			delete pConnection;
		}
	}
	catch (CInternetException* pEx)
	{
		pConnection = NULL;
		pEx->Delete();
	}

	session.Close();

	return bResult;
}
