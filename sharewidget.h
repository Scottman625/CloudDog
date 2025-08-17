#ifndef SHAREWIDGET_H
#define SHAREWIDGET_H

#include <QWidget>
#include "common.h"
#include "logininfoinstance.h"
#include "mymenu.h"
#include "fileinfo.h"
#include "downloadtask.h"
#include <QNetworkAccessManager>
#include <QTimer>

namespace Ui {
class ShareWidget;
}

class ShareWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ShareWidget(QWidget *parent = 0);
    ~ShareWidget();
    //刷新共享檔案列表
    void refreshFiles();

private slots:
    void onRightMenu(const QPoint &pos);

protected:
    void paintEvent(QPaintEvent *event);

signals:
    //重新登入
    void gotoTransform(TransformStatus status);

private:
    Ui::ShareWidget *ui;

    Common *m_common;
    LoginInfoInstance *m_loginInfo;
    DownloadTask *m_downloadTask;
    QNetworkAccessManager *m_manager;

    MyMenu *m_menuItem;
    MyMenu *m_menuEmpty;

    QAction *m_actionDownload;
    QAction *m_actionProperty;
    QAction *m_actionCancel;
    QAction *m_actionSave;

    QAction *m_actionRefresh;

    int m_shareFilesCount;
    QList<FileInfo*> m_fileList; //任務列表

    //定時器
    QTimer m_downloadFileTimer;

    void initListWidget();
    void addMenu();
    void menuActions();

    void dealfile(QString cmd);
    void showFileProperty(FileInfo *fileInfo);
    void cancelShareFile(FileInfo *fileInfo);

    //獲取共享檔案數量
    void getShareFilesCount();
    //獲取共享檔案列表
    void getShareFilesList();

    void clearItems();
    void clearFileList();
    void refreshFileItems();

    void showFileItems();
    void addListWidgetItem(FileInfo *fileInfo);

    void addDownloadFiles();
    //定時器
    void checkTaskList();
    void downloadFilesAction();

    void dealFilePv(QString md5, QString fileName);
    void saveFileToMyAccount(FileInfo *fileInfo);

};

#endif // SHAREWIDGET_H
