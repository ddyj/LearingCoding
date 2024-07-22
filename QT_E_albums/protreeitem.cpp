#include "protreeitem.h"

ProTreeItem::ProTreeItem(QTreeWidget *treeview, const QString &name,
                         const QString &path, int type)
     : QTreeWidgetItem(treeview,type), _path(path), _name(name),
     _root(nullptr),_next_item(nullptr),_pre_item(nullptr)
{

}

ProTreeItem::ProTreeItem(QTreeWidgetItem *parent, const QString& name,const QString& path,
              QTreeWidgetItem* root,int type)
      : QTreeWidgetItem(parent,type), _path(path), _name(name),
            _root(root),_next_item(nullptr),_pre_item(nullptr)
{

}

const QString &ProTreeItem::Getname() const
{
    return  _name;
}

const QString &ProTreeItem::Getpath() const
{
    return _path;
}

QTreeWidgetItem *ProTreeItem::Getroot() const
{
    return _root;
}

void ProTreeItem::SetPreItem(QTreeWidgetItem *pre_item)
{
    this->_pre_item = dynamic_cast<ProTreeItem*>(pre_item);
}

void ProTreeItem::SetNextItem(QTreeWidgetItem *next_item)
{
    this->_next_item = dynamic_cast<ProTreeItem*>(next_item);
}

ProTreeItem* ProTreeItem::GetPreItem()
{
    return dynamic_cast<ProTreeItem*>(_pre_item);
}

ProTreeItem* ProTreeItem::GetNextItem()
{
    return dynamic_cast<ProTreeItem*>(_next_item);
}

ProTreeItem* ProTreeItem::GetFirstPicChild() {
    if(this->type() == TreeItemPic){
        return nullptr;
    }
    auto child_count = this->childCount();
    if(child_count == 0){
        return nullptr;
    }
    for(int i=0;i < child_count;++i){
        auto * first_child = this->child(i);
        auto * first_tree_child = dynamic_cast<ProTreeItem*>(first_child);
        auto item_type = first_tree_child->type();
        if(item_type == TreeItemPic){
            return first_tree_child;
        }
        first_child = first_tree_child->GetFirstPicChild();
        if(first_child == nullptr){
            continue;
        }
        first_tree_child = dynamic_cast<ProTreeItem*>(first_child);
        return first_tree_child;
    }
    return nullptr;
}

ProTreeItem* ProTreeItem:: () {
    // 如果是图片结构  没有子节点 所以就返回空
    // 只有此时是文件夹的时候 才会有子节点
    if(this->type() == TreeItemPic){
        return nullptr;
    }
    // 获取子节点数量 此时是从树的角度获取子节点 就是包含文件夹数量的
    // 所以查找的逻辑就是从后向前 如果最后一个节点是文件夹 就递归进入文件夹找到最后一个节点 如果是图就直接返回了
    /*
     * pro
     *  -file1
     *  -file2
     *  1.png
     *  2.png
     *  此时child_count就是4
     * */
    auto child_count = this->childCount();
    if(child_count == 0){
        return nullptr;
    }
    for(int i = child_count - 1; i >= 0 ;++i){
        auto * last_child = this->child(i);
        auto * last_tree_item = dynamic_cast<ProTreeItem*>(last_child);

        int item_type = last_tree_item->type();
        if(item_type == TreeItemPic){
            return last_tree_item;
        }
        last_child = last_tree_item->GetLastPicChild();
        //如果最后一个item文件夹里没有图 就从child_count - 2 继续找
        if(last_child == nullptr){
            continue;
        }
        last_tree_item = dynamic_cast<ProTreeItem*>(last_child);
        return last_tree_item;
    }
    return nullptr;
}

