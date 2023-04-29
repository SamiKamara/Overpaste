// explorer.hh

#ifndef EXPLORER_HH
#define EXPLORER_HH

#include <QWidget>
#include <QTreeView>
#include <QFileSystemModel>

class Explorer : public QWidget
{
    Q_OBJECT

public:
    explicit Explorer(QWidget *parent = nullptr);

private:
    QTreeView *m_treeView;
    QFileSystemModel *m_model;
};

#endif // EXPLORER_HH
