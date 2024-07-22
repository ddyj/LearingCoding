#include "picbutton.h"

PicButton::PicButton(QWidget *parent) : QPushButton(parent)
{

}

void PicButton::SetIcon(const QString &normal, const QString &hover, const QString &pressed) {
    _normal = normal;
    _hover = hover;
    _pressed = pressed;

    QPixmap tmpPixmap;
    tmpPixmap.load(normal);
    this->resize(tmpPixmap.width(), tmpPixmap.height());
    this->setIcon(tmpPixmap);
    this->setIconSize(tmpPixmap.size());
}

void PicButton::setNormalIcon(){
    QPixmap tmpPixmap;
    tmpPixmap.load(_normal);
    this->resize(tmpPixmap.width(), tmpPixmap.height());
    this->setIcon(tmpPixmap);
    this->setIconSize(tmpPixmap.size());
}
void PicButton::setHoverIcon(){
    QPixmap tmpPixmap;
    tmpPixmap.load(_hover);
    this->resize(tmpPixmap.width(), tmpPixmap.height());
    this->setIcon(tmpPixmap);
    this->setIconSize(tmpPixmap.size());
}
void PicButton::setPressedIcon(){
    QPixmap tmpPixmap;
    tmpPixmap.load(_pressed);
    this->resize(tmpPixmap.width(), tmpPixmap.height());
    this->setIcon(tmpPixmap);
    this->setIconSize(tmpPixmap.size());
}

//检查鼠标事件 如果鼠标是进入 就是悬浮状态图标 如果是离开就恢复为正常图标
//如果是点击就是点击 如果是鼠标松开 就要恢复成悬浮状态
bool PicButton::event(QEvent *event){
    switch (event->type()){
        case QEvent::Enter:
            setHoverIcon();
            break;
        case QEvent::Leave:
            setNormalIcon();
            break;
        case QEvent::MouseButtonPress:
            setPressedIcon();
            break;
        case QEvent::MouseButtonRelease:
            setHoverIcon();
            break;
        default:
            break;
    }
    return QPushButton::event(event);
}