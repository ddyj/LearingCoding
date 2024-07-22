#include "prelistwid.h"
#include "protreeitem.h"
#include "const.h"

PreListWid::PreListWid(QWidget *parent) : QListWidget(parent),_global(0),_last_index(17)
{
    //设置图片展示模式 这里设置只展示图标
    this->setViewMode(QListWidget::IconMode);
    this->setIconSize(QSize(PREICON_SIZE, PREICON_SIZE));
    //设置间隔
    this->setSpacing(5);
    //调用双击事件函数itemPressed 触发SlotItemPressed
    connect(this, &PreListWid::itemPressed,this,&PreListWid::SlotItemPressed);
}
PreListWid::~PreListWid(){

};

void PreListWid::SlotUpPreList(QTreeWidgetItem *item) {
    if(item== nullptr){
        return;
    }
    auto * pro_item = dynamic_cast<ProTreeItem*>(item);
    auto path = pro_item->Getpath();
    auto iter = _set_list.find(path);
    if(iter != _set_list.end()){
        qDebug() << "path " << path << " exists" << endl;
        return;
    }
    AddListItem(path);
}


void PreListWid::AddListItem(const QString& path){
    QPixmap src_pixmap(path);
    //scaled只是拉伸 并不是resize 是指让原始图片按照自身宽高比例一侧达到目标尺寸
    src_pixmap = src_pixmap.scaled(PREICON_SIZE,PREICON_SIZE,Qt::KeepAspectRatio);
    QPixmap dst_pixmap(QSize(PREICON_SIZE,PREICON_SIZE));

    //填充背景图 此时背景图是90*90的 原图可能是 60*90 的（长边为90 短边保持比例）
    dst_pixmap.fill(QColor(220,220,220,50));
    QPainter painter(&dst_pixmap);
    auto src_width = src_pixmap.width();
    auto src_height = src_pixmap.height();
    auto dst_width = dst_pixmap.width();
    auto dst_height = dst_pixmap.height();
    //居中对齐
    auto x = (dst_width - src_width) / 2;
    auto y = (dst_height - src_height) / 2;
    painter.drawPixmap(x,y,src_pixmap);
    _global++;
    PreListItem * pItem = new PreListItem(QIcon(dst_pixmap),path,_global,this);
    pItem->setSizeHint(QSize(PREITEM_SIZE,PREITEM_SIZE));
    this->addItem(pItem);
    _set_list[path] = pItem;
    //如果是第一张图就记录一下位置
    if(_global == 1){
        _pos_origin = this->pos();
        qDebug() << "_pos_origin is "<<_pos_origin<<endl;
    }
}

//当选择下方滚动栏的图像，首先会显示边框，然后会在上方进行一个显示
void PreListWid::SlotSelectPreList(QTreeWidgetItem *item) {
    if(item == nullptr){
        return;
    }
    auto * pro_item = dynamic_cast<ProTreeItem*>(item);
    auto path = pro_item->Getpath();
    auto iter = _set_list.find(path);
    if(iter == _set_list.end()){
        return;
    }
    auto * list_item = dynamic_cast<PreListItem*>(iter.value());
    auto index = list_item->GetIndex();
    //如果一页大于17张 就让其翻页
    //更新 这里需要计算一下 当前的pos_cur是否还需要递减还是递增
    if(index > 17){
        // 如果index和_global一样 就说明此时是在增加图片导致的递增 此时向后滑动一页图即可
        if(index == _global){
            this->move(this->pos().x() - 110, this->pos().y());
        }
        // 此时 是手动选择滑动窗口导致
        else if(index < _global){
            // 显示图像数量不满一页 就从前面隐藏的部分补充
            if(cal_showPics(this->pos(),index) < _last_index){
                this->move(this->pos().x() + 110, this->pos().y());
            }
            else{
                this->move(this->pos().x() - 110, this->pos().y());
            }
        }
    }
    else{
        this->move(_pos_origin);
        _last_index = 17;
    }
    this->setCurrentItem(iter.value());
}

void PreListWid::SlotItemPressed(QListWidgetItem *item){
    if(QGuiApplication::mouseButtons() != Qt::LeftButton){
        return;
    }

    auto * list_item = dynamic_cast<PreListItem*>(item);
    auto cur_index = list_item->GetIndex();
    auto path = list_item->GetPath();
    this->setCurrentItem(item);
    emit SigUpSelectShow(path);
}

int PreListWid::cal_showPics(QPoint pos,int index){
    int hid_num = (_pos_origin.x() - pos.x()) / (100 + 2*5);
    return _global - hid_num;
}