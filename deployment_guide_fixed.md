# 修正版服務端部署指南

## 🎯 問題解決方案

您已經有了修正版的服務端代碼 `upload_cgi_fastdfs_fixed.c`，這個版本解決了致命的二進位數據處理問題。

## 🔧 關鍵修正點

### 1. **解決二進位數據處理問題**
- **原問題**：使用 `strstr` 處理包含 `\0` 字節的二進位文件數據
- **修正**：使用 `memmem` 進行二進位安全的記憶體搜尋
- **影響**：現在可以正確處理包含 NUL 字節的文件（如圖片、壓縮檔等）

### 2. **安全的邊界提取**
- **原問題**：`strncpy` 可能包含額外參數
- **修正**：`extract_boundary` 函數正確處理引號和分號

### 3. **修正回傳碼格式**
- **原問題**：回傳 `{"code": 0}` 但前端期望 `"008"`
- **修正**：回傳 `{"code": "008"}` 和 `{"code": "009"}`

## 📋 部署步驟

### 1. 編譯修正版代碼
```bash
# 在服務器上編譯修正版代碼
gcc -o upload_cgi_fastdfs_fixed upload_cgi_fastdfs_fixed.c \
    -lfcgi -lssl -lcrypto -lfdfsclient -lfastcommon
```

### 2. 備份原文件
```bash
# 備份原始文件
cp upload_cgi_fastdfs upload_cgi_fastdfs_backup
```

### 3. 部署新文件
```bash
# 替換為修正版
cp upload_cgi_fastdfs_fixed upload_cgi_fastdfs
chmod +x upload_cgi_fastdfs
```

### 4. 重啟 FastCGI 進程
```bash
# 重啟 FastCGI 進程（根據您的配置）
sudo systemctl restart fcgiwrap
# 或者
sudo killall upload_cgi_fastdfs
# 然後重新啟動進程
```

### 5. 測試部署
```bash
# 測試修正後的服務端
curl -v -F "user=scott2" \
     -F "token=f35e77a875abc8cf3b3e2012822d22a5" \
     -F "filename=test.txt" \
     -F "md5=86c4ee2106096f284077943cfc1f6d43" \
     -F "size=49" \
     -F "file=@test_upload.txt" \
     "http://100.65.29.35:8088/upload"
```

## 🔍 預期結果

### 部署成功後，應該看到：
```json
{"code": "008", "msg": "Upload successful", "data": {"url": "...", "md5": "...", "size": 49, "filename": "test.txt"}}
```

### 而不是之前的錯誤：
```json
{"code": 400, "msg": "Failed to parse multipart data"}
```

## 🛠️ 調試功能

修正版代碼包含調試輸出，會在 stderr 中顯示：
- Content-Type 和 Content-Length
- 提取的 boundary
- 前 256 字節的十六進制數據

這有助於診斷問題：
```bash
# 查看調試輸出
tail -f /var/log/nginx/error.log
```

## 📝 技術細節

### 1. **二進位安全解析**
- 使用 `memmem` 替代 `strstr`
- 不添加 `\0` 終止符到二進位數據
- 正確處理包含 NUL 字節的文件

### 2. **完整的 multipart 解析**
- 正確解析所有字段（user, token, filename, md5, size, file）
- 支持獨立的 filename 文字欄位或 file part 的 filename 屬性
- 嚴格驗證結束邊界 `--boundary--`

### 3. **前端兼容性**
- 回傳碼格式與前端期望一致
- 成功：`"008"`
- 失敗：`"009"`

## 🎉 完成後

部署成功後：
1. **Qt 客戶端應該能夠正常上傳文件**
2. **不再出現 "Failed to parse multipart data" 錯誤**
3. **可以處理包含 NUL 字節的二進位文件**
4. **前端會正確顯示上傳成功/失敗狀態**

## 🚨 注意事項

1. **確保 FastDFS 配置正確**：修正版代碼需要 FastDFS 客戶端庫
2. **檢查權限**：確保新文件有執行權限
3. **監控日誌**：部署後監控服務端日誌確認無錯誤
4. **測試各種文件類型**：特別是圖片、壓縮檔等可能包含 NUL 字節的文件

---

**下一步**：部署修正版服務端代碼後，重新測試 Qt 客戶端上傳功能。
