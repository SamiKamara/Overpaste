#include "overlaywindow.hh"
#include <QScreen>
#include <QApplication>
#include <QKeyEvent>
#include <QCursor>

OverlayWindow::OverlayWindow(MainWindow *parent)
    : QWidget(parent), m_mainWindow(parent)
{    
    setWindowTitle("Overlay Window");
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_NoSystemBackground, false);

    //replace the lines under these with these commented out ones to get fullscreen overlay
    //QScreen *screen = QApplication::primaryScreen();
    //QRect screenGeometry = screen->geometry();
    //resize(screenGeometry.size());

    QScreen *screen = QApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int newWidth = screenGeometry.height() / 2;
    int newHeight = screenGeometry.height() / 2;
    resize(newWidth, newHeight);

    m_sidebar = new Sidebar(this);

    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(m_sidebar);

    QWidget *contentWidget = new QWidget(splitter);
    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);

    m_explorer = new Explorer(m_mainWindow, contentWidget);
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

    KeyListener *keyListener = new KeyListener(this);
    connect(keyListener, &KeyListener::hotKeyPressed, m_mainWindow, &MainWindow::toggleFullscreen);
    connect(m_explorer, &Explorer::targetFolderChanged, m_sidebar, &Sidebar::updateButtonStyles);

    mainLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setContentsMargins(0, 0, 0, 0);
}

void OverlayWindow::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    QPoint cursorPos = QCursor::pos();
    int newX = cursorPos.x() - width() / 2;
    int newY = cursorPos.y() - height() / 2;
    move(newX, newY);

    raise();
    activateWindow();
}

