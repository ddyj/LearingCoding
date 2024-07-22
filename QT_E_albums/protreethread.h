#ifndef PROTREETHREAD_H
#define PROTREETHREAD_H

#include <QThread>
#include <QString>
#include <QTreeWidgetItem>
#include <QDir>
#include <QStringList>
#include <QDebug>

#include "const.h"
#include "protreeitem.h"
/*
 * 在QT中加载文件夹是一个很耗时的任务，一般不会将其放在主进程中去
 * 这里基于QThread将其创建了加载文件的异步线程的方式
 *
 * Qthread的线程退出方式 需要手动的控制  默认的exit太生硬了
*/


class ProTreeThread : public QThread
{
    Q_OBJECT
public:
    ProTreeThread(const QString &src_path,const QString& dis_path,
                  QTreeWidgetItem* parent_item,int file_count,
                  QTreeWidget* self,QTreeWidgetItem* root,QObject * parent = nullptr);
    ~ProTreeThread();


    //注:qt原版的获取后缀有问题 原版是从前向后查找第一个.标志的 当文件中有如111.11.jpg 后缀将无法匹配
    QString cal_completeSuffix(const QString & file_name);

//！请注意，一般重写函数就是放入在protected中去，这样重写就仅仅局限于子类和子类的友元
protected:
    //在Qt中线程的启动是使用run函数进行的 这里使用虚函数进行重写
    virtual void run() override;
private:
    void CreateProTree(const QString& src_path,const QString& dis_path,
                       QTreeWidgetItem* parent_item,int &file_count,QTreeWidget* self,
                       QTreeWidgetItem* root,QTreeWidgetItem* &pre_item);

    QString _src_path;
    QString _dis_path;
    int _file_count;
    QTreeWidgetItem* _parent_item;
    QTreeWidget* _self;
    QTreeWidgetItem* _root;
    //判断线程是否退出
    bool _bstop;

signals:
    //更新进度
    void SigUpdateProgress(int);
    //完成创建任务
    void SigFinishProgress(int);
//外部连接 public
public slots:
    void SlotCancelProgress();
};

#endif // PROTREETHREAD_H
