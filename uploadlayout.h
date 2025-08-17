#ifndef UPLOADLAYOUT_H
#define UPLOADLAYOUT_H

#include <QWidget>
#include <QVBoxLayout>

//上傳進度佈局類, 單例模式
class UploadLayout
{
public:
    static UploadLayout* getInstance();
    void setUploadLayout(QWidget *parent);

    //獲取上傳的佈局變量
    QVBoxLayout* getUploadLayout();

private:
    UploadLayout();
    ~UploadLayout();


    static UploadLayout *m_instance;

    QVBoxLayout *m_vLayout;
};

#endif // UPLOADLAYOUT_H
