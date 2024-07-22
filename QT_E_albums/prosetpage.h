#ifndef PROSETPAGE_H
#define PROSETPAGE_H

#include <QWizardPage>
#include <QLineEdit>
#include <QDir>
#include <QCursor>
#include <QString>
#include <QFileDialog>
#include <QDebug>
namespace Ui {
class ProSetPage;
}

/*
 * 创建新的文件夹的时候提示窗口1 用来输入项目名称和项目路径 并将信息回传
 * */
class ProSetPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ProSetPage(QWidget *parent = nullptr);
    ~ProSetPage();
    void GetProSettings(QString &name,QString &path);
protected:
    virtual bool isComplete() const override;
private slots:
    void on_pushButton_clicked();

private:
    Ui::ProSetPage *ui;
};

#endif // PROSETPAGE_H
