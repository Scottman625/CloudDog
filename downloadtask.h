#ifndef DOWNLOADTASK_H
#define DOWNLOADTASK_H


#include <QString>
#include <QList>
#include "filedataprogress.h"
#include "fileinfo.h"
#include <QFile>

struct DownloadFileInfo
{
    QFile *file;            //文件
    QString user;           //下載用户
    QString md5;            //文件的md5值
    QString fileName;       //文件名稱
    QString filePath;       //文件路徑
    QString url;            //下載地址
    bool isShareTask;       //是否是共享檔案任務, true是共享檔案任務,false不是共享檔案任務
    FileDataProgress *fdp;  //進度條
};

//上傳任務列表類， 單例模式
class DownloadTask
{
public :
    //獲取UploadTask唯一實例對象
    static DownloadTask* getInstance();

    //判斷任務列表是否為空
    bool isEmpty();

    //取出任務
    DownloadFileInfo* takeTask();

    //添加檔案到上傳任務列表中
    int appendDownloadTask(FileInfo *fileInfo, QString filePath, bool isShareTask = false);

    //刪除任務
    void delDownloadTask();

    void clearList();
private:
    DownloadTask();
    ~DownloadTask();

    //靜態資料成員，類中聲明，類外必須定義
    static DownloadTask* m_instance;

private:
    QList<DownloadFileInfo*> m_fileList; //下載的文件

};

#endif // DOWNLOADTASK_H
