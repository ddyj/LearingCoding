#ifndef PICBUTTON_H
#define PICBUTTON_H

#include <QPushButton>
#include <QPixmap>
#include <QEvent>

/*
 * 图片按钮类 在图片显示区域中 前向和后向btn渐变  《   》 两个前向后向图标的自定义类
 * */
class PicButton : public QPushButton
{
public:
    PicButton(QWidget *parent = Q_NULLPTR);
    void SetIcon(const QString& normal, const QString& hover,const QString& pressed);
protected:
    bool event(QEvent *event) override;
private:
    void setNormalIcon();
    void setHoverIcon();
    void setPressedIcon();

    //正常加载图片路径
    QString _normal;
    //悬浮加载图片路径
    QString _hover;
    //点击加载图片路径
    QString _pressed;

};

#endif // PICBUTTON_H
