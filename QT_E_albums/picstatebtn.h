#ifndef PICSTATEBTN_H
#define PICSTATEBTN_H

#include <QObject>
#include <QPushButton>
#include <QEvent>
#include "const.h"

/*
 * 在slid播放界面 将播放按键更改成点击播放 并更改图标为暂停 再点一下就会暂停，图标变为播放
*/


class PicStateBtn : public QPushButton
{
    Q_OBJECT
public:
    explicit PicStateBtn(QWidget *parent = Q_NULLPTR);
    void SetIcons(const QString& normal,const QString& hover,const QString& pressed,
                  const QString& normal_2,const QString& hover_2,const QString& pressed_2);
    ~PicStateBtn();

protected:
    virtual bool event(QEvent *event) override;
private:
    //播放状态图标的正常、悬浮、点击图标
    QString _normal;
    QString _hover;
    QString _pressed;
    //暂停状态图标的正常、悬浮、点击图标
    QString _normal_2;
    QString _hover_2;
    QString _pressed_2;
    //记录当前是播放状态还是暂停状态
    int _cur_state;

    void setNormalIcon();
    void setHoverIcon();
    void setPressedIcon();

    void setNormal2Icon();
    void setHover2Icon();
    void setPressed2Icon();

public slots:
    void SlotStart();
    void SlotStop();
};

#endif // PICSTATEBTN_H
