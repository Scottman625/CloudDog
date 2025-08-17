// upload_cgi_fastdfs_fixed.c - 修正版（解決二進位數據處理問題）
#define _GNU_SOURCE
#include <linux/limits.h>
#ifndef PATH_MAX
#define PATH_MAX 4096
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <pthread.h>
#include <fcgi_stdio.h>
#include <openssl/md5.h>
#include <sys/stat.h>
#include <errno.h>

// FastDFS 頭文件
#include <fastdfs/fdfs_client.h>
#include <fastdfs/tracker_client.h>
#include <fastdfs/storage_client1.h>
#include <fastcommon/logger.h>

#define MAX_FILE_SIZE (100 * 1024 * 1024) // 100MB
#define MAX_FILENAME_LEN 256
#define MAX_URL_LEN 512
#define MAX_FIELD_LEN 1024

// 結構體用於存儲解析的字段
typedef struct {
    char user[256];
    char token[256];
    char filename[256];
    char md5[33];
    char size[32];
    const char* file_data;
    size_t file_size;
} multipart_fields_t;

// 全局變量
static int g_fastdfs_initialized = 0;

// 安全的 memmem 實現（如果系統沒有提供）
static const void* my_memmem(const void* haystack, size_t haystack_len,
                            const void* needle, size_t needle_len) {
    if (!haystack || !needle || needle_len == 0 || haystack_len < needle_len) 
        return NULL;
    
    const unsigned char *h = (const unsigned char*)haystack;
    const unsigned char *n = (const unsigned char*)needle;
    
    for (size_t i = 0; i + needle_len <= haystack_len; ++i) {
        if (h[i] == n[0] && memcmp(h + i, n, needle_len) == 0) 
            return h + i;
    }
    return NULL;
}

// 使用系統的 memmem 或我們的實現
#ifdef __GLIBC__
#define SAFE_MEMMEM memmem
#else
#define SAFE_MEMMEM my_memmem
#endif

// 初始化 FastDFS 客戶端
int init_fastdfs() {
    if (g_fastdfs_initialized) {
        return 0;
    }

    if (fdfs_client_init("/etc/fdfs/client.conf") != 0) {
        return -1;
    }

    g_fastdfs_initialized = 1;
    return 0;
}

// 上傳文件到 FastDFS
int upload_to_fastdfs(const char* file_data, size_t file_size,
                      const char* file_ext, char* file_url, size_t url_size) {
    ConnectionInfo *pTrackerServer = NULL;
    ConnectionInfo storageServer;
    int store_path_index = 0;
    char group_name[FDFS_GROUP_NAME_MAX_LEN + 1] = {0};
    char file_id[256] = {0};
    int result;

    if (!g_fastdfs_initialized) {
        return -1;
    }

    pTrackerServer = tracker_get_connection();
    if (pTrackerServer == NULL) {
        return -1;
    }

    result = tracker_query_storage_store(pTrackerServer, &storageServer,
                                       group_name, &store_path_index);

    if (result != 0) {
        tracker_close_connection_ex(pTrackerServer, true);
        return -1;
    }

    result = storage_upload_by_filebuff1(pTrackerServer, &storageServer,
                                       store_path_index, file_data, file_size,
                                       file_ext, NULL, 0, group_name, file_id);

    if (result == 0) {
        snprintf(file_url, url_size, "http://your-fastdfs-server/%s", file_id);
    }

    tracker_close_connection_ex(pTrackerServer, true);
    return result;
}

// 生成文件的 MD5 哈希
void generate_md5(const char *data, size_t len, char *md5_str) {
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, data, len);
    MD5_Final(digest, &ctx);

    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        sprintf(&md5_str[i*2], "%02x", digest[i]);
    }
    md5_str[32] = '\0';
}

// 獲取文件擴展名
const char* get_file_extension(const char* filename) {
    const char* dot = strrchr(filename, '.');
    if (!dot || dot == filename) return "";
    return dot + 1;
}

// 安全的邊界提取
int extract_boundary(const char* content_type, char* out, size_t out_size) {
    char *b = strstr(content_type, "boundary=");
    if (!b) return -1;
    b += 9;
    
    // 跳過空格
    while (*b == ' ') b++;
    
    // 處理引號
    if (*b == '"') {
        b++;
        char *q = strchr(b, '"');
        if (q) *q = '\0';
    } else {
        char *semi = strchr(b, ';');
        if (semi) *semi = '\0';
    }
    
    strncpy(out, b, out_size - 1);
    out[out_size - 1] = '\0';
    return 0;
}

// 重新寫的 multipart 解析（修正結束邊界處理邏輯）
int parse_multipart_data_bin(const char* buf, size_t len,
                            const char* boundary, multipart_fields_t* outf) {
    memset(outf, 0, sizeof(*outf));
    
    char delim[300];
    char end_delim[300];
    int delim_len = snprintf(delim, sizeof(delim), "--%s", boundary);      // --boundary
    int end_delim_len = snprintf(end_delim, sizeof(end_delim), "--%s--", boundary); // --boundary--

    const unsigned char *start = (const unsigned char*)buf;
    const unsigned char *end = start + len;

    // 找第一個 --boundary
    const unsigned char *cur = SAFE_MEMMEM(start, end - start, delim, delim_len);
    if (!cur) return -10;
    cur += delim_len;
    // 第一個分隔線後必須是 \r\n
    if (cur + 2 > end || cur[0] != '\r' || cur[1] != '\n') return -11;
    cur += 2; // 進入第一個 part header 開頭

    int finished = 0;

    while (!finished && cur < end) {

        // 如果 cur 正好在結束邊界起始（理論上不會發生，因為上一輪會處理），仍容錯：
        if ((size_t)(end - cur) >= (size_t)end_delim_len &&
            memcmp(cur, end_delim, end_delim_len) == 0) {
            finished = 1;
            break;
        }

        // 1. 尋找 headers 與正文分隔 (\r\n\r\n)
        const unsigned char *h_end = NULL;
        for (const unsigned char *q = cur; q + 3 < end; ++q) {
            if (q[0] == '\r' && q[1] == '\n' && q[2] == '\r' && q[3] == '\n') {
                h_end = q;
                break;
            }
        }
        if (!h_end) return -12;

        // 2. 複製 headers
        size_t headers_len = h_end - cur;
        char *headers = (char*)malloc(headers_len + 1);
        if (!headers) return -13;
        memcpy(headers, cur, headers_len);
        headers[headers_len] = '\0';

        const unsigned char *data_start = h_end + 4;

        // 3. 尋找下一個邊界的起點（必須是 \r\n--boundary 或 \r\n--boundary--）
        const unsigned char *next_boundary_marker = NULL;
        const unsigned char pattern[] = "\r\n--";
        const size_t pattern_len = 4;
        const unsigned char *search_from = data_start;

        while (1) {
            const unsigned char *candidate = SAFE_MEMMEM(search_from, end - search_from,
                                                         pattern, pattern_len);
            if (!candidate) { free(headers); return -14; }
            // candidate 指向 \r\n-- 之 \r
            const unsigned char *dashdash = candidate + 2; // 指向 "--boundary..."
            if ((size_t)(end - dashdash) >= (size_t)end_delim_len &&
                memcmp(dashdash, end_delim, end_delim_len) == 0) {
                next_boundary_marker = candidate; // 指向 \r
                break;
            }
            if ((size_t)(end - dashdash) >= (size_t)delim_len &&
                memcmp(dashdash, delim, delim_len) == 0) {
                next_boundary_marker = candidate; // 指向 \r
                break;
            }
            // 否則這只是正文內的偶然字串，繼續往後找
            search_from = candidate + pattern_len;
        }

        if (!next_boundary_marker) {
            free(headers);
            return -15;
        }

        const unsigned char *data_end = next_boundary_marker; // 指向前一個 \r
        if (data_end < data_start) { free(headers); return -16; }
        size_t data_len = data_end - data_start;

        // 4. 解析 headers 裡的 name / filename（大小寫不敏感）
        char *name = NULL;
        char *filename = NULL;
        {
            // 建立可修改副本進行大小寫不敏感解析
            char headers_copy[2048];
            strncpy(headers_copy, headers, sizeof(headers_copy) - 1);
            headers_copy[sizeof(headers_copy) - 1] = '\0';
            
            // 轉小寫進行匹配
            for (char *p = headers_copy; *p; ++p) {
                *p = (char)tolower((unsigned char)*p);
            }
            
            const char *cd = strstr(headers_copy, "content-disposition:");
            if (cd) {
                // 在原始 headers 中找到對應位置
                size_t offset = cd - headers_copy;
                const char *cd_orig = headers + offset;
                
                // 解析 name
                const char *n = strstr(cd, "name=\"");
                if (n) {
                    n += 6;
                    const char *n2 = strchr(n, '"');
                    if (n2) {
                        size_t L = n2 - n;
                        name = (char*)malloc(L + 1);
                        if (name) { 
                            memcpy(name, n, L); 
                            name[L] = '\0'; 
                        }
                    }
                }
                
                // 解析 filename
                const char *fn = strstr(cd, "filename=\"");
                if (fn) {
                    fn += 10;
                    const char *fn2 = strchr(fn, '"');
                    if (fn2) {
                        size_t L = fn2 - fn;
                        filename = (char*)malloc(L + 1);
                        if (filename) { 
                            memcpy(filename, fn, L); 
                            filename[L] = '\0'; 
                        }
                    }
                }
            }
        }

        // 判斷是否為檔案 part
        int is_file_part = 0;
        if (name && strcmp(name, "file") == 0) {
            is_file_part = 1;
        } else if (filename && outf->file_data == NULL) {
            // 客戶端用了其他 name（例如 "upload"），但有 filename= 就視為檔案
            is_file_part = 1;
        }

        // 剪掉尾端 CRLF（文字欄位與檔案都可視情況處理）
        size_t effective_len = data_len;
        if (effective_len >= 2 && data_start[effective_len - 2] == '\r' && data_start[effective_len - 1] == '\n') {
            effective_len -= 2;
        }

        if (is_file_part) {
            outf->file_data = (const char*)data_start;
            outf->file_size = effective_len;
            if (filename && outf->filename[0] == '\0') {
                strncpy(outf->filename, filename, sizeof(outf->filename) - 1);
            }
        } else if (name) {
            // 根據 name 填文字欄位（同樣用 effective_len）
            if (strcmp(name, "user") == 0) {
                size_t cp = effective_len < sizeof(outf->user) - 1 ? effective_len : sizeof(outf->user) - 1;
                memcpy(outf->user, data_start, cp);
                outf->user[cp] = '\0';
            } else if (strcmp(name, "token") == 0) {
                size_t cp = effective_len < sizeof(outf->token) - 1 ? effective_len : sizeof(outf->token) - 1;
                memcpy(outf->token, data_start, cp);
                outf->token[cp] = '\0';
            } else if (strcmp(name, "md5") == 0) {
                size_t cp = effective_len < sizeof(outf->md5) - 1 ? effective_len : sizeof(outf->md5) - 1;
                memcpy(outf->md5, data_start, cp);
                outf->md5[cp] = '\0';
            } else if (strcmp(name, "size") == 0) {
                size_t cp = effective_len < sizeof(outf->size) - 1 ? effective_len : sizeof(outf->size) - 1;
                memcpy(outf->size, data_start, cp);
                outf->size[cp] = '\0';
            } else if (strcmp(name, "filename") == 0) {
                if (outf->filename[0] == '\0') {
                    size_t cp = effective_len < sizeof(outf->filename) - 1 ? effective_len : sizeof(outf->filename) - 1;
                    memcpy(outf->filename, data_start, cp);
                    outf->filename[cp] = '\0';
                }
            }
        }

        // 調試輸出：每個 part 的處理結果
        size_t preview = effective_len < 30 ? effective_len : 30;
        fprintf(stderr, "[DEBUG][PART] name='%s' filename='%s' data_len=%zu effective_len=%zu preview='",
                name ? name : "(null)", filename ? filename : "(null)", data_len, effective_len);
        for (size_t i = 0; i < preview; i++) {
            unsigned char c = data_start[i];
            fputc((c >= 32 && c < 127) ? c : '.', stderr);
        }
        fprintf(stderr, "'\n");

        free(name);
        free(filename);
        free(headers);

        // 5. 判斷 next_boundary_marker 對應的是普通分隔線還是結束分隔線
        const unsigned char *dashdash = next_boundary_marker + 2; // 指向 "--boundary..."
        int is_final = 0;
        if ((size_t)(end - dashdash) >= (size_t)end_delim_len &&
            memcmp(dashdash, end_delim, end_delim_len) == 0) {
            is_final = 1;
        }

        if (is_final) {
            finished = 1;
            break;  // 完整結束
        } else {
            // 普通下一個 part：格式 "\r\n--boundary\r\n"
            const unsigned char *after = dashdash + delim_len;
            if (after + 2 > end || after[0] != '\r' || after[1] != '\n') {
                return -17;
            }
            cur = after + 2; // 下一個 part headers 開頭
        }
    }

    // 最終調試輸出
    fprintf(stderr,
        "[DEBUG][FINAL] user='%s' token='%s' filename='%s' file_data=%p file_size=%zu md5='%s' size='%s'\n",
        outf->user, outf->token, outf->filename,
        outf->file_data, outf->file_size, outf->md5, outf->size);

    if (!outf->file_data || outf->file_size == 0 ||
        outf->user[0] == '\0' || outf->token[0] == '\0' ||
        outf->filename[0] == '\0') {
        fprintf(stderr, "[DEBUG][-18] user_empty=%d token_empty=%d filename_empty=%d file_missing=%d\n",
                outf->user[0] == 0, outf->token[0] == 0, outf->filename[0] == 0, outf->file_data == NULL);
        return -18;
    }
    return 0;
}

// 發送 JSON 響應（修正為與前端一致的格式）
void send_json_response(const char* code, const char* msg, const char* data) {
    printf("Content-Type: application/json\r\n\r\n");
    if (data) {
        printf("{\"code\": \"%s\", \"msg\": \"%s\", \"data\": %s}", code, msg, data);
    } else {
        printf("{\"code\": \"%s\", \"msg\": \"%s\"}", code, msg);
    }
}

int main() {
    char *content_length_str;
    char *content_type;
    long content_length = 0;
    char *request_data = NULL;
    char boundary[256] = {0};
    multipart_fields_t fields;
    char md5_hash[33] = {0};
    char file_url[MAX_URL_LEN] = {0};

    // 初始化 FastDFS 客戶端
    if (init_fastdfs() != 0) {
        send_json_response("009", "FastDFS client initialization failed", NULL);
        return 1;
    }

    while (FCGI_Accept() >= 0) {
        // 重置變量
        memset(&fields, 0, sizeof(fields));
        memset(md5_hash, 0, sizeof(md5_hash));
        memset(file_url, 0, sizeof(file_url));

        // 獲取內容長度
        content_length_str = getenv("CONTENT_LENGTH");
        if (!content_length_str) {
            send_json_response("009", "Missing Content-Length header", NULL);
            continue;
        }

        content_length = atol(content_length_str);
        if (content_length <= 0) {
            send_json_response("009", "Invalid Content-Length", NULL);
            continue;
        }

        if (content_length > MAX_FILE_SIZE) {
            send_json_response("009", "File too large", NULL);
            continue;
        }

        // 獲取內容類型
        content_type = getenv("CONTENT_TYPE");
        if (!content_type || strncmp(content_type, "multipart/form-data", 19) != 0) {
            send_json_response("009", "Invalid Content-Type", NULL);
            continue;
        }

        // 提取 boundary（使用安全的提取方法）
        if (extract_boundary(content_type, boundary, sizeof(boundary)) != 0) {
            send_json_response("009", "Missing boundary in Content-Type", NULL);
            continue;
        }
        fprintf(stderr, "[DEBUG] boundary(clean)='%s'\n", boundary);

        // 分配內存讀取請求數據（不要加 '\0'）
        request_data = malloc(content_length);
        if (!request_data) {
            send_json_response("009", "Memory allocation failed", NULL);
            continue;
        }

        // 循環讀取請求數據（FastCGI 不保證一次讀滿）
        size_t bytes_read = 0;
        while (bytes_read < (size_t)content_length) {
            size_t n = fread(request_data + bytes_read, 1,
                             content_length - bytes_read, stdin);
            if (n == 0) {
                send_json_response("009", "Failed to read request data", NULL);
                free(request_data);
                request_data = NULL;
                goto loop_end;
            }
            bytes_read += n;
        }

        // 調試輸出
        fprintf(stderr, "[DEBUG] CONTENT_TYPE=%s\n", content_type);
        fprintf(stderr, "[DEBUG] CONTENT_LENGTH=%ld bytes_read=%zu\n", content_length, bytes_read);
        fprintf(stderr, "[DEBUG] boundary='%s' len=%zu\n", boundary, strlen(boundary));
        fprintf(stderr, "[DEBUG] First 256 bytes (hex): ");
        for (int i = 0; i < bytes_read && i < 256; i++) {
            fprintf(stderr, "%02X ", (unsigned char)request_data[i]);
        }
        fprintf(stderr, "\n");

        // 原始 body dump 功能
        {
            FILE *df = fopen("/tmp/upload_body_last.bin", "wb");
            if (df) {
                fwrite(request_data, 1, bytes_read, df);
                fclose(df);
                fprintf(stderr, "[DEBUG] Raw body dumped to /tmp/upload_body_last.bin\n");
            }
        }

        // 解析 multipart 數據（使用二進位安全的方法）
        int pcode = parse_multipart_data_bin(request_data, bytes_read, boundary, &fields);
        if (pcode != 0) {
            fprintf(stderr, "[DEBUG] parse_multipart_data_bin return=%d\n", pcode);
            send_json_response("009", "Failed to parse multipart data", NULL);
            free(request_data);
            request_data = NULL;
            goto loop_end;
        }

        // 解析成功時的調試輸出
        fprintf(stderr, "[DEBUG] user=%s filename=%s file_size=%zu md5=%s size_field=%s\n", 
                fields.user, fields.filename, fields.file_size, fields.md5, fields.size);

        // 驗證文件大小
        if (fields.file_size == 0) {
            send_json_response("009", "Empty file", NULL);
            free(request_data);
            continue;
        }

        // 生成 MD5 哈希並驗證
        generate_md5(fields.file_data, fields.file_size, md5_hash);
        if (strlen(fields.md5) > 0 && strcmp(md5_hash, fields.md5) != 0) {
            send_json_response("009", "MD5 verification failed", NULL);
            free(request_data);
            continue;
        }

        // 驗證文件大小
        if (strlen(fields.size) > 0) {
            long expected_size = atol(fields.size);
            if (expected_size != (long)fields.file_size) {
                send_json_response("009", "File size mismatch", NULL);
                free(request_data);
                continue;
            }
        }

        // TODO: 在這裡添加用戶身份驗證邏輯
        // if (!verify_user_token(fields.user, fields.token)) {
        //     send_json_response("009", "Invalid user token", NULL);
        //     free(request_data);
        //     continue;
        // }

        // 上傳到 FastDFS
        const char* file_ext = get_file_extension(fields.filename);
        int upload_result = upload_to_fastdfs(fields.file_data, fields.file_size, file_ext,
                                            file_url, sizeof(file_url));

        if (upload_result == 0) {
            // 構建成功響應數據
            char response_data[1024];
            snprintf(response_data, sizeof(response_data),
                    "{\"url\": \"%s\", \"md5\": \"%s\", \"size\": %zu, \"filename\": \"%s\"}",
                    file_url, md5_hash, fields.file_size, fields.filename);
            send_json_response("008", "Upload successful", response_data);
        } else {
            send_json_response("009", "Upload to FastDFS failed", NULL);
        }

        free(request_data);
        request_data = NULL;
    }

loop_end:
    // 清理 FastDFS 客戶端
    if (g_fastdfs_initialized) {
        fdfs_client_destroy();
    }

    return 0;
}
