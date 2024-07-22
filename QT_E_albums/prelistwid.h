#ifndef PRELISTWID_H
#define PRELISTWID_H
#include <QListWidget>
#include <QtWidgets>
#include <QTreeWidgetItem>
#include <QMap>
#include <QDebug>
#include <QPainter>
#include <QGuiApplication>

#include "prelistitem.h"
/*
 * 在滑动显示slid窗口的下侧的QListwidget里 滚轴的显示播放的图片
 * */



class PreListWid : public QListWidget
{
    Q_OBJECT
public:
    PreListWid(QWidget *parent = Q_NULLPTR);
    virtual ~PreListWid();
private:
    QMap<QString,QListWidgetItem*> _set_list;
    void AddListItem(const QString &name);
    //记录存储图数
    int _global;
    QPoint _pos_origin;
    //记录上一张图的id
    int _last_index;

    int cal_showPics(QPoint pos,int index);
public slots:
    void SlotUpPreList(QTreeWidgetItem *item);
    void SlotSelectPreList(QTreeWidgetItem *item);
    void SlotItemPressed(QListWidgetItem *item);
signals:
    void SigUpSelectShow(const QString &path);
};

#endif // PRELISTWID_H
