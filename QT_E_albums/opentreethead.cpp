#include "opentreethead.h"

OpenTreeThead::OpenTreeThead(const QString &src_path, int& file_count,
        QTreeWidget *self, QObject *parent)
    :QThread(parent),_bstop(false),_src_path(src_path),_file_count(file_count),
        _self(self),_root(nullptr)
{

}

void OpenTreeThead::OpenProTree(const QString &src_path, int &file_count, QTreeWidget *self)
{
    QDir src_dir(src_path);
    auto name = src_dir.dirName();
    auto * item = new ProTreeItem(self,name,src_path,TreeItemPro);
    item->setData(0,Qt::DisplayRole,name);
    item->setData(0,Qt::DecorationRole,QIcon(":/icon/dir.png"));
    item->setData(0,Qt::ToolTipRole,src_path);
    _root = item;
    QTreeWidgetItem* preitem = nullptr;
    RecursiveProTree(std::ref(src_path),std::ref(file_count),
            item,_root, preitem);
}

void OpenTreeThead::run()
{
    OpenProTree(_src_path,_file_count,_self);
    //如果对话框取消 但是此时已经建立了新的item 就需要删除了
    if(_bstop && _root){
        auto path = dynamic_cast<ProTreeItem*>(_root)->Getpath();
        auto index = _self->indexOfTopLevelItem(_root);
        delete _self->takeTopLevelItem(index);
        QDir dir(path);
        dir.removeRecursively();
        return;
    }
    emit SigFinishProgress(_file_count);
}

void OpenTreeThead::RecursiveProTree(const QString &src_path, int& file_count,
        QTreeWidgetItem *self, QTreeWidgetItem *parent, QTreeWidgetItem* &preitem)
{
    QDir src_dir(src_path);
    src_dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    src_dir.setSorting(QDir::Name);
    //获取目标文件的list
    QFileInfoList list = src_dir.entryInfoList();
    qDebug() << "list size is " << list.size();
    for(int i = 0;i < list.size();++i){
        if(_bstop){
            return;
        }
        QFileInfo fileInfo = list.at(i);
        bool bIsDir = fileInfo.isDir();
        if(bIsDir){
            if(_bstop){
                return;
            }
            ++file_count;
            emit SigUpdateProgress(file_count);
            auto *item = new ProTreeItem(parent,fileInfo.fileName(),
                    fileInfo.absoluteFilePath(),_root,TreeItemDir);
            item->setData(0,Qt::DisplayRole,fileInfo.fileName());
            item->setData(0,Qt::DecorationRole,QIcon(":/icon/dir.png"));
            item->setData(0,Qt::ToolTipRole,fileInfo.absoluteFilePath());
            //这里传递的是指针的引用 所以同一个项目下子文件夹下的图片会和子文件外的图片产生链表连接
            RecursiveProTree(fileInfo.absoluteFilePath(),file_count,self,item,preitem);
        }
        else{
            if(_bstop){
                return;
            }
            const QString suffix = cal_completeSuffix(fileInfo.fileName());
            if(suffix!="png" && suffix!="jpg" && suffix!="jpeg"){
                qDebug() << "suffix is not pic " << suffix << endl;
                continue;
            }
            ++file_count;
            emit SigUpdateProgress(file_count);

            auto * item = new ProTreeItem(parent,fileInfo.fileName(),
                fileInfo.absoluteFilePath(),_root,TreeItemPic);

            item->setData(0,Qt::DisplayRole,fileInfo.fileName());
            item->setData(0,Qt::DecorationRole,QIcon(":/icon/pic.png"));
            item->setData(0,Qt::ToolTipRole,fileInfo.absoluteFilePath());
            if(preitem != nullptr){
                auto* pre_proitem = dynamic_cast<ProTreeItem*>(preitem);
                pre_proitem->SetNextItem(item);
            }
            item->SetPreItem(preitem);
            preitem = item;
        }
    }
    emit SigFinishProgress(file_count);
}


QString OpenTreeThead::cal_completeSuffix(const QString & file_name){

    int index = file_name.lastIndexOf('.');
    if (index != -1) {
        return file_name.mid(index + 1);  // 从点的下一个字符开始取到结尾
    }
    return QString();  // 如果没有找到点，返回空字符串
}

void OpenTreeThead::SlotCancelOpenProgress(){
    _bstop = true;
}

QTreeWidgetItem * OpenTreeThead::GetRoot(){
    return _root;
}