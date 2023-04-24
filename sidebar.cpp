#include "sidebar.hh"

Sidebar::Sidebar(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *sidebarLayout = new QVBoxLayout(this);
    listWidget = new QListWidget(this);
    listWidget->addItems({"Item 1", "Item 2", "Item 3"});
    sidebarLayout->addWidget(listWidget);
}
