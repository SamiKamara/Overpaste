// explorer.cpp

#include "explorer.hh"
#include <QCoreApplication>
#include <QVBoxLayout>

Explorer::Explorer(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    m_treeView = new QTreeView(this);
    layout->addWidget(m_treeView);

    m_model = new QFileSystemModel(this);
    m_model->setRootPath(QCoreApplication::applicationDirPath() + "/files");
    m_treeView->setModel(m_model);

    // Set the root index of the tree view to the /files folder
    m_treeView->setRootIndex(m_model->index(QCoreApplication::applicationDirPath() + "/files"));

    // Hide columns other than the name column
    for (int i = 1; i < m_model->columnCount(); ++i) {
        m_treeView->hideColumn(i);
    }

    // Set the header text
    m_model->setHeaderData(0, Qt::Horizontal, "File Explorer");

    // Set the tree view behavior
    m_treeView->setExpandsOnDoubleClick(false);
    m_treeView->setAnimated(true);

    // Set up the layout
    layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(layout);
}
