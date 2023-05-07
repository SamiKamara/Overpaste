#include "overlaywindow.hh"
#include <QScreen>
#include <QApplication>
#include <QKeyEvent>

OverlayWindow::OverlayWindow(MainWindow *parent)
    : QWidget(parent), m_mainWindow(parent)
{
    setWindowTitle("Overlay Window");
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_NoSystemBackground, false);
    setStyleSheet("background-color: rgba(255, 0, 0, 127);");

    QScreen *screen = QApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    resize(screenGeometry.size());

    installEventFilter(this);
}

bool OverlayWindow::eventFilter(QObject *watched, QEvent *event) {
    if (watched == this && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        return m_mainWindow->handleOverlayKeyPressEvent(keyEvent);
    }

    return QWidget::eventFilter(watched, event);
}
