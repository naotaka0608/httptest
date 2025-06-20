
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

#include "json.hpp"
using json = nlohmann::json;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#include <string> // For std::string

// C# 'Token' class equivalent
struct CTokenData
{
	std::string AccessToken;
	std::string ExpirationTerm; // Can be int or string based on actual JSON
	std::string CreatedDate;    // Can be std::string or a date/time type
	std::string RefreshToken;
	std::string UserId;         // Can be int or string
	std::string LoginId;
};

// C# 'RapidAccessToken' class equivalent
struct CRapidAccessToken
{
	CTokenData Token;
};

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
	//RequestHTTP_POST();
	//RequestHTTP_POST2();
	RequestHTTP_POST3();
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
	//CString uri = _T("https://httpbin.org/response-headers");
	CString uri = _T("http://127.0.0.1:80/access_token");
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
			strHeader.Append(_T("Content-Type: multipart/form-data; boundary=%s\r\n"));
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


UINT ChttptestDlg::RequestHTTP_POST2()
{
	// CMyDialog* pDlg = reinterpret_cast<CMyDialog*>(pParam); // If reporting back to UI
	// Or, pass a struct with all necessary data for the request and a way to return result.

	// Define constants
	const CString strServer = _T("127.0.0.1"); // Replace with your actual domain part of uriBase
	const INTERNET_PORT nPort = 80;               // Or 443 for HTTPS
	const CString strObject = _T("/access_token");
	const CString strBoundary = _T("---------------7e03108149e"); // A unique boundary string

	CInternetSession session(_T("MyApiClient"));
	CHttpConnection* pConnection = nullptr;
	CHttpFile* pFile = nullptr;

	try
	{
		pConnection = session.GetHttpConnection(strServer, INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_SECURE, nPort); // Add INTERNET_FLAG_SECURE for HTTPS

		// Build the HTTP headers for MultipartFormData
		CString strHeaders;
		strHeaders.Format(_T("Content-Type: multipart/form-data; boundary=%s\r\n"), strBoundary);

		pFile = pConnection->OpenRequest(
			CHttpConnection::HTTP_VERB_POST,
			strObject,
			nullptr, // Not needed for simple path
			1,       // HTTP 1.1
			nullptr, // Accepts
			nullptr, // Referer
			INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_NO_AUTO_REDIRECT // Use existing connection
		);

		// Prepare the POST data
		CString strPostData;
		strPostData.Format(
			_T("--%s\r\n")         // Boundary
			_T("Content-Disposition: form-data; name=\"exec\"\r\n\r\n") // Field 1 header
			_T("create\r\n")       // Field 1 value
			_T("--%s\r\n")         // Boundary
			_T("Content-Disposition: form-data; name=\"login_id\"\r\n\r\n") // Field 2 header
			_T("aksaks\r\n")       // Field 2 value
			_T("--%s\r\n")         // Boundary
			_T("Content-Disposition: form-data; name=\"user_password\"\r\n\r\n") // Field 3 header
			_T("gazou2022\r\n")    // Field 3 value
			_T("--%s--\r\n"),      // Final boundary
			strBoundary, strBoundary, strBoundary, strBoundary
		);

		// Send the request
		pFile->AddRequestHeaders(strHeaders);
		pFile->SendRequest(strHeaders, strPostData.GetLength(), (LPVOID)(LPCTSTR)strPostData, strPostData.GetLength());

		// Get the response status
		DWORD dwRet;
		pFile->QueryInfoStatusCode(dwRet);
		TRACE(_T("HTTP Status: %lu\n"), dwRet); // For debugging

		if (dwRet == HTTP_STATUS_OK) // Check for success (200 OK)
		{
			// Read the response content
			std::string responseBody;
			char buffer[4096];
			DWORD dwBytesRead;
			while ((dwBytesRead = pFile->Read(buffer, sizeof(buffer) - 1)) > 0)
			{
				buffer[dwBytesRead] = '\0'; // Null-terminate for string conversion
				responseBody += buffer;
			}

			
			// --- JSON Deserialization (using nlohmann/json) ---
			try
			{
				json j = json::parse(responseBody);

				CRapidAccessToken rapidAccessToken;
				rapidAccessToken.Token.AccessToken = j["Token"]["AccessToken"].get<std::string>();
				rapidAccessToken.Token.ExpirationTerm = j["Token"]["ExpirationTerm"].get<std::string>(); // Adjust type as needed
				rapidAccessToken.Token.CreatedDate = j["Token"]["CreatedDate"].get<std::string>();
				rapidAccessToken.Token.RefreshToken = j["Token"]["RefreshToken"].get<std::string>();
				rapidAccessToken.Token.UserId = j["Token"]["UserId"].get<std::string>(); // Adjust type as needed
				rapidAccessToken.Token.LoginId = j["Token"]["LoginId"].get<std::string>();

				// Now you have the data in rapidAccessToken
				// You can send this back to the UI thread using PostMessage or similar
				//TRACE(_T("accessToken : %s\n"), CA2CT(rapidAccessToken.Token.AccessToken.c_str()));
				//TRACE(_T("expirationTerm : %s\n"), CA2CT(rapidAccessToken.Token.ExpirationTerm.c_str()));
				// ... log other values
			}
			catch (const json::exception& e)
			{
				//TRACE(_T("JSON Parsing Error: %s\n"), CA2CT(e.what()));
				// Handle JSON parsing error
			}
			
		}
		else
		{
			// Handle HTTP error (e.g., 404, 500, etc.)
			TRACE(_T("HTTP Request Failed with status: %lu\n"), dwRet);
		}
	}
	catch (CInternetException* pEx)
	{
		TCHAR szErr[256];
		pEx->GetErrorMessage(szErr, sizeof(szErr) / sizeof(TCHAR));
		TRACE(_T("Internet Exception: %s\n"), szErr);
		pEx->Delete();
	}
	catch (const std::exception& e)
	{
		//TRACE(_T("Standard Exception: %s\n"), CA2CT(e.what()));
	}
	catch (...)
	{
		TRACE(_T("Unknown Exception occurred.\n"));
	}

	// Clean up
	if (pFile) pFile->Close();
	if (pConnection) pConnection->Close();
	session.Close();

	// If you need to update UI, use PostMessage to send results back to the main thread
	// pDlg->PostMessage(WM_MY_CUSTOM_MESSAGE, 0, (LPARAM)&rapidAccessToken); // Example
	return 0; // Thread ends
}

#include "httplib.h" // httplib.h をインクルード

UINT ChttptestDlg::RequestHTTP_POST3()
{
	// CMyDialog* pDlg = reinterpret_cast<CMyDialog*>(pParam); // If reporting back to UI
	// Or, pass a struct with all necessary data for the request and a way to return result.

	// Define constants
	const char* host = "127.0.0.1";
	int port = 8000;
	const char* path = "/access_token";
	bool is_https = false; // 必要に応じて true に変更

	CInternetSession session(_T("MyApiClient"));
	CHttpConnection* pConnection = nullptr;
	CHttpFile* pFile = nullptr;


	std::unique_ptr<httplib::Client> cli;

	cli = std::make_unique<httplib::Client>(host, port);

	if (!cli) {
		TRACE(_T("Failed to create HTTP client.\n"));
		return 1;
	}


	// 必要に応じてタイムアウト設定
	cli->set_connection_timeout(10); // 10秒
	cli->set_read_timeout(30);       // 30秒
	cli->set_write_timeout(30);      // 30秒

	httplib::MultipartFormDataItems items;
	items.push_back({ "exec", "create", "", "" }); // name, content, filename, content_type
	items.push_back({ "login_id", "aksaks", "", "" });
	items.push_back({ "user_password", "gazou2022", "", "" });

	// リクエストを実行
	auto res = cli->Post(path, items);

	if (res) {
		TRACE(_T("HTTP Status Code: %d\n"), res->status);

		if (res->status == 200) {
			//TRACE(_T("Response Body:\n%s\n"), CA2CT(res->body.c_str()));

			// --- JSON Deserialization (using nlohmann/json) ---
			try
			{
				json j = json::parse(res->body);

				CRapidAccessToken rapidAccessToken;
				rapidAccessToken.Token.AccessToken = j["Token"]["AccessToken"].get<std::string>();
				rapidAccessToken.Token.ExpirationTerm = j["Token"]["ExpirationTerm"].get<std::string>(); // Adjust type as needed
				rapidAccessToken.Token.CreatedDate = j["Token"]["CreatedDate"].get<std::string>();
				rapidAccessToken.Token.RefreshToken = j["Token"]["RefreshToken"].get<std::string>();
				rapidAccessToken.Token.UserId = j["Token"]["UserId"].get<std::string>(); // Adjust type as needed
				rapidAccessToken.Token.LoginId = j["Token"]["LoginId"].get<std::string>();

				// Now you have the data in rapidAccessToken
				// You can send this back to the UI thread using PostMessage or similar
				//TRACE(_T("accessToken : %s\n"), CA2CT(rapidAccessToken.Token.AccessToken.c_str()));
				//TRACE(_T("expirationTerm : %s\n"), CA2CT(rapidAccessToken.Token.ExpirationTerm.c_str()));
				// ... log other values
			}
			catch (const json::exception& e)
			{
				//TRACE(_T("JSON Parsing Error: %s\n"), CA2CT(e.what()));
				// Handle JSON parsing error
			}
		}


	}




	// If you need to update UI, use PostMessage to send results back to the main thread
	// pDlg->PostMessage(WM_MY_CUSTOM_MESSAGE, 0, (LPARAM)&rapidAccessToken); // Example
	return 0; // Thread ends
}