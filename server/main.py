# main.py
from fastapi import FastAPI, Form, HTTPException
from pydantic import BaseModel
import datetime
from fastapi import Request

app = FastAPI()

# C#クライアントが期待するレスポンスのデータ構造を定義
# Tokenクラスは、C#のRapidAccessToken.Tokenに対応
class Token(BaseModel):
    AccessToken: str
    ExpirationTerm: str # 例: "3600s"
    CreatedDate: str    # 例: "2025-06-09T10:30:00Z"
    RefreshToken: str
    UserId: str
    LoginId: str

# RapidAccessTokenクラスは、C#のRapidAccessTokenに対応
class RapidAccessToken(BaseModel):
    Token: Token

@app.post("/access_token", response_model=RapidAccessToken)
async def create_access_token(
    exec: str = Form(...),
    login_id: str = Form(...),
    user_password: str = Form(...)
):
    
    """
    C#クライアントからのアクセストークン作成リクエストを処理します。
    POSTリクエストのmultipart/form-dataを受け取り、トークン情報を返します。
    """
    print(f"Received 'exec': {exec}")
    print(f"Received 'login_id': {login_id}")
    print(f"Received 'user_password': {user_password}")

    # ここに認証ロジックを実装します。
    # 例: データベースでlogin_idとuser_passwordを検証する
    if login_id == "aksaks" and user_password == "gazou2022":
        # 認証成功の場合、ダミーのトークン情報を生成
        current_time = datetime.datetime.utcnow().isoformat(timespec='seconds') + 'Z'
        dummy_token = Token(
            AccessToken=f"dummy_access_token_{login_id}_{datetime.datetime.now().timestamp()}",
            ExpirationTerm="3600s", # 例: 1時間
            CreatedDate=current_time,
            RefreshToken=f"dummy_refresh_token_{login_id}",
            UserId="12345",
            LoginId=login_id
        )
        return RapidAccessToken(Token=dummy_token)
    else:
        # 認証失敗の場合
        raise HTTPException(status_code=401, detail="Invalid credentials")

@app.get("/")
async def root():
    return {"message": "Welcome to the FastAPI server!"}