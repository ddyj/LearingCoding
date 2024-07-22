#ifndef SLIDESHOWDLG_H
#define SLIDESHOWDLG_H

#include <QDialog>
#include <QTreeWidgetItem>
#include <QDebug>

#include "picanimationwid.h"
#include "prelistwid.h"
#include "picstatebtn.h"
#include "protreewidget.h"

namespace Ui {
class SlideShowDlg;
}

/*
 *  幻灯片播放窗口实现
 */

class SlideShowDlg : public QDialog
{
    Q_OBJECT

public:
    //传入参数 告诉我们从那张图开始播放，那张图结束播放
    explicit SlideShowDlg(QWidget *parent = nullptr,
            QTreeWidgetItem* first_item = nullptr,
            QTreeWidgetItem* last_item = nullptr);

    ~SlideShowDlg();

private:
    Ui::SlideShowDlg *ui;

    QTreeWidgetItem* _first_item;
    QTreeWidgetItem* _last_item;
public slots:
    void SlotSlidNext();
    void SlotSlidPre();
};

#endif // SLIDESHOWDLG_H
