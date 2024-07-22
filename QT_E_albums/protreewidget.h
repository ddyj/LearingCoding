#ifndef PROTREEWIDGET_H
#define PROTREEWIDGET_H
#pragma once

#include <QTreeWidget>
#include <QDir>
#include <QSet>
#include <QHeaderView>
#include <QGuiApplication>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QDebug>
#include <QProgressDialog>

#include "protreethread.h"
#include "opentreethead.h"
#include "removeprodialog.h"
#include "slideshowdlg.h"

#include <memory>

#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QMediaPlaylist>

/*项目目录栏的widget*/

class SlideShowDlg;
class ProTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    ProTreeWidget(QWidget* parent = nullptr);
    void AddProToTree(const QString& name,const QString& path);
private:
    //判断是否重名
    QSet<QString>_set_path;
    //鼠标右键的条目元素 -- 项目item
    QTreeWidgetItem * _right_btn_item;
    //保存当前启动项目的元素
    QTreeWidgetItem * _active_item;
    //保存选中条目元素 -- 删除时使用
    QTreeWidgetItem * _select_item;
    QAction* _action_import;
    QAction* _action_setstart;
    QAction* _action_closepro;
    QAction* _action_slideshow;
    //进度对话框
    QProgressDialog * _dialog_progress;
    //打开项目文件进度框
    QProgressDialog* _open_progressdlg;
    //使用智能指针管理创建文件的线程
    std::shared_ptr<ProTreeThread>_thread_create_pro;
    //使用智能指针管理打开文件的线程
    std::shared_ptr<OpenTreeThead>_thread_open_pro;
    //使用智能指针管理滑动播放窗口
    std::shared_ptr<SlideShowDlg> _slide_show_dlg;

    QMediaPlayer* _player;
    QMediaPlaylist * _playerlist;
public slots:
    void SlotItemPressed(QTreeWidgetItem* pressedItem,int column);
    void SlotImport();
    void SlotUpdateProgress(int count);
    void SlotFinishProgress();
    void SlotUpdateOpenProgress(int count);
    void SlotFinishOpenProgress();
    void SlotCancelProgress();
    void SlotCancelOpenProgress();
    void SlotSetActive();
    void SlotCloseActive();
    void SlotOpenPro(const QString& open_path);
    void SlotDoubleClickedItem(QTreeWidgetItem *doubleitem, int column);

    void SlotPreShow();
    void SlotNextShow();
    void SlotSlidShow();
    void SlotSetMusic();
    void SlotStartMusic();
    void SlotStopMusic();
signals:
    void SigCancelProgress();
    void SigCancelOpenProgress();
    void SigUpdateSelected(const QString& pic_path);
    void SigUpdatePic(const QString& pic_path);
    void SigClearSelected();
};

#endif // PROTREEWIDGET_H
