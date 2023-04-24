#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>
#include <QVBoxLayout>
#include <QListWidget>

class Sidebar : public QWidget {
    Q_OBJECT
public:
    Sidebar(QWidget *parent = nullptr);

private:
    QListWidget *listWidget;
};

#endif // SIDEBAR_H
