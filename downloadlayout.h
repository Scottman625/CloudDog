#ifndef DOWNLOADLAYOUT_H
#define DOWNLOADLAYOUT_H

#include <QWidget>
#include <QVBoxLayout>

//下載進度布局類，單例模式
class DownloadLayout
{
public:
    static DownloadLayout* getInstance();
    void setDownloadLayout(QWidget *parent);

    //獲取下載的佈局變數
    QVBoxLayout* getDownloadLayout();

private:
    DownloadLayout();
    ~DownloadLayout();


    static DownloadLayout *m_instance;

    QVBoxLayout *m_vLayout;
};

#endif // DOWNLOADLAYOUT_H
