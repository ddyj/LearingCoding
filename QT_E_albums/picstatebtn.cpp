#include "picstatebtn.h"

PicStateBtn::PicStateBtn(QWidget *parent)
{

}


void PicStateBtn::SetIcons(const QString& normal,const QString& hover,const QString& pressed,
                                const QString& normal_2,const QString& hover_2,const QString& pressed_2){
    _normal = normal;
    _hover = hover;
    _pressed = pressed;

    _normal_2 = normal_2;
    _hover_2 = hover_2;
    _pressed_2 = pressed_2;

    QPixmap tmpPixmap;
    tmpPixmap.load(normal);
    this->resize(tmpPixmap.width(),tmpPixmap.height());
    this->setIcon(tmpPixmap);
    this->setIconSize(tmpPixmap.size());
    //设置当前状态
    _cur_state = PicBtnStateNormal;
}



bool PicStateBtn::event(QEvent *event){
    switch (event->type()){
        case QEvent::Enter:
            //判断是否是播放图标 加载播放图标的悬浮状态
            if (_cur_state < PicBtnState2Normal){
                setHoverIcon();
            }
            //此时是暂停图标 加载暂停图标的悬浮状态
            else{
                setHover2Icon();
            }
            break;
        case QEvent::Leave:
            //判断是否是播放图标 加载播放图标的悬浮状态
            if (_cur_state < PicBtnState2Normal){
                setNormalIcon();
            }
                //此时是暂停图标 加载暂停图标的悬浮状态
            else{
                setNormal2Icon();
            }
            break;
        case QEvent::MouseButtonPress:
            //判断是否是播放图标 加载播放图标的悬浮状态
            if (_cur_state < PicBtnState2Normal){
                setPressedIcon();
            }
                //此时是暂停图标 加载暂停图标的悬浮状态
            else{
                setPressed2Icon();
            }
            break;
        case QEvent::MouseButtonRelease:
            //判断是否是播放图标 加载播放图标的悬浮状态
            if (_cur_state < PicBtnState2Normal){
                setHoverIcon();
            }
                //此时是暂停图标 加载暂停图标的悬浮状态
            else{
                setHover2Icon();
            }
            break;
        default:
            break;
    }
    return QPushButton::event(event);
}

void PicStateBtn::setNormalIcon(){
    QPixmap tmpPixmap;
    tmpPixmap.load(_normal);
    this->setIcon(tmpPixmap);
    _cur_state = PicBtnStateNormal;
}
void PicStateBtn::setHoverIcon(){
    QPixmap tmpPixmap;
    tmpPixmap.load(_hover);
    this->setIcon(tmpPixmap);
    _cur_state = PicBtnStateHover;
}
void PicStateBtn::setPressedIcon(){
    QPixmap tmpPixmap;
    tmpPixmap.load(_pressed);
    this->setIcon(tmpPixmap);
    _cur_state = PicBtnStatePressed;
}

void PicStateBtn::setNormal2Icon(){
    QPixmap tmpPixmap;
    tmpPixmap.load(_normal_2);
    this->setIcon(tmpPixmap);
    _cur_state = PicBtnState2Normal;
}
void PicStateBtn::setHover2Icon(){
    QPixmap tmpPixmap;
    tmpPixmap.load(_hover_2);
    this->setIcon(tmpPixmap);
    _cur_state = PicBtnStateHover;
}
void PicStateBtn::setPressed2Icon(){
    QPixmap tmpPixmap;
    tmpPixmap.load(_pressed_2);
    this->setIcon(tmpPixmap);
    _cur_state = PicBtnState2Pressed;
}

PicStateBtn::~PicStateBtn(){

}


void PicStateBtn::SlotStart() {
    setNormal2Icon();
}
void PicStateBtn::SlotStop() {
    setNormalIcon();
}