#ifndef FILEINFO_H
#define FILEINFO_H

#include <QString>

/*
 *{
    "files":	[{
            "user":	"milo",
            "md5":	"7474b5b4ce9b5fc93740758a3734b195",
            "create_time":	"2020-10-31 01:53:48",
            "file_name":	"libvtv-0.dll",
            "share_status":	0,
            "pv":	0,
            "url":	"http://192.168.139.131:80/group1/M00/00/00/wKiLg1-dJhyACXI_AAA6DiX82dA943.dll",
            "size":	14862,
            "type":	"dll"
        }]
}*/
struct FileInfo
{
    QString user;           //用户名
    QString md5;            //md5
    QString createTime;     //上傳時間
    QString fileName;       //檔案名稱
    int shareStatus;        //共享狀態, 0為沒有共享，1為共享
    int pv;                 //檔案下載量，下載一次加1
    QString url;            //文件url
    int size;               //文件大小,單位:字節
    QString type;           //文件類型
};


struct FileDownload
{
    QString fileName;       //檔案名稱
    int pv;                 //檔案下載量，下載一次加1
};

#endif // FILEINFO_H
