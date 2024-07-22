#ifndef REMOVEPRODIALOG_H
#define REMOVEPRODIALOG_H

#include <QDialog>

namespace Ui {
class RemoveProDialog;
}
/*
 * 当在点击关闭项目的时候，会触发这个窗口用来提示是否删除项目的同时删除本地的数据文件
 */
class RemoveProDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RemoveProDialog(QWidget *parent = nullptr);
    ~RemoveProDialog();
    //判断是否建立删除文件的check选项
    bool IsRemove();
private:
    Ui::RemoveProDialog *ui;
};

#endif // REMOVEPRODIALOG_H
