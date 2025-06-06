// app.js
const express = require('express'); // Express フレームワーク
const formidable = require('formidable'); // multipart/form-data 解析用ライブラリ
const path = require('path'); // パス操作用モジュール
const fs = require('fs'); // ファイルシステム操作用モジュール

const app = express(); // Express アプリケーションのインスタンスを作成

// アップロードファイルを保存するディレクトリ
const UPLOAD_DIR = path.join(__dirname, 'uploads');

// 'uploads' ディレクトリが存在しない場合は作成
if (!fs.existsSync(UPLOAD_DIR)) {
    fs.mkdirSync(UPLOAD_DIR);
}

// CORS ヘッダーを設定するミドルウェア
app.use((req, res, next) => {
    res.setHeader('Access-Control-Allow-Origin', '*'); // すべてのオリジンからのアクセスを許可
    res.setHeader('Access-Control-Allow-Methods', 'POST, GET, OPTIONS');
    res.setHeader('Access-Control-Allow-Headers', 'Content-Type');
    // OPTIONS メソッドの Preflight リクエストに対応 (CORS のため)
    if (req.method === 'OPTIONS') {
        res.writeHead(204); // No Content
        res.end();
        return;
    }
    next(); // 次のミドルウェアまたはルートハンドラへ
});

// ルート定義
// GET リクエストに対するハンドラ
app.get('/', (req, res) => {
    res.writeHead(200, { 'Content-Type': 'text/html; charset=utf-8' });
    res.end('<h1>Node.js Multipart Upload Server (Express)</h1><p>POST your multipart/form-data to /upload</p>');
});

// POST リクエストに対するハンドラ (ファイルアップロード)
app.post('/upload', (req, res) => {
    const form = formidable({
        uploadDir: UPLOAD_DIR, // ファイルのアップロード先ディレクトリ
        keepExtensions: true, // オリジナルの拡張子を保持
        maxFileSize: 10 * 1024 * 1024 // 最大ファイルサイズ (例: 10MB)
    });

    // フォームデータを解析
    form.parse(req, (err, fields, files) => {
        const response = {
            status: 'success',
            message: 'Upload processed successfully.',
            data: {
                received_message: null,
                uploaded_file: null
            },
            errors: []
        };

        if (err) {
            console.error('Error parsing form data:', err);
            response.status = 'error';
            response.message = 'Error parsing form data.';
            response.errors.push(err.message);
            res.status(500).json(response); // Express のレスポンスメソッドを使用
            return;
        }

        // テキストフィールドの処理
        // formidable@3.x では fields は配列形式
        if (fields.message && fields.message.length > 0) {
            response.data.received_message = fields.message[0];
        } else {
            response.errors.push('Message (text field) not found.');
        }

        // アップロードされたファイルの処理
        // formidable@3.x では files は配列形式
        if (files.uploadedFile && files.uploadedFile.length > 0) {
            const uploadedFile = files.uploadedFile[0]; // 最初のファイルを取得
            const oldPath = uploadedFile.filepath; // formidable が一時的に保存したパス
            const newFileName = uploadedFile.originalFilename; // 元のファイル名
            const newPath = path.join(UPLOAD_DIR, newFileName); // 最終保存先パス

            fs.rename(oldPath, newPath, (renameErr) => {
                if (renameErr) {
                    console.error('Error renaming file:', renameErr);
                    response.status = 'error';
                    response.message = 'Failed to save uploaded file.';
                    response.errors.push(`Failed to save file: ${renameErr.message}`);
                    res.status(500).json(response);
                } else {
                    response.data.uploaded_file = {
                        name: uploadedFile.originalFilename,
                        type: uploadedFile.mimetype,
                        size: uploadedFile.size,
                        saved_path: newPath
                    };

                    if (response.errors.length > 0) {
                        response.status = 'error';
                        res.status(400).json(response); // Bad Request
                    } else {
                        res.status(200).json(response); // OK
                    }
                }
            });
        } else {
            response.errors.push('Uploaded file not found.');
            // ファイルがない場合でも、テキストデータが成功していれば status は success のまま
            if (response.errors.length > 0) {
                response.status = 'error';
                res.status(400).json(response); // Bad Request
            } else {
                res.status(200).json(response); // OK
            }
        }
    });
});

// サーバーをリッスン開始
const PORT = 3000;
app.listen(PORT, () => {
    console.log(`Express Server running at http://localhost:${PORT}/`);
    console.log(`Upload directory: ${UPLOAD_DIR}`);
});
