#include "picshow.h"
#include "ui_picshow.h"

PicShow::PicShow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PicShow)
{
    ui->setupUi(this);

    ui->previousBtn->SetIcon(":/icon/previous.png",
                             ":/icon/previous_hover.png",
                             ":/icon/previous_press.png");
    ui->nextBtn->SetIcon(":/icon/next.png",
                             ":/icon/next_hover.png",
                             ":/icon/next_press.png");
    QGraphicsOpacityEffect * opacity_pre  =  new QGraphicsOpacityEffect(this);
    opacity_pre->setOpacity(0);
    ui->previousBtn->setGraphicsEffect(opacity_pre);

    QGraphicsOpacityEffect * opacity_next  =  new QGraphicsOpacityEffect(this);
    opacity_next->setOpacity(0);
    ui->nextBtn->setGraphicsEffect(opacity_next);
    //动画设置 渐变类型为线性 持续事件500ms
    _ani_show_pre = new QPropertyAnimation(opacity_pre,"opacity",this);
    _ani_show_pre->setEasingCurve(QEasingCurve::Linear);
    _ani_show_pre->setDuration(500);

    _ani_show_next = new QPropertyAnimation(opacity_next,"opacity",this);
    _ani_show_next->setEasingCurve(QEasingCurve::Linear);
    _ani_show_next->setDuration(500);

    _b_btn_show = false;

    //当点击向前、向后的按钮 会发送信号 在mainwindow中和protreewidget联系，更新图像信息
    connect(ui->nextBtn,&QPushButton::clicked,this,&PicShow::SigNextClicked);
    connect(ui->previousBtn,&QPushButton::clicked,this,&PicShow::SigPreClicked);
}

PicShow::~PicShow()
{
    delete ui;
}

bool PicShow::event(QEvent *event)
{
    switch (event->type()) {
         case QEvent::Enter:
            ShowPreNextBtns(true);
            break;
         case QEvent::Leave:
            ShowPreNextBtns(false);
            break;
        default:
            break;
    }
    return QDialog::event(event);
}


/*
 * b_show 表示当前是否需要显示
 * _b_btn_show 记录当前按钮是否已经设置为显示
 *  从显示到隐藏 则是将start 0 - end 1 变化
 */

void PicShow::ShowPreNextBtns(bool b_show)
{
    if(!b_show && _b_btn_show){
        _ani_show_pre->stop();
        _ani_show_pre->setStartValue(1);
        _ani_show_pre->setEndValue(0);
        _ani_show_pre->start();

        _ani_show_next->stop();
        _ani_show_next->setStartValue(1);
        _ani_show_next->setEndValue(0);
        _ani_show_next->start();

        _b_btn_show = false;
    }
    if(b_show && !_b_btn_show){
        _ani_show_pre->stop();
        _ani_show_pre->setStartValue(0);
        _ani_show_pre->setEndValue(1);
        _ani_show_pre->start();

        _ani_show_next->stop();
        _ani_show_next->setStartValue(0);
        _ani_show_next->setEndValue(1);
        _ani_show_next->start();

        _b_btn_show = true;
    }

}


void PicShow::SlotSelectItem(const QString &path) {
    _selected_path = path;
    _pix_map.load(path);

    //设置一个显示图片在当前窗口的宽高
    auto width = this->width() - 20;
    auto height = this->height() - 20;
    //Qt::KeepAspectRatio 保证宽高比留不变
    _pix_map = _pix_map.scaled(width, height,Qt::KeepAspectRatio);
    //显示图片
    ui->label->setPixmap(_pix_map);
}

void PicShow::Reload() {
    if(_selected_path != ""){
        //获取矩形区域的宽高 维持宽高比例
        const auto &width = ui->gridLayout->geometry().width();
        const auto &height = ui->gridLayout->geometry().height();
        _pix_map.load(_selected_path);
        _pix_map = _pix_map.scaled(width, height,Qt::KeepAspectRatio);
        ui->label->setPixmap(_pix_map);
    }
}


void PicShow::SlotUpdateItem(const QString &path) {
    _selected_path = path;
    if(_selected_path == "") {
        return;
    }
    const auto &width = ui->gridLayout->geometry().width();
    const auto &height = ui->gridLayout->geometry().height();
    _pix_map.load(_selected_path);
    _pix_map = _pix_map.scaled(width, height,Qt::KeepAspectRatio);
    ui->label->setPixmap(_pix_map);
}

void PicShow::SlotClearItem(){
    _selected_path = "";
}