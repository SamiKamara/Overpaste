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

void setupTitleBar(QFrame *titleBar, QWidget *parent);
QPushButton *createTitleBarButton(const QString &text, QWidget *parent);
QFrame *createGripBar(QWidget *parent);

class ImageDropArea : public QLabel {
public:
    ImageDropArea(QWidget *parent = nullptr) : QLabel(parent) {
        setAcceptDrops(true);
        setAlignment(Qt::AlignCenter);
        setText("Drop .png file here, to save as pasta.png");
        setFrameStyle(QFrame::Box | QFrame::Sunken);
    }

protected:
    void dragEnterEvent(QDragEnterEvent *event) override {
        if (event->mimeData()->hasUrls()) {
            event->acceptProposedAction();
        }
    }

    void dropEvent(QDropEvent *event) override {
        const QMimeData *mimeData = event->mimeData();
        if (mimeData->hasUrls()) {
            QList<QUrl> urlList = mimeData->urls();
            for (const QUrl &url : urlList) {
                QFileInfo fileInfo(url.toLocalFile());
                if (fileInfo.suffix().toLower() == "png") {
                    QFile file(url.toLocalFile());
                    QString buildPath = QApplication::applicationDirPath() + "/pasta.png";
                    QFile oldFile(buildPath);
                    if (oldFile.exists()) {
                        oldFile.remove();
                    }
                    file.copy(buildPath);
                    setText("File saved as pasta.png");
                    break;
                }
            }
        }
    }
};

class CustomWindow : public QWidget {
public:
    explicit CustomWindow(QWidget *parent = nullptr) : QWidget(parent) {
        setWindowFlags(Qt::FramelessWindowHint);
        setWindowTitle("Custom Title Bar");

        QFrame *titleBar = new QFrame(this);
        setupTitleBar(titleBar, this);

        QWidget *contentWidget = new QWidget(this);
        QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
        contentLayout->addWidget(new QLabel("This is content", contentWidget));

        // Create the first button
        QPushButton *copyButton1 = new QPushButton("Copy 'hello pasta'", contentWidget);
        contentLayout->addWidget(copyButton1);
        connect(copyButton1, &QPushButton::clicked, this, [=]() { copyTextToClipboard("hello pasta"); });

        // Create the second button
        QPushButton *copyButton2 = new QPushButton("Copy 'hello over'", contentWidget);
        contentLayout->addWidget(copyButton2);
        connect(copyButton2, &QPushButton::clicked, this, [=]() { copyTextToClipboard("hello over"); });

        // Create the image copy button
        QPushButton *copyImageButton = new QPushButton("Copy 'pasta.png' image", contentWidget);
        contentLayout->addWidget(copyImageButton);
        connect(copyImageButton, &QPushButton::clicked, this, &CustomWindow::copyImageToClipboard);

        // Create the image drop area
        ImageDropArea *dropArea = new ImageDropArea(contentWidget);
        contentLayout->addWidget(dropArea);

        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->addWidget(titleBar);
        mainLayout->addWidget(contentWidget, 1);

        setFixedSize(500, 500);
    }

protected:
    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
            event->accept();
        }
    }

    void mouseMoveEvent(QMouseEvent *event) override {
        if (event->buttons() & Qt::MouseButton::LeftButton) {
            move(event->globalPosition().toPoint() - m_dragPosition);
            event->accept();
        }
    }

private slots:
    void copyTextToClipboard(const QString &text) {
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(text);
    }

    void copyImageToClipboard() {
        QString imagePath = QApplication::applicationDirPath() + "/pasta.png";
        QImage image(imagePath);
        if (!image.isNull()) {
            QClipboard *clipboard = QApplication::clipboard();
            clipboard->setImage(image);
        } else {
            qDebug("Failed to load image.");
        }
    }

private:
    QPoint m_dragPosition;
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    CustomWindow window;
    window.show();

    return app.exec();
}

void setupTitleBar(QFrame *titleBar, QWidget *parent) {
    titleBar->setFixedHeight(50);
    titleBar->setStyleSheet("background-color: #21252b;");

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
    });


    QFrame *gripBar = createGripBar(parent);

    QHBoxLayout *titleLayout = new QHBoxLayout(titleBar);
    titleLayout->addWidget(gripBar, 1);

    QSpacerItem* verticalSpacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
    titleLayout->addItem(verticalSpacer);

    titleLayout->addWidget(minimizeButton, 0, Qt::AlignCenter);
    titleLayout->addWidget(restoreButton, 0, Qt::AlignRight);
    titleLayout->addWidget(closeButton, 0, Qt::AlignRight);
    titleLayout->setContentsMargins(0, 0, 10, 0);
}

// Function to create title bar button with SVG icon
QPushButton* createTitleBarButton(const QString& iconPath, QWidget* parent)
{
    QPushButton* button = new QPushButton(parent);
    button->setFixedSize(32, 32);

    // If close button, change the hover and pressed colors
    const QString baseStyleSheet = "QPushButton { border: none; border-radius: 5px; }";
    const QString hoverColor = iconPath == ":/icons/window-close.svg" ? "#ff5f56" : "#393d47";
    const QString pressedColor = iconPath == ":/icons/window-close.svg" ? "#e81123" : "#2a2e37";

    button->setStyleSheet(baseStyleSheet +
                          "QPushButton:hover { background-color: " + hoverColor + "; }"
                          "QPushButton:pressed { background-color: " + pressedColor + "; }");


    // Load the SVG file and render it onto a QPixmap
    QSvgRenderer renderer(iconPath);
    QPixmap pixmap(32, 32);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    renderer.render(&painter);

    // Set the pixmap as the button icon
    button->setIcon(QIcon(pixmap));

    return button;
}

QFrame *createGripBar(QWidget *parent) {
    QFrame *gripBar = new QFrame(parent);
    gripBar->setStyleSheet("background-color: #21252b;");
    return gripBar;
}
