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

    QScreen *screen = QApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    resize(screenGeometry.size());

    installEventFilter(this);

    m_sidebar = new Sidebar(this);

    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(m_sidebar);

    QWidget *contentWidget = new QWidget(splitter);
    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);

    m_explorer = new Explorer(contentWidget);
    contentLayout->addWidget(m_explorer);

    connect(m_sidebar, &Sidebar::imagesButtonClicked, m_explorer, &Explorer::onImagesButtonClicked);
    connect(m_sidebar, &Sidebar::allFilesButtonClicked, m_explorer, &Explorer::onAllFilesButtonClicked);
    connect(m_sidebar, &Sidebar::gifsButtonClicked, m_explorer, &Explorer::onGifsButtonClicked);
    connect(m_sidebar, &Sidebar::textsButtonClicked, m_explorer, &Explorer::onTextsButtonClicked);
    connect(m_sidebar, &Sidebar::videosButtonClicked, m_explorer, &Explorer::onVideosButtonClicked);

    splitter->addWidget(contentWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(splitter);

    setStyleSheet("background-color: rgba(22, 22, 22, 80);");
}

bool OverlayWindow::eventFilter(QObject *watched, QEvent *event) {
    if (watched == this && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        return m_mainWindow->handleOverlayKeyPressEvent(keyEvent);
    }

    return QWidget::eventFilter(watched, event);
}
