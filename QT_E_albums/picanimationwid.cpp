#include "picanimationwid.h"

PicAnimationWid::PicAnimationWid(QWidget *parent) : QWidget(parent),
    _factor(0.0),_cur_item(nullptr),_b_start(false)
{
    _timer = new QTimer(this);
    connect(_timer,&QTimer::timeout,this,&PicAnimationWid::Timeout);
}

void PicAnimationWid::start(){
    emit SigStart();
    emit SigStartMusic();
    _factor = 0.0;
    _timer->start(25);
    _b_start = true;
}
void PicAnimationWid::stop() {
    emit SigStop();
    emit SigStopMusic();
    _timer->stop();
    _factor = 0.0;
    _b_start = false;
}
void PicAnimationWid::Timeout(){
    if(_cur_item == nullptr){
        stop();
        update();
        return;
    }
    // 每次将动画val像是递增0.01  当大于等1的时候播放下一张图
    // 计时器25ms更新一次 从一张图片播放 25ms * 100 = 2.5s
    // 大于1 就开始播放下一张图
    _factor = _factor + 0.01;
    if(_factor > 1.0){
        _factor = 0;
        auto * cur_pro_item = dynamic_cast<ProTreeItem*>(_cur_item);
        auto * next_pro_item = cur_pro_item->GetNextItem();
        //如果此时是最后一张 就停止播放
        if(next_pro_item == nullptr){
            stop();
            update();
            return;
        }
        SetPixmap(next_pro_item);
        update();
        return;
    }
    update();
}


void PicAnimationWid::SetPixmap(QTreeWidgetItem *item){
    if(item == nullptr){
        return;
    }
    auto * tree_item = dynamic_cast<ProTreeItem*>(item);
    auto path = tree_item->Getpath();
    _pixmap1.load(path);
    _cur_item = tree_item;
    //判断路径是否在map中存在 没有就加如maps
    if(_map_items.find(path) == _map_items.end()){
        _map_items[path] = tree_item;
        //发送更新链表逻辑
        emit SigUpPreList(item);
    }
    emit SigSelectPreList(item);
    auto * next_item = tree_item->GetNextItem();
    if(next_item == nullptr){
        return;
    }
    auto next_path = next_item->Getpath();
    _pixmap2.load(next_path);
    if(_map_items.find(next_path) == _map_items.end()){
        _map_items[next_path] = tree_item;
        //发送更新链表逻辑
        emit SigUpPreList(next_item);
    }
}

void PicAnimationWid::paintEvent(QPaintEvent *event){
    if(_pixmap1.isNull()){
        return;
    }
    QPainter painter(this);
    //添加一个防锯齿逻辑 提升图像质量
    painter.setRenderHint(QPainter::Antialiasing,true);
    QRect rect = geometry();
    int width = rect.width();
    int height = rect.height();
    _pixmap1 = _pixmap1.scaled(width, height,Qt::KeepAspectRatio);
    //通过动画参数设置图像不透明度 实现图像的渐变显示消失
    int alpha = 255 * (1.0f - _factor);
    //透明遮罩图
    QPixmap alphaPixmap(_pixmap1.size());
    //默认填充为透明
    alphaPixmap.fill(Qt::transparent);
    //绘制透明遮罩图
    QPainter p1(&alphaPixmap);


    /* CompositionMode_Source 右上角渐渐出现
     * CompositionMode_Destination 左上角渐隐藏
     * 通过绘制透明度随着factor不断减小的alpha遮罩 逐渐绘制的图像 实现图像渐隐
     * */
    p1.setCompositionMode(QPainter::CompositionMode_Source);
    p1.drawPixmap(0,0,_pixmap1);
    p1.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    p1.fillRect(alphaPixmap.rect(),QColor(0,0,0,alpha));
    p1.end();

    //居中 让alpha图像是从图像起点开始绘制的
    int x = (width - _pixmap1.width()) / 2;
    int y = (height - _pixmap1.height()) / 2;
    painter.drawPixmap(x,y,alphaPixmap);

    if(_pixmap2.isNull()){
        return;
    }
    _pixmap2 = _pixmap2.scaled(width,height,Qt::KeepAspectRatio);
    //这里逻辑和_pixmap1是相反的 _pixmap1 渐隐藏的时候 _pixmap2就要渐现
    alpha = 255 * (_factor);
    QPixmap alphaPixmap2(_pixmap2.size());
    alphaPixmap2.fill(Qt::transparent);
    QPainter p2(&alphaPixmap2);
    p2.setCompositionMode(QPainter::CompositionMode_Source);
    p2.drawPixmap(0,0,_pixmap2);
    p2.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    p2.fillRect(alphaPixmap.rect(),QColor(0,0,0,alpha));
    p2.fillRect(alphaPixmap2.rect(),QColor(0,0,0,alpha));
    p2.end();

    //居中 让alpha图像是从图像起点开始绘制的
    x = (width - _pixmap2.width()) / 2;
    y = (height - _pixmap2.height()) / 2;
    painter.drawPixmap(x,y,alphaPixmap2);

    return QWidget::paintEvent(event);
}


PicAnimationWid::~PicAnimationWid(){

}


void PicAnimationWid::SlidNext() {
    stop();
    if(_cur_item == nullptr){
        return;
    }
    auto* cur_pro_item = dynamic_cast<ProTreeItem*>(_cur_item);
    auto* next_item = cur_pro_item->GetNextItem();
    if( next_item ==nullptr){
        return;
    }
    SetPixmap(next_item);
    update();
}
void PicAnimationWid::SlidPre(){
    stop();
    if(_cur_item == nullptr){
        return;
    }
    auto* cur_pro_item = dynamic_cast<ProTreeItem*>(_cur_item);
    auto* pre_item = cur_pro_item->GetPreItem();
    if(pre_item ==nullptr){
        return;
    }
    SetPixmap(pre_item);
    update();
}

void PicAnimationWid::SlotUpSelectShow(const QString& path){
    auto iter = _map_items.find(path);
    //点击图片不在maps中
    if(iter == _map_items.end()){
        return;
    }
    UpSelectPixmap(iter.value());
    update();
}
void PicAnimationWid::UpSelectPixmap(QTreeWidgetItem *item) {
    if(item == nullptr){
        return;
    }
    auto * tree_item = dynamic_cast<ProTreeItem*>(item);
    auto path = tree_item->Getpath();
    _pixmap1.load(path);
    _cur_item = tree_item;
    if(_map_items.find(path) == _map_items.end()){
        _map_items[path] = tree_item;
    }
    auto *next_item = tree_item->GetNextItem();
    if(next_item == nullptr){
        return;
    }
    auto next_path = next_item->Getpath();
    _pixmap2.load(next_path);
    if(_map_items.find(next_path) == _map_items.end()){
        _map_items[next_path] = next_item;
    }
}


void PicAnimationWid::SlotStartOrStop() {
    if(!_b_start){
        start();
    }
    else{
        stop();
        update();
    }
}
