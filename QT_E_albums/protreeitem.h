#ifndef PROTREEITEM_H
#define PROTREEITEM_H

#include <QStringList>
#include <QTreeWidgetItem>

#include "const.h"

class ProTreeItem : public QTreeWidgetItem
{
public:
    ProTreeItem(QTreeWidget *treeview, const QString &name,
                    const QString &path, int type = Type);

    ProTreeItem(QTreeWidgetItem *parent, const QString& name,const QString& path,
                QTreeWidgetItem* root,int type = Type);

    const QString &Getname() const;
    const QString &Getpath() const;
    QTreeWidgetItem *Getroot() const;
    void SetPreItem(QTreeWidgetItem* pre_item);
    void SetNextItem(QTreeWidgetItem* next_item);

    ProTreeItem* GetPreItem();
    ProTreeItem* GetNextItem();
    ProTreeItem* GetLastPicChild();
    ProTreeItem* GetFirstPicChild();

private:
    QString _name;
    QString _path;
    //在轮播的时候 我们使用一个双向链表进行轮询
    //在双向链表中 只有在同pro下的图才有前后节点 例如
    /*
     * pro
     *  -file
     *      -1.png
     *      -2.png
     *      -3.png
     *  -5.png
     *  -6.png
     *
     *
     * 此时双向链表有  1 - 2 -3 - 5 - 6 并不包含file
     * */

    QTreeWidgetItem* _root;
    QTreeWidgetItem* _pre_item;
    QTreeWidgetItem* _next_item;
};

#endif // PROTREEITEM_H
