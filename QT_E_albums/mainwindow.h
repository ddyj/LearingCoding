#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QResizeEvent>

#include "wizard.h"
#include "prosetpage.h"
#include "confirmpage.h"
#include "protree.h"
#include "protreewidget.h"
#include "picshow.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //基本Qss设置
    //background-colo:rgb(46,47,48);

protected:
    //设置窗口改变
    void resizeEvent(QResizeEvent *event) override;
private:
    Ui::MainWindow *ui;
    QWidget* _protree;
    QWidget* _picshow;


private slots:
    void SlotCreatePro(bool checked=false);
    void SlotOpenPro(bool checked=false);
signals:
    void SigOpenPro(const QString & import_path);
};
#endif // MAINWINDOW_H
