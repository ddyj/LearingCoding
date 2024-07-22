#include "protreewidget.h"
#include "protreeitem.h"


ProTreeWidget::ProTreeWidget(QWidget* parent)
    : _active_item(nullptr),_right_btn_item(nullptr),
    _dialog_progress(nullptr),_select_item(nullptr),
    _thread_create_pro(nullptr),_thread_open_pro(nullptr),
    _open_progressdlg(nullptr)
{
    //将头部隐藏，不然的话会有数字显示
    this->header()->hide();
    //右键点击生成操作菜单
    connect(this,&ProTreeWidget::itemPressed,this,&ProTreeWidget::SlotItemPressed);

    _action_import = new QAction(QIcon(":/icon/import.png"),tr("导入文件"),this);
    _action_setstart = new QAction(QIcon(":/icon/core.png"),tr("设置活动项"),this);
    _action_closepro = new QAction(QIcon(":/icon/close.png"),tr("关闭项目"),this);
    _action_slideshow = new QAction(QIcon(":/icon/slideshow.png"),tr("轮播图播放"),this);
    //--------------------------------事件绑定 -----------------------------------
    connect(_action_import,&QAction::triggered,this,&ProTreeWidget::SlotImport);
    connect(_action_setstart,&QAction::triggered,this,&ProTreeWidget::SlotSetActive);
    connect(_action_closepro,&QAction::triggered,this,&ProTreeWidget::SlotCloseActive);
    //双击weightitem的时候发送信号 用来进行双击显示图像
    connect(this,&ProTreeWidget::itemDoubleClicked,this,&ProTreeWidget::SlotDoubleClickedItem);

    //轮播图播放
    connect(_action_slideshow,&QAction::triggered,this,&ProTreeWidget::SlotSlidShow);

    _player = new QMediaPlayer(this);
    _playerlist = new QMediaPlaylist(this);
    _playerlist->setPlaybackMode(QMediaPlaylist::Loop);
    _player->setPlaylist(_playerlist);
}

void ProTreeWidget::AddProToTree(const QString &name, const QString &path)
{
    QDir dir(path);
    QString file_path = dir.absoluteFilePath(name);
    if(_set_path.find(file_path) != _set_path.end()){
        return ;
    }
    QDir pro_dir(file_path);
    if(!pro_dir.exists()){
        bool enable = pro_dir.mkpath(file_path);
        if(!enable){return ;}
    }
    _set_path.insert(file_path);
    auto *item = new ProTreeItem(this,name,file_path,TreeItemPro);
    item->setData(0,Qt::DisplayRole,name);
    item->setData(0,Qt::DecorationRole,QIcon(":/icon/dir.png"));
    item->setData(0,Qt::ToolTipRole,file_path);
    this->addTopLevelItem(item);
}

void ProTreeWidget::SlotItemPressed(QTreeWidgetItem *pressedItem, int column)
{
    if(QGuiApplication::mouseButtons() == Qt::LeftButton){
        _right_btn_item = pressedItem;
    }
    //使用QGuiApplication监听 如果当前是鼠标事件 - 右键
    if(QGuiApplication::mouseButtons() == Qt::RightButton){
        QMenu menu(this);
        int itemtype = pressedItem->type();
        if(itemtype == TreeItemPro){
            //_right_btn_item 记录了当前鼠标右键点击的元素 作为父节点传入 这里存储的是项目item
            _right_btn_item = pressedItem;
            menu.addAction(_action_import);
            menu.addAction(_action_setstart);
            menu.addAction(_action_closepro);
            menu.addAction(_action_slideshow);
            menu.exec(QCursor::pos());
        }
    }
}


void ProTreeWidget::SlotImport() {
    QFileDialog file_dialog;
    file_dialog.setFileMode(QFileDialog::Directory);
    file_dialog.setWindowTitle(tr("选择导入的文件夹"));
    QString path = "";
    if (!_right_btn_item) {
        qDebug() << "_right_btn_item is empty" << endl;
        return;
    }

    path = dynamic_cast<ProTreeItem *>(_right_btn_item)->Getpath();
    file_dialog.setDirectory(path);
    file_dialog.setViewMode(QFileDialog::Detail);

    QStringList fileNames;
    if (file_dialog.exec()) {
        fileNames = file_dialog.selectedFiles();
    }
    if (fileNames.size() == 0) {
        return;
    }
    QString import_path = fileNames.at(0);
    int file_count = 0;

    _dialog_progress = new QProgressDialog(this);

    //将线程传入主进程
    _thread_create_pro = std::make_shared<ProTreeThread>(std::ref(import_path),std::ref(path),
                                    _right_btn_item,file_count,this,_right_btn_item,nullptr);


    //将线程中修改信号进行连接
    //进度更新
    connect(_thread_create_pro.get(),&ProTreeThread::SigUpdateProgress,
            this,&ProTreeWidget::SlotUpdateProgress);
    connect(_thread_create_pro.get(),&ProTreeThread::SigFinishProgress,
            this,&ProTreeWidget::SlotFinishProgress);
    //添加取消槽函数 当点击取消的时候就会发送cancel消息
    connect(_dialog_progress,&QProgressDialog::canceled,
            this,&ProTreeWidget::SlotCancelProgress);
    //接受到取消信号后 线程就会执行函数
    connect(this,&ProTreeWidget::SigCancelProgress,
            _thread_create_pro.get(),&ProTreeThread::SlotCancelProgress);
    _thread_create_pro->start();
    _dialog_progress->setWindowTitle("Please Wait...");
    _dialog_progress->setFixedWidth(PROGRESS_MAX_WIDTH);
    //设置一个波动范围
    _dialog_progress->setRange(0, PROGRESS_MAX_WIDTH);
    _dialog_progress->exec();
}

void ProTreeWidget::SlotUpdateProgress(int count){
    //检查这个对话框是否存在 不存在说明点击关闭了 就不会做相应了
    if(!_dialog_progress){
        return;
    }
    //如果当前count比这个值大 就将其设置为99%
    if(count >= PROGRESS_MAX_WIDTH){
        _dialog_progress->setValue(PROGRESS_MAX_WIDTH-1);
    }
    //小于就直接设置
    else{
        _dialog_progress->setValue(count);
    }
}
void ProTreeWidget::SlotUpdateOpenProgress(int count){
    //检查这个对话框是否存在 不存在说明点击关闭了 就不会做相应了
    if(!_open_progressdlg){
        return;
    }
    //如果当前count比这个值大 就将其设置为99%
    if(count >= PROGRESS_MAX_WIDTH){
        _open_progressdlg->setValue(PROGRESS_MAX_WIDTH-1);
    }
        //小于就直接设置
    else{
        _open_progressdlg->setValue(count);
    }
}

void ProTreeWidget::SlotFinishProgress() {
    if(_dialog_progress == nullptr){
        return;
    }
    //一旦拷贝完成 就将进度条更新为满
    _dialog_progress->setValue(PROGRESS_MAX_WIDTH);
    //释放对话框
    _dialog_progress->deleteLater();
}
void ProTreeWidget::SlotFinishOpenProgress() {
    if(_open_progressdlg == nullptr){
        return;
    }
    //一旦拷贝完成 就将进度条更新为满
    _open_progressdlg->setValue(PROGRESS_MAX_WIDTH);
    _right_btn_item = _thread_open_pro->GetRoot();
    if(_open_progressdlg == nullptr) {
        return;
    }
    _open_progressdlg->deleteLater();
}
void ProTreeWidget::SlotCancelProgress() {
    //发送对话框取消信号 这个信号使用来通知线程 告诉线程已经取消
    emit SigCancelProgress();
    if(_dialog_progress!=nullptr){
        delete _dialog_progress;
        _dialog_progress == nullptr;
    }

}
void ProTreeWidget::SlotCancelOpenProgress(){
    emit SigCancelOpenProgress();
    if(_open_progressdlg != nullptr){
        delete _open_progressdlg;
        _open_progressdlg == nullptr;
    }
}

//设置活动项的槽函数
void ProTreeWidget::SlotSetActive(){
    if(!_right_btn_item){
        return;
    }
    QFont nullFont;
    //是否加粗 - false
    nullFont.setBold(false);
    //如果之前激活过了 就将字体设置为普通字体
    if(_active_item){
        _active_item->setFont(0,nullFont);
    }
    _active_item = _right_btn_item;
    nullFont.setBold(true);
    _active_item->setFont(0,nullFont);
}

void ProTreeWidget::SlotCloseActive() {
    RemoveProDialog remove_pro_dialog;
    auto res = remove_pro_dialog.exec();
    //如果没有接受到对话框信息，就返回
    if (res != QDialog::Accepted) {
        return;
    }
    //获取是否删除元素
    bool b_remvoe = remove_pro_dialog.IsRemove();
    //获取右键点击元素的索引 -- 删除元素
    auto index_right_btn = this->indexOfTopLevelItem(_right_btn_item);
    //获取当前右键的条目
    auto *protreeitem = dynamic_cast<ProTreeItem *>(_right_btn_item);
    auto *selecteditem = dynamic_cast<ProTreeItem *>(_select_item);
    //获取右键条目的路径
    auto delete_path = protreeitem->Getpath();
    //在记录文件的哈希表中删除元素
    _set_path.remove(delete_path);
    if (b_remvoe) {
        QDir delete_dir(delete_path);
        delete_dir.removeRecursively();
    }
    //如果当前删除条目和激活条目是同一个 就将激活条目置位为空
    if (protreeitem == _active_item) {
        _active_item == nullptr;
    }
    //在关闭项目的时候 需要添加选择 判断如果此时右侧窗口正在播放图片，此时要关闭了项目 就需要同住显示端
    if(selecteditem!=nullptr && protreeitem!=nullptr && protreeitem == selecteditem->Getroot()) {
        selecteditem = nullptr;
        _select_item = nullptr;
        emit SigClearSelected();
    }

    //在窗口删除 并且删除右键指向
    delete this->takeTopLevelItem(index_right_btn);
    _right_btn_item = nullptr;
}


//打开项目槽函数
void ProTreeWidget::SlotOpenPro(const QString &open_path) {
    //判断文件是否已经打开了
    if(_set_path.find(open_path) != _set_path.end()) {
        qDebug() << "files has already opened" << endl;
        return;
    }
    _set_path.insert(open_path);
    int file_count = 0;
    QDir pro_dir(open_path);
    const QString& proname = pro_dir.dirName();

    _thread_open_pro = std::make_shared<OpenTreeThead>(std::ref(open_path),
            std::ref(file_count),this,nullptr);


    _open_progressdlg = new QProgressDialog(this);

    //连接更新进度框操作
    connect(_thread_open_pro.get(), &OpenTreeThead::SigUpdateProgress,
            this,&ProTreeWidget::SlotUpdateOpenProgress);
    //连接关闭进度操作
    connect(_thread_open_pro.get(), &OpenTreeThead::SigFinishProgress,
            this,&ProTreeWidget::SlotFinishOpenProgress);
    //添加取消槽函数 当点击取消的时候就会发送cancel消息
    connect(_open_progressdlg,&QProgressDialog::canceled,
            this,&ProTreeWidget::SlotCancelOpenProgress);
    //接受到取消信号后 线程就会执行函数
    connect(this,&ProTreeWidget::SigCancelOpenProgress,
            _thread_open_pro.get(),&OpenTreeThead::SlotCancelOpenProgress);


    _thread_open_pro->start();
    _open_progressdlg->setWindowTitle(tr("Please wait..."));
    _open_progressdlg->setFixedWidth(PROGRESS_MAX_WIDTH);
    _open_progressdlg->setRange(0, PROGRESS_MAX_WIDTH);
    _open_progressdlg->exec();

}

void ProTreeWidget::SlotDoubleClickedItem(QTreeWidgetItem *doubleitem, int column){
    //判断是否是鼠标左键双击
    if(QGuiApplication::mouseButtons() == Qt::LeftButton){
        auto * tree_doubleitem = dynamic_cast<ProTreeItem*>(doubleitem);
        if(!tree_doubleitem){
            return;
        }
        //检查当前点击是否是图像类型 如果是就将点击item的路径发送出去
        int itemtype = tree_doubleitem->type();
        if(itemtype == TreeItemPic){
            emit SigUpdateSelected(tree_doubleitem->Getpath());
            _select_item = tree_doubleitem;
        }
    }
}

//点击前一张图显示
void ProTreeWidget::SlotPreShow(){
    if(_select_item == nullptr){
        return;
    }
    auto curItem = dynamic_cast<ProTreeItem*>(_select_item)->GetPreItem();
    if(curItem == nullptr){
        return;
    }

    emit SigUpdatePic(curItem->Getpath());
    _select_item = curItem;
    //高亮显示当前选中项
    this->setCurrentItem(_select_item);
}
//点击后一张图显示
void ProTreeWidget:: SlotNextShow(){
    if(_select_item == nullptr){
        return;
    }
    auto curItem = dynamic_cast<ProTreeItem*>(_select_item)->GetNextItem();
    if(curItem == nullptr){
        return;
    }

    emit SigUpdatePic(curItem->Getpath());
    _select_item = curItem;
    //高亮显示当前选中项
    this->setCurrentItem(_select_item);
}

void ProTreeWidget::SlotSlidShow(){
    if(_right_btn_item==nullptr){
        qDebug() << "xxx  " <<endl;
        return;
    }
    auto _right_pro_item = dynamic_cast<ProTreeItem*>(_right_btn_item);
    auto * last_child_item = _right_pro_item->GetLastPicChild();
    if(last_child_item == nullptr){
        return;
    }
    auto * first_child_item = _right_pro_item->GetFirstPicChild();
    if(first_child_item == nullptr){
        return;
    }
    //创建对话框
    _slide_show_dlg = std::make_shared<SlideShowDlg>(this,first_child_item,last_child_item);
    _slide_show_dlg->setModal(true);
    //最大化
    _slide_show_dlg->showMaximized();
}



void ProTreeWidget::SlotSetMusic() {
    QFileDialog file_dialog;
    file_dialog.setFileMode(QFileDialog::ExistingFiles);
    file_dialog.setWindowTitle(tr("选择导入音频文件"));
    file_dialog.setDirectory(QDir::currentPath());
    file_dialog.setViewMode(QFileDialog::Detail);
    file_dialog.setNameFilter("(*.mp3)");
    QStringList filesNames;
    if(file_dialog.exec()){
        filesNames = file_dialog.selectedFiles();
    }
    else{
        return;
    }
    if(filesNames.length() <= 0 ){
        return;
    }
    _playerlist->clear();
    for(auto filename : filesNames){
        if(filename.endsWith(".mp3")){
            _playerlist->addMedia(QUrl::fromLocalFile(filename));
        }
    }
    if(_player->state() != QMediaPlayer::PlayingState){
        _playerlist->setCurrentIndex(0);
    }
}

void ProTreeWidget::SlotStartMusic(){
    if(_player == nullptr){
        return;
    }
    _player->play();
}

void ProTreeWidget::SlotStopMusic(){
    if(_player == nullptr){
        return;
    }
    _player->stop();
}
