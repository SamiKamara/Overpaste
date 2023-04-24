#ifndef SIDEBAR_HH
#define SIDEBAR_HH

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSpacerItem>

class Sidebar : public QWidget
{
    Q_OBJECT

public:
    explicit Sidebar(QWidget *parent = nullptr);

private:
    QVBoxLayout *sidebarLayout;

};

#endif // SIDEBAR_HH
