#ifndef CONFIRMPAGE_H
#define CONFIRMPAGE_H

#include <QWizardPage>

namespace Ui {
class ConfirmPage;
}
/*
 * 创建新的文件夹的时候提示窗口2 用来提示已经创建项目
 * */
class ConfirmPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ConfirmPage(QWidget *parent = nullptr);
    ~ConfirmPage();

private:
    Ui::ConfirmPage *ui;
};

#endif // CONFIRMPAGE_H
