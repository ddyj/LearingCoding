#ifndef OPENTREETHEAD_H
#define OPENTREETHEAD_H

#pragma once

#include <QThread>
#include <QTreeWidget>
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
#include <QDebug>

#include "protreeitem.h"
#include "const.h"


/*
 * 打开项目的时候，导入文件数据时的线程
 */
class OpenTreeThead : public QThread
{
    Q_OBJECT
public:
    explicit OpenTreeThead(const QString& src_path,int& file_count,
                           QTreeWidget* self,QObject * parent = nullptr);
    void OpenProTree(const QString& src_path,int &file_count,QTreeWidget* self);
public slots:
    void SlotCancelOpenProgress();
    QTreeWidgetItem * GetRoot();
protected:
    virtual void run() override;
private:
    //递归创建文件夹
    void RecursiveProTree(const QString& src_path,int& file_count,
                          QTreeWidgetItem* self,QTreeWidgetItem* parent,
                          QTreeWidgetItem* &preitem);
    QString cal_completeSuffix(const QString & file_name);
    QString _src_path;
    int _file_count;
    QTreeWidget* _self;
    bool _bstop;
    QTreeWidgetItem * _root;
signals:
    void SigFinishProgress(int);
    void SigUpdateProgress(int);
};

#endif // OPENTREETHEAD_H
