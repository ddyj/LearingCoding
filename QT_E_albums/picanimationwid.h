#ifndef PICANIMATIONWID_H
#define PICANIMATIONWID_H

#include <QWidget>
#include <QTreeWidgetItem>
#include <QMap>
#include <QTimer>
#include <QPainter>
#include <QPaintEvent>
#include "protreeitem.h"

/*
 * 在slidshow窗口中 在播放的窗口中  我们创建自定义的Qwidget 提供轮播的动画操作
 * */
class PicAnimationWid : public QWidget
{
    Q_OBJECT
public:
    explicit PicAnimationWid(QWidget *parent = nullptr);
    ~PicAnimationWid();
    // 设置第一张图片
    void SetPixmap(QTreeWidgetItem *item);
    // 开始播放
    void start();
    //停止
    void stop();

    void SlidNext();
    void SlidPre();
protected:
    //QWidget 用来进行绘图控制的函数
    virtual void paintEvent(QPaintEvent *event) override;
private:
    // 动画设置的value
    float _factor;
    QTimer *_timer;
    QPixmap _pixmap1;
    QPixmap _pixmap2;
    QTreeWidgetItem * _cur_item;
    //记录是否已经轮播过 如果有播放记录 就不会做打开处理
    QMap<QString,QTreeWidgetItem*>_map_items;
    //是否打开
    bool _b_start;

    void UpSelectPixmap(QTreeWidgetItem *item);
public slots:
    void SlotUpSelectShow(const QString& path);
    void SlotStartOrStop();
private slots:
    void Timeout();
signals:
    void SigUpPreList(QTreeWidgetItem* item);
    //告诉预览窗口选中那个item
    void SigSelectPreList(QTreeWidgetItem* item);
    void SigStart();
    void SigStop();
    void SigStartMusic();
    void SigStopMusic();
};

#endif // PICANIMATIONWID_H
