#ifndef PROTREE_H
#define PROTREE_H

#include <QDialog>
#include <QTreeWidget>

namespace Ui {
class ProTree;
}

/*
 * 文件目录tree 直接和mainwindow中prolayout建立联系 通过ui调用自定义treewidget
 * */

class ProTree : public QDialog
{
    Q_OBJECT

public:
    explicit ProTree(QWidget *parent = nullptr);
    ~ProTree();
    QTreeWidget * GetTreeWidget();
private:
    Ui::ProTree *ui;
public slots:
    void AddProTree(const QString& name,const QString& path);
};

#endif // PROTREE_H
