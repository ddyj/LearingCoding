#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->setMinimumSize(1600,800);
    ui->setupUi(this);
    //创建菜单
    QMenu* menu_file = menuBar()->addMenu(tr("文件(&F)"));
    //创建项目动作
    QAction* act_create_pro = new QAction(QIcon(":/icon/createpro.png"), tr("创建项目"),this);
    //设置快捷键
    act_create_pro->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    menu_file->addAction(act_create_pro);

    //打开项目动作
    QAction* act_open_pro = new QAction(QIcon(":/icon/openpro.png"),tr("打开项目"),this);
    act_open_pro->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
    menu_file->addAction(act_open_pro);

    //创建项目设置菜单
    QMenu* menu_set = menuBar()->addMenu(tr("设置(&S)"));
    //背景音乐设置
    QAction* act_music = new QAction(QIcon(":/icon/music.png"),tr("背景音乐"),this);
    act_music->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_M));
    menu_set->addAction(act_music);

    //连接信号和槽
    //连接创建项目槽函数
    connect(act_create_pro, &QAction::triggered,this,&MainWindow::SlotCreatePro);
    //连接打开项目槽函数
    connect(act_open_pro,&QAction::triggered,this,&MainWindow::SlotOpenPro);

    //在ui->proLayout中添加目录
    _protree = new ProTree();
    ui->proLayout->addWidget(_protree);

    QTreeWidget* temp_widget = dynamic_cast<ProTree*>(_protree)->GetTreeWidget();
    auto * pro_tree_widget  = dynamic_cast<ProTreeWidget*>(temp_widget);
    //连接打开项目信号
    connect(this,&MainWindow::SigOpenPro,
            pro_tree_widget,&ProTreeWidget::SlotOpenPro);

    //创建图片显示窗口
    _picshow = new PicShow(this);
    //加载picshow窗口
    ui->picLayout->addWidget(_picshow);
    auto * _pro_picshow = dynamic_cast<PicShow*>(_picshow);
    //!注意这里通过connect两个对象的方式将两个没有耦合的类ProTreeWidget ProPicshow的事件进行连接 下面几个相同
    connect(pro_tree_widget,&ProTreeWidget::SigUpdateSelected,
            _pro_picshow,&PicShow::SlotSelectItem);

    connect(_pro_picshow,&PicShow::SigNextClicked,
            pro_tree_widget,&ProTreeWidget::SlotNextShow);
    connect(_pro_picshow,&PicShow::SigPreClicked,
            pro_tree_widget,&ProTreeWidget::SlotPreShow);
    connect(pro_tree_widget,&ProTreeWidget::SigUpdatePic,
            _pro_picshow,&PicShow::SlotUpdateItem);
    connect(pro_tree_widget,&ProTreeWidget::SigClearSelected,
            _pro_picshow,&PicShow::SlotClearItem);


    //音乐播放和打开
    connect(act_music,&QAction::triggered,
            pro_tree_widget,&ProTreeWidget::SlotSetMusic);
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SlotCreatePro(bool checked)
{
    Wizard wizard(this);
    wizard.setWindowTitle(tr("创建项目"));
    auto * page = wizard.page(0);
    page->setTitle(tr("设置项目配置"));
    //连接信号和槽 把项目配置传回来 并通过connect告诉文件目录类ProTree按照回传的path、name添加item
    connect(&wizard,&Wizard::SigProSettings,
            dynamic_cast<ProTree*>(_protree),&ProTree::AddProTree);
    wizard.show();
    wizard.exec();
    //断开所有信号
    disconnect(&wizard);
}

void MainWindow::SlotOpenPro(bool checked)
{
    QFileDialog file_dialog;
    file_dialog.setFileMode(QFileDialog::Directory);
    file_dialog.setWindowTitle(tr("选择你要打开的项目"));
    file_dialog.setDirectory(QDir::currentPath());
    file_dialog.setViewMode(QFileDialog::Detail);

    QStringList fileNames;
    if(file_dialog.exec()){
        fileNames = file_dialog.selectedFiles();
    }
    if(fileNames.size() <= 0){
        return;
    }
    QString import_path = fileNames.at(0);
    emit SigOpenPro(import_path);
}


//当主窗口发生大小改变的时候 告诉pichshow区域保持图像宽高比例
void MainWindow::resizeEvent(QResizeEvent *event){
    auto * pro_pic_show = dynamic_cast<PicShow*>(_picshow);
    pro_pic_show->Reload();
    QMainWindow::resizeEvent(event);
}