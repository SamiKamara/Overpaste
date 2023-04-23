#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QFrame>
#include <QGridLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>

void setupTitleBar(QFrame *titleBar, QWidget *parent);
QPushButton *createTitleBarButton(const QString &text, QWidget *parent);
QFrame *createGripBar(QWidget *parent);

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

        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(0, 0, 0, 0); // Lisää tämä rivi poistaaksesi ylimääräisen sisennyksen
        mainLayout->addWidget(titleBar);
        mainLayout->addWidget(contentWidget, 1);

        setWindowState(Qt::WindowMaximized);
    }

protected:
    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            m_dragPosition = event->globalPos() - frameGeometry().topLeft();
            event->accept();
        }
    }

    void mouseMoveEvent(QMouseEvent *event) override {
        if (event->buttons() & Qt::LeftButton) {
            move(event->globalPos() - m_dragPosition);
            event->accept();
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
    titleBar->setFixedHeight(30);
    titleBar->setStyleSheet("background-color: #007acc;");

    QPushButton *closeButton = createTitleBarButton("X", parent);
    QObject::connect(closeButton, &QPushButton::clicked, parent, &QWidget::close);
    QPushButton *minimizeButton = createTitleBarButton("-", parent);
    QObject::connect(minimizeButton, &QPushButton::clicked, parent, &QWidget::showMinimized);
    QPushButton *restoreButton = createTitleBarButton("□", parent);

    QObject::connect(restoreButton, &QPushButton::clicked, [parent](){
        if (parent->isMaximized()) {
            parent->showNormal();
        } else {
            parent->showMaximized();
        }
    });

    QFrame *gripBar = createGripBar(parent);

    QHBoxLayout *titleLayout = new QHBoxLayout(titleBar);
    titleLayout->addWidget(gripBar, 1);
    titleLayout->addWidget(minimizeButton, 0, Qt::AlignRight | Qt::AlignTop);
    titleLayout->addWidget(restoreButton, 0, Qt::AlignRight | Qt::AlignTop);
    titleLayout->addWidget(closeButton, 0, Qt::AlignRight | Qt::AlignTop);
    titleLayout->setContentsMargins(0, 0, 0, 0);
}

QPushButton *createTitleBarButton(const QString &text, QWidget *parent) {
    QPushButton *button = new QPushButton(text, parent);
    button->setStyleSheet("color: #ffffff; font-size: 16px; background-color: #007acc; border: none;");
    button->setFixedSize(25, 25);
    return button;
}

QFrame *createGripBar(QWidget *parent) {
    QFrame *gripBar = new QFrame(parent);
    gripBar->setStyleSheet("background-color: #007acc;");
    return gripBar;
}
