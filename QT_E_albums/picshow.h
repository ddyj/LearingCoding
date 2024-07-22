#ifndef PICSHOW_H
#define PICSHOW_H

#include <QDialog>
//让前向、后向按钮有渐变功能
//透明度
#include <QGraphicsOpacityEffect>
//动画
#include <QPropertyAnimation>

/*
 * 右侧显示图像窗口
 * */
namespace Ui {
class PicShow;
}

class PicShow : public QDialog
{
    Q_OBJECT

public:
    explicit PicShow(QWidget *parent = nullptr);
    ~PicShow();

    void Reload();
protected:
    bool event(QEvent *event) override;
private:
    Ui::PicShow *ui;
    //向前按钮的动画设置
    QPropertyAnimation * _ani_show_pre;
    //向后按钮的动画设置
    QPropertyAnimation * _ani_show_next;
    //是否显示
    bool _b_btn_show;
    void ShowPreNextBtns(bool b_show);

    //记录从TreeWidget选中的图像路径和图像信息
    QString _selected_path;
    QPixmap _pix_map;
public slots:
    void SlotSelectItem(const QString &path);
    void SlotUpdateItem(const QString &path);
    void SlotClearItem();
signals:
     void SigNextClicked();
     void SigPreClicked();
};

#endif // PICSHOW_H
