#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QString>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //添加qss设置  使用qstring的方式读取并将其设置给整个window
    QFile qss(":/style/style.qss");
    if(qss.open(QFile::ReadOnly)){
        qDebug() << "Open qss success";
        QString style = QLatin1String(qss.readAll());
        a.setStyleSheet(style);
        qss.close();
    }
    else{
        qDebug()<<"open qss filed";
        return 0;
    }
    MainWindow w;
    w.setWindowTitle("电子相册项目");
    //最大化显示
    w.showMaximized();
    w.show();
    return a.exec();
}
