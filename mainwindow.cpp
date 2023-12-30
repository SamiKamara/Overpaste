#include "mainwindow.hh"
#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QFrame>
#include <QGridLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QClipboard>
#include <QImage>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QUrl>
#include <QMimeData>
#include <QFileInfo>
#include <QFile>
#include <QtSvg/QSvgRenderer>
#include <QPainter>
#include <sidebar.hh>
#include <QSplitter>
#include <mediadroparea.hh>
#include <borderwidget.hh>
#include "overlaywindow.hh"

void setupTitleBar(QFrame *titleBar, QWidget *parent);
QPushButton *createTitleBarButton(const QString &text, QWidget *parent);
QFrame *createGripBar(QWidget *parent);

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setObjectName("MainWindow");
    fullscreenOn = false;
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowTitle("Custom Title Bar");
    setStyleSheet("background-color: #282c34;");
    QFrame *titleBar = new QFrame(this);
    setupTitleBar(titleBar, this);
    Sidebar *sidebar = new Sidebar(this);
    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(sidebar);
    QWidget *contentWidget = new QWidget(splitter);
    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
    Explorer *explorer = new Explorer(this, contentWidget);
    contentLayout->addWidget(explorer);
    connect(sidebar, &Sidebar::imagesButtonClicked, explorer, &Explorer::onImagesButtonClicked);
    connect(sidebar, &Sidebar::allFilesButtonClicked, explorer, &Explorer::onAllFilesButtonClicked);
    connect(sidebar, &Sidebar::gifsButtonClicked, explorer, &Explorer::onGifsButtonClicked);
    connect(sidebar, &Sidebar::textsButtonClicked, explorer, &Explorer::onTextsButtonClicked);
    connect(sidebar, &Sidebar::videosButtonClicked, explorer, &Explorer::onVideosButtonClicked);
    QStackedLayout *stackedLayout = new QStackedLayout();
    stackedLayout->addWidget(splitter);
    dropArea = new MediaDropArea(this);
    dropArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    stackedLayout->addWidget(dropArea);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(titleBar);
    mainLayout->addLayout(stackedLayout);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    m_borderWidget = new BorderWidget(this);
    m_borderWidget->setGeometry(0, 0, width(), height());
    m_isDragging = false;
    connect(explorer, &Explorer::targetFolderChanged, sidebar, &Sidebar::updateButtonStyles);
    resize(800, 500);
    dropArea->setGeometry(0, 0, width(), height());
    QSizeGrip *sizeGrip = new QSizeGrip(this);
    sizeGrip->setFixedSize(10, 10);
    sizeGrip->setStyleSheet("background-color: rgba(105, 105, 105, 0);");
    sizeGrip->move(width() - sizeGrip->width(), height() - sizeGrip->height());
    sizeGrip->raise();
    showMediaDropArea(true);
    setAcceptDrops(true);
    KeyListener *m_keyListener = new KeyListener(this);
    connect(m_keyListener, &KeyListener::hotKeyPressed, this, &MainWindow::toggleFullscreen);
    fullscreenOn = true;
    overlayWindow = new OverlayWindow(this);
    fullscreenOn = false;
}

MainWindow::~MainWindow() {
}

// Handles mouse press events to initiate window dragging.
void MainWindow::mousePressEvent(QMouseEvent *event) {
    m_isDragging = false;
    int right_side_limit = width() - 100;

    if (event->button() == Qt::LeftButton && event->pos().y() < 50 && event->pos().x() < right_side_limit) {
        if (isMaximized()) {
            resetWindowGeometry();
        } else {
            m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        }

        event->accept();
        m_isDragging = true;
    }
}

// Manages window movement during mouse drag.
void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (m_isDragging && event->buttons() & Qt::MouseButton::LeftButton) {
        move(event->globalPosition().toPoint() - m_dragPosition);
        event->accept();
    }
}

// Handles mouse release events to end window dragging.
void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (m_isDragging && event->button() == Qt::LeftButton) {
        m_isDragging = false;
        onDragFinished();
        event->accept();
    }
}

// Finalizes the dragging process.
void MainWindow::onDragFinished() {
    m_isDragging = false;
}

// Manages window resize events, adjusting size grip and other widgets.
void MainWindow::resizeEvent(QResizeEvent* event) {
    QSizeGrip *sizeGrip = findChild<QSizeGrip *>();
    if (sizeGrip) {
        sizeGrip->move(width() - sizeGrip->width(), height() - sizeGrip->height());
    }

    m_borderWidget->setGeometry(0, 0, width(), height());
    dropArea->setGeometry(0, 0, width(), height());

    QWidget::resizeEvent(event);
}

// Sets up the title bar with logo, buttons, and style.
void setupTitleBar(QFrame *titleBar, QWidget *parent) {
    titleBar->setFixedHeight(50);
    titleBar->setStyleSheet("background-color: #21252b; border-bottom: 3px solid #2c313a;");
    QLabel *logoLabel = new QLabel(titleBar);
    logoLabel->setPixmap(QPixmap(":/icons/logo.svg").scaled(150, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    logoLabel->setStyleSheet("border: none;");

    QPushButton *closeButton = createTitleBarButton(":/icons/window-close.svg", parent);
    QObject::connect(closeButton, &QPushButton::clicked, parent, &QWidget::close);

    QPushButton *minimizeButton = createTitleBarButton(":/icons/window-minimize.svg", parent);
    QObject::connect(minimizeButton, &QPushButton::clicked, parent, &QWidget::showMinimized);

    QPushButton *restoreButton = createTitleBarButton(parent->isMaximized() ? ":/icons/window-restore.svg" : ":/icons/window-maximize.svg", parent);
    restoreButton->setObjectName("restoreButton");

    QObject::connect(restoreButton, &QPushButton::clicked, parent, [parent, restoreButton]() {
        MainWindow* mainWindow = static_cast<MainWindow*>(parent);
        if (parent->isMaximized()) {
            parent->showNormal();
            restoreButton->setIcon(QIcon::fromTheme(":/icons/window-maximize.svg"));
        } else {
            mainWindow->storedNormalWindowWidth = mainWindow->width();
            parent->showMaximized();
            restoreButton->setIcon(QIcon::fromTheme(":/icons/window-restore.svg"));
        }
    });

    QFrame *gripBar = createGripBar(parent);

    QHBoxLayout *titleLayout = new QHBoxLayout(titleBar);
    titleLayout->addWidget(logoLabel, 0, Qt::AlignLeft | Qt::AlignVCenter);
    titleLayout->addWidget(gripBar, 1);
    titleLayout->addWidget(minimizeButton, 0, Qt::AlignCenter);
    titleLayout->addWidget(restoreButton, 0, Qt::AlignRight);
    titleLayout->addWidget(closeButton, 0, Qt::AlignRight);
    titleLayout->setContentsMargins(10, 0, 10, 0);
}

// Creates a custom button for the title bar with specified icon and styles.
QPushButton* createTitleBarButton(const QString& iconPath, QWidget* parent) {
    QPushButton* button = new QPushButton(parent);
    button->setFixedSize(32, 32);
    const QString baseStyleSheet = "QPushButton { border: none; border-radius: 5px; }";
    const QString hoverColor = iconPath == ":/icons/window-close.svg" ? "#ff5f56" : "#393d47";
    const QString pressedColor = iconPath == ":/icons/window-close.svg" ? "#e81123" : "#2a2e37";

    button->setStyleSheet(baseStyleSheet +
                          "QPushButton:hover { background-color: " + hoverColor + "; }"
                          "QPushButton:pressed { background-color: " + pressedColor + "; }");

    QSvgRenderer renderer(iconPath);
    QPixmap pixmap(32, 32);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    renderer.render(&painter);

    button->setIcon(QIcon(pixmap));

    return button;
}

// Creates a grip bar for window resizing.
QFrame *createGripBar(QWidget *parent) {
    QFrame *gripBar = new QFrame(parent);
    gripBar->setStyleSheet("background-color: #21252b;");
    return gripBar;
}

// Toggles the window between fullscreen and normal state.
void MainWindow::toggleFullscreen() {
    fullscreenOn = !fullscreenOn;

    if (fullscreenOn) {
        if (!overlayWindow) {
            overlayWindow = new OverlayWindow(this);
        }
        overlayWindow->show();
    } else {
        if (overlayWindow) {
            overlayWindow->close();
        }
    }
}

// Resets window geometry when exiting from maximized state.
void MainWindow::resetWindowGeometry() {
    if (isMaximized()) {
        showNormal();

        QPushButton *restoreButton = findChild<QPushButton *>("restoreButton");

        if (restoreButton) {
            restoreButton->setIcon(QIcon::fromTheme(":/icons/window-maximize.svg"));
        }

        m_dragPosition = QPoint(storedNormalWindowWidth / 2, 14);
    }
}

// Toggles the visibility of the media drop area.
void MainWindow::showMediaDropArea(bool visible) {
    MediaDropArea *dropArea = findChild<MediaDropArea *>();
    if (dropArea) {
        dropArea->setVisible(visible);
        if (visible) {
            dropArea->raise();
        }
    }
}

// Handles drag enter events for enabling drag-and-drop functionality.
void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasUrls()) {
        dropArea->setAttribute(Qt::WA_TransparentForMouseEvents, false);
        event->acceptProposedAction();
    }
}

// Handles drag leave events in the application.
void MainWindow::dragLeaveEvent(QDragLeaveEvent *event) {
    event->accept();
}

// Checks if the window is in fullscreen mode.
bool MainWindow::isFullscreenOn() const {
    return fullscreenOn;
}

