#include "slideshowdlg.h"
#include "ui_slideshowdlg.h"

SlideShowDlg::SlideShowDlg(QWidget* parent,QTreeWidgetItem* first_item ,
        QTreeWidgetItem* last_item ) :
    QDialog(parent),_first_item(first_item),_last_item(last_item),
    ui(new Ui::SlideShowDlg)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("图片预览"));
    //隐藏上侧白框
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    //加载图标
    ui->slidprebtn->SetIcon(":/icon/previous.png",
            ":/icon/previous_hover.png",":/icon/previous_press.png");
    ui->slidnextbtn->SetIcon(":/icon/next.png",
                              ":/icon/next_hover.png",":/icon/next_press.png");
    ui->closebtn->SetIcon(":/icon/closeshow.png",
                          ":/icon/closeshow_hover.png",":/icon/closeshow_press.png");
    ui->playbtn->SetIcons(":/icon/play.png",":/icon/play_hover.png",":/icon/play_press.png",
                          ":/icon/pause.png",":/icon/pause_hover.png",":/icon/pause_press.png");
    //关闭事件绑定
    connect(ui->closebtn,&QPushButton::clicked,this,&SlideShowDlg::close);
    //点击下一个
    connect(ui->slidnextbtn,&QPushButton::clicked,this,&SlideShowDlg::SlotSlidNext);
    //点击上一个
    connect(ui->slidprebtn,&QPushButton::clicked,this,&SlideShowDlg::SlotSlidPre);
    //提前连接号信号 这样就不会导致丢失数据
    auto * preListWid = dynamic_cast<PreListWid*>(ui->prelistwidget);
    connect(ui->picAnimation,&PicAnimationWid::SigUpPreList,
            preListWid,&PreListWid::SlotUpPreList);
    connect(ui->picAnimation,&PicAnimationWid::SigSelectPreList,
            preListWid,&PreListWid::SlotSelectPreList);
    //点击图像 预览显示
    connect(preListWid,&PreListWid::SigUpSelectShow,ui->picAnimation,&PicAnimationWid::SlotUpSelectShow);
    //播放/暂停
    connect(ui->playbtn,&QPushButton::clicked,ui->picAnimation,&PicAnimationWid::SlotStartOrStop);
    //更具动画区域是否正在播放 更新播放的图标
    connect(ui->picAnimation,&PicAnimationWid::SigStart,ui->playbtn,&PicStateBtn::SlotStart);
    connect(ui->picAnimation,&PicAnimationWid::SigStop,ui->playbtn,&PicStateBtn::SlotStop);

    //音乐播放连接
    auto * _protree_widget = dynamic_cast<ProTreeWidget*>(parent);
    connect(ui->picAnimation,&PicAnimationWid::SigStartMusic,
            _protree_widget,&ProTreeWidget::SlotStartMusic);
    connect(ui->picAnimation,&PicAnimationWid::SigStopMusic,
            _protree_widget,&ProTreeWidget::SlotStopMusic);

    ui->picAnimation->SetPixmap(_first_item);
    ui->picAnimation->start();
}

SlideShowDlg::~SlideShowDlg()
{
    delete ui;
}

void SlideShowDlg::SlotSlidNext(){
    ui->picAnimation->SlidNext();
}
void SlideShowDlg::SlotSlidPre(){
    ui->picAnimation->SlidPre();
}
