#ifndef UPLOADTASK_H
#define UPLOADTASK_H

#include <QString>
#include <QList>
#include "filedataprogress.h"

#define UPLOAD_NOT          0   //未上傳
#define UPLOADING           1   //正在上傳
#define UPLOAD_FINISHED     2   //上傳完成
#define UPLOAD_FAILD        3   //上傳失败
#define UPLOAF_FILE_EXISTE  4   //上傳的檔案已存在

struct UploadFileInfo
{
    QString md5;            //檔案的md5值
    QString fileName;       //檔案名稱
    QString filePath;       //檔案路徑
    qint64 size;            //檔案大小
    FileDataProgress *fdp;  //進度條
    int uploadStatus;       //0.未上傳，1.正在上傳，2.上傳完成
};


//上傳任務列表类, 单例模式
class UploadTask
{
public :
    //获取UploadTask唯一实例对象
    static UploadTask* getInstance();

    //添加檔案到上傳任務列表中
    int appendUploadTask(QString filePath);

    //判斷任務列表是否为空
    bool isEmpty();

    //取出任務
    UploadFileInfo* takeTask();

    //删除任務
    void delUploadTask();

    //清空列表
    void clearList();

private:
    UploadTask();
    ~UploadTask();

    //靜態資料成員，類中聲明，類外必須定義
    static UploadTask* m_instance;

private:
    QList<UploadFileInfo*> m_fileList;

};

#endif // UPLOADTASK_H
