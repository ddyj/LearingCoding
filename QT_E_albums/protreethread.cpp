#include "protreethread.h"



ProTreeThread::ProTreeThread(const QString &src_path, const QString &dis_path,
                             QTreeWidgetItem *parent_item, int file_count,
                             QTreeWidget *self, QTreeWidgetItem *root, QObject *parent)
    : QThread(parent),_src_path(src_path),_dis_path(dis_path),
    _file_count(file_count),_parent_item(parent_item),
    _self(self),_root(root),_bstop(false)
{

}

ProTreeThread::~ProTreeThread()
{

}
//创建目录树  pre_item: 当前链表中的前一个节点
void ProTreeThread::CreateProTree(const QString &src_path, const QString &dis_path,
        QTreeWidgetItem *parent_item, int &file_count, QTreeWidget *self,
        QTreeWidgetItem *root, QTreeWidgetItem * &pre_item)
{
    if(_bstop){
        return;
    }
    //判断是否需要拷贝 -- 如果srcpath dispath是否相同 相同就不拷贝
    bool needcopy = true;
    if(src_path == dis_path){
        needcopy = false;
    }
    QDir import_dir(src_path);

    //设置过滤器
    QStringList nameFilters;
    //除了. .. 获取文件夹、文件名称
    import_dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    import_dir.setSorting(QDir::Name);
    //获取过滤结束后的结果
    QFileInfoList list = import_dir.entryInfoList();
    //遍历
    for(int i=0;i<list.size();++i){
        if(_bstop){
            return;
        }
        QFileInfo fileInfo = list.at(i);
        //这里取出的内容可能是文件夹 也可能是文件 所以需要判断是否是文件夹，如果是文件夹就还需要继续向下递归
        bool bIsDir = fileInfo.isDir();
        if(bIsDir){
            if(_bstop){
                return;
            }
            file_count++;
            emit SigUpdateProgress(file_count);
            //获取目的路径下和源文件夹相应的位置 方便下一轮递归调用
            QDir dist_dir(dis_path);
            QString sub_dist_path = dist_dir.absoluteFilePath(fileInfo.fileName());
            QDir sub_dist_dir(sub_dist_path);
            //如果子文件夹下没有 就创建
            if(!sub_dist_dir.exists()){
                if(!sub_dist_dir.mkpath(sub_dist_path)){
                    qDebug()<< "sub_dist_dir mkpath failed"<< endl;
                    continue;
                }
            }
            //创建这个文件夹的item
            auto * item = new ProTreeItem(parent_item,fileInfo.fileName(),
                    sub_dist_path,root,TreeItemDir);
            item->setData(0,Qt::DisplayRole,fileInfo.fileName());
            //图标
            item->setData(0,Qt::DecorationRole,QIcon(":/icon/dir.png"));
            //提示 文件的路径
            item->setData(0,Qt::ToolTipRole,sub_dist_path);
            //递归调用  pre_item 连接的是图像 所以这里不会更新pre_item
            //这里传递的是指针的引用 所以同一个项目下子文件夹下的图片会和子文件外的图片产生链表连接
            CreateProTree(fileInfo.absoluteFilePath(),sub_dist_path,
                    item,file_count,self,root,pre_item);
        }
        //如果是文件
        else{
            if(_bstop){
                return;
            }
            //获取文件后缀 通过后缀判断是不是图像文件
            //注:qt原版的获取后缀有问题 原版是从前向后查找第一个.标志的 当文件中有如111.11.jpg 后缀将无法匹配
            //const QString& suffix = fileInfo.completeSuffix();
            QString suffix = this->cal_completeSuffix(fileInfo.fileName());
            if(suffix != "png" && suffix!="jpg" && suffix!="jpeg"){
                continue;
            }
            //如果是图片文件 就进行copy copy过程前先对文件计数++
            file_count++;
            //通知外界文件数量更新
            emit SigUpdateProgress(file_count);
            if(!needcopy){ continue;}
            //如果需要拷贝 先创建文件夹
            QDir dist_dir(dis_path);
            //拼接 形成新的路径 返回文件的绝对位置+文件名
            QString dist_file_path = dist_dir.absoluteFilePath(fileInfo.fileName());
            //判断是否拷贝成功
            if(!QFile::copy(fileInfo.absoluteFilePath(),dist_file_path)){
                continue;
            }

            auto * item = new ProTreeItem(parent_item,fileInfo.fileName(),
                    dist_file_path,root,TreeItemPic);
            item->setData(0,Qt::DisplayRole,fileInfo.fileName());
            //图标
            item->setData(0,Qt::DecorationRole,QIcon(":/icon/pic.png"));
            //提示 文件的路径
            item->setData(0,Qt::ToolTipRole,dist_file_path);

            //如果有前节点 需要将其连接
            if(pre_item){
                auto * pre_proitem = dynamic_cast<ProTreeItem*>(pre_item);
                pre_proitem->SetNextItem(item);
            }
            item->SetPreItem(pre_item);
            //更新pre_item 让其指向最后一位(始终在插入节点的前一位)
            pre_item = item;
        }
    }
}


void ProTreeThread::run() {
    QTreeWidgetItem* pre_item = nullptr;
    CreateProTree(_src_path,_dis_path,
            _parent_item,_file_count,_self,_root,pre_item);
    if(_bstop){
        auto path = dynamic_cast<ProTreeItem*>(_root)->Getpath();
        //查询当前的index节点 将其在dialog中删除
        auto index = _self->indexOfTopLevelItem(_root);
        delete _self->takeTopLevelItem(index);
        //如果是拷贝了一半就点击了删除，就将这个创建的文件夹删除
        QDir dir(path);
        dir.removeRecursively();
        return;
    }
    emit SigFinishProgress(_file_count);
}

void ProTreeThread::SlotCancelProgress() {
    _bstop = true;
}
QString ProTreeThread::cal_completeSuffix(const QString & file_name){

    int index = file_name.lastIndexOf('.');
    if (index != -1) {
        return file_name.mid(index + 1);  // 从点的下一个字符开始取到结尾
    }
    return QString();  // 如果没有找到点，返回空字符串
}