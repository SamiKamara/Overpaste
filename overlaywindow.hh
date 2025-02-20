#ifndef OVERLAYWINDOW_HH
#define OVERLAYWINDOW_HH

#include <QWidget>
#include "mainwindow.hh"
#include "explorer.hh"
#include "sidebar.hh"
#include <QVBoxLayout>
#include <QSplitter>

class MainWindow;

class OverlayWindow : public QWidget
{
    Q_OBJECT

public:
    explicit OverlayWindow(MainWindow *parent = nullptr);

protected:

private:
    MainWindow *m_mainWindow;
    Explorer *m_explorer;
    Sidebar *m_sidebar;
    void showEvent(QShowEvent *event);
};

#endif // OVERLAYWINDOW_HH
