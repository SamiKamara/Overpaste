#ifndef OVERLAYWINDOW_HH
#define OVERLAYWINDOW_HH

#include <QWidget>
#include "mainwindow.hh"

class MainWindow;

class OverlayWindow : public QWidget
{
    Q_OBJECT

public:
    explicit OverlayWindow(MainWindow *parent = nullptr);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    MainWindow *m_mainWindow;
};

#endif // OVERLAYWINDOW_HH
