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
#include "explorer.hh"
#include "overlaywindow.hh"

void setupTitleBar(QFrame *titleBar, QWidget *parent);
QPushButton *createTitleBarButton(const QString &text, QWidget *parent);
QFrame *createGripBar(QWidget *parent);

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
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

    Explorer *explorer = new Explorer(contentWidget);
    contentLayout->addWidget(explorer);

    connect(sidebar, &Sidebar::imagesButtonClicked, explorer, &Explorer::onImagesButtonClicked);
    connect(sidebar, &Sidebar::allFilesButtonClicked, explorer, &Explorer::onAllFilesButtonClicked);
    connect(sidebar, &Sidebar::gifsButtonClicked, explorer, &Explorer::onGifsButtonClicked);
    connect(sidebar, &Sidebar::textsButtonClicked, explorer, &Explorer::onTextsButtonClicked);
    connect(sidebar, &Sidebar::videosButtonClicked, explorer, &Explorer::onVideosButtonClicked);

    // Create the image drop area
    MediaDropArea *dropArea = new MediaDropArea(contentWidget);
    dropArea->setMinimumHeight(150);
    contentLayout->addWidget(dropArea);
    splitter->addWidget(contentWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(titleBar);
    mainLayout->addWidget(splitter);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setContentsMargins(0, 0, 0, 0);

    m_borderWidget = new BorderWidget(this);
    m_borderWidget->setGeometry(0, 0, width(), height());

    m_isDragging = false;

    connect(explorer, &Explorer::targetFolderChanged, sidebar, &Sidebar::updateButtonStyles);

    resize(800, 500);

    //This sizeGrip is added mainly for debugging purposes. full resizing solution is to be added later
    QSizeGrip *sizeGrip = new QSizeGrip(this);
    sizeGrip->setFixedSize(10, 10);
    sizeGrip->setStyleSheet("background-color: rgba(105, 105, 105, 0);"); //edit the last number to make this visible for debugging

    sizeGrip->move(width() - sizeGrip->width(), height() - sizeGrip->height());
    sizeGrip->raise();

    fullscreenOn = false;
}

MainWindow::~MainWindow() {
}

void MainWindow::mousePressEvent(QMouseEvent *event) {

    m_isDragging = false;
    int right_side_limit = width() - 100;

    if (event->button() == Qt::LeftButton && event->pos().y() < 50 && event->pos().x() < right_side_limit) {
        m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
        m_isDragging = true;
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (m_isDragging && event->buttons() & Qt::MouseButton::LeftButton) {
        move(event->globalPosition().toPoint() - m_dragPosition);
        event->accept();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (m_isDragging && event->button() == Qt::LeftButton) {
        m_isDragging = false;
        onDragFinished();
        event->accept();
    }
}

void MainWindow::onDragFinished() {
    m_isDragging = false;
}

void MainWindow::resizeEvent(QResizeEvent* event) {
    QSizeGrip *sizeGrip = findChild<QSizeGrip *>();
    if (sizeGrip) {
        sizeGrip->move(width() - sizeGrip->width(), height() - sizeGrip->height());
    }

    m_borderWidget->setGeometry(0, 0, width(), height());

    QWidget::resizeEvent(event);
}

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
    QObject::connect(restoreButton, &QPushButton::clicked, parent, [parent, restoreButton]() {
        if (parent->isMaximized()) {
            parent->showNormal();
            restoreButton->setIcon(QIcon::fromTheme(":/icons/window-maximize.svg"));
        } else {
            parent->showMaximized();
            restoreButton->setIcon(QIcon::fromTheme(":/icons/window-restore.svg"));
        }
    }
    );

    QFrame *gripBar = createGripBar(parent);

    QHBoxLayout *titleLayout = new QHBoxLayout(titleBar);
    titleLayout->addWidget(logoLabel, 0, Qt::AlignLeft | Qt::AlignVCenter);
    titleLayout->addWidget(gripBar, 1);
    titleLayout->addWidget(minimizeButton, 0, Qt::AlignCenter);
    titleLayout->addWidget(restoreButton, 0, Qt::AlignRight);
    titleLayout->addWidget(closeButton, 0, Qt::AlignRight);
    titleLayout->setContentsMargins(10, 0, 10, 0);
}

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

QFrame *createGripBar(QWidget *parent) {
    QFrame *gripBar = new QFrame(parent);
    gripBar->setStyleSheet("background-color: #21252b;");
    return gripBar;
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if ((event->modifiers() & Qt::ShiftModifier) && event->key() == Qt::Key_N) {
        toggleFullscreen();
    } else {
        QWidget::keyPressEvent(event);
    }
}

bool MainWindow::handleOverlayKeyPressEvent(QKeyEvent *event) {
    if ((event->modifiers() & Qt::ShiftModifier) && event->key() == Qt::Key_N) {
        toggleFullscreen();
        return true;
    }

    return false;
}


void MainWindow::toggleFullscreen() {
    fullscreenOn = !fullscreenOn;

    if (fullscreenOn) {
        overlayWindow = new OverlayWindow(this);
        overlayWindow->show();

    } else {
        overlayWindow->close();
        delete overlayWindow;
        overlayWindow = nullptr;
    }

    qDebug() << "Fullscreen mode:" << (fullscreenOn ? "ON" : "OFF");
}




