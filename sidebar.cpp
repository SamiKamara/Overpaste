#include "sidebar.hh"
#include <QFrame>
#include <QLabel>

QString createLabelStyle() {
    return
    "color: #9aa0a6;"
    "font-size: 10px;"
    "font-weight: 700;"
    "padding: 10px;"
    "text-transform: uppercase;"
    "font-weight: 700;"
    "margin-bottom: -5px;";
}

Sidebar::Sidebar(QWidget *parent)
    : QWidget(parent)
{
    sidebarLayout = new QVBoxLayout(this);
    sidebarLayout->setContentsMargins(0, 0, 0, 0);
    setFixedWidth(250);

    QFrame *sidebarFrame = new QFrame(this);
    sidebarFrame->setStyleSheet("background-color: #21252b;");

    QVBoxLayout *sidebarContentLayout = new QVBoxLayout(sidebarFrame);
    sidebarContentLayout->setContentsMargins(0, 0, 0, 0);
    sidebarContentLayout->setSpacing(0);

    const QString buttonStyle =
        "QPushButton {"
        "text-align: left;"
        "color: #9aa0a6;"
        "background-color: #21252b;"
        "border: none;"
        "padding: 10px;"
        "font-size: 14px;"
        "font-weight: 700;}"
        "QPushButton:hover {"
        "background-color: #393d47 !important;}"
    ;

    sidebarContentLayout->addWidget(
        createButton(" Toggle sidebar", QIcon(":/icons/menu.svg"), buttonStyle));

    QLabel *filesLabel = new QLabel("Files", this);
    const QString labelStyle = createLabelStyle();

    filesLabel->setStyleSheet(labelStyle);

    sidebarContentLayout->addWidget(filesLabel);

    allFilesButton = createButton(" All files", QIcon(":/icons/all.svg"), buttonStyle);
    connect(allFilesButton, &QPushButton::clicked, this, &Sidebar::allFilesButtonClicked);
    sidebarContentLayout->addWidget(allFilesButton);

    QLabel *categoriesLabel = new QLabel("Categories", this);

    categoriesLabel->setStyleSheet(labelStyle);

    sidebarContentLayout->addWidget(categoriesLabel);

    imagesButton = createButton(" Images", QIcon(":/icons/images.svg"), buttonStyle);
    connect(imagesButton, &QPushButton::clicked, this, &Sidebar::imagesButtonClicked);
    sidebarContentLayout->addWidget(imagesButton);

    gifsButton = createButton(" GIFs", QIcon(":/icons/gifs.svg"), buttonStyle);
    connect(gifsButton, &QPushButton::clicked, this, &Sidebar::gifsButtonClicked);
    sidebarContentLayout->addWidget(gifsButton);

    textsButton = createButton(" Texts", QIcon(":/icons/texts.svg"), buttonStyle);
    connect(textsButton, &QPushButton::clicked, this, &Sidebar::textsButtonClicked);
    sidebarContentLayout->addWidget(textsButton);

    videosButton = createButton(" Videos", QIcon(":/icons/videos.svg"), buttonStyle);
    connect(videosButton, &QPushButton::clicked, this, &Sidebar::videosButtonClicked);
    sidebarContentLayout->addWidget(videosButton);

    QSpacerItem *spacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    sidebarContentLayout->addItem(spacer);
    sidebarLayout->addWidget(sidebarFrame);
}

QPushButton* Sidebar::createButton(const QString& text, const QIcon& icon, const QString& style)
{
    QPushButton *button = new QPushButton(this);
    button->setIcon(icon);
    button->setIconSize(QSize(22, 22));
    button->setText(text);
    button->setStyleSheet(style);
    button->setFixedSize(250, 40);
    button->setCursor(Qt::PointingHandCursor);
    return button;
}

void Sidebar::updateButtonStyles(int targetFolderId) {
    const QString normalStyle =
        "QPushButton {"
        "text-align: left;"
        "color: #9aa0a6;"
        "background-color: #21252b;"
        "border: none;"
        "padding: 10px;"
        "font-size: 14px;"
        "font-weight: 700;}"
        "QPushButton:hover {"
        "background-color: #393d47 !important;}"
        ;

    const QString hoverStyle =
        "QPushButton {"
        "text-align: left;"
        "color: #9aa0a6;"
        "background-color: #393d47;"
        "border: none;"
        "padding: 10px;"
        "font-size: 14px;"
        "font-weight: 700;}"
        "QPushButton:hover {"
        "background-color: #5a5e6a !important;}"
        ;

    allFilesButton->setStyleSheet(targetFolderId == 0 ? hoverStyle : normalStyle);
    imagesButton->setStyleSheet(targetFolderId == 1 ? hoverStyle : normalStyle);
    gifsButton->setStyleSheet(targetFolderId == 2 ? hoverStyle : normalStyle);
    textsButton->setStyleSheet(targetFolderId == 3 ? hoverStyle : normalStyle);
    videosButton->setStyleSheet(targetFolderId == 4 ? hoverStyle : normalStyle);
}
