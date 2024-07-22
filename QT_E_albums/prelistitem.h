#ifndef PRELISTITEM_H
#define PRELISTITEM_H


/*
 * 在slid的prelistwid中 重构了他的元素
 * */
#include <QListWidget>
#include <QListWidgetItem>
#include <QIcon>

class PreListItem : public QListWidgetItem
{
public:
    PreListItem(const QIcon& icon,const QString& text,const int& index,
            QListWidget *view = Q_NULLPTR, int type = Type);
    int GetIndex();
    QString GetPath();
private:
    int _index;
    QString _path;
};

#endif // PRELISTITEM_H
