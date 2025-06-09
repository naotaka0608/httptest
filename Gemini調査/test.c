// In your MFC Dialog or Logic Class (e.g., CMyApiClient.cpp)
// Add necessary includes:
#include <afxinet.h>    // For CInternetSession, CHttpConnection, CHttpFile
#include <afxsock.h>    // For AfxSocketInit() if not already done
#include <vector>       // For std::vector<BYTE>
#include <memory>       // For std::unique_ptr
#include "RapidAccessToken.h" // Your defined JSON structures
// For JSON parsing (assuming you've included nlohmann/json.hpp)
#include <nlohmann/json.hpp>
using json = nlohmann::json;

// --- Function to perform the HTTP POST (should be called in a separate thread) ---
UINT MyHttpPostThreadProc(LPVOID pParam)
{
    // CMyDialog* pDlg = reinterpret_cast<CMyDialog*>(pParam); // If reporting back to UI
    // Or, pass a struct with all necessary data for the request and a way to return result.

    // Define constants
    const CString strServer = _T("example.com"); // Replace with your actual domain part of uriBase
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
                TRACE(_T("accessToken : %s\n"), CA2CT(rapidAccessToken.Token.AccessToken.c_str()));
                TRACE(_T("expirationTerm : %s\n"), CA2CT(rapidAccessToken.Token.ExpirationTerm.c_str()));
                // ... log other values
            }
            catch (const json::exception& e)
            {
                TRACE(_T("JSON Parsing Error: %s\n"), CA2CT(e.what()));
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
        TRACE(_T("Standard Exception: %s\n"), CA2CT(e.what()));
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