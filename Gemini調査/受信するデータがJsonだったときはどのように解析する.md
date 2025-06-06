JSON データを解析する場合、MFC 標準ライブラリには直接的な JSON パーサーが含まれていません。そのため、外部の JSON ライブラリを使用するのが一般的です。C++ でよく使われる軽量かつ強力な JSON ライブラリとして、[**nlohmann/json**](https://github.com/nlohmann/json) があります。これはヘッダーオンリーライブラリなので、プロジェクトへの組み込みが非常に簡単です。

ここでは、既存の mfc-multipart-uploader プログラムがサーバーから受信した JSON データを解析し、その内容を表示するように修正します。

### ---

**nlohmann/json ライブラリのセットアップ**

まず、プロジェクトに nlohmann/json ライブラリを追加する必要があります。

1. **GitHub からダウンロード**: [https://github.com/nlohmann/json/releases](https://github.com/nlohmann/json/releases) から最新版の json.hpp ファイルをダウンロードします。  
2. **プロジェクトへの追加**: ダウンロードした json.hpp ファイルを、MFC プロジェクトのソースファイルと同じディレクトリ（例: MultipartFormDlg.cpp がある場所）にコピーします。または、プロジェクトのインクルードパスが設定されている任意の場所に置いても構いません。

これで、プロジェクトのどこからでも json.hpp をインクルードして nlohmann/json ライブラリを使用できるようになります。

### ---

**JSON データ解析のためのコード修正**

既存の CMultipartFormDlg::OnBnClickedButtonSend 関数を修正し、サーバーからのレスポンスが JSON 形式であると仮定して解析と表示を行います。

### **変更点の概要**

1. **json.hpp のインクルード**:  
   * MultipartFormDlg.h に \#include "json.hpp" を追加しました。  
2. **srand の呼び出し**:  
   * OnInitDialog に srand(static\_cast\<unsigned int\>(GetTickCount())); を追加しました。これは GenerateMultipartBoundary 関数で使用される rand() 関数のシードを設定するためのもので、通常はアプリケーションの起動時に一度だけ呼び出します。  
3. **レスポンス受信後の JSON 解析ロジック**:  
   * OnBnClickedButtonSend 関数内の if (dwStatusCode \== HTTP\_STATUS\_OK) ブロック内に JSON 解析のための try-catch ブロックを追加しました。  
   * **CString から std::string への変換**: MFC の CString は Unicode (UTF-16) またはマルチバイト文字セット (\_UNICODE が定義されているか否かによる) で表現されるため、nlohmann/json が通常扱う std::string (UTF-8 を推奨) へと適切に変換する必要があります。WideCharToMultiByte を使用して UTF-8 に変換しています。  
   * **nlohmann::json::parse()**: 受信した生のレスポンス文字列 (std::string 形式) を nlohmann::json::parse() を使って解析します。  
   * **整形して表示**: 解析された json\_data オブジェクトを json\_data.dump(4) で整形された文字列に変換し、m\_editResponse に表示します。4 はインデントのスペース数を意味します。  
   * **std::string から CString への変換**: 表示のために、整形された std::string (UTF-8) を再度 CString (UTF-16) に変換しています。  
   * **エラーハンドリング**: nlohmann::json::parse\_error をキャッチすることで、JSON のフォーマットが不正な場合にエラーメッセージを表示します。

### **サーバー側の変更 (推奨)**

この修正を最大限に活用するには、PHP サーバー側のスクリプトが JSON レスポンスを返すように変更することをお勧めします。

**例: upload.php を JSON レスポンスを返すように修正**

PHP

\<?php  
// upload.php  
header('Content-Type: application/json; charset=UTF-8'); // レスポンスのContent-TypeをJSONに設定

$response \= \[  
    'status' \=\> 'success',  
    'message' \=\> 'Upload processed successfully.',  
    'data' \=\> \[  
        'received\_message' \=\> null,  
        'uploaded\_file' \=\> null  
    \],  
    'errors' \=\> \[\]  
\];

// テキストフィールドのデータを出力  
if (isset($\_POST\['message'\])) {  
    $response\['data'\]\['received\_message'\] \= $\_POST\['message'\];  
} else {  
    $response\['errors'\]\[\] \= 'Message (text field) not found.';  
}

// アップロードされたファイルを出力  
if (isset($\_FILES\['uploadedFile'\])) {  
    $file \= $\_FILES\['uploadedFile'\];  
    $response\['data'\]\['uploaded\_file'\] \= \[  
        'name' \=\> $file\['name'\],  
        'type' \=\> $file\['type'\],  
        'size' \=\> $file\['size'\],  
        'tmp\_name' \=\> $file\['tmp\_name'\]  
    \];

    $upload\_dir \= \_\_DIR\_\_ . '/uploads/'; // 'uploads' ディレクトリがWebサーバーから書き込み可能であることを確認  
    if (\!is\_dir($upload\_dir)) {  
        if (\!mkdir($upload\_dir, 0777, true)) {  
            $response\['status'\] \= 'error';  
            $response\['errors'\]\[\] \= 'Failed to create upload directory.';  
        }  
    }  
      
    $destination \= $upload\_dir . basename($file\['name'\]);  
    if (move\_uploaded\_file($file\['tmp\_name'\], $destination)) {  
        $response\['data'\]\['uploaded\_file'\]\['saved\_path'\] \= $destination;  
    } else {  
        $response\['status'\] \= 'error';  
        $response\['errors'\]\[\] \= 'Failed to move uploaded file. Error code: ' . $file\['error'\];  
    }  
} else {  
    $response\['errors'\]\[\] \= 'Uploaded File not found.';  
}

// エラーがある場合はステータスをerrorにする  
if (\!empty($response\['errors'\])) {  
    $response\['status'\] \= 'error';  
    http\_response\_code(400); // Bad Request  
} else {  
    http\_response\_code(200); // OK  
}

echo json\_encode($response, JSON\_PRETTY\_PRINT | JSON\_UNESCAPED\_UNICODE);  
?\>

この PHP スクリプトは、受け取ったデータを JSON 形式で返すため、MFC クライアント側でその JSON を適切に解析し、結果を表示できるようになります。